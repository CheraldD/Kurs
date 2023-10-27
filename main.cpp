//#include "ui.h"
//#include "communicator.h"
//#include "base.h"
#include "logger.h"
int main(int argc, char* argv[])
{
    //UI interface (argc,argv);
    logger log;
    log.write_log("log.txt","Проверка");
    log.write_log("log.txt","Проверка1");
    log.write_log("log.txt","Проверка2");
    log.write_log("log.txt","Проверка3");
    return 0;
}
