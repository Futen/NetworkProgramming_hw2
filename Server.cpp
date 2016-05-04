#include "DataBaseHeader.h"

UserClass userObject;
ArticalClass articalObject;

void WaitForData(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);
void CommandProcess(int command, int sockfd, struct sockaddr *pcliaddr, socklen_t clilen, string IP, int port);

int main(int argc, char* argv[]){
    int sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    WaitForData(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    
    return 0;
}
void WaitForData(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen){
    int maxfd;
    int nready;
    int command;
    int port;
    string IP;
    struct timeval timeout;
    fd_set allset;

    userObject.Init();
    articalObject.Init();
    articalObject.ShowArtical();

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    maxfd = sockfd;
    
    FD_ZERO(&allset);
    FD_SET(sockfd, &allset);

    //articalObject.ShowArtical();

    while(true){
        nready = select(maxfd+1, &allset, NULL, NULL, NULL);
        FD_SET(sockfd, &allset);
        if(nready > 0 && FD_ISSET(sockfd, &allset)){
            int nbytes;
            char line[MAXLINE];
            string data;
            nbytes = recvfrom(sockfd, line, MAXLINE, 0, pcliaddr, &clilen);
            IP = inet_ntoa(((struct sockaddr_in*)pcliaddr)->sin_addr);
            port = ((struct sockaddr_in*)pcliaddr)->sin_port;
            if(nbytes > 0){
                data = string(line);
                //cout << data << endl;
                //sendto(sockfd, line, strlen(line)+1, 0, pcliaddr, clilen);
                command = CommandChoose(data);
                CommandProcess(command, sockfd, pcliaddr, clilen, IP, port);
            }
        }
    }
}
void CommandProcess(int command, int sockfd, struct sockaddr *pcliaddr, socklen_t clilen, string IP, int port){
    int nbytes;
    char recvline[MAXLINE];
    char success[] = "success";
    char fail[] = "fail";
    User* check;
    User* who;
    string account;
    string password;
    string nickname;
    string birthday;

    switch(command){
        case LOGIN:
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            account = recvline;
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            password = recvline;
            //cout << account << endl;
            //cout << password << endl;
            check = userObject.UserLogin(account, password, IP, port);
            if(check != NULL){
                sendto(sockfd, success, strlen(success)+1, 0, pcliaddr, clilen);
                sendto(sockfd, check->nickname.c_str(), strlen(check->nickname.c_str())+1, 0, pcliaddr, clilen);
                sendto(sockfd, check->birthday.c_str(), strlen(check->birthday.c_str())+1, 0, pcliaddr, clilen);
                cout << "Account Login:{" << endl;
                cout << "\taccount: " << check->account << endl;
                cout << "\tpassword: " << check->password << endl;
                cout << "\tnickname: " << check->nickname << endl;
                cout << "\tbirthday: " << check->birthday << endl;
                cout << "\tIP: " << check->IP << endl;
                cout << "\tport: " << check->port << endl;
                cout << "}" << endl;
            }
            else
                sendto(sockfd, fail, strlen(fail)+1, 0, pcliaddr, clilen);
            break;
        case CREATEACCOUNT:
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            account = recvline;
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            password = recvline;
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            nickname = recvline;
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            birthday = recvline;
            //cout << account << endl;
            //cout << password << endl;
            userObject.CreateUser(account, password, nickname, birthday);
            check = userObject.UserLogin(account, password, IP, port);
            if(check != NULL){
                sendto(sockfd, success, strlen(success)+1, 0, pcliaddr, clilen);
                sendto(sockfd, check->nickname.c_str(), strlen(check->nickname.c_str())+1, 0, pcliaddr, clilen);
                sendto(sockfd, check->birthday.c_str(), strlen(check->birthday.c_str())+1, 0, pcliaddr, clilen);
                userObject.SaveUserList();
                cout << "Account Login:{" << endl;
                cout << "\taccount: " << check->account << endl;
                cout << "\tpassword: " << check->password << endl;
                cout << "\tnickname: " << check->nickname << endl;
                cout << "\tbirthday: " << check->birthday << endl;
                cout << "\tIP: " << check->IP << endl;
                cout << "\tport: " << check->port << endl;
                cout << "}" << endl;
            }
            else
                sendto(sockfd, fail, strlen(fail)+1, 0, pcliaddr, clilen);
            break;
        case DELETEMYACCOUNT:
            if(userObject.DeleteUser(IP, port)){
                sendto(sockfd, success, strlen(success)+1, 0, pcliaddr, clilen);
                userObject.SaveUserList();
            }
            else
                sendto(sockfd, fail, strlen(fail)+1, 0, pcliaddr, clilen);

            break;
        case LOGOUT:
            if((check = userObject.UserLogout(IP, port)) != NULL){
                sendto(sockfd, success, strlen(success)+1, 0, pcliaddr, clilen);
                cout << "Account Logout:{" << endl;
                cout << "\taccount: " << check->account << endl;
                cout << "\tpassword: " << check->password << endl;
                cout << "\tnickname: " << check->nickname << endl;
                cout << "\tbirthday: " << check->birthday << endl;
                cout << "\tIP: " << check->IP << endl;
                cout << "\tport: " << check->port << endl;
                cout << "}" << endl;
            }
            else{
                sendto(sockfd, fail, strlen(fail)+1, 0, pcliaddr, clilen);
            }
            break;
        case MODIFYACCOUNT:
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            password = recvline;
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            nickname = recvline;
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0, pcliaddr, &clilen);
            birthday = recvline;
            if((who = userObject.UserModify(IP, port, password, nickname, birthday)) != NULL){
                userObject.SaveUserList();
                sendto(sockfd, success, strlen(success)+1, 0, pcliaddr, clilen);
                cout << "Account Modify:{" << endl;
                cout << "\taccount: " << who->account << endl;
                cout << "\tpassword: " << who->password << endl;
                cout << "\tnickname: " << who->nickname << endl;
                cout << "\tbirthday: " << who->birthday << endl;
                cout << "\tIP: " << who->IP << endl;
                cout << "\tport: " << who->port << endl;
                cout << "}" << endl;
            }
            else
                sendto(sockfd, fail, strlen(fail)+1, 0, pcliaddr, clilen);
    }
}








