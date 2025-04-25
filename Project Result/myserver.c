#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for string response
#include <unistd.h> //for POSIX close() call
#include <signal.h> //for signal handling - signal(SIGPIPE, SIG_IGN); in main function

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

//recognize the extension of requested file and return the content type
char *get_content_type(char *file_name){
	if (strstr(file_name, ".html")) return "text/html";
    if (strstr(file_name, ".jpg") || strstr(file_name, ".jpeg")) return "image/jpeg";
    if (strstr(file_name, ".png")) return "image/png";
    if (strstr(file_name, ".gif")) return "image/gif";
    if (strstr(file_name, ".pdf")) return "application/pdf";
    if (strstr(file_name, ".mp3")) return "audio/mpeg";
    return "application/octet-stream";
}

int is_binary_file(char *file_name) {
    return (strstr(file_name, ".jpg") || strstr(file_name, ".jpeg") || strstr(file_name, ".png") ||
            strstr(file_name, ".gif") || strstr(file_name, ".pdf") || strstr(file_name, ".mp3"));
}

int main(int argc, char *argv[]){
	signal(SIGPIPE, SIG_IGN);

	//create a socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//define the address;
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	//customize the port number
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	listen(server_socket, 5);

	int client_socket;

	//server continue to serve in response to request
	while(1){
		client_socket = accept(server_socket,NULL,NULL);
		//receives the request data from the client
		char buffer[4096];
		ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
		if(bytes_received<0){
			close(client_socket);
			continue;
		}
		
		//Part A - Print the HTTP reqeust sent by the client
		//printf("%s",buffer); - Problem! binary files written as broken letters
		fwrite(buffer, 1, bytes_received, stdout);
		//Using fwrite instead of printf prevents broken or missing characters
		//when binary data (e.g. mp3, jpg) is included in the request

		//figuring out which file does the client want
		char *method = strtok(buffer, " "); // ex: GET POST PUT DELETE
		char *file_name = strtok(NULL, " "); // ex: "/index.html"
		if(method == NULL || file_name == NULL){
			close(client_socket);
			continue;
		}
		file_name++; // remove '/'
		//figure out the content type and how to open the file
		char *content_type = get_content_type(file_name);
		char *mode = is_binary_file(file_name) ? "rb" : "r";
		//open a file to serve
		FILE *html_data = fopen(file_name,mode);
		if (html_data == NULL) {
    		//send 404 Not Found if the file is not availale
			char not_found_response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
			send(client_socket, not_found_response, sizeof(not_found_response), 0);
			close(client_socket);
			continue;
		}
		//You need to measure the file size to add information in content-length header
		//You can use fseek() to do that.
		fseek(html_data, 0, SEEK_END);
		long file_size = ftell(html_data);
		rewind(html_data);

		//Code for DEBUG - Fixing segmentation fault when I sent socket.pdf file
		//printf("[INFO] Serving file: %s (size: %ld bytes)\n", file_name, file_size);

		char http_header[2048];
		//We want to connect response body and http header
		//You can use sprintf to manipulate String type easily
		sprintf(http_header,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %ld\r\n"
			"\r\n", content_type, file_size);
		//sending HTTP Response header
		//sizeof(http_header) returned blank screen on client side.
		//so I changed it to strlen(http_header)
		send(client_socket, http_header, strlen(http_header),0);
		//sending actual file to client
		char file_buffer[4096];
		size_t bytes_read;
		while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), html_data)) > 0) {
			size_t bytes_sent = send(client_socket, file_buffer, bytes_read, 0);
			//checking bytes_sent for error debugging
			if(bytes_sent<0){
				fprintf(stderr, "ERROR - send() failed for %s\n",file_name);
				break;
			}
		}
		//close the file for safe resource release
		fclose(html_data);
		close(client_socket);
	}
	return 0;
}
