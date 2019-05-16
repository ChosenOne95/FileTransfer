#include "TCPServer.h" 
#include <fstream>


using namespace std;
string TCPServer::Message;

TCPServer tcp;
// int DecryptFile()
// {
//     Aes Decryption;
//     Decryption.inputKey();
//     //Decryption.setFilePath();
//     Decryption.initAes();
//     if(Decryption.decode() == 123){
//         cout<<"file open fail, please input file path again!\n";
//     }else{//加密成功
//         cout<<"Decode success!\n";
//     }
//     return 0;
// }

void* TCPServer::Task(void *arg)
{
    ofstream out("rece",ios::out);
	int n;
	int newsockfd = (long)arg;
	char msg[MAXPACKETSIZE];
	//pthread_detach(pthread_this());
	pthread_detach(pthread_self());
	cout<<"Receive a file"<<endl;
	while(1)
	{
        
		n=recv(newsockfd,msg,MAXPACKETSIZE,0);
		if(n==0)
		{
		   close(newsockfd);
		   break;
		}

		for(int i=0; i<1024; i++){
			if( msg[i] >= 'A' &&  msg[i] <= 'Z')
			{
 				msg[i] = (( msg[i]-'A')+21)%26+'A';
			}
			else if( msg[i] >= 'a' &&  msg[i] <= 'z')
			{
 				msg[i] = (( msg[i]-'a')+21)%26+'a';
			}
		}
		msg[n]=0;
		Message = string(msg);
		if(Message != "")
		{
            out<<Message;
		}
    }
    out.close();
	// DecryptFile();
    return 0;
}

void TCPServer::setup(int port)
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);
 	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(port);
	bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
 	listen(sockfd,5);
}

string TCPServer::fileReceive()
{
    string str;
	while(1)
	{
		socklen_t sosize  = sizeof(clientAddress);
		newsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
		str = inet_ntoa(clientAddress.sin_addr);
		pthread_create(&serverThread,NULL,&Task,(void *)newsockfd);
	}
    return str;
}

string TCPServer::getMessage()
{
	return Message;
}

void TCPServer::Send(string msg)
{
	// send(newsockfd,msg.c_str(),msg.length(),0);
	send(newsockfd,msg.c_str(),1024,0);
}

void TCPServer::clean()
{
	Message = "";
	memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach()
{
	close(sockfd);
	close(newsockfd);
} 