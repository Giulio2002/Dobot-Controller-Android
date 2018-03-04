#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<ros/ros.h>
#include"botapp2/setPTPCMD.h"
#include<string>
#define XMAX 330
#define XMIN 160
#define YMAX 235
#define YMIN -230

int main(int argc , char *argv[])
{
    ros::init(argc, argv, "Dobot_Infos");
    short int x = 150;
    short int y = 50;
    short int z = 10;
    ros::NodeHandle n;
    ros::ServiceClient chatter_pub = n.serviceClient<botapp2::setPTPCMD>("setPTP");
    botapp2::setPTPCMD msg;
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
    ros::ServiceClient chatter_pub = n.serviceClient<botapp2::setPTPCMD>("setPTP");
    botapp2::setPTPCMD msg;
    std::string tmp;
    switch( Message ){
        case 'x' :
            tmp = (const char *)client_message;
            tmp.erase(tmp.begin() + 0);
            if(x + atoi(tmp.c_str()) >= XMAX || x + atoi(tmp.c_str()) <= XMIN) break;
            x += atoi(tmp.c_str()); 
            break;
        case 'y' :
            tmp = (const char *)client_message;
            tmp.erase(tmp.begin() + 0);
            if(y + atoi(tmp.c_str()) >= YMAX || y + atoi(tmp.c_str()) <= YMIN) break;
            y += atoi(tmp.c_str());
            break;
        case 'z' :
            tmp = (const char *)client_message;
            tmp.erase(tmp.begin() + 0);
            z += atoi(tmp.c_str()); 
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
	printf("%s\n",client_message);
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