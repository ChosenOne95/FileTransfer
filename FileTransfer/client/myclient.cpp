#include "myclient.h"
#include <sstream>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <fstream>

#define BUF_SIZE 1024

using namespace std;



static int getIP(string net_name, string &strIP)
{
    int sock_fd;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUF_SIZE] = {0};
    int num;
    int i;
 
    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if ( sock_fd < 0 )     
		return -1;
 
    conf.ifc_len = BUF_SIZE;
    conf.ifc_buf = buff;
 
    if ( ioctl(sock_fd, SIOCGIFCONF, &conf) < 0 )
    {
        close(sock_fd);
        return -1;
    }
 
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;
 
    for(i = 0; i < num; i++)
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
 
        if ( ioctl(sock_fd, SIOCGIFFLAGS, ifr) < 0 )
        {
                close(sock_fd);
                return -1;
        }
 
        if ( (ifr->ifr_flags & IFF_UP) && strcmp(net_name.c_str(),ifr->ifr_name) == 0 )
        {
                strIP = inet_ntoa(sin->sin_addr);
                close(sock_fd);
 
                return 0;
        }
		
        ifr++;
    }
	
    close(sock_fd);
 
    return -1;
}


myClient::myClient(){
    id = -1;
    username = "";
    password = "";
    client = TCPClient();
}

int myClient::Connect()
{
    int flag = -1;
    if(client.setup("192.168.43.163",8889)){
        cout<<"connect server success!"<<endl;
        flag = 0;   
    }else{
        cout<<"connect server failed, try again!"<<endl;
        flag = -1;
    }
    return flag;
}

int myClient::LogIn()
{
    string cmd;
    string result;
    cout<<"Enter you username: ";
    cin>>username;
    cout<<"Enter your password: ";
    cin>>password;
    cmd = "select id from user where username = \"" + username + "\" and password = \"" + password+ "\"";
    
    client.Send(cmd);
    result =client.receive(1024).c_str();
    cout<<"your id: "<<result<<endl;
    id = atoi(result.c_str());
    if(id>0){
         cout<<"Log in success!"<<endl;
    }else{
        cout<<"Log in error!"<<endl;
    }
    
    stringstream sstr;
    sstr<<id;
    cmd = "update user set online = 1 where id = " + sstr.str();
    client.Send(cmd);
    result =client.receive(1024).c_str();
    
    string Ip;
    getIP("ens33",Ip);
    cmd = "update user set ip = \"" + Ip + "\" where id = " + sstr.str();
    sstr.str("");
    sstr.clear();
    client.Send(cmd);
    result =client.receive(1024).c_str();
    return 0;
}

int myClient::SignUp()
{
    string cmd;
    string result;
    cout<<"Enter you username: ";
    cin>>username;
    cout<<"Enter your password: ";
    cin>>password;
    cmd = "insert into user (username, password) VALUE(\"" + username + "\", \"" + password+ "\")";
    client.Send(cmd);
    result = client.receive(1024);
    cout<<result<<endl;
    return 0;
}

int myClient::AddFriend()
{
    string cmd;
    string result;
    string friendID;
    stringstream sstr;
    cout<<"Enter the id of the firend you want to add: ";
    cin>>friendID;
    sstr<<id;
    cmd = "insert into friend (uid, fid) VALUE(" + sstr.str() + ", " + friendID+ ")";
    sstr.str("");
    sstr.clear();
    client.Send(cmd);

    result = client.receive(1024);
    cout<<result<<endl;
    return 0;
}

int myClient::ShowOnlineFriends()
{
    string cmd;
    string result;
    stringstream sstr;
    sstr<<id;
    //cmd = "select fid from friend where uid = " + sstr.str();
    cmd = "select id from user where online = 1 and id in (select fid from friend where uid =" + sstr.str() +")";
    sstr.str("");
    sstr.clear();
    
    client.Send(cmd);
    result = client.receive(1024);
    cout<<result<<endl;
    return 0;
}

int myClient::SendFile()
{
    string cmd;
    string result;
    vector <string> targetFriend;
    int num;
    string temp;
    char buffer[1024];
    cout<<"ipunt the file path:"<<endl;
    string filename;
    cin>>filename;
    cout<<"enter how many friend you want to send: ";
    cin>>num;
    cout<<"enter your online friend id: ";
    for(int i = 0; i < num; i++)
    {
        cin>>temp;
        targetFriend.push_back(temp);
    }
    vector<string>::iterator ite = targetFriend.begin();
    for(ite; ite != targetFriend.end(); ite++)
    {
        TCPClient newClient;
        cmd = "select ip from user where id = " + *ite;
        client.Send(cmd);
        result = client.receive(1024);
        if(newClient.setup(result,9999)){
            cout<<"connect server success!"<<endl;
        }else{
            cout<<"connect server failed, try again!"<<endl;
        }
        ifstream in(filename,ios::in);
        while (!in.eof() )  
        {  
            in.getline(buffer,1024);  
            for(int i=0; i<1024; i++)
            {
                if(buffer[i] >= 'A' && buffer[i] <= 'Z')
                {
                    buffer[i] = ((buffer[i]-'A')+5)%26+'A';
                }
                else if(buffer[i] >= 'a' && buffer[i] <= 'z')
                {
                    buffer[i] = ((buffer[i]-'a')+5)%26+'a';
                }

            }
            newClient.Send(buffer); 
        } 
        newClient.exit();
    }

    return 0;
}

int myClient::DisConnect()
{
    string cmd;
    string result;
    stringstream sstr;
    sstr<<id;
    cmd = "update user set online = 0 where id = " + sstr.str();
    sstr.str("");
    sstr.clear();
    
    client.Send(cmd);
    result = client.receive(1024);
    cout<<result<<endl;
    client.exit();
    return 0;   
}