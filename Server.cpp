#include "DataBaseHeader.h"

UserClass userObject;
ArticalClass articalObject;

void WaitForData(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);
void CommandProcess(int command, char *line, int sockfd, struct sockaddr *pcliaddr, socklen_t clilen, string IP, int port);

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
            Packet* packet;
            nbytes = recvfrom(sockfd, line, MAXLINE, 0,pcliaddr, &clilen);
            packet = (Packet*)line;
            IP = inet_ntoa(((struct sockaddr_in*)pcliaddr)->sin_addr);
            port = ((struct sockaddr_in*)pcliaddr)->sin_port;
            if(nbytes > 0){
                data = string(packet->buf[0]);
                cout << data << endl;
                //sendto(sockfd, line, strlen(line)+1, 0, pcliaddr, clilen);
                command = CommandChoose(data);
                CommandProcess(command, line, sockfd, pcliaddr, clilen, IP, port);
            }
        }
    }
}
void CommandProcess(int command, char* line, int sockfd, struct sockaddr *pcliaddr, socklen_t clilen, string IP, int port){
    int nbytes;
    int artical_index;
    int message_index;
    char recvline[MAXLINE];
    char success[] = "success";
    char fail[] = "fail";
    User* check;
    User* who;
    Artical* artical;
    string recvData;
    string account;
    string password;
    string nickname;
    string birthday;
    Packet* packet = (Packet*)line;
    Packet* packet_tmp;

    switch(command){
        case TEST:
            nbytes = recvfrom(sockfd, recvline, MAXLINE, 0,pcliaddr, &clilen);
            packet = (Packet*)recvline;
            cout << packet->count << endl;
            cout << packet->buf[0] << endl;
            cout << packet->buf[1] << endl;
            cout << packet->buf[2] << endl;
            cout << packet->buf[3] << endl;
            break;
        case LOGIN:
            account = packet->buf[1];
            password = packet->buf[2];
            //cout << account << endl;
            //cout << password << endl;
            check = userObject.UserLogin(account, password, IP, port);
            if(check != NULL){
                packet_tmp = NewPacket(0);
                PacketPush(packet_tmp, string(success));
                PacketPush(packet_tmp, check->nickname);
                PacketPush(packet_tmp, check->birthday);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
                delete packet_tmp;
                cout << "Account Login:{" << endl;
                cout << "\taccount: " << check->account << endl;
                cout << "\tpassword: " << check->password << endl;
                cout << "\tnickname: " << check->nickname << endl;
                cout << "\tbirthday: " << check->birthday << endl;
                cout << "\tIP: " << check->IP << endl;
                cout << "\tport: " << check->port << endl;
                cout << "}" << endl;
            }
            else{
                packet_tmp = NewPacket(0);
                PacketPush(packet_tmp, string(fail));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
                delete packet_tmp;
            }
            break;
        case CREATEACCOUNT:
            account = packet->buf[1];
            password = packet->buf[2];
            nickname = packet->buf[3];
            birthday = packet->buf[4];
            //cout << account << endl;
            //cout << password << endl;
            userObject.CreateUser(account, password, nickname, birthday);
            check = userObject.UserLogin(account, password, IP, port);
            if(check != NULL){
                packet = NewPacket(0);
                PacketPush(packet, string(success));
                PacketPush(packet, check->nickname);
                PacketPush(packet, check->birthday);
                sendto(sockfd, (char*)packet, sizeof(Packet), 0, pcliaddr, clilen);
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
            else{
                packet = NewPacket(0);
                PacketPush(packet, string(fail));
                sendto(sockfd, fail, strlen(fail)+1, 0, pcliaddr, clilen);
            }
            delete packet;
            break;
        case DELETEMYACCOUNT:
            packet_tmp = NewPacket(0);
            if(userObject.DeleteUser(IP, port)){
                userObject.SaveArtical();
                PacketPush(packet_tmp, success);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
                userObject.SaveUserList();
            }
            else{
                PacketPush(packet_tmp, fail);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case LOGOUT:
            if((check = userObject.UserLogout(IP, port)) != NULL){
                packet_tmp = NewPacket(0);
                PacketPush(packet_tmp, success);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
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
                packet_tmp = NewPacket(0);
                PacketPush(packet_tmp, fail);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case MODIFYACCOUNT:
            password = string(packet->buf[1]);
            nickname = string(packet->buf[2]);
            birthday = string(packet->buf[3]);
            packet_tmp = NewPacket(0);
            if((who = userObject.UserModify(IP, port, password, nickname, birthday)) != NULL){
                userObject.SaveUserList();
                PacketPush(packet_tmp , success);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
                cout << "Account Modify:{" << endl;
                cout << "\taccount: " << who->account << endl;
                cout << "\tpassword: " << who->password << endl;
                cout << "\tnickname: " << who->nickname << endl;
                cout << "\tbirthday: " << who->birthday << endl;
                cout << "\tIP: " << who->IP << endl;
                cout << "\tport: " << who->port << endl;
                cout << "}" << endl;
            }
            else{
                PacketPush(packet_tmp , fail);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case NEWARTICAL:
            recvData = string(packet->artical);
            packet_tmp = NewPacket(0);
            if((artical = userObject.NewUserArtical(IP, port, recvData)) != NULL){
                userObject.SaveArtical();
                PacketPush(packet_tmp, success);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
                cout << artical->author  << " Create New Artical:{" << endl << "\t";
                cout << recvData << endl << "}" << endl;
            }
            else{
                PacketPush(packet_tmp, fail);
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case SHOWUSERARTICAL:
            packet_tmp = NewPacket(0);
            recvData = userObject.ShowUserArtical(IP, port);
            PacketPush(packet_tmp, success);
            PacketPushArtical(packet_tmp, recvData);
            sendto(sockfd, (char*) packet_tmp, sizeof(Packet), 0,pcliaddr, clilen);
            delete packet_tmp;
            break;
        case NEWMESSAGE:
            account = packet->buf[1];
            artical_index = atoi(packet->buf[2]);
            recvData = packet->artical;
            packet_tmp = NewPacket(0);
            if( userObject.NewUserMessage(IP, port, account, artical_index, recvData) != NULL ){
                userObject.SaveArtical();
                PacketPush(packet_tmp, string(success));
                sendto(sockfd, packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            else{
                PacketPush(packet_tmp, string(fail));
                sendto(sockfd, packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case DELETEUSERARTICAL:
            recvData = packet->buf[1];
            packet_tmp = NewPacket(0);
            if(userObject.DeleteUserArtical(IP, port, atoi(recvData.c_str()))){
                userObject.SaveArtical();
                PacketPush(packet_tmp, string(success));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            else{
                PacketPush(packet_tmp, string(fail));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case DELETEUSERMESSAGE:
            account = packet->buf[1];
            artical_index = atoi(packet->buf[2]);
            message_index = atoi(packet->buf[3]);
            packet_tmp = NewPacket(0);
            if(userObject.DeleteUserMessage(IP, port, account, artical_index, message_index)){
                userObject.SaveArtical();
                PacketPush(packet_tmp, string(success));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            else{
                PacketPush(packet_tmp, string(fail));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case MODIFYARTICAL:
            artical_index = atoi(packet->buf[1]);
            recvData = packet->artical;
            packet_tmp = NewPacket(0);
            if(userObject.ModifyArtical(IP, port, artical_index, recvData) != NULL){
                userObject.SaveArtical();
                PacketPush(packet_tmp, string(success));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            else{
                PacketPush(packet_tmp, string(fail));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
        case MODIFYMESSAGE:
            account = packet->buf[1];
            artical_index = atoi(packet->buf[2]);
            message_index = atoi(packet->buf[3]);
            recvData = packet->artical;
            packet_tmp = NewPacket(0);
            if(userObject.ModifyMessage(IP, port, account, artical_index, message_index, recvData) != NULL){
                userObject.SaveArtical();
                PacketPush(packet_tmp, string(success));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            else{
                PacketPush(packet_tmp, string(fail));
                sendto(sockfd, (char*)packet_tmp, sizeof(Packet), 0, pcliaddr, clilen);
            }
            delete packet_tmp;
            break;
    }
}








