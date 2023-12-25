/** @file
* @author Стригин А.В.
* @version 1.0
* @date 23.12.2023
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модуля base
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "logger.h"
#include "error.h"
/** @brief Класс базы данных
* @details Осуществляется работа с базой данных.
* Чтение осуществляется в методе read_base
* Получение данных о пользователе в методах id_s и password_s
*/
class base{
    private:
    /** @brief Вектор для хранения логинов
    */
    std::vector<std::string> logins;
    /** @brief Вектор для хранения паролей
    */
    std::vector<std::string> passwords;
    /** @brief Вектор для хранения логинов/паролей
    */
    std::vector<std::string> clients;
    /**
    * @brief Метод разбора логинов
    * @details Из строки логин:пароль получается логин
    * @param [in] logins Вектор для хранения логинов
    */
    std::vector<std::string> id_s(std::vector<std::string> logins);
    /**
    * @brief Метод разбора паролей
    * @details Из строки логин:пароль получается пароль
    * @param [in] pswds Вектор для хранения паролей
    */
    std::vector<std::string> password_s(std::vector<std::string> pswds);  
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
    * @brief Объект потока ifstream для чтения базы данных
    */
    std::ifstream cl_base;
    /**
    * @brief Метод для получения вектора с логинами
    */
    std::vector<std::string> get_logins();
    /**
    * @brief Метод для получения вектора с паролями
    */
    std::vector<std::string> get_passwords();
    /**
    * @brief Метод чтения базы данных
    * @details Построчно считывается содержимое базы данных. Содержимое в формате логин/пароль
    * @param [in] base Объект потока ifstream с открытой базой
    * @throw critical_error, если в строке встретиться разделитель ":" больше 1 раза или не встретиться
    */
    void read_base(std::ifstream &b);
    /**
    * @brief Конструктор инициализации
    * @details Открывается файл базы данных
    * @param [in] base_loc Расположения файла базы данных
    * @param [in] log_loc Расположения лог файла
    * @throw critical_error, если не удалось открыть файл с базой данных
    */
    base(std::string base_loc,std::string log_loc);
    /**
    * @brief Деструктор класса
    * @details Закрывается файл с базой данных
    */
    ~base();
};