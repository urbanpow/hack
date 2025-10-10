#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) { perror("socket failed"); exit(EXIT_FAILURE); }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    int addrlen = sizeof(address);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) { perror("listen failed"); exit(EXIT_FAILURE); }

    std::cout << "HTTP server running on http://localhost:8080\n";

    while (true) {
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) { perror("accept"); continue; }

        char buffer[4096] = {0};
        read(client_socket, buffer, 4096);
        std::string request(buffer);

        std::cout << "📩 Received Request:\n" << request << std::endl;

        // Very simple HTTP parsing
        std::string response;
        if (request.find("GET /") == 0) {
            response = 
                "HTTP/1.1 200 OK\n"
                "Content-Type: text/plain\n"
                "Connection: close\n\n"
                "Hello! This is a GET response";
        } else if (request.find("POST /") == 0) {
            // Extract POST body (naive)
            auto pos = request.find("\r\n\r\n");
            std::string body = (pos != std::string::npos) ? request.substr(pos + 4) : "";
            response =
                "HTTP/1.1 200 OK\n"
                "Content-Type: text/plain\n"
                "Connection: close\n\n"
                "Received POST data: " + body;
        } else {
            response =
                "HTTP/1.1 404 Not Found\n"
                "Content-Type: text/plain\n\n"
                "Not Found";
        }

        write(client_socket, response.c_str(), response.size());
        close(client_socket);
    }

    return 0;
}
