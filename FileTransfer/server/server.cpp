#include <iostream>
#include "sql.h"
#include "TCPServer.h"
#include <vector>

using namespace std;

TCPServer tcp;
MyDB DB = MyDB("FT","localhost","root","20141153");


 
// void * loop(void * m)
// {
//     if(!DB.ping())
//     {
//         DB.ConnectDB();
//     }
    
//     pthread_detach(pthread_self());//auto release the memeory
	
//     while(1)
// 	{
		
// 		string str = tcp.getMessage();
//         string result = DB.OperateDB(str);
// 		if( str != "" )
// 		{
// 			cout << "Message:" << str << endl;
//             cout << "Result:" << result << endl;
// 			tcp.Send(result);
// 			tcp.clean();
// 		}
// 		//usleep(1000);
// 	}
// 	tcp.detach();
// }

int main()
{
    DB.ConnectDB();

    pthread_t msg;
	tcp.setup(8889);
	// if( pthread_create(&msg, NULL, loop, (void *)0) == 0)
	// {
	tcp.receive();
	// }
    return 0;    
}