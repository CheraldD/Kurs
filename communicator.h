#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <random>
#include "base.h"
#include "data_handler.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
class communicator
{    
private:
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    std::string base_location, log_location, cl_id/*информация полученная от клиента*/;
    std::vector<std::string> cl_ids,cl_passes;
    char buffer[1024];
    std::string digits[16] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
public:
    void connect_to_cl(); // готово
    //обработкой ошибки доставки сообщения
    int client_auth();//не готово
    std::string SALT_generate();// в теории готово
    std::string convert_to_hex(uint64_t);// готово, включить в диаграмму классов
    void send_data(std::string data);
    std::string recv_data();
    void close_sock();
    communicator(uint port,std::string base_loc,std::string log_loc);//только создание сокета сервера
};
