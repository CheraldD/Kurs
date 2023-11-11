#include "base.h"
void base:: read_base(std::ifstream & b){
    while(!b.eof()){
        std::string temp;
        getline(b,temp);
        if (temp.empty()){
            continue;
        }
        else{
            clients.push_back(temp);
        }
    }

}
std::vector<std::string> base::id_s(std::vector<std::string> c){
    std::vector<std::string> temp;
    for (int i = 0; i < c.size(); i++)
    {
        c[i].erase(c[i].find("/"));
        temp.push_back(c[i]);
    }
    return temp;
}
std::vector<std::string> base::password_s(std::vector<std::string> c){
    std::vector<std::string> temp1;
    for (int i = 0; i < c.size(); i++)
    {
        temp1.push_back(c[i].substr(c[i].find("/")+1));
    }
    return temp1;
}
base::base(std::string base_loc,std::string log_loc){
    log_location=log_loc;
    cl_base.open(base_loc, std::ios::in);
    if(cl_base.is_open()){
        log.write_log(log_location,"База данных успешно прочитана");
        read_base(cl_base);
        
    }
    else{
        log.write_log(log_location,"Ошибка чтения базы данных");
        std::cout<<"Такого файла базы данных клиента не существует"<<std::endl;
        exit(0);
    }
}
base::~base(){
    cl_base.close();
}