#include "tcp_server.h"
#include "../storage/kv_store.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// 🔥 Client handler (runs in separate thread)
void handleClient(int client_socket, KVStore& kv) {
    while (true) {
        std::string input;
        char ch;

        // Read full line
        while (true) {
            int bytes = recv(client_socket, &ch, 1, 0);

            if (bytes <= 0) {
                closesocket(client_socket);
                return;
            }

            if (ch == '\n') {
                break;
            }

            if (ch != '\r') {
                input += ch;
            }
        }

        if (input.empty()) {
            continue;
        }

        std::stringstream ss(input);
        std::string word;
        std::vector<std::string> tokens;

        while (ss >> word) {
            tokens.push_back(word);
        }

        std::string response;

        if (tokens.size() > 0) {
            std::string command = tokens[0];

            if (command == "SET" && tokens.size() == 3) {
                kv.set(tokens[1], tokens[2]);
                response = "OK\n";
            }
            else if (command == "GET" && tokens.size() == 2) {
                response = kv.get(tokens[1]) + "\n";
            }
            else if (command == "DEL" && tokens.size() == 2) {
                bool result = kv.del(tokens[1]);
                response = result ? "Deleted\n" : "Not Found\n";
            }
            else if (command == "EXIT") {
                response = "Bye\n";
                send(client_socket, response.c_str(), response.size(), 0);
                closesocket(client_socket);
                return;
            }
            else {
                response = "Invalid Command\n";
            }
        }
        else {
            response = "Invalid Command\n";
        }

        send(client_socket, response.c_str(), response.size(), 0);
    }
}

void TCPServer::start(int port) {
    KVStore kv;

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server started on port " << port << std::endl;

    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        std::cout << "Client connected\n";

        std::thread t(handleClient, new_socket, std::ref(kv));
        t.detach();  // run independently
    }

    WSACleanup();
}