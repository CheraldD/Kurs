#include "communicator.h"
data_handler::data_handler(uint32_t nums_of_vec, communicator &server):server(server){
    for (uint32_t i = 0; i < nums_of_vec; i++)
    {
        double res=0;
        uint32_t size_of_vec;
        int recv_b = recv(server.clientSocket,&size_of_vec,sizeof size_of_vec,0);
        log.write_log(server.log_location,"Получен размер вектора: "+std::to_string(size_of_vec));
        for(uint32_t j =0;j<size_of_vec;j++){
            double vec;
            recv_b = recv(server.clientSocket,&vec,sizeof vec,0);
            log.write_log(server.log_location,"Получены данные вектора: "+std::to_string(vec));
            res+=vec*vec;
        }
        int send_b=send(server.clientSocket,&res,sizeof res,0);
        log.write_log(server.log_location,"Отправлен результат вычислений для вектора: "+std::to_string(res));
    }
    std::cout << "Вычисления выполнены" << std::endl;
    
}