#include "logger.h"
void logger::write_log(std::string log_loc,std::string message){
    log.open(log_loc, std::ios::app | std::ios::out);
    if(!log.is_open()){
        std::cout<<"Такого лог файла не существует"<<std::endl;
        exit(0);
    }
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    log<<std::ctime(&end_time)<<"/"<<message<<'\n';
    log.flush();
    log.close();
}
