#include "data_handler.h"
#include "communicator.h"
data_handler::data_handler(int nums_of_vec, communicator &server){
    int size_of_vec=stoi(server.recv_data());
    std::cout<<size_of_vec<<std::endl;
    for (int i = 0; i < nums_of_vec; i++)
    {
        
    }
    
}