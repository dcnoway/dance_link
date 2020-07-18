#include <iostream>
#include <memory>
#include <stack>

template<typename T>
class _LinkNode
{
    private:
    T val;
    std::shared_ptr<_LinkNode<T>> right;
    std::shared_ptr<_LinkNode<T>> down;
    std::weak_ptr<_LinkNode<T>> up;
    std::weak_ptr<_LinkNode<T>> left;
    public:
    // Do NOT disable copy, we need push obj to stack
    // Disable temprorily to check rvalue functions is working or not.
    _LinkNode(const _LinkNode<T> & a) noexcept;//=delete;
    _LinkNode<T>& operator=(const _LinkNode<T> & a) noexcept;//=delete;

    _LinkNode():val(){};
    _LinkNode(const T && v):val(v){};
    _LinkNode(_LinkNode<T> && rval):right(rval.right),left(rval.left),up(rval.up),down(rval.down){
        rval.right = rval.down = nullptr;
        rval.left.reset();
        rval.up.reset();
    }
};


template<typename T>
class DancingLink
{
    using node_t = _LinkNode<T>;
    private:
    node_t _origin;
    std::shared_ptr<node_t> p_origin;
    std::stack<node_t> _state_stack;
    protected:
    public:
    DancingLink(){_origin=std::make_shared(_origin);}

    //Disable copy
    DancingLink(const DancingLink &)=delete;
    DancingLink & operator = (const DancingLink &)=delete;

    void push_state(){_state_stack.push(*p_origin);}//Incorrect, need impl
    bool pop_state(){p_origin=_state_stack.top();_state_stack.pop();}//Incorrect, need impl
};
