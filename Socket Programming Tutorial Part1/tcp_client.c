#include <stdio.h>
#include <stdlib.h>

/* headers for socket programming */
#include <sys/types.h>
#include <sys/socket.h>

/* for the network*/
#include <netinet/in.h>

//This is the  client side of the socket
int main(){
	//create a socket
	int network_socket;
	//we will use TCP and default protocol
	network_socket = socket(AF_INET, SOCK_STREAM, 0);

	//connect to another socket
	//to get the address we use netinet header file
	
	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	//specify the port you want to connect remotely
	server_address.sin_port = htons(9002);
	//real server address we are connecting to
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	
	//check for error with the connection
	if(connection_status==1){
		printf("There's something wrong with the connection of the socket\n");
	}
	
	//receive data from the server
	char server_response[256];
	recv(network_socket, &server_response, sizeof(server_response), 0);

	//print out the server's response
	printf("The server sent the data :%s\n", server_response);

	//and then we close the socket
	close(network_socket);

	return 0;
}
