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
    this->ArticalInit();
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
void UserClass::ShowArtical(User* user){
    Artical* artical_tmp;
    Message* message_tmp;
    for(Artical* artical_tmp=user->first_artical; artical_tmp!=NULL; artical_tmp=artical_tmp->next){
        cout << "Artical ID: " << artical_tmp->index << "\tAuthor: " << artical_tmp->author << endl;
        cout << "\tBegin\n"; 
        cout << "\t\t"  << artical_tmp->artical << endl;
        cout << "\tEnd\n";
        for(Message* message_tmp = artical_tmp->first_message; message_tmp != NULL; message_tmp = message_tmp->next){
            cout << "\tMessage ID: " << message_tmp->index << "\twho: " << message_tmp->who << endl;
            cout << "\t\tBegin" << endl;
            cout << "\t\t\t" << message_tmp->message << endl;
            cout << "\t\tEnd" << endl;
        }
    }
}
Artical* UserClass::FindArticalFromIndex(string account, int artical_index){
    User *user_tmp = this->FindUser(string("account"), account);
    Artical* tmp;
    if(user_tmp != NULL){
        for(tmp = user_tmp->first_artical; tmp != NULL; tmp = tmp->next){
            if(tmp->index == artical_index)
                return tmp;
        }
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
void UserClass::SaveArtical(){
    system("rm Artical/*");
    int count;
    User* user_tmp;
    Artical* art_tmp;
    Message* mess_tmp;
    for(user_tmp = this->first_user, count = 0; user_tmp != NULL; user_tmp = user_tmp->next){
        for(art_tmp = user_tmp->first_artical ; art_tmp != NULL; art_tmp = art_tmp->next, count++){
            char buf[256];
            sprintf(buf, "%d", count);
            string file_name = string("Artical/artical_") + string(buf) + string(".txt");
            ofstream file;
            file.open(file_name.c_str(), fstream::out);
            file << "New Artical:\n";
            file << (art_tmp->artical + "\n");
            file << "Author:\n";
            file << (art_tmp->author + "\n");
            for(mess_tmp = art_tmp->first_message; mess_tmp != NULL; mess_tmp = mess_tmp->next){
                file << "A message:\n";
                file << (mess_tmp->message + "\n");
                file << "Who:\n";
                file << (mess_tmp->who + "\n");
            }
            file.close();
        }
    }
}
Artical* UserClass::ModifyArtical(string IP, int port, int artical_index, string artical){
    User* user = this->FindUserFromIPAndPort(IP, port);
    if(user != NULL){
        Artical* tmp = this->FindArticalFromIndex(user->account, artical_index);
        if(tmp != NULL){
            tmp->artical = artical;
            return tmp;
        }
    }
    return NULL;
}
Message* UserClass::ModifyMessage(string IP, int port, int artical_index, int message_index, string message){
    User *user = this->FindUserFromIPAndPort(IP, port);
    if(user != NULL){
        Message* tmp = this->FindMessageFromIndex(user->account, artical_index, message_index);
        if(tmp != NULL){
            tmp->message = message;
            return tmp;
        }
    }
    return NULL;
}
Artical* UserClass::NewUserArtical(string IP, int port, string artical){
    User* user = this->FindUserFromIPAndPort(IP, port);
    if(user != NULL){
        cout << "Find user\n";
        return this->CreateArtical(user, artical);
    }
    return NULL;
}
Message* UserClass::NewUserMessage(string IP, int port, string author_account, int artical_index, string message){
    User* user = this->FindUserFromIPAndPort(IP, port);
    User* user_author = this->FindUser(string("account"), author_account);
    Artical* artical = this->FindArticalFromIndex(user_author->account, artical_index);
    if(artical != NULL && user != NULL && user_author != NULL){
        return this->CreateMessage(artical, user->account, message);
    }
    return NULL;
}
string UserClass::ShowUserArtical(string IP, int port){
    string output = "";
    char line[MAXLINE];
    User* user = this->FindUserFromIPAndPort(IP, port);
    Artical* artical_tmp;
    Message* message_tmp;
    for(Artical* artical_tmp=user->first_artical; artical_tmp!=NULL; artical_tmp=artical_tmp->next){
        sprintf(line, "Artical ID: %d\tAuthor: %s\n", artical_tmp->index, artical_tmp->author.c_str());
        output += string(line);
        sprintf(line, "\tBegin\n\t\t%s\n\tEnd\n", artical_tmp->artical.c_str());
        output += string(line);
        for(Message* message_tmp = artical_tmp->first_message; message_tmp != NULL; message_tmp = message_tmp->next){
            sprintf(line, "\tMessage ID: %d\twho: %s\n", message_tmp->index, message_tmp->who.c_str());
            output += string(line);
            sprintf(line, "\t\tBegin\n\t\t\t%s\n\t\tEnd\n", message_tmp->message.c_str());
            output += string(line);
        }
    }
    return output;
}
Artical* UserClass::DeleteArticalMessage(Artical* artical){
    Message* tmp;
    Message* tmp2;
    for(tmp = artical->first_message; tmp != NULL; tmp = tmp2){
        tmp2 = tmp->next;
        delete tmp;
    }
    return artical;
}
bool UserClass::DeleteUserArtical(string IP, int port, int artical_index){
    User* user = this->FindUserFromIPAndPort(IP, port);
    if(user != NULL){
        Artical* tmp = this->FindArticalFromIndex(user->account, artical_index);
        if(tmp != NULL){
            Message* message;
            if(tmp->previous != NULL && tmp->next != NULL){
                tmp->previous->next = tmp->next;
                tmp->next->previous = tmp->previous;
            }
            else if(tmp->previous == NULL && tmp->next != NULL){
                user->first_artical = tmp->next;
                tmp->next->previous = NULL;
            }
            else if(tmp->previous != NULL && tmp->next == NULL){
                user->last_artical = tmp->previous;
                tmp->previous->next = NULL;
            }
            else{
                user->first_artical = NULL;
                user->last_artical = NULL;
            }
            this->DeleteArticalMessage(tmp);
            delete tmp;
            return true;
        }
    }
    return false;
}
bool UserClass::DeleteUserMessage(string IP, int port, string author_account, int artical_index, int message_index){
    User* user = this->FindUserFromIPAndPort(IP, port);
    Artical* artical = this->FindArticalFromIndex(author_account, artical_index);
    Message* message = this->FindMessageFromIndex(author_account, artical_index, message_index);
    if(user != NULL && message != NULL && artical != NULL && user->account == message->who){
        if(message->previous != NULL && message->next != NULL){
            message->previous->next = message->next;
            message->next->previous = message->previous;
        }
        else if(message->previous == NULL && message->next != NULL){
            artical->first_message = message->next;
            message->next->previous = NULL;
        }
        else if(message->previous != NULL && message->next == NULL){
            artical->last_message = message->previous;
            message->previous->next = NULL;
        }
        else{
            artical->first_message = NULL;
            artical->last_message = NULL;
        }
        delete message;
        return true;
    }
    return false;
}








