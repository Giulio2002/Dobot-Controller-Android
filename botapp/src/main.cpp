#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<ros/ros.h>
#include"botapp/setPTPCMD.h"
#define XMAX 330
#define XMIN 160
#define YMAX 235
#define YMIN -230
int main(int argc , char *argv[])
{
    ros::init(argc, argv, "Dobot_Infos");
    short int x = 250;
    short int y = 0;
    short int z = 30;
    ros::NodeHandle n;
    ros::ServiceClient chatter_pub = n.serviceClient<botapp::setPTPCMD>("setPTP");
    botapp::setPTPCMD msg;
    msg.request.x = x;
    msg.request.y = y;
    msg.request.z = z;
    chatter_pub.call(msg);
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 4040 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);
         puts("Waiting for incoming connections...");

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
	char Message = client_message[0];
    ros::ServiceClient chatter_pub = n.serviceClient<botapp::setPTPCMD>("setPTP");
    botapp::setPTPCMD msg;
    switch( Message ){
        case '1' :
            if(x >= XMAX) break;
            x += 5; 
            break;
        case '0' :            
            if(x <= XMIN) break;
            x -= 5;
            break;
        case '2' :
            if(y >= YMAX) break;
            y += 5; 
            break;
        case '3' :
            if(y <= YMIN) break;
            y -= 5;
            break;
        case 'A':
            x = 250;
            y = 0;
            z = 30;
            break;
        default:
            break;
    }
    msg.request.x = x;
    msg.request.y = y;
    msg.request.z = z;
    chatter_pub.call(msg);
	printf("%d ,%d , %d\n",x,y,z);
    //Send the message back to client
    write(client_sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}