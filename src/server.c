//Made by Gerworks-HS (@itsgerliz)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main(int argc, char **argv) {
	//Check for correct app usage
	if (argc != 2) {
		fprintf(stdout, "Usage: %s <port>\n", argv[0]);
		return 1;
	}
	//Parse port from argv to a long using strtol
	uint16_t localPort; //Variable for later use
	long parsedPort = strtol(argv[1], NULL, 10);
	if (parsedPort < 1 || parsedPort > 65535) { //Check if it overflows or underflows the valid range
		fprintf(stderr, "Port <%s> is not in a valid range\n", argv[1]);
		return 1;
	} else {
		localPort = (uint16_t)parsedPort; //Type-cast the long to an unsigned 16-bit integer (0-65535)
		fprintf(stdout, "Server will listen on port <%d>\n", localPort);
	}
	//Create server local socket
	const int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //Create server local socket (TCP/IP)
	if (serverSocket == -1) { //Check for errors
		fprintf(stderr, "Failed trying to create server local socket\n");
		return 1;
	} else {
		fprintf(stdout, "Server local socket created, file descriptor is <%d>\n", serverSocket);
	}
	//Bind server local address to local server socket
	///Creation of the server address structure
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(localPort); //To convert port to network byte order
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); //To convert loopback address to network byte order
	///Bind function and error check
	if (bind(serverSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		fprintf(stderr, "Failed to bind server address to local socket\n");
		close(serverSocket);
		return 1;
	} else {
		fprintf(stdout, "Server address bound to local socket\n");
	}
	//Set local socket as listening
	if (listen(serverSocket, 5) == -1) {
		fprintf(stderr, "Error trying to set local socket as listening\n");
		close(serverSocket);
		return 1;
	} else {
		fprintf(stdout, "Local socket is now listening for incoming connections\n");
	}
	//Start main loop to accept incoming connections
	///We will use poll() to check if there is data to retrieve or send
	struct sockaddr_in clientAddress; //Declare structure to save client address
	socklen_t clientAddress_len = sizeof(clientAddress); //Save size of clientAddress struct in this variable
	while (1) {
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddress_len);
		if (clientSocket == -1) {
			fprintf("Failed trying to accept next incoming connection, retrying...\n");
			continue; //Proceed to next iteration when no incoming connection is found to accept
		} else {
			char clientIP[INET_ADDRSTRLEN]; //Declare variable to save client IP with max size defined by the macro
			inet_ntop(AF_INET, &clientAddress.sin_addr, &clientIP, sizeof(clientIP)); //Convert IP to human notation
			fprintf(stdout, "Incoming connection from <%lu>\n", &clientIP;
			fprintf(stdout, "Connection stablished\n");
		}
	}
	close(serverSocket); //Close the server local socket file descriptor
	fprintf(stdout, "Server local socket closed, shutting down...\n");
	return 0; //Main return
}
//Made by Gerworks-HS (@itsgerliz)
