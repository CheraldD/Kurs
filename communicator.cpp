#include "communicator.h"
int communicator::client_auth(){
    auto result = std::find(cl_ids.begin(),cl_ids.end(),cl_id);
    std::string salt=SALT_generate();
    std::string serv_hash;
    std::string flag="ERR";
    if(result==cl_ids.end()){
        std::cout<<"Клиент с таким айди не найден"<<std::endl;
        log.write_log(log_location,"Клиента с айди: "+cl_id+" нет в базе данных");
        send_data(flag);
        return 1;
    }
    log.write_log(log_location,"Начало фазы аутентификации для пользователя: "+cl_id);
    std::cout << "Фаза аутентификации для пользователя: "<<cl_id<< std::endl;
    CryptoPP::Weak::MD5 hash;
    
    auto pos = std::distance(cl_ids.begin(), result); //для нахождения пароля
    log.write_log(log_location,"Отправлена соль: "+salt);
    send_data(salt);
    std::string cl_hash=recv_data();
    log.write_log(log_location,"От клиента принят хеш: "+cl_hash);
    CryptoPP::StringSource(salt+ cl_passes[pos], true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(serv_hash))));
    if(serv_hash!=cl_hash){
        log.write_log(log_location,"Ошибка при аутентификации: хеши не соответствуют друг другу");
        std::cout<<"Пароль неверный"<<std::endl;
        send_data(flag);
        return 1;
    }
    std::cout<<"Пользователь "<<cl_id<<" аутентифицорован"<<std::endl;
    send_data("OK");
    return 0;
}

void communicator::connect_to_cl()
{
    if (listen(serverSocket, 10) == 0) {
        std::cout << "Сервер слушает..." << std::endl;
    } else {
        std::cout << "Ошибка при прослушивании" << std::endl;
    }
    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
    if(clientSocket<0) {
        std::cout<<"Ошибка принятия соединения клиента"<<std::endl;
        close(clientSocket);
    } else {
        std::cout << "Соединение установлено" << std::endl;
    }
    cl_id=recv_data();
    std::cout<<cl_id<<std::endl;
    
}
communicator::communicator(uint port, std::string base_loc, std::string log_loc)
{
    base_location=base_loc;
    log_location=log_loc;
    base cl_b(base_location,log_location);
    cl_ids=cl_b.id_s(cl_b.clients);
    cl_passes=cl_b.password_s(cl_b.clients);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        log.write_log(log_location,"Ошибка при создании сокета сервера");
        exit(1);
    }
    log.write_log(log_location,"Сокет для сервера создан");
    std::cout << "Сокет создан" << std::endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        log.write_log(log_location,"Ошибка при привязке сокета");
        exit(1);
    }
    log.write_log(log_location,"Cокет привязан");
    std::cout << "Сокет привязан" << std::endl;
}
std::string communicator::recv_data()
{
    int rc=0;
    while (true) {
        rc = recv(clientSocket, buffer.get(), buflen, MSG_PEEK); // принять данные
        if (rc < buflen) // данные уместились?
            break; // да, выходим из цикла
        buflen *= 2; // нет, увеличиваем размер массива вдвое
        buffer = std::unique_ptr<char[]>(new char[buflen]); // и пересоздаем массив
    }
    std::string msg(buffer.get(), rc); // сохраняем массив в строку
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
    log.write_log(log_location,"Соль сгенерирована");
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
void communicator::send_data(std::string data){
    std::unique_ptr<char[]> temp {new char[data.length()+1]};
    strcpy(temp.get(), data.c_str());
    buffer = std::move(temp);
    int sb = send(clientSocket, buffer.get(), data.length(), 0);
}
void communicator::close_sock(){
    close(clientSocket);
}