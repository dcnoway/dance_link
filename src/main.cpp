#include<iostream>
#include "dancing_link.hpp"

int main(int argc, char const *argv[])
{
    std::string str =R"(C:\Windows)";
    std::cout <<"windows path is "<<str <<std::endl;
    std::cout<<"C++ version: "<<__cplusplus<<std::endl;
    std::cout << "main()" << std::endl;
    dlx::link<int> link;
    link.init_cols(10);
    std::cout << std::boolalpha<< link.empty() <<std::endl;
    link.delete_all();
    std::cout << std::boolalpha<< link.empty() <<std::endl;
    return 0;
}
