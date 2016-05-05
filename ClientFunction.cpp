#include "ClientHeader.h"

string user_account;
string user_password;
string user_nickname;
string user_birthday;

void ConnectProcess(int sockfd, struct sockaddr *server_addr, socklen_t servlen){
    int nbytes;
    int shutdown;
    int nready;
    int maxfd;
    int islogin;
    int fileno_stdin;
    int command;
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    string sendData;
    string recvData;
    struct timeval timeout;
    fd_set allset;
    
    islogin = 0;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    fileno_stdin = fileno(stdin);
    FD_ZERO(&allset);
    FD_SET(sockfd, &allset);
    FD_SET(fileno_stdin, &allset);
    maxfd = sockfd > fileno_stdin?sockfd:fileno_stdin;
    
    shutdown = 0;
    while(!islogin){
        islogin = Login(sockfd, server_addr, servlen);
    }
    cout << "Welcome !!!!!" << endl;
    ShowCommand();
    while(shutdown == 0){
        nready = select(maxfd+1, &allset, NULL, NULL, &timeout);
        FD_SET(sockfd, &allset);
        FD_SET(fileno_stdin, &allset);
        if(nready > 0){
            if(FD_ISSET(fileno_stdin, &allset)){
                fgets(sendline, MAXLINE, stdin);
                sendline[strlen(sendline)-1] = '\0';
                sendData = GetCommandString(string(sendline));
                /*
                //cout << senddata << endl;
                sendto(sockfd, sendline, strlen(sendline)+1, 0, server_addr, servlen);
                //recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
                //recvdata = recvline;
                //cout << recvdata << endl;
                */
                command = CommandChoose(sendData);
                ProcessCommand(command, sendData, sockfd, server_addr, servlen);
            }
            ShowCommand();
        }
    }
}
int Login(int sockfd, struct sockaddr *server_addr, socklen_t servlen){
    char recvline[MAXLINE];
    string login_mode = "LOGIN";
    string create_mode = "CREATEACCOUNT";
    string recvData;
    string command;
    cout << "$ Create?" << endl;
    cout << "$ Login?" << endl;
    cin >> command;
    if(command != "Login" && command != "Create")
        return 0;
    cout << "Account: ";
    cin >> user_account;
    cout << "Password: ";
    cin >> user_password;
    if(command == "Login")
        sendto(sockfd, login_mode.c_str(), strlen(login_mode.c_str())+1, 0, server_addr, servlen);
    else if(command == "Create"){
        cout << "Nickname: ";
        cin >> user_nickname;
        cout << "Birthday(yy/mm/dd): ";
        cin >> user_birthday;
        sendto(sockfd, create_mode.c_str(), strlen(create_mode.c_str())+1, 0, server_addr, servlen);
    }
    else
        return 0;
    sendto(sockfd, user_account.c_str(), strlen(user_account.c_str())+1, 0, server_addr, servlen);
    sendto(sockfd, user_password.c_str(), strlen(user_password.c_str())+1, 0, server_addr, servlen);
    if(command == "Create"){
        sendto(sockfd, user_nickname.c_str(), strlen(user_nickname.c_str())+1, 0, server_addr, servlen);
        sendto(sockfd, user_birthday.c_str(), strlen(user_birthday.c_str())+1, 0, server_addr, servlen);
    }
    recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
    recvData = recvline;
    if(recvData == "success"){
        recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
        user_nickname = recvline;
        recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
        user_birthday = recvline;
        cout << "Login Success " << user_nickname  << "!!" << endl;
        return 1;
    }
    else{
        cout << "Error account or password" << endl;
        return 0;
    }
}
void ShowCommand(){
    cout << "***************" << endl;
    cout << "$ ShowMyInfo" << endl;
    cout << "$ NewArtical" << endl;
    cout << "$ ShowMyArtical" << endl;
    cout << "$ NewMessage" << endl;
    cout << "$ DeleteMyAccount" << endl;
    cout << "$ ModifyMyAccount" << endl;
    cout << "$ Logout" << endl;
    cout << "***************" << endl;
}
string GetCommandString(string input){
    if(input == "ShowMyInfo")   return string("SHOWMYINFO");
    else if(input == "DeleteMyAccount") return string("DELETEMYACCOUNT");
    else if(input == "Logout")  return string("LOGOUT");
    else if(input == "ModifyMyAccount") return string("MODIFYACCOUNT");
    else if(input == "NewArtical") return string("NEWARTICAL");
    else if(input == "ShowMyArtical") return string("SHOWUSERARTICAL");
    else if(input == "NewMessage") return string("NEWMESSAGE");
    else    return string("UNKNOWN");
}
void ProcessCommand(int command, string sendData, int sockfd, struct sockaddr *server_addr, socklen_t servlen){
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    string buf;
    string recvData;
    string success = "success";
    string fail = "fail";

    switch(command){
        case SHOWMYINFO:
            cout << "My Account:{" << endl;
            cout << "\taccount: " << user_account << endl;
            cout << "\tpassword: " << user_password << endl;
            cout << "\tnickname: " << user_nickname << endl;
            cout << "\tbirthday: " << user_birthday << endl;
            cout << "}" << endl;
            break;
        case DELETEMYACCOUNT:
            sendto(sockfd, sendData.c_str(), strlen(sendData.c_str())+1, 0, server_addr, servlen);
            recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
            recvData = recvline;
            if(recvData == success){
                cout << "Account Delete Success!!" << endl;
            }
            else{
                cout << "Account Delete Fail!!" << endl;
            }
            exit(0);
            break;
        case LOGOUT:
            sendto(sockfd, sendData.c_str(), strlen(sendData.c_str())+1, 0, server_addr, servlen);
            recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
            recvData = recvline;
            if(recvData == success) cout << "Account Logout Success!!" << endl;
            else    cout << "Account Logout Fail!!" << endl;
            exit(0);
            break;
        case MODIFYACCOUNT:
            sendto(sockfd, sendData.c_str(), strlen(sendData.c_str())+1, 0, server_addr, servlen);
            cout << "New Password: ";
            cin >> user_password;
            cout << "New Nickname: ";
            cin >> user_nickname;
            cout << "New Birthday: ";
            cin >> user_birthday;
            sendto(sockfd, user_password.c_str(), strlen(user_password.c_str())+1, 0, server_addr, servlen);
            sendto(sockfd, user_nickname.c_str(), strlen(user_nickname.c_str())+1, 0, server_addr, servlen);
            sendto(sockfd, user_birthday.c_str(), strlen(user_birthday.c_str())+1, 0, server_addr, servlen);
            recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
            recvData = recvline;
            if(recvData == success) cout << "Account Modify Success!!" << endl;
            else    cout << "Account Modify Fail!!" << endl;
            break;
        case NEWARTICAL:
            sendto(sockfd, sendData.c_str(), strlen(sendData.c_str())+1, 0, server_addr, servlen);
            cout << "New Artical:" << endl;
            fgets(sendline, MAXLINE, stdin);
            sendline[strlen(sendline)-1] = '\0';
            sendto(sockfd, sendline, strlen(sendline)+1, 0, server_addr, servlen);
            recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
            recvData = recvline;
            if(recvData == success) cout << "Create Artical Success!!" << endl;
            else    cout << "Create Artical Fail!!" << endl;
            break;
        case SHOWUSERARTICAL:
            sendto(sockfd, sendData.c_str(), strlen(sendData.c_str())+1, 0, server_addr, servlen);
            recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
            recvData = recvline;
            if(recvData == success){
                recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
                recvData = recvline;
                cout << recvData;
            }
            else
                cout << "Show My Artical Fail!!" << endl;
            break;
        case NEWMESSAGE:
            sendto(sockfd, sendData.c_str(), strlen(sendData.c_str())+1, 0, server_addr, servlen);
            cout << "The Artical Author: ";
            getline(cin, buf);
            sendto(sockfd, buf.c_str(), strlen(buf.c_str())+1, 0, server_addr, servlen);
            cout << "The Artical Index: ";
            getline(cin, buf);
            sendto(sockfd, buf.c_str(), strlen(buf.c_str())+1, 0, server_addr, servlen);
            cout << "The Message: " << endl;
            getline(cin, buf);
            sendto(sockfd, buf.c_str(), strlen(buf.c_str())+1, 0, server_addr, servlen);
            recvfrom(sockfd, recvline, MAXLINE, 0, server_addr, &servlen);
            recvData = recvline;
            if(recvData == success) cout << "Create Message Success!!" << endl;
            else    cout << "Create Message Fail!!" << endl;
            break;
        default:
            cout << "Unknown command" << endl;
    }
}









