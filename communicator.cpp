#include "communicator.h"
void communicator::client_auth(){
    auto result = std::find(cl_ids.begin(),cl_ids.end(),cl_id);
    std::string salt=SALT_generate();
    if(result==cl_ids.end()){
        std::cout<<"Клиент с таким айди не найден"<<std::endl;
        strncpy(buffer, "ERR", sizeof(buffer));
        int bytes_sended = send(clientSocket, buffer, sizeof(buffer), 0);
        memset(buffer,0,sizeof(buffer));
        close(clientSocket);
    }
    else{
        auto pos = std::distance(cl_ids.begin(), result);
        strncpy(buffer, salt.c_str(), sizeof(buffer));
        int bytes_sended = send(clientSocket, buffer, sizeof(buffer), 0);
        memset(buffer,0,sizeof(buffer));
        int bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0);
    }
    //int bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
    //std::string cl_hash(buffer,bytes_received-1);
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
        std::cerr<<"Ошибка принятия соединения клиента"<<std::endl;
    } else {
        std::cout << "Соединение установлено" << std::endl;
    }
    cl_id=get_cl_id();
    
}
communicator::communicator(uint port, std::string base_loc, std::string log_loc)
{
    base_location=base_loc;
    log_location=log_loc;
    base cl_b(base_location);
    cl_ids=cl_b.id_s(cl_b.clients);
    cl_passes=cl_b.password_s(cl_b.clients);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(1);
    }
    while (true)
    {
        connect_to_cl();
        client_auth();
    }
    
}
std::string communicator::get_cl_id()
{
    int bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string received_data(buffer, bytes_received-1);
    memset(buffer,0,sizeof(buffer));
    return received_data;
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
