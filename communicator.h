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

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
class communicator
{
public:
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    std::string base_location, log_location, cl_id/*информация полученная от клиента*/;
    std::vector<std::string> cl_ids,cl_passes;
    char buffer[1024];
    std::string digits[16] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
    void connect_to_cl(); // готово
    std::string get_cl_id(); // нужна доработка с обработкой переполнения буфера и конвертацией в другой формат,
    //обработкой ошибки доставки сообщения
    std::string get_cl_pass();//нужна доработка с обработкой переполнения буфера и конвертацией в другой формат,
    //обработкой ошибки доставки сообщения
    void client_auth();//не готово
    std::string SALT_generate();// в теории готово
    std::string convert_to_hex(uint64_t);// готово, включить в диаграмму классов
    communicator(uint port,std::string base_loc,std::string log_loc);//только создание сокета сервера
    
};
