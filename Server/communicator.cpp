/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл для модуля communicator*/
#include "communicator.h"
int communicator::client_auth(){
    auto result = std::find(cl_ids.begin(),cl_ids.end(),cl_id);
    std::string salt=SALT_generate();
    std::string flag="ERR";
    if(result==cl_ids.end()){
        std::cout<<"Клиент с таким айди не найден"<<std::endl;
        log.write_log(log_location,"Работа модуля: communicator. Клиента с айди: "+cl_id+" нет в базе данных");
        send_data(flag,"Работа модуля: communicator. Ошибка при отправке ERR");
        return 1;
    }
    log.write_log(log_location,"Работа модуля: communicator. Начало фазы аутентификации для пользователя: "+cl_id);
    std::cout << "Фаза аутентификации для пользователя: "<<cl_id<< std::endl;
    auto pos = std::distance(cl_ids.begin(), result); 
    send_data(salt,"Работа модуля: communicator. Ошибка при отправке соли");
    log.write_log(log_location,"Работа модуля: communicator. Отправлена соль");
    std::string cl_hash=recv_data("Ошибка при приеме хеша");
    log.write_log(log_location,"Работа модуля: communicator. От клиента принят хеш");
    std::string serv_hash=hash_gen(salt,cl_passes[pos]);
    if(serv_hash!=cl_hash){
        log.write_log(log_location,"Работа модуля: communicator. Ошибка при аутентификации: хеши не соответствуют друг другу");
        std::cout<<"Пароль неверный"<<std::endl;
        send_data(flag,"Работа модуля: communicator. Ошибка при отправке ERR");
        return 1;
    }
    std::cout<<"Пользователь "<<cl_id<<" аутентифицорован"<<std::endl;
    send_data("OK","Работа модуля: communicator. Ошибка при отправке OK");
    return 0;
}

void communicator::connect_to_cl()
{
    if (listen(serverSocket, 10) == 0) {
        std::cout << "Сервер слушает..." << std::endl;
        log.write_log(log_location,"Работа модуля: communicator. Сервер встал на прослушку порта");
    } else {
        log.write_log(log_location,"Работа модуля: communicator. Ошибка при прослушивании порта");
        std::cout << "Ошибка при прослушивании" << std::endl;
        throw critical_error ("Север не встал на прослушку порта");
    }
    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
    if(clientSocket<0) {
        log.write_log(log_location,"Работа модуля: communicator. Ошибка при аутентификации: ошибка принятия соединения клиента");
        std::cout<<"Ошибка принятия соединения клиента"<<std::endl;
        close(clientSocket);
    } else {
        log.write_log(log_location,"Работа модуля: communicator. Соединение с клиентом установлено");
        std::cout << "Соединение установлено" << std::endl;
    }
    cl_id=recv_data("Работа модуля: communicator. Ошибка при приеме айди клиента");
    std::cout<<"Подсоединился пользователь: "+cl_id<<std::endl;
    
}
communicator::communicator(uint port, std::string base_loc, std::string log_loc){
    p=port;
    base_location=base_loc;
    log_location=log_loc;
}
void communicator::start()
{
    base cl_b(base_location,log_location);
    cl_ids=cl_b.get_logins();
    cl_passes=cl_b.get_passwords();
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        log.write_log(log_location,"Работа модуля: communicator. Ошибка при создании сокета сервера");
        throw critical_error ("Сокет не был создан");
    }
    log.write_log(log_location,"Работа модуля: communicator. Сокет для сервера создан");
    std::cout << "Сокет создан" << std::endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(p);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        log.write_log(log_location,"Работа модуля: communicator. Ошибка при привязке сокета");
        throw critical_error ("Сокет не был привязан");
    }
    log.write_log(log_location,"Работа модуля: communicator. Cокет привязан");
    std::cout << "Сокет привязан" << std::endl;
}
std::string communicator::recv_data(std::string messg)
{
    int rc=0;
    while (true) {
        buffer = std::unique_ptr<char[]>(new char[buflen]); 
        rc = recv(clientSocket, buffer.get(), buflen, MSG_PEEK);
        if (rc==0){
            close_sock();
            log.write_log(log_location,"Клиент закрыл соединение");
        }
        else if (rc<0){
            close_sock();
            log.write_log(log_location,messg);
        }
        if (rc < buflen) 
            break; 
        buflen *= 2; 
    }
    std::string msg(buffer.get(), rc);
    recv(clientSocket, nullptr, rc, MSG_TRUNC);
    return msg;
}
std::string communicator::SALT_generate()
{
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
    std::mt19937_64 rnd(seed);
    uint64_t int_salt=rnd();
    std::string hex_salt=convert_to_hex(int_salt);
    while(hex_salt.length()<16) {
        hex_salt="0"+hex_salt;
    }
    log.write_log(log_location,"Работа модуля: communicator. Соль сгенерирована");
    return hex_salt;

}
std::string communicator::convert_to_hex(uint64_t x)
{
    std::string hex;
    do {
        hex.insert (0, digits[x % 16]);
        x /= 16;
    } while (x!=0);
    return hex;
}
void communicator::send_data(std::string data,std::string msg){
    std::unique_ptr<char[]> temp {new char[data.length()+1]};
    strcpy(temp.get(), data.c_str());
    buffer = std::move(temp);
    int sb = send(clientSocket, buffer.get(), data.length(), 0);
    if(sb<0){
        log.write_log(log_location,msg);
        close_sock();
    }
}
void communicator::close_sock(){
    close(clientSocket);
    log.write_log(log_location,"Работа модуля communicator. Разорвано соединение с клиентом");
}
std::string communicator::hash_gen(std::string &salt,std::string &password){
    CryptoPP::Weak::MD5 hash;
    std::string serv_hash;
    CryptoPP::StringSource(salt+ password, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(serv_hash))));
    return serv_hash;
}