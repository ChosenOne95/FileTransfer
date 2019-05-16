#include <iostream>
#include "TCPClient.h"
#include "myclient.h"
#include "TCPServer.h"
using namespace std;

// int EncryptFile()
// {
//     Aes Encryption;
//     Encryption.inputKey();
//     Encryption.setFilePath();
//     Encryption.initAes();
//     if(Encryption.encode() == 123){
//         cout<<"file open fail, please input file path again!\n";
//     }else{//加密成功
//         cout<<"Encode success!\n";
//     }
//     return 0;
// }

void * loop(void * m)
{
    tcp.fileReceive();
}

int main ()
{
    pthread_t scoketlis;
    tcp.setup(9999);
    pthread_create(&scoketlis, NULL, loop, (void *)0);
    myClient client;
    int a = -1;
    while(true)
    {
        cout<<"------------------------------"<<endl;
        cout<<"0. Connecet server."<<endl;
        cout<<"1. Sign up."<<endl;
        cout<<"2. Log in."<<endl;
        cout<<"3. Add a friend."<<endl;
        cout<<"4. Show my friend."<<endl;
        cout<<"5. Send file to friend(s)."<<endl;
        cout<<"6. Quit."<<endl;
        cout<<"------------------------------"<<endl;

        cout<<"Enter the number of the options:";
        cin>>a;

        switch(a)
        {
            case 0:
                client.Connect();
                break;
            case 1:
                client.SignUp();
                break;
            case 2:
                client.LogIn();
                break;
            case 3:
                client.AddFriend();
                break;
            case 4:
                client.ShowOnlineFriends();
                break;
            case 5:
                // EncryptFile();
                client.SendFile();
                break;
            default:
                client.DisConnect();
                return 0;
        }
    }
}