#include <iostream>
#include <memory>
#include <stack>

template<typename T>
class _LinkNode:public std::enable_shared_from_this<_LinkNode<T>>
{
    public:
    T val;
    std::shared_ptr<_LinkNode<T>> sp_right;
    std::shared_ptr<_LinkNode<T>> sp_down;
    std::weak_ptr<_LinkNode<T>> sp_up;
    std::weak_ptr<_LinkNode<T>> sp_left;
    // Do NOT disable copy, we need push obj to stack
    // Disable temprorily to check rvalue functions is working or not.
    _LinkNode(const _LinkNode<T> & a) noexcept;//=delete;
    _LinkNode<T>& operator=(const _LinkNode<T> & a) noexcept;//=delete;

    _LinkNode():val(){};
    _LinkNode(const T && v):val(v){};
    _LinkNode(_LinkNode<T> && rval):sp_right(rval.sp_right),sp_left(rval.sp_left),sp_up(rval.sp_up),sp_down(rval.sp_down){
        rval.sp_right = rval.sp_down = nullptr;
        rval.sp_left.reset();
        rval.sp_up.reset();
    }

    using std::enable_shared_from_this<_LinkNode<T>>::shared_from_this;
};


template<typename T>
class DancingLink
{
    using node_t = _LinkNode<T>;
    private:
    node_t _origin;
    std::shared_ptr<node_t> sp_origin, sp_curr;
    std::stack<node_t> _state_stack;
    protected:
    public:
    DancingLink():_state_stack(){sp_origin=std::make_shared(_origin);sp_curr=sp_origin;}

    //Disable copy
    DancingLink(const DancingLink &)=delete;
    DancingLink & operator = (const DancingLink &)=delete;

    void push_state(){_state_stack.push(*sp_origin);}//Incorrect, need impl
    bool pop_state(){sp_origin=_state_stack.top();_state_stack.pop();}//Incorrect, need impl
};
