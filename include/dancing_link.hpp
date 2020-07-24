#include <iostream>
#include <memory>
#include <stack>
#include "ptr_util.hpp"

namespace dlx
{
    /*
    Rules for smart pointers in dancing link nodes
    1. sp_right sp_down using shared_ptr
    2. wp_left wp_up using weak_pter
    3. wp_col using weak_pter
    4. there are only bidirectional link, NO circle link
    */
    template <typename T>
    class node //: public std::enable_shared_from_this<node<T>>
    {
    public:
        T val;
        std::shared_ptr<node<T>> sp_right;
        std::shared_ptr<node<T>> sp_down;
        std::weak_ptr<node<T>> wp_up;
        std::weak_ptr<node<T>> wp_left;
        std::weak_ptr<node<T>> wp_col;
        size_t row,col;

        node() : val(){};
        node(const T &&v) : val(v), row(0),col(0),
            sp_right(nullptr),sp_down(nullptr){};

        node(node<T> &&rval) : val(rval.val), row(0), col(0),
            sp_right(rval.sp_right), sp_down(rval.sp_down)
        {
            rval.sp_right = rval.sp_down = nullptr;
            rval.wp_left.reset();
            rval.wp_up.reset();
            rval.wp_col.reset();
        }

        //using std::enable_shared_from_this<node<T>>::shared_from_this;
    };

    template<typename T>
    using pnode_t = typename std::shared_ptr<node<T>>;

    template <typename T>
    class link
    {
    private:
        std::shared_ptr<node<T>> sp_head, sp_tail;
        std::stack<pnode_t<T>> _state_stack;


    protected:
        const pnode_t<T> clone_all()  noexcept;

    public:
        //Disable copy
        link(const link &) = delete;
        link &operator=(const link &) = delete;

        //Constractors
        link() : _state_stack() 
        {
            std::cout << "link constractor link()" << std::endl;
            sp_tail = sp_head = std::make_shared<node<T>>(0);
        }

        link(const size_t num_of_cols): _state_stack()
        {
            std::cout << "link constractor link()" << std::endl;
            sp_tail = sp_head = std::make_shared<node<T>>(0);
            init_cols(num_of_cols);
        }

        //column initlizer
        void init_cols(const size_t num_of_cols);

        //link state functions
        void delete_all();
        bool empty(){return (sp_head->sp_right == nullptr);}

        //insert operation
        pnode_t<T> insert(const size_t col,const size_t row, const T val);

        //MARK REMOVE operation
        pnode_t<T> mark_remove(const size_t col,const size_t row);
        pnode_t<T> mark_remove(const pnode_t<T>);

        //MARK RESTORE operation
        pnode_t<T> mark_restore(const pnode_t<T>);
    };


    template <typename T>
    const pnode_t<T> link<T>::clone_all()  noexcept
    {
        std::cout << "Not implemented yet!" << std::endl;
        return nullptr;
    }

    template <typename T>
    void link<T>::init_cols(const size_t  num_of_cols)
    {
        if(empty()){
            //auto sp_curr = sp_head;
            for(size_t i =1;i<= num_of_cols;++i){
                //new a node
                auto pnew = std::make_shared<node<T>>(0);
                //insert node into column nodes
                sp_tail->sp_right = pnew;
                pnew->wp_left = sp_tail;

                //init row and col variants
                pnew->row = 0;
                pnew->col = i;
                //move cursor to the new created node
                sp_tail = pnew;
            }
        }
        else
        {
            throw "Call link<T>::init_cols() while link is NOT empty!";
        }
        
    }

    template<typename T>
    void link<T>::delete_all()
    {
        if(sp_head!=nullptr){
            sp_head->wp_left.reset();
            sp_tail=sp_head;
            sp_head->sp_right=nullptr;
        }
    }

    template<typename T>
    pnode_t<T> link<T>::insert(const size_t col, const size_t row,const T val)
    {
        if(empty())throw "Call insert(col,row,val) on a empty link object";
        if(sp_head->wp_left.lock()->col<col)
            throw "insert a node with a col greater than max col of link";
        
        auto pnew = std::make_shared<node<T>>(val);
        pnew->col = col;
        pnew->row = row;

        //find the col header node
        auto p = sp_head;
        while(p!=nullptr){
            if(p->col == col){
                pnew->wp_col = p;
                break;
            }
            p=p->sp_right;
        }

        //find the last row node that has a row is the greatest of the all the node in the col nodes link which row is less than p
        //in the 1st iter, p must be the col header that has a ZERO row variant
        while (p->sp_down != nullptr)
        {
            if(row < p->sp_down->row)break;
            else if(row == p->sp_down->row){
                std::cerr<<"link<T>::insert(col,row) insert a node in a exists node with same col and row!"<<std::endl;
                break;
            }
            p = p->sp_down;
        }

        //the p var MUST NOT nullptr
        if(p == nullptr)return nullptr;

        pnew->wp_up = p;
        pnew->sp_down = p->sp_down;

        p->sp_down = pnew;
        if(pnew->sp_down!=nullptr)pnew->sp_down->wp_up = pnew;

        //find the left and right node in the same row
        //find left node in the same row
        pnode_t<T> pleft = nullptr;
        p = pnew->wp_col.lock();
        while(nullptr != (p = p->wp_left.lock())){
            if(pleft!=nullptr)break;
            while(p->sp_down!=nullptr){
                p = p->sp_down;
                if(p->row == row){
                    pleft = p;
                    break;
                }
                else if(p->row >row)break;
            }
        }
        if(pleft!=nullptr){
            pnew->wp_left = pleft;
            pleft->sp_right = pnew;
        }
        //find right node in the same row
        pnode_t<T> pright  = nullptr;
        p = pnew->wp_col.lock();
        while(nullptr != p->sp_right){
            if(pright!=nullptr)break;
            p = p->sp_right;
            while(p->sp_down!=nullptr){
                p = p->sp_down;
                if(p->row == row){
                    pright = p;
                    break;
                }
                else if(p->row > row) break;
            }
        }
        if(pright!=nullptr)
        {
            pnew->sp_right = pright;
            pright->wp_left = pnew;
        }

        return pnew;
    }

    //TODO: reine the remove of col header node
    template<typename T>
    pnode_t<T> link<T>::mark_remove(const size_t col, const size_t row)
    {
        auto p = sp_head;
        while(p!=nullptr){
            if(p->col == col){
                while(p!=nullptr){
                    if(p->row == row)
                        return mark_remove(p);
                    p = p->sp_down;
                }
            }
            p = p->sp_right;
        }
        return nullptr;
    }

    //TODO: reine the remove of col header node
    template<typename T>
    pnode_t<T> link<T>::mark_remove(const pnode_t<T> p)
    {
        //Check p validation
        if(!is_uninitialized<node<T>>(p->wp_up)
            && p->wp_up.expired()){
            std::cerr<< "link<T>::mark_remove() on a node that has a expired weak_ptr to up node"<<std::endl;
            return nullptr;
        }
        if(!is_uninitialized<node<T>>(p->wp_left) //
            && p->wp_left.expired()){
            std::cerr <<"link<T>::mark_remove() on a node that has a expired weak_ptr to left"<<std::endl;
            return nullptr;
        }

        //col header node
        if(is_uninitialized<node<T>>(p->wp_up)){
            if(p->row != 0)std::cerr << "link<T>::mark_remove(), remove a node without wp_up and col is NOT ZERO"<<std::endl;
            return nullptr;
        }
        else {
            p -> wp_up.lock()->sp_down = p->sp_down;
        }

        if(!is_uninitialized<node<T>>(p->wp_left)){
            p -> wp_left.lock() -> sp_right = p->sp_right;
        }
        
        //p is NOT the last node in the col
        if(p->sp_down != nullptr)
            p->sp_down->wp_up = p->wp_up.lock();

        if(p->sp_right != nullptr)
            p->sp_right->wp_left = p->wp_left.lock();

        return p;
    }

    template<typename T>
    pnode_t<T> link<T>::mark_restore(const pnode_t<T> p)
    {
        pnode_t<T> ptr;
        if(nullptr != (ptr =p->wp_up.lock())){
            ptr->sp_down = p;
        }
        if(nullptr != (ptr = p->wp_left.lock())){
            ptr ->sp_right = p;
        }
        if(nullptr != p->sp_right){
            p->sp_right->wp_left = p;
        }
        if(nullptr != p->sp_down){
            p->sp_down->wp_up = p;
        }
    }

} // namespace link