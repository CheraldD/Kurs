/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модулей communicator и data_handler
*/
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
#include <chrono>
#include <thread>
#include <limits>
#include "base.h"
#include "logger.h"
#include "error.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
/** @brief Класс коммуникатор
* @details Порт, расположение логфайла и базы данных устанавливаются в конструкторе \n Для начала работы сервера используется метод start
* Сетевое взаимодействие осуществляется в методах connect_to_cl и client_auth
*/
class communicator
{    
private:
    /**
    * @brief Структуры адреса сервера и клиента
    */
    struct sockaddr_in serverAddr, clientAddr;
    /**
    * @brief Размер адреса
    */
    socklen_t addr_size;
    /**
    * @brief Расположение базы данных
    */
    std::string base_location;
    /**
    * @brief Вектора с логинами и паролями
    */
    std::vector<std::string> cl_ids,cl_passes;
    /**
    * @brief Размер буффера
    */
    size_t buflen =1024;
    /**
    * @brief Unique_ptr для приема и отправки сообщений
    */
    std::unique_ptr <char[]> buffer{new char[buflen]};
    /**
    * @brief Порт
    */
    uint p;
    /**
    * @brief Набор 16-ричных символов для генерации соли
    */
    std::string digits[16] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
public:
    /**
    * @brief Сокеты сервера и клиента
    */
    int serverSocket, clientSocket;
    /**
    * @brief Объекта класса loger для ведения журнала
    */
    logger log;
    /**
    * @brief Айди клиента, расположение лог файла
    */
    std::string cl_id,log_location;
    /**
    * @brief Подсоединение к клиенту
    * @details Осуществляется установление соединения с клиентом. Сервер работает в режиме listen
    * @return Функция ничего не возвращает
    * @throw critical_error, если не удалось встать на прослушку порта
    */
    void connect_to_cl();
    /**
    * @brief Аутентификация клиента
    * @details Осуществляется аутентификация клиента посредством генерации хеша из соль+пароль
    * @return Функция возвращает 1, если не удалось аутентифицировать клиента
    */
    int client_auth();
    /**
    * @brief Генерация соли
    * @details Осуществляется генерация 64-разрядного числа, затем число конвертируется в 16-ричную строку, которая дополняется 0 до длины 16 при необходимости
    * @return Функция возвращает сгенерированну соль
    */
    std::string SALT_generate();
    /**
    * @brief Конвертирование в 16-ричный формат
    * @details Осуществляется конвертирование числа в 16-ричную строку. Используются в методе SALT_generate()
    * @param [in] d_salt Случайно сгенерированное чилсо для конвертации
    * @return Функция возвращает сгенерированную соль
    */
    std::string convert_to_hex(uint64_t);
     /**
    * @brief Отправка данных клиенту
    * @details Осуществляется отправка данных клиенту с помощью unique_ptr
    * @param [in] data Строка, которую необходимо отправить
    * @param [in] log_msg Строка, которая запишется в лог файл при возникновении ошибки во время отправки
    * @throw В случае возникновения ошибки обрывается соединение с клиентом и записывается соответствующее сообщение в лог файл
    */
    void send_data(std::string data,std::string msg);
     /**
    * @brief Прием данных от клиента
    * @details Осуществляется прием данных от клиента с помощью unique_ptr. Во внутреннем цикле обрабатывается ситуация, когда размер сообщения, больше размера буффера
    * @param [in] msg Строка для записи принятого сообщения
    * @throw В случае возникновения ошибки обрывается соединение с клиентом и записывается соответствующее сообщение в лог файл
    */
    std::string recv_data(std::string messg);
    /**
    * @brief Генерация хеша
    * @details Хеш генерируется на основе соли и пароля
    * @param [in] salt Соль, используемая для генерации
    * @param [in] pswd Пароль клиента, используемый для генерации
    * @return Функция возвращает сгенерированный хеш
    */
    std::string hash_gen(std::string &salt,std::string &password);
    /**
    * @brief Закрытие сокета
    * @details Метод для закрытия сокета клиента, используемый вне модуля communicator
    */
    void close_sock();
    /**
    * @brief Запуск сервера
    * @details Создание сокета сервера и его привязка к локальному адресу
    * @throw critical_error, если не удалось создать или привязать сокет
    */
    void start();
    /**
    * @brief Конструктор инициализации
    * @param [in] port Номер порта для привязки сокета
    * @param [in] base_loc Расположение базы данных
    * @param [in] log_loc Расположение лог файла
    * @details Инициализируется значение порта, расположения базы данных и лог файла
    */
    communicator(uint port,std::string base_loc,std::string log_loc);
};
/** @brief Класс обработчик данных
* @details Обработка данных, необходимых для вычислений: прием, отправка, вычисления
* Прием и отправка осуществляются в методе handle_data. Вычисления в методе calculation
*/
class data_handler{
    private:
    /**
    * @brief Объект модуля communicator
    * @details Используется для доступа к сокету клиента, чтобы отправлять результаты вычислений
    */
    communicator &server;
    /**
    * @brief Количество векторов, которые необходимо обработать
    */
    uint32_t nums;
    public:
    /**
    * @brief Объекта класса loger для ведения журнала
    */
    logger log;
    /**
    * @brief Расположение лог файла
    */
    std::string log_location;
    /**
    * @brief Переменная для хранения результата вычислений
    */
    double result;
    /**
    * @brief Калькулятор
    * @details Производит возведение элемента вектора в квадрат
    * @param [in] number Число полученное в ходе работы метода handle_data
    * @warning В случае переполнения возвращается максимум или минимум типа данных
    * @return Функция возвращает число, возведенное в квадрат
    */
    double calculation(double number);
    /**
    * @brief Обработка данных векторов
    * @details Производит прием данных векторов от клиента и отправка результата вычислений по этим данным
    * @warning В случае приема неверного типа данных соединение с клиентом обрывается
    */
    int handle_data();
     /**
    * @brief Конструктор инициализации
    * @details Инициализируется объект модуля communicator, расположение лог файла
    * Принимается от клиента значение числа векторов
    * @param [in] server Объект модуля communicator
    * @param [in] log Расположение лог файла
    * @warning В случае приема неверного типа данных соединение с клиентом обрывается
    */
    data_handler(communicator &server, std::string log);
};
