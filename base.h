#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "logger.h"
class base{
    public:
    logger log;
    std::string log_location;
    std::ifstream cl_base;
    std::vector<std::string> clients;
    void read_base(std::ifstream &b);
    std::vector<std::string> id_s(std::vector<std::string> c);
    std::vector<std::string> password_s(std::vector<std::string> c);  
    base(std::string base_loc,std::string log_loc);
    ~base();
};