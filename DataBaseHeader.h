#ifndef DataBase
#define DataBase
#include "Header.h"
struct Message{
    int index;
    string who;
    string message;
    Message* previous;
    Message* next;
};
struct Artical{
    int index;
    int message_count;
    string author;
    string artical;
    Message* first_message;
    Message* last_message;
    Artical* next;
    Artical* previous;
};
struct User{
    int index;
    int artical_index;
    string account;
    string nickname;
    string password;
    string birthday;
    string IP;
    int port;
    int islogin;
    User* next;
    User* previous;
    Artical* first_artical;
    Artical* last_artical;
};
class UserClass;
class ArticalClass;

class ArticalClass{
private:
    int artical_index;
    Artical* first_artical;
    Artical* last_artical;
protected:
    Artical* FindArticalFromIndex(int index);
    Message* FindMessageFromIndex(int art_index, int mess_index);
public:
    //int artical_index;
    void Init();
    void SaveArtical();
    int CreateArtical(string account, string artical);
    void ShowArtical();
    Message* CreateMessage(int parent_artical_index, string account, string message);
    Message* ModifyMessage(int art_index, int mess_index, string message);
    bool DeleteMessage(int art_index, int mess_index);
    bool DeleteArtical(int art_index);
};

class UserClass{
private:
    int user_index;
    User* first_user;
    User* last_user;
protected:
    Artical* CreateArtical(User* user, string artical);
    Message* CreateMessage(Artical* artical, string who, string message);
    Artical* FindArticalFromIndex(string account, int artical_index);
    Message* FindMessageFromIndex(string account, int artical_index, int message_index);
    User* FindUserFromIPAndPort(string IP, int port);
    Artical* DeleteArticalMessage(Artical* artical);
public:
    /****************/
    void Init();
    void SaveUserList();
    User* CreateUser(string account, string password, string nickname, string birthday);
    User* FindUser(string whatkind, string info);
    User* UserLogin(string account, string password, string IP, int port);
    User* UserLogout(string IP, int port);
    User* UserModify(string IP, int port, string password, string nickname, string birthday);
    bool DeleteUser(string IP, int port);
    /***************/
    void ArticalInit();
    void ShowArtical(User* user);
    void SaveArtical();
    Artical* NewUserArtical(string IP, int port, string artical);
    Message* NewUserMessage(string IP, int port, string author_account, int artical_index, string message);
    bool DeleteUserArtical(string IP, int port, int artical_index);
    bool DeleteUserMessage(string IP, int port, string author_account, int artical_index, int message_index);
    string ShowUserArtical(string IP, int port);
    Artical* ModifyArtical(string IP, int port, int artical_index, string artical);
    Message* ModifyMessage(string IP, int port, int artical_index, int message_index, string message);
    /***************/
};
#endif
