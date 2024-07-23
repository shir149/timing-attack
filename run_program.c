#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    int choice;

    // Start PHP built-in server in the background
    system("start /b php -S localhost:8000");

    // Give the server some time to start
    Sleep(2000);

    // Make a web request to client.php
    system("curl http://localhost:8000/client.php");

    // Ask user to choose between protect.c and server.c
    printf("Please choose a program to run:\n");
    printf("1.Timing Attack protection\n");
    printf("2. Timing Attack\n");

    printf("Enter the number of your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            system("gcc protect.c -o protect -lws2_32 && protect");

            break;
        case 2:
            system("gcc server.c -o server -lws2_32 && server");
            system("php -S localhost:8000");
            break;
        default:
            printf("Invalid choice. Please choose 1 or 2.\n");
            return 1; // Exit if invalid choice
    }

    // Open index.html in the default web browser
        system("start http://localhost:8000/client.php");

    return 0;
}
