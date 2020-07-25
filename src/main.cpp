#include<iostream>
#include<string>
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

    link.init_cols(10);
    auto p1 = link.insert(2,2,1);
    std::cout<<*p1<<std::endl;

    link.insert(3,1,31);
    link.insert(3,2,32);
    auto p2 = link.insert(3,3,33);
    if(p2){
        std::cout<<"After insert"<<std::endl<<"Node:";
        std::cout<<*p2;
        std::cout<<"Column header:";
        std::cout<<*(p2->wp_up.lock());

        p2 = link.mark_remove(p2);
        std::cout<<"After mark_remove"<<std::endl<<"Node:";
        std::cout<<*p2;
        std::cout<<"Column header:";
        std::cout<<*(p2->wp_up.lock());

        p2 = link.mark_restore(p2);
        std::cout<<"After mark_restore"<<std::endl<<"Node:";
        std::cout<<*p2;
        std::cout<<"Column header:";
        std::cout<<*(p2->wp_up.lock());
    }
    return 0;
}
