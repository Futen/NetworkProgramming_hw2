//#include "Header.h"
#include "DataBaseHeader.h"

int main(){
    UserClass A;
    User *tmp;
    A.Init();
    tmp = A.FindUser(string("account"), string("fulton84717"));
    A.ShowArtical(tmp);
    system("rm Artical/*");
    A.SaveArtical();
    return 0;
}
