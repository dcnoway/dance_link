#include <iostream>
#include <memory>
#include <stack>

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
        pnode_t<T> mark_remove(pnode_t<T>);
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

        if(auto p = sp_head;p->sp_right!=sp_head){

        }
        return nullptr;
    }

    template<typename T>
    pnode_t<T> link<T>::mark_remove(const size_t col, const size_t row)
    {
        return nullptr;
    }
    template<typename T>
    pnode_t<T> link<T>::mark_remove(pnode_t<T> p)
    {
        return p;
    }

} // namespace link