#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>
class logger{
    public:
    std::ofstream log;
    void write_log(std::string log_loc,std::string message);
};