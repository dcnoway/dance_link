#include<iostream>
#include<string>
#include "dancing_link.hpp"

int main(int argc, char const *argv[])
{
    // std::string str =R"(C:\Windows)";
    // std::cout <<"Check raw string: "<<str <<std::endl;
    // std::cout<<"Check C++ standard version: "<<__cplusplus<<std::endl;
    // std::cout << "Word Length:" <<sizeof(&argc)*8<<" bit"<< std::endl;
    dlx::link<int> link;
    link.init_cols(10);

    auto p1 = link.insert(2,2,1);
    link.insert(3,1,31);
    link.insert(3,2,32);
    auto p2 = link.insert(3,3,33);

    std::cout <<"Before remove col header:"<<std::endl;
    std::cout <<"Col header addr:" << p1->wp_col.lock() <<std::endl;
    std::cout <<"Right of col is " <<*(p1->wp_col.lock()->sp_right)<<std::endl;
    std::cout <<"Left of col is " << *(p1->wp_col.lock()->wp_left.lock())<<std::endl;
    auto col = link.remove_col(p1->wp_col.lock());
    if(col){
        std::cout <<"After remove col header:" <<std::endl;
        std::cout <<"Right of col is " <<*(p1->wp_col.lock()->sp_right)<<std::endl;
        std::cout <<"Left of col is " <<*(p1->wp_col.lock()->wp_left.lock())<<std::endl;

        link.restore_col(col);
        std::cout <<"After restore col header:" <<std::endl;
        std::cout <<"Right of col is " <<*(p1->wp_col.lock()->sp_right)<<std::endl;
        std::cout <<"Left of col is " <<*(p1->wp_col.lock()->wp_left.lock())<<std::endl;
    }
    auto solution = link.solve();
    if(solution.has_value()){
        std::cout<<"done!"<<std::endl;
    }
    else std::cout<<"Can not solve the problem!"<<std::endl;
    return 0;
}
