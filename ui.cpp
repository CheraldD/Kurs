#include "ui.h"
#include "logger.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
UI::UI(int argc, char* argv[])
{
    logger log;
    desc.add_options()
    ("help,h", "Помощь")
    ("Log_loc,l", po::value<std::vector<std::string>>()->multitoken()->required(), "Путь для log файла")
    ("Port,p", po::value<std::vector<uint>>()->multitoken()->required(), "Порт сервера")
    ("Base_loc,b", po::value<std::vector<std::string>>()->multitoken()->required(), "Путь для базы данных клиентов");
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        base_loc=get_base_loc();
        log_loc=get_log_loc();
        port=get_port();
        communicator server (port,base_loc,log_loc);
        while (true)
        {
            server.connect_to_cl();
            log.write_log(log_loc,"Сервер установил соединение с клиентом "+server.cl_id);
            int flag = server.client_auth();
            if(flag==1){
                log.write_log(log_loc,"Серверу не удалось аутентифицировать клиента "+server.cl_id);
                server.close_sock();
            }
            else{
                log.write_log(log_loc,"Аутентификация прошла успешно, начало фазы вычислений");
                uint32_t num_of_vecc;
                int recv_b = recv(server.clientSocket,&num_of_vecc,sizeof num_of_vecc,0);
                data_handler inf(num_of_vecc,server);
                server.close_sock();
                log.write_log(log_loc,"Работа с клиентом "+server.cl_id+" успешно завершена"+"\n");
                std::cout << "Сервер завершил работу для пользователя: "<<server.cl_id << std::endl;
            }
        }
    } catch (po::error& e) {
        std::cout << e.what() << std::endl;
    }
}
uint UI::get_port()
{
    if (vm.count("Port")) {
        const std::vector<uint>& ports = vm["Port"].as<std::vector<uint>>();
        return ports.back();
    } else {
        std::cout << desc << std::endl;
        std::exit(1);
    }
}
std::string UI::get_base_loc()
{
    if (vm.count("Base_loc")) {
        const std::vector<std::string>& base_loc = vm["Base_loc"].as<std::vector<std::string>>();
        return base_loc.back();
    } else {
        std::cout << desc << std::endl;
        return "";
    }
}
std::string UI::get_log_loc()
{
    if (vm.count("Log_loc")) {
        const std::vector<std::string>& log_loc = vm["Log_loc"].as<std::vector<std::string>>();
        return log_loc.back();
    } else {
        std::cout << desc << std::endl;
        return "";
    }
}