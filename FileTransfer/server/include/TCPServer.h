#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "sql.h"



using namespace std;

#define MAXPACKETSIZE 1024

class TCPServer
{
	public:
	int sockfd, newsockfd, n, pid;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	pthread_t serverThread;
	char msg[ MAXPACKETSIZE ];
	static string Message;

	void setup(int port);
	string receive();
	string getMessage();
	void Send(string msg);
	void detach();
	void clean();

	private:
	static void * Task(void * argv);
};


extern MyDB DB;
extern TCPServer tcp;
#endif