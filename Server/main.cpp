/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Главный файл проекта
*/
#include "ui.h"
#include "base.h"
#include "communicator.h"
#include "logger.h"
#include "error.h"
int main(int argc, char* argv[])
{
    UI interface (argc,argv);
    return 0;
}
