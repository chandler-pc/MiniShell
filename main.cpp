#include <iostream>
#include <vector>
#include <ctime>
#include <unistd.h>
#define clear() std::cout<<"\033[H\033[J"
class MiniShell{
    private:
        std::string ActualInput;
        std::string ActualCmd;
        std::vector<std::string> Args;        
        char* ArgsChar[100];
        std::vector<std::string> Functions;
        std::string User;
        time_t initTime;
        void parse();
        void input();
        void execute();
    private:
        void whoami();
    public:
        MiniShell();
        void run();
};

MiniShell::MiniShell(){
    User = getenv("USER");
}

void MiniShell::run(){
    clear();
    Functions.push_back("whoami");
    std::cout<<"Bienvenido a MiniShell UNI\n";
    std::cout<<"Usuario : "<<User<<std::endl;
    struct tm* localTime;
    time(&initTime);
    localTime = localtime(&initTime);
    std::cout<<"Inicio de sesión -> "<<localTime->tm_hour<<" : "<<localTime->tm_min<<" : "<<localTime->tm_sec<<std::endl;
    input();
}

void MiniShell::input(){
    std::cout<<std::endl;
    while(true){
        std::cout<<User<<">";
        std::cout.flush();
        std::getline(std::cin,ActualInput);
        parse();
        execute();
    }
}

void MiniShell::parse(){    
    ActualInput = ActualInput.substr(ActualInput.find_first_not_of(" "));
    ActualInput = ActualInput.substr(0,ActualInput.find_last_not_of(" ")+1);
    int size = ActualInput.size();
    int i = 0;
    for(i = 0; i< size; i++){
        if(ActualInput[i] == ' ' || i == size-1){
            break;
        }
    }
    ActualCmd = ActualInput.substr(0,i+1);
    ArgsChar[0] = (char*)ActualCmd.c_str();
    ActualInput = ActualInput.substr(i);
    ActualInput = ActualInput.substr(ActualInput.find_first_not_of(" "));
    int end = ActualInput.find(" ");
    size = ActualInput.size();
    i = 1;
    while(end != -1){
        std::string arg = ActualInput.substr(0,end);
        ArgsChar[i] = (char*)arg.c_str();
        Args.push_back(arg);
        ActualInput = ActualInput.substr(end, size - end+1);
        ActualInput = ActualInput.substr(ActualInput.find_first_not_of(" "));
        size = ActualInput.size();
        end = ActualInput.find(" ");
        i++;
    }
    Args.push_back(ActualInput);
    ArgsChar[i] = (char*)ActualInput.c_str();
    ActualInput = "";
    ArgsChar[i+1] = 0;
    std::cout.flush();
}

void MiniShell::whoami(){
    std::cout<<User<<std::endl;
}

void MiniShell::execute(){
    bool bOwnCmd = false;
    int t = 0;
    for(std::string s : Functions){
        if(s==ActualCmd){
            bOwnCmd = true;
            break;
        }
        t++;
    }
    switch(t){
        case 0:whoami();break;
    }
    if(bOwnCmd){
        return;
    }
    pid_t pid = fork();
    if(pid == -1){
        std::cout<<"Child fork failed..."<<std::endl;
        return;
    }
    if(pid == 0){
        int res = execvp(ArgsChar[0],ArgsChar);
        std::cout<<ArgsChar[0]<<std::endl;
        std::cout<<ArgsChar[1]<<std::endl;
        std::cout<<ArgsChar[2]<<std::endl;
        if(res < 0){
            std::cout<<"Failed to execute command..."<<std::endl;
        }
    }
}

int main(){
    MiniShell shell = MiniShell();
    shell.run();
    return 0;
}