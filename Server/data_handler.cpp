/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл для модуля data_handler
*/
#include "communicator.h"
data_handler::data_handler(communicator &server, std::string log):server(server),log_location(log){
    int recv_b = recv(server.clientSocket,&nums,sizeof nums,0);
    
}
double data_handler::calculation(double number){
    double res;
    if(number>std::numeric_limits<double>::max()){
            number=std::numeric_limits<double>::max();
        }
    else if(number<std::numeric_limits<double>::lowest()) {
            number = std::numeric_limits<double>::lowest();
        }
    res=number*number;
    if(res>std::numeric_limits<double>::max()){
            res=std::numeric_limits<double>::max();
        }
    else if(res<std::numeric_limits<double>::lowest()) {
            res = std::numeric_limits<double>::lowest();
        }
    return res;
}
int data_handler::handle_data(){
    for (uint32_t i = 0; i < nums; i++)
    {
        double res=0;
        uint32_t size_of_vec;
        int recv_b = recv(server.clientSocket,&size_of_vec,sizeof size_of_vec,0);
        if(recv_b<0){
                log.write_log(log_location,"Работа модуля: data_handler. Ошибка принятия размера вектора");
                return 1;
            }
        log.write_log(log_location,"Работа модуля: data_handler. Получен размер вектора: "+std::to_string(size_of_vec));
        for(uint32_t j =0;j<size_of_vec;j++){
            double vec;
            std::chrono::milliseconds duration(100);
            std::this_thread::sleep_for(duration);
            recv_b = recv(server.clientSocket,&vec,sizeof vec,MSG_DONTWAIT);
            if(recv_b!=8){
                log.write_log(log_location,"Работа модуля: data_handler. Получен неверный тип данных");
                return 1;
            }
            log.write_log(log_location,"Работа модуля: data_handler. Получены данные вектора");
            res+=calculation(vec);
        }
        int send_b=send(server.clientSocket,&res,sizeof res,0);
        if(send_b<0){
                log.write_log(log_location,"Работа модуля: data_handler. Ошибка отправки результата вычислений");
                return 1;
            }
        log.write_log(log_location,"Работа модуля: data_handler. Отправлен результат вычислений для вектора: "+std::to_string(res));
    }
    std::cout << "Вычисления выполнены" << std::endl;
    return 0;
}