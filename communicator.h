#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <random>

#include "base.h"
#include "logger.h"

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
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    std::string base_location;
    std::vector<std::string> cl_ids,cl_passes;
    size_t buflen =1024;
    std::unique_ptr <char[]> buffer{new char[buflen]};
    std::string digits[16] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
public:
    logger log;
    int serverSocket, clientSocket;
    std::string cl_id,log_location;
    void connect_to_cl();
    int client_auth();
    std::string SALT_generate();
    std::string convert_to_hex(uint64_t);
    void send_data(std::string data);
    std::string recv_data();
    void close_sock();
    communicator(uint port,std::string base_loc,std::string log_loc);//только создание сокета сервера
};
class data_handler{
    public:
    communicator &server;
    logger log;
    double result;
    data_handler(uint32_t nums_of_vec,communicator &server);
};
