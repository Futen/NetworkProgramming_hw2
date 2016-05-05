#include "Header.h"

int CommandChoose(string command){
    if (command == "LOGIN")
        return LOGIN;
    else if(command == "TEST")
        return TEST;
    else if(command == "LOGOUT")
        return LOGOUT;
    else if(command == "CREATEACCOUNT")
        return CREATEACCOUNT;
    else if(command == "SHOWMYINFO")
        return SHOWMYINFO;
    else if(command == "DELETEMYACCOUNT")
        return DELETEMYACCOUNT;
    else if(command == "MODIFYACCOUNT")
        return MODIFYACCOUNT;
    else if(command == "NEWARTICAL")
        return NEWARTICAL;
    else if(command == "SHOWUSERARTICAL")
        return SHOWUSERARTICAL;
    else if(command == "NEWMESSAGE")
        return NEWMESSAGE;
    else if(command == "DELETEUSERARTICAL")
        return DELETEUSERARTICAL;
    else if(command == "DELETEUSERMESSAGE")
        return DELETEUSERMESSAGE;
    else
        return DEFAULT;
}
Packet* NewPacket(){
    Packet* tmp = new Packet;
    tmp->count = 0;
    return tmp;
}
Packet* PacketPush(Packet* packet, string data){
    /*
    switch(packet->count){
        case 0:
            strcpy(packet->buf_0, data.c_str());
            break;
        case 1:
            strcpy(packet->buf_1, data.c_str());
            break;
        case 2:
            strcpy(packet->buf_2, data.c_str());
            break;
        case 3:
            strcpy(packet->buf_3, data.c_str());
    }
    */
    //packet->data.push_back(data);
    strcpy(packet->buf[packet->count], data.c_str());
    packet->count++;
    return packet;
}
