#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/timeb.h> // For timing

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUF_SIZE 1024
#define PASSWORD "secret123"
#define FIXED_DELAY 100 // Fixed delay in milliseconds

void handle_client(SOCKET client_socket) {
    char buffer[BUF_SIZE];
    int valread;
    int password_length = strlen(PASSWORD);

    // Read username and password
    valread = recv(client_socket, buffer, BUF_SIZE, 0);
    buffer[valread] = '\0';
    char *username = strtok(buffer, "\n");
    char *password = strtok(NULL, "\n");

    if (username != NULL) {
        printf("Username: %s\n", username);
    }
    if (password != NULL) {
        printf("Password attempt: %s\n", password);
    }

    int match = 1;
    for (int i = 0; i < password_length; i++) {
        // Timing
        struct timeb start, end;
        ftime(&start);

        if (password[i] != PASSWORD[i]) {
            match = 0;
        }

        // Always introduce the same delay
        Sleep(FIXED_DELAY);

        ftime(&end);
        double elapsed_time = (end.time - start.time) * 1000.0 + (end.millitm - start.millitm); // Convert to milliseconds
        printf("Time taken for character %d: %.2f ms\n", i, elapsed_time);
    }

    // Final check if the password is correct
    if (match && strlen(password) == password_length) {
        printf("Password is correct.\n");
        send(client_socket, "CORRECT", strlen("CORRECT"), 0);
    } else {
        printf("Password is incorrect.\n");
        send(client_socket, "INCORRECT", strlen("INCORRECT"), 0);
    }

    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        handle_client(client_socket);
    }

    // Cleanup
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
