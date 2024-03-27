#include <arpa/inet.h>
#include <errno.h> //additional libraries
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1" //loopback address since we are not using VM
#define PORT 8091 //PORT 8091
#define BUFFER_SIZE 1024 //buffersize for the message

int main() {
    int client_fd, max_fd; //store file descriptor, this is used for socket I/O, we also need the max value for file descriptor 
    struct sockaddr_in server_addr; ///this is the socket address data structure storing server address. 
    char buffer[BUFFER_SIZE]; //this is using th e buffer size declared in VARCHAR
    fd_set read_fds; //setting filedescriptor that we will use for select()


    client_fd = socket(AF_INET, SOCK_STREAM, 0);  //socket function that takes in 3 arguments, socket IPV4 protocol, socket stream and its protocol number set to 0 
    if (client_fd == -1) { //this is just standard error checking because it cannot be negative number
        perror("socket"); //perror is used for errors in C library
        exit(EXIT_FAILURE); //exit
    }

    //this is setting the flag to non blocking I/O operation, so server and client can communicate through real time 
    int flags = fcntl(client_fd, F_GETFL, 0); //setting flags for non blocking I/O 
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK); //this is setting the client filedescriptor for non block using previous flags

    server_addr.sin_family = AF_INET; //specifies what type of IP version protocol we are looking at 
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); //this calls the constance of serverIP being an argument
    server_addr.sin_port = htons(PORT); //while specifying the port

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) { //connect() will initiate a connection to the server, while taking in 3 arguments, file descriptor pointing to the socket with the sizeof() for calculation
        if (errno == EINPROGRESS) { //it is checking now if errno is the same as EINPROGRESS, it is a way to utilise errno for checking connection in progress
             FD_ZERO(&read_fds); 
             FD_SET(client_fd,&read_fds); 
        if (select(client_fd + 1,NULL,&read_fds,NULL,NULL) == -1) { //this is a form of blocking to check for error again, most of perror() is just error checking
             perror("select");  
             exit(EXIT_FAILURE); //exit
    } 
    } else { //this part checks while connection error if in progress
        perror("connect");  
        exit(EXIT_FAILURE);
     }
    }

     while (1) {  //this part is the actual sending 
        FD_ZERO(&read_fds); //if the connection is complete, now we can initiate a macro(conversation) through read and write
        FD_SET(STDIN_FILENO,&read_fds);  //standard input stream andincoming data 
        FD_SET(client_fd,&read_fds);  //this part is interesting, at the beginning of every iteration in the while loop, we are able to macro fd_set variable 

        max_fd = (STDIN_FILENO > client_fd) ? STDIN_FILENO : client_fd; //this max_fd is calculated as the maximum value between the file and client descriptor 
        //why? because this is how select() operates as it requires maximum value of the file descriptor 

        if (select(max_fd + 1,&read_fds,NULL,NULL,NULL) == -1) { //this part will let the program block only until there is data available as an incoming stream
                perror("select");
                exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO,&read_fds)) {   //check if the file descriptor is part of fd_set
                fgets(buffer,BUFFER_SIZE,stdin); //fgets is used to store the messages in the buffer variable
                send(client_fd,buffer,strlen(buffer),0); //send it directly via client to server using send() 
        }

        if (FD_ISSET(client_fd,&read_fds)) { //check again
                memset(buffer,0,BUFFER_SIZE); //all variables in the buffer is set to 0, this is reading 
                ssize_t bytes_read = read(client_fd,buffer,BUFFER_SIZE); //this will be reading the buffer size after it is set to 0, making sure that it is only message has data 
        if (bytes_read == 0) { //if there is no data, there is obviously no connection
            printf("Server disconnected.\n"); //disconnect it 
            break; //break out of the while loop
        } else {
         printf("Server: %s",buffer); //otherwise, just normal conversation 
        }
    }
 }

    close(client_fd); //closes the client connection

    return 0; //return statement for the main program
}



//reference: https://stackoverflow.com/questions/10219340/using-stdin-with-select-in-c
//reference2: https://stackoverflow.com/questions/26456306/c-trying-to-understand-select-and-fd-isset

