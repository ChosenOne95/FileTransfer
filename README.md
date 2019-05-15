# FileTransfer
A simple program for transfering file in linux<br>
Compile server with：
```
g++ *.cpp -o server -Iinclude -lpthread -lmysqlpp
```
Compile client with:
```
g++ *.cpp -o client -Iinclude -lpthread
```
The database is mysql8.5 on ubuntu desktop 18.04<br>
You also need to change the serverIP in myclient.cpp
