#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>


#define PORT 8091 //server port 
#define BUFFER_SIZE 1024 //buffer size
  
int main() {
    int server_fd, client_fd = -1, max_fd, addr_len; 
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    fd_set read_fds;

    server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd == -1) { //checking if the file descriptor is -1 which is impossible
        perror("socket"); 
        exit(EXIT_FAILURE); //exit when there is an error for server descriptor 
    }

    //this is setting the flag to non blocking I/O operation, so server and client can communicate through real time 
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    server_addr.sin_family = AF_INET; //specifies what type of IP version protocol we are looking at 
    server_addr.sin_addr.s_addr = INADDR_ANY; //this calls the constance of serverIP being an argument
    server_addr.sin_port = htons(PORT); //while specifying the port

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) { //on client side is connect, server side is binding
        perror("bind"); //this is another check for errors
        exit(EXIT_FAILURE); //alternative exit
    }

    if (listen(server_fd, 1) == -1) { //this is checking for listening errors 
        perror("listen");
        exit(EXIT_FAILURE); //alternative exit
    }

    while (1) {
        if (client_fd == -1) { //checking for connection of client file descriptor 
            addr_len = sizeof(client_addr); //address length is set to the size of client address
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len); //accepts the connection 
            if (client_fd == -1) {  //if the client_fd is -1 
                if (errno == EAGAIN || errno == EWOULDBLOCK) { //EAGAIN and EWOULDBLOCK simply checks for blocking mode, although it is sort of the same 
                    continue; //continue 
                } else {
                    perror("accept"); //otherwise accept 
                    exit(EXIT_FAILURE); //exit
                }
            }
            //this is again setting it to non blocking mode, since we are dealing with an error for blocking 
            flags = fcntl(client_fd, F_GETFL, 0); 
            fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
        }

        FD_ZERO(&read_fds); //if the connection is complete, now we can initiate macro through read
        FD_SET(STDIN_FILENO,&read_fds); //point that the read_filedescriptor and set it for standard stream 
        FD_SET(client_fd,&read_fds);  //set this to client filedescriptor and pointer to read filedescriptor 

        max_fd = (STDIN_FILENO > client_fd) ? STDIN_FILENO : client_fd;//this max_fd is calculated as the maximum value between the file and client descriptor 
        //why? because this is how select() operates as it requires maximum value of the file descriptor 

        if (select(max_fd + 1,&read_fds,NULL,NULL,NULL) == -1) { //this is for incoming data error check, as it uses select clause
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO,&read_fds)) { 
            fgets(buffer,BUFFER_SIZE,stdin);  //read data from standard input
            send(client_fd,buffer,strlen(buffer),0); //send the data to the client side
        }

        if (FD_ISSET(client_fd,&read_fds)) { //reading client chat
            memset(buffer,0,BUFFER_SIZE); //setting memory to 0, so buffer can adequately store client messages
            ssize_t bytes_read = read(client_fd,buffer,BUFFER_SIZE); //read client messages from the buffer
            if (bytes_read <= 0) { //this will close the client side if the bytes_read is less or equals to 0 
                close(client_fd); //close 
                client_fd = -1; //set numeric value to -1, which when it comes to reading bytes is impossible
                continue; //simply continue 
            }
            printf("Client: %s",buffer); //otherwise print out Client chat 
        }
    
    }

    close(client_fd); //this will close both connections, since it is closing both the client side
    close(server_fd); //and server side

    return 0;
}
