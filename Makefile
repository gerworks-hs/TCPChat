all:
	gcc -o client ./src/client.c
	gcc -o server ./src/server.c
	echo "Finished compiling both client and server"
client:
	gcc -o client ./src/client.c
	echo "Finished compiling client"
server:
	gcc -o server ./src/server.c
	echo "Finished compiling server"
clean:
	rm -vf server
	rm -vf client
	echo "Both executables has been deleted"
