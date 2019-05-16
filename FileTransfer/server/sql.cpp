#include "sql.h"

MyDB::MyDB(string dbName, string dbAddr, string dbUser, string dbPassword)
{
    this->database = dbName;
    this->addr = dbAddr;
    this->username = dbUser;
    this->password = dbPassword;

}

int MyDB::ConnectDB()
{
    mysqlpp::Connection con(false);
    con.set_option(new mysqlpp::SetCharsetNameOption("utf8"));
    if(!con.connect(this->database.c_str(), this->addr.c_str(), this->username.c_str(), this->password.c_str()))
    {
        cout<<"can't connect,check the user and passwd"<<endl;
        return -1;
    }
    cout<<"mysql connect successfully!"<<endl;
    this->mycon = con;
    return 0;
}

int MyDB::ping()
{
    return this->mycon.ping();
}

string MyDB::OperateDB (string cmd)
{   string result;
    stringstream sstr;
    mysqlpp::Query query = mycon.query(cmd);
    mysqlpp::StoreQueryResult res = query.store();
    if(res)
    {
        for(size_t i = 0; i < res.num_rows(); i++)
        {
            //if "id" in res[i].fieldlist();
            sstr<<res[i].front();
            result = result + sstr.str() + " ";
            sstr.str("");
            sstr.clear();
        }
    }
    
    return result;
}

