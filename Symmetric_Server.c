#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int n;

    while (1) {
        // Receive message from client
        memset(buffer, 0, sizeof(buffer));
        n = recv(client_socket, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        }
        printf("Client: %s", buffer);

        // Send message to client
        printf("Enter message for client: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    int port;
    printf("Enter port number to start the server (e.g., 8080): ");
    scanf("%d", &port);

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces
    server_addr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Error listening on socket");
        close(server_socket);
        exit(1);
    }

    printf("Server is listening on port %d...\n", port);

    // Accept client connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Error accepting connection");
        close(server_socket);
        exit(1);
    }

    printf("Client connected.\n");

    // Handle client communication
    handle_client(client_socket);

    // Close the server socket
    close(server_socket);

    return 0;
}
