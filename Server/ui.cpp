/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл для модуля ui
*/
#include "ui.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
UI::UI(int argc, char* argv[])
{
    desc.add_options()
    ("help,h", "Помощь")
    ("Log_loc,l", po::value<std::vector<std::string>>()->multitoken(), "Путь для log файла")
    ("Port,p", po::value<std::vector<uint>>()->multitoken(), "Порт сервера")
    ("Base_loc,b", po::value<std::vector<std::string>>()->multitoken(), "Путь для базы данных клиентов");
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        base_loc=get_base_loc();
        log_loc=get_log_loc();
        port=get_port();
        if(base_loc=="" or log_loc=="" or port == 1){
            std::cout<<desc<<std::endl;
            exit(1);
        }
        communicator server (port,base_loc,log_loc);
        server.start();
        while (true)
        {
            server.connect_to_cl();
            log.write_log(log_loc,"Работа модуля: UI. Сервер установил соединение с клиентом: "+server.cl_id);
            int flag = server.client_auth();
            if(flag==1){
                log.write_log(log_loc,"Серверу не удалось аутентифицировать клиента "+server.cl_id);
                server.close_sock();
            }
            else{
                log.write_log(log_loc,"Работа модуля: UI. Аутентификация прошла успешно, начало фазы вычислений");
                data_handler inf(server,log_loc);
                if(inf.handle_data()==1){
                    std::cout<<"Клиент прислал неверные данные"<<std::endl;
                    server.close_sock();
                    continue;
                }
                server.close_sock();
                log.write_log(log_loc,"Работа модуля: UI. Работа с клиентом "+server.cl_id+" успешно завершена"+"\n");
                std::cout << "Сервер завершил работу для пользователя: "<<server.cl_id << std::endl;
            }
        }
    } catch (po::error& e) {
        std::cout << e.what() << std::endl;
    }
    catch(critical_error &e){
        std::cout<<"Критическая ошибка: "<<e.what()<<std::endl;
    }
}
uint UI::get_port()
{
    if (vm.count("Port")) {
        const std::vector<uint>& ports = vm["Port"].as<std::vector<uint>>();
        if(ports.back()<1024){
            log.write_log(log_loc,"Работа модуля: UI. Пользователь ввел системный порт");
            throw critical_error("Выбран системный порт");
            
        }
        return ports.back();
    } else {
        return 1;
    }
}
std::string UI::get_base_loc()
{
    if (vm.count("Base_loc")) {
        const std::vector<std::string>& base_loc = vm["Base_loc"].as<std::vector<std::string>>();
        return base_loc.back();
    } else {
        return "";
    }
}
std::string UI::get_log_loc()
{
    if (vm.count("Log_loc")) {
        const std::vector<std::string>& log_loc = vm["Log_loc"].as<std::vector<std::string>>();
        return log_loc.back();
    } else {
        return "";
    }
}