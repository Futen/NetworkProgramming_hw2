#ifndef HEAD
#define HEAD
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>

#define LISTENQ 10
#define SERV_PORT 8000
#define MAXLINE 3000
#define BUFFSIZE 2048

#define TEST -1
#define DEFAULT 0
#define LOGIN 1
#define LOGOUT 2
#define CREATEACCOUNT 3
#define SHOWMYINFO 4
#define DELETEMYACCOUNT 5
#define MODIFYACCOUNT 6
#define NEWARTICAL 7
#define SHOWUSERARTICAL 8
#define NEWMESSAGE 9
#define DELETEUSERARTICAL 10
#define DELETEUSERMESSAGE 11

using namespace std;
typedef struct PACKET{
    int count;
    char buf[5][150];
    //vector<string> data;
    /*
    char buf_0[50];
    char buf_1[50];
    char buf_2[50];
    char buf_3[50];
    */
}Packet;

int CommandChoose(string command);
Packet* NewPacket();
Packet* PacketPush(Packet* packet, string data);
#endif
