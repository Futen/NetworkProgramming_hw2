#include "Header.h"

int CommandChoose(string command){
    if (command == "LOGIN")
        return LOGIN;
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
    else
        return DEFAULT;
}
