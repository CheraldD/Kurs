#pragma once
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "communicator.h"
namespace po = boost::program_options;

class UI
{
public:

    po::options_description desc;
    po::variables_map vm;
    std::string base_loc;
    std::string log_loc;
    uint port;
    UI(int argc, char* argv[]);
    uint get_port();
    std::string get_base_loc();
    std::string get_log_loc();
};