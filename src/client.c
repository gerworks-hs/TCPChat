#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//Define socket var to use it outside main()
int targetSocket = -100;
//SIGINT handler function
void sigint_handler(int signal) {
	close(targetSocket);
	fprintf(stderr, "\nControl+C(SIGINT) catched\n");
	exit(EXIT_FAILURE);
}
//Main function
void main(int argc, char **argv) {
	//SIGINT handler function
	signal(SIGINT, sigint_handler);
	//Check for correct app usage
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <Server IP> <Port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	//Parse port from argv to a long using strtol
	uint16_t targetPort; //Will be used later in the code
	long parsedPort = strtol(argv[2], NULL, 10);
	if (parsedPort < 1 || parsedPort > 65535) { //Check if it underflows or overflows the valid range
		fprintf(stderr, "Port <%s> is not in a valid range\n");
		exit(EXIT_FAILURE);
	} else {
		targetPort = (uint16_t)parsedPort; //Type-cast the long to an unsigned 16bit integer (0-65535)
	}
	//Parse IP from argv to network byte order
	struct in_addr serverIP;
	if (inet_pton(AF_INET, argv[1], &serverIP.s_addr) != 1) {
		fprintf(stderr, "IP <%s> is not valid", argv[1]);
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "We will connect to <%s> on port <%s>", argv[1], argv[2]);
	//Create socket
	targetSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (targetSocket < 0) {
		fprintf(stderr, "Failed trying to create socket\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Client socket created, file descriptor is <%d>\n", targetSocket);
	}
	//Connect to server
	///Create sockaddr_in structure to save server address
	struct sockaddr_in serverAddress = {
		AF_INET,
		htons(targetPort),
		serverIP
	};
	if (connect(targetSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0) {
		fprintf(stderr, "Failed trying to conect to server\n");
		close(targetSocket);
		exit(EXIT_FAILURE);
	} else {
		char buffer[256]; //Buffer for messages
		fprintf(stdout, "Connection established\n");
		fprintf(stdout, "----------------------\n");
		while (1) { //Start main poll() loop
			struct pollfd pollfds[2] = {
				{
					0, //Standard input aka stdin (file descriptor 0)
					POLLIN, //Check for data to be read
					-1 //Init returned events as -1, it will be overriden in the future
				},
				{ //The same changing the fd 0 (stdin) to the file descriptor of the new client socket
					targetSocket,
					POLLIN,
					-1
				}
			};
			memset(buffer, 0, sizeof(buffer));
			poll(pollfds, 2, -1);
			//Poll will wait until some file descriptor has data to read
			if (pollfds[0].revents & POLLIN) {
				int bytesRead = read(0, buffer, sizeof(buffer));
				buffer[bytesRead] = '\0'; //Null terminate the string
				for (int i = 0, i == sizeof(buffer), i++) { //Remove all new lines from the buffer
					if (buffer[i] == '\n') {
						buffer[i] == '\0';
					}
				}
				if (strcmp(buffer, "@@close@@") == 0) {
					fprintf(stdout, "----------------------\n");
					fprintf(stdout, "Connection closed\n");
					break;
				} else {
					send(targetSocket, buffer, bytesRead, 0);
				}
			} else if (pollfds[1].revents & POLLIN) {
				int bytesReceived = recv(targetSocket, buffer, sizeof(buffer), 0);
				fprintf(stdout, "Server: <%s> Bytes: <%d>\n", buffer, bytesReceived);
			}
		}
	}
	close(targetSocket);
	fprintf(stdout, "Exiting...\n");
	exit(EXIT_SUCCESS);
}
