//#include "Header.h"
#include "DataBaseHeader.h"

int main(){
    /*
    string a = "555";
    string b = "555ff";
    printf("%d\n",a == "555");
    */
    /*
    UserClass tmp;
    tmp.Init();
    tmp.CreateUser(string("gg"), string("dsfsdf"), string("dasad"));
    User *g = tmp.FindUser(string("account"), string("a"));
    cout<< g->nickname << endl;
    tmp.SaveUserList();
    */
    ArticalClass A,B;
    A.Init();
    /*
    int r = A.CreateArtical(string("acount1"), string("Test content1"));
    int g = A.CreateArtical(string("account2"), string("Test content2"));
    A.CreateMessage(r, string("account2"), string("test message1"));
    A.CreateMessage(r, string("account3"), string("test message2"));
    A.CreateMessage(g, string("account3"), string("test message3"));
    */
    A.ShowArtical();
    A.SaveArtical();
    return 0;
}
