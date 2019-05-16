#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_
#include "TCPClient.h"

class myClient
{
    public:
    int id;
    string username;
    string password;
    TCPClient client;

    myClient();
    int Connect();
    int SignUp();
    int LogIn();
    int AddFriend();
    int ShowFriends();
    int ShowOnlineFriends();
    int SendFile();
    int DisConnect();
};

#endif