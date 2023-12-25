/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл для модуля base
*/
#include "base.h"
void base:: read_base(std::ifstream & b){
    while(!b.eof()){
        std::string temp;
        getline(b,temp);
        if (temp.empty()){
            continue;
        }
        if(std::count(temp.begin(),temp.end(),':')>1 or std::count(temp.begin(),temp.end(),':')<1){
            log.write_log(log_location,"Работа модуля base. Неверный формат данных о пользователе");
            throw critical_error("Неверный формат данных о пользователе");
        }
        else{
            clients.push_back(temp);
        }
    }
    logins=id_s(clients);
    passwords=password_s(clients);
}
std::vector<std::string> base::id_s(std::vector<std::string> logins){
    std::vector<std::string> temp;
    for (int i = 0; i < logins.size(); i++)
    {
        logins[i].erase(logins[i].find(":"));
        temp.push_back(logins[i]);
    }
    return temp;
}
std::vector<std::string> base::password_s(std::vector<std::string> pswds){
    std::vector<std::string> temp1;
    for (int i = 0; i < pswds.size(); i++)
    {
        temp1.push_back(pswds[i].substr(pswds[i].find(":")+1));
    }
    return temp1;
}
base::base(std::string base_loc,std::string log_loc){
    log_location=log_loc;
    cl_base.open(base_loc, std::ios::in);
    if(cl_base.is_open()){
        log.write_log(log_location,"Работа модуля base. База данных успешно прочитана");
        read_base(cl_base);
        
    }
    else{
        log.write_log(log_location,"Работа модуля base. Ошибка чтения базы данных");
        std::cout<<"Такого файла базы данных клиента не существует"<<std::endl;
        throw critical_error ("Не удалось прочитать базу данных");
    }
}
base::~base(){
    cl_base.close();
}
std::vector <std::string> base::get_logins(){
    return logins;
}
std::vector <std::string> base::get_passwords(){
    return passwords;
}