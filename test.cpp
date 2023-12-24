#include <iostream>
#include <limits>
#include "includer.h"
#include <UnitTest++/UnitTest++.h>
#include <random>
#include <thread>
SUITE(logger_test){
    TEST(incorrect_path){
        logger log;
        CHECK_THROW(log.write_log("_!@#$%^","test"),critical_error);
    }
    TEST(correct_path){
        logger log;
        int temp = log.write_log("ll.txt","test");
        CHECK_EQUAL(0,temp);
    }
    TEST(empty_path){
        logger log;
        CHECK_THROW(log.write_log("","test"),critical_error);
    }
    TEST(newline_in_path){
        logger log;
        CHECK_THROW(log.write_log("lll.txt\n","test"),critical_error);
    }
}
SUITE(data_handler_test){
    TEST(max_overflow){
        communicator server(5555,"b.txt","ll.txt");
        data_handler handle(server,"ll.txt");
        double temp = std::numeric_limits<double>::max();
        double res = handle.calculation(temp);
        CHECK_EQUAL(std::numeric_limits<double>::max(),res);

    }
    TEST(min_overflow){
        communicator server(5555,"b.txt","ll.txt");
        data_handler handle(server,"ll.txt");
        double temp = std::numeric_limits<double>::lowest();
        double res = handle.calculation(temp);
        CHECK_EQUAL(std::numeric_limits<double>::max(),res);

    }
    TEST(standart_numbers){
        communicator server(5555,"b.txt","ll.txt");
        data_handler handle(server,"ll.txt");
        double temp = 451.1;
        double res = handle.calculation(temp);
        std::cout<<res<<std::endl;
        CHECK_EQUAL(temp*temp,res);

    }
    TEST(negative_num){
        communicator server(5555,"b.txt","ll.txt");
        data_handler handle(server,"ll.txt");
        double temp = -345.7;
        double res = handle.calculation(temp);
        CHECK_EQUAL(temp*temp,res);
    }
}
SUITE(communicator_test){
    TEST(salt_gen_test){
        communicator serv (3333,"b.txt","ll.txt");
        std::string salt1 = serv.SALT_generate();
        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
        std::string salt2 = serv.SALT_generate();
        if(salt1==salt2){
            CHECK(false);
        }
        else{
            CHECK(true);
        }
    }
    TEST(len_salt){
        communicator serv (3333,"b.txt","ll.txt");
        std::string salt1 = serv.SALT_generate();
        if(salt1.length()==16){
            CHECK(true);
        }
        else{
            CHECK(false);
        }
    }
    TEST(diff_salt_pass_md5){
        communicator serv (3333,"b.txt","ll.txt");
        std::string salt1 = serv.SALT_generate();
        std::string p1="qwerty";
        std::chrono::milliseconds duration(1000);
        std::this_thread::sleep_for(duration);
        std::string salt2 = serv.SALT_generate();
        std::string p2="abcdef";
        if(serv.hash_gen(salt1,p1)!=serv.hash_gen(salt2,p2)){
            CHECK(true);
        }
        else{
            CHECK(false);
        }

    }
    TEST(eq_salt_pass_md5){
        communicator serv (3333,"b.txt","ll.txt");
        std::string salt1 = serv.SALT_generate();
        std::string p1="qwerty";
        if(serv.hash_gen(salt1,p1)==serv.hash_gen(salt1,p1)){
            CHECK(true);
        }
        else{
            CHECK(false);
        }
    }

}
SUITE(base_test){
    TEST(correct_path){
        CHECK_THROW(base b("b.txt","ll.txt"),critical_error);
    }
    TEST(incorrect_path){
        CHECK_THROW(base b("a.txt","ll.txt"),critical_error);
    }
    TEST(newline_in_path){
        CHECK_THROW(base b("b\n.txt","ll.txt"),critical_error);
    }
    TEST(empty_path){
        CHECK_THROW(base b("","ll.txt"),critical_error);
    }
    TEST(get_logins){
        base b("b.txt","ll.txt");
        std::vector<std::string> logins = b.get_logins();
        CHECK_EQUAL("login",logins[0]);
    }
    TEST(get_passwords){
        base b("b.txt","ll.txt");
        std::vector<std::string> pass = b.get_passwords();
        CHECK_EQUAL("password",pass[0]);
    }
}
int main(){
    return UnitTest::RunAllTests();
}