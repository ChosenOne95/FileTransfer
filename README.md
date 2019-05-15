# FileTransfer
a simple program for transfering file in linux
compile server with：
```
g++ *.cpp -o server -Iinclude -lpthread -lmysqlpp
```
compile client with:
```
g++ *.cpp -o client -Iinclude -lpthread
```
the database is mysql8.5 on ubuntu desktop 18.04
you also need change the serverIP in myclient.cpp
