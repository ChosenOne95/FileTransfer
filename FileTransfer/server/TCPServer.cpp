#include "TCPServer.h" 
#include "sql.h"
string TCPServer::Message;

// void* KeepAlive(void *args)  
// { 
// 	pthread_detach(pthread_self());
// 	int alive = 1;  
// 	if (setsockopt
// 		(socket, SOL_SOCKET, SO_KEEPALIVE, (const char *) &alive, sizeof alive) != 0)  
//   	{  
//     //log_warn ("Set keep alive error: %s.\n", strerror (errno));  
//     return 0;  
//   	} 
// 	return 0;  
//     // int ret = 0;  
//     // int keep_alive = 1;    
//     // ret = setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keep_alive, sizeof(keep_alive));    
  
//     // if (ret == SOCKET_ERROR)    
//     // {  
//     //     printf("setsockopt failed: %d \n", WSAGetLastError());  
// 	// 	DB.OperateDB("update user set online = 0 where ip = " + id);
//     //     return -1;    
//     // } 
  
//     // struct tcp_keepalive in_keep_alive = {0};  
//     // unsigned long ul_in_len = sizeof(struct tcp_keepalive);  
//     // struct tcp_keepalive out_keep_alive = {0};  
//     // unsigned long ul_out_len = sizeof(struct tcp_keepalive);  
//     // unsigned long ul_bytes_return = 0;  
  
//     // in_keep_alive.onoff = 1;                    /*打开keepalive*/  
//     // in_keep_alive.keepaliveinterval = 5000; /*发送keepalive心跳时间间隔-单位为毫秒*/  
//     // in_keep_alive.keepalivetime = 1000;         /*多长时间没有报文开始发送keepalive心跳包-单位为毫秒*/  
  
//     // ret = WSAIoctl(socket, SIO_KEEPALIVE_VALS, (LPVOID)&in_keep_alive, ul_in_len,   
//     //                       (LPVOID)&out_keep_alive, ul_out_len, &ul_bytes_return, NULL, NULL);  
  
//     // if (ret == SOCKET_ERROR)    
//     // {    
//     //     printf("WSAIoctl failed: %d \n", WSAGetLastError());    
// 	// 	DB.OperateDB("update user set online = 0 where ip = " + id);
//     //     return -1;    
//     // }   
// } 

void* TCPServer::Task(void *arg)
{
	int n;
	int newsockfd = (long)arg;
	char msg[MAXPACKETSIZE];
	struct sockaddr_in sa;
	socklen_t len = sizeof(sa);
	string ip ;
	//pthread_create(&taskThread,NULL,&KeepAlive,(void *)newsockfd);
	//pthread_detach(pthread_this());
	int alive = 1;  
	if (setsockopt
		(newsockfd, SOL_SOCKET, SO_KEEPALIVE, (const char *) &alive, sizeof(alive)) != 0)  
  	{
		cout<<"keep alive error!"<<endl;
  	}   
	pthread_detach(pthread_self());

	if(!getpeername(newsockfd, (struct sockaddr *)&sa, &len))
	{
		ip = inet_ntoa(sa.sin_addr);
	}
	string cmd;
	while(1)
	{
		n=recv(newsockfd,msg,MAXPACKETSIZE,0);
		if(n==0)//network interrupt
		{
			cout<<"close a sock: "<<newsockfd<<endl;
			close(newsockfd);
			cmd = "update user set online = 0 where ip = " + ip;
			DB.OperateDB(cmd);
			break;
		}
		msg[n]=0;
		//send(newsockfd,msg,n,0);
		Message = string(msg);
		if(Message != "")
		{
			cout<<"Message: "<<Message<<endl;
			string result = DB.OperateDB(Message);
			cout << "Result: " << result << endl;
			//tcp.Send(result);
			send(newsockfd,result.c_str(),1024,0);
			tcp.clean();
			Message = "";
		}
  	}
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

string TCPServer::receive()
{
	string str;
	while(1)
	{
		socklen_t sosize  = sizeof(clientAddress);
		newsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
		str = inet_ntoa(clientAddress.sin_addr);
		cout<<str<<endl;
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