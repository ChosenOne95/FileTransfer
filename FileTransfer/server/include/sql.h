#ifndef _SQL_H_
#define _SQL_H_

#include<iostream>
#include<mysql++/mysql++.h>
#include<string>
#include<vector>

using namespace std;

class MyDB{
    public:
    string database;
    string addr;
    string username;
    string password;
    mysqlpp::Connection mycon;

    MyDB(string dbName, string dbAddr, string dbUser, string dbPassword);
    int ping();
    int ConnectDB();
    string OperateDB(string cmd);
};


#endif