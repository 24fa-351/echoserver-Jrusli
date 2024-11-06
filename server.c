#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handle_client(int client_socket) {
    char buffer[MAX_BUFFER];
    int bytes_received;

    while (1) {
        memset(buffer, 0, MAX_BUFFER);

        bytes_received = recv(client_socket, buffer, MAX_BUFFER - 1, 0);

        if (bytes_received <= 0) {
            // If no data received, close the connection
            if (bytes_received == 0) {
                printf("Client closed connection.\n");
            } else {
                perror("Error receiving data");
            }
            break;
        }

        buffer[bytes_received] = '\0'; 

        printf("Received request:\n%s\n", buffer);

       
        if (strstr(buffer, "GET /favicon.ico") != NULL) {
        
            const char *response = "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, response, strlen(response), 0);
            close(client_socket);  
            return;
        }

       
        const char *html_content_template = "<h1>Echo: </h1><p>%s</p>";
        char html_content[MAX_BUFFER];

        snprintf(html_content, sizeof(html_content), html_content_template, buffer);

        char http_response[MAX_BUFFER];
        int content_length = strlen(html_content);

        snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "\r\n"
            "%s", content_length, html_content);

        send(client_socket, http_response, strlen(http_response), 0);
    }

    close(client_socket);
}



void start_server(int port) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);


    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

   
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to bind socket");
        close(server_socket);
        exit(1);
    }


    if (listen(server_socket, 1) == -1) {
        perror("Failed to listen on socket");
        close(server_socket);
        exit(1);
    }

    printf("Server is listening on port %d...\n", port);

   
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        printf("Client connected, handling communication...\n");

        handle_client(client_socket);

        printf("Client disconnected, waiting for a new connection...\n");
    }

    close(server_socket);
}
