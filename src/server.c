//Made by Gerworks-HS (@itsgerliz)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
int main(int argc, char **argv) {
	//Check for correct app usage
	if (argc != 2) {
		fprintf(stdout, "Usage: %s <port>\n", argv[0]);
		return 1;
	}
	//Parse port from argv to a long using strtol
	long parsedPort = strtol(argv[1], NULL, 10);
	if (parsedPort < 1 || parsedPort > 65535) { //Check if it overflows or underflows the valid range
		fprintf(stderr, "Port <%s> is not in a valid range\n", argv[1]);
		return 1;
	} else {
		const uint16_t localPort = (uint16_t)parsedPort; //Type-cast the long to an unsigned 16-bit integer (0-65535)
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
	close(serverSocket); //Close the server local socket file descriptor
	fprintf(stdout, "Server local socket closed, shutting down...\n");
	return 0; //Main return
}
//Made by Gerworks-HS (@itsgerliz)
