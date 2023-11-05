#pragma once
#include "communicator.h"
#include <string>
class data_handler{
    public:
    communicator &server;
    double result;
    data_handler(int nums_of_vec,communicator &server);
};