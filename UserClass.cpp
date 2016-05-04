#include "DataBaseHeader.h"

void UserClass::Init(){
    first_user =  NULL;
    last_user = NULL;
    user_index = 0;
    ifstream file;
    file.open("USER_LST.txt", fstream::in);
    if(file.is_open()){
        int count = 1;
        string line[4];
        while(getline(file, line[count-1])){
            //printf("%d %s\n", count-1, line[count-1].c_str());
            if (count<4)
                count++;
            else{
                CreateUser(line[0], line[1], line[2], line[3]);
                count = 1;
            }
        }
        file.close();
    }
}
void UserClass::SaveUserList(){
    ofstream file;
    file.open("USER_LST.txt", fstream::out);
    if(file.is_open()){
        for(User *tmp=first_user; tmp!=NULL; tmp=tmp->next){
            file << (tmp->account + "\n");
            file << (tmp->password + "\n");
            file << (tmp->nickname + "\n");
            file << (tmp->birthday + "\n");
        }
        file.close();
    }
}
User* UserClass::CreateUser(string account, string password, string nickname, string birthday){
    //User *tmp = (User*)malloc(sizeof(User));
    User *tmp = new User;
    tmp->account = account;
    tmp->password = password;
    tmp->nickname = nickname;
    tmp->birthday = birthday;
    tmp->index = user_index;
    tmp->islogin = 0;
    tmp->artical_index = 0;
    user_index++;
    tmp->previous = NULL;
    tmp->next = NULL;
    tmp->first_artical = NULL;
    tmp->last_artical = NULL;
    if(last_user == NULL){
        first_user = tmp;
        last_user = tmp;
    }
    else{
        tmp->previous = last_user;
        last_user->next = tmp;
        last_user = tmp;
    }
    return last_user;
}
bool UserClass::DeleteUser(string IP, int port){
    User* who = this->FindUserFromIPAndPort(IP, port);
    if (who == NULL)
        return false;
    else{
        if(who->previous != NULL && who->next != NULL){
            who->previous->next = who->next;
            who->next->previous = who->previous;
        }
        else if(who->previous == NULL && who->next != NULL){
            this->first_user = who->next;
            who->next->previous = NULL;
        }
        else if(who->previous != NULL && who->next == NULL){
            this->last_user = who->previous;
            who->previous->next = NULL;
        }
        delete who;
        return true;
    }
}
User* UserClass::FindUser(string whatkind, string info){
    for(User* tmp = first_user; tmp != NULL; tmp = tmp->next){
        if(whatkind == "account" && tmp->account == info)
            return tmp;
        else if(whatkind == "nickname" && tmp->nickname == info)
            return tmp;
    }
    return NULL;
}
User* UserClass::FindUserFromIPAndPort(string IP, int port){
    User* tmp;
    for(tmp = this->first_user; tmp != NULL; tmp = tmp->next){
        if(tmp->islogin == 1){
            if(tmp->IP == IP && tmp->port == port)
                return tmp;
        }
    }
    return NULL;
}
User* UserClass::UserLogin(string account, string password, string IP, int port){
    int shutdown = 0;
    User *answer, *tmp;
    for(tmp = first_user, answer = NULL; tmp != NULL && shutdown == 0; tmp = tmp->next){
        //cout << tmp <<endl;
        if(tmp->account == account && tmp->password == password){
            answer = tmp;
            shutdown = 1;
        }
    }
    if(answer != NULL){
        answer->islogin = 1;
        answer->IP = IP;
        answer->port = port;
        return answer;
    }
    return NULL;
}
User* UserClass::UserLogout(string IP, int port){
    User *who = this->FindUserFromIPAndPort(IP, port);
    if(who != NULL){
        who->islogin = 0;
        return who;
    }
    else
        return NULL;
}
User* UserClass::UserModify(string IP, int port, string password, string nickname, string birthday){
    User *who = this->FindUserFromIPAndPort(IP, port);
    if(who != NULL){
        who->password = password;
        who->nickname = nickname;
        who->birthday = birthday;
        return who;
    }
    else
        return NULL;
}
void UserClass::ArticalInit(){
    DIR *dp;
    dirent *d;

    system("mkdir -p Artical/");
    dp = opendir("Artical/");
    while((d = readdir(dp)) != NULL){
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")){
            continue;
        }
        ifstream file;
        file.open((string("Artical/") + string(d->d_name)).c_str());
        if(file.is_open()){
            string line;
            string account;
            string artical;
            string message;
            string who;
            User* user_tmp;
            Artical* artical_tmp;
            Message* message_tmp;
            while(getline(file, line)){
                if(line == "New Artical:"){
                    getline(file, line);
                    artical = line;
                }
                else if(line == "Author:"){
                    getline(file, line);
                    account = line;
                    user_tmp = this->FindUser(string("account"), account);
                    artical_tmp = this->CreateArtical(user_tmp, artical);
                }   
                else if(line == "A message:"){
                    getline(file, line);
                    message = line;
                }
                else if(line == "Who:"){
                    getline(file, line);
                    who = line;
                    message_tmp = this->CreateMessage(artical_tmp, who, message);
                }
            }
        }
    }
    closedir(dp);
}
Artical* UserClass::CreateArtical(User *user, string artical){
    Artical* tmp = new Artical;
    tmp->author = user->account;
    tmp->artical = artical;
    tmp->index = user->artical_index;
    tmp->message_count = 0;
    user->artical_index++;
    tmp->previous = NULL;
    tmp->next = NULL;
    tmp->first_message = NULL;
    tmp->last_message = NULL;
    if(user->first_artical == NULL && user->last_artical == NULL){
        user->first_artical = tmp;
        user->last_artical = tmp;
    }
    else{
        tmp->previous = user->last_artical;
        user->last_artical->next = tmp;
        user->last_artical = tmp;
    }
    return tmp;
}
Message* UserClass::CreateMessage(Artical* artical, string who, string message){
    Message* tmp = new Message;
    tmp->who = who;
    tmp->message = message;
    tmp->index = artical->message_count;
    artical->message_count++;
    tmp->previous = NULL;
    tmp->next = NULL;
    if(artical->first_message == NULL && artical->last_message == NULL){
        artical->first_message = tmp;
        artical->last_message = tmp;
    }
    else{
        tmp->previous = artical->last_message;
        artical->last_message->next = tmp;
        artical->last_message = tmp;
    }
    return tmp;
}
Artical* UserClass::FindArticalFromIndex(string account, int artical_index){
    User *user_tmp = this->FindUser(string("account"), account);
    Artical* tmp;
    for(tmp = user_tmp->first_artical; tmp != NULL; tmp = tmp->next){
        if(tmp->index == artical_index)
            return tmp;
    }
    return NULL;
}
Message* UserClass::FindMessageFromIndex(string account, int artical_index, int message_index){
    Artical* art_tmp = this->FindArticalFromIndex(account, artical_index);
    Message* tmp;
    if(art_tmp != NULL){
        for(tmp = art_tmp->first_message; tmp != NULL; tmp = tmp->next){
            if(tmp->index == message_index)
                return tmp;
        }
    }
    return NULL;
}









