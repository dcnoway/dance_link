#include "../include/dancing_link.hpp"

template<typename T>
_LinkNode<T>::_LinkNode(const _LinkNode<T> & v) noexcept
{
    std::cout<<" Copy constraction function called!" <<std::endl;
    val = v.val;
    left = v.left;
    right = v.right;
    up = v.up;
    down = v.down;
}

template<typename T>
_LinkNode<T> & _LinkNode<T>::operator=(const _LinkNode<T>& v) noexcept
{
    std::cout<<"Copy operator = function called!" <<std::endl;
    val = v.val;
    left = v.left;
    right = v.right;
    up = v.up;
    down = v.down;
    return *this;
}

//     //insert _LinkNode to given direction, return a reference to the inserted node
// template<typename T>
// _LinkNode<T>  _LinkNode<T>::insertRight(_LinkNode<T> && val) noexcept
// {
//     auto pval = std::make_shared<_LinkNode<T>>(val);
//     if(right!=nullptr){
//         //auto temp = right;
//         right->left = pval;
//         val.right = right;
//     }
//     right = pval;
//     val.left = std::make_shared<_LinkNode<T>>(this);
//     return std::move(val);
// }

// template<typename T>
// _LinkNode<T> _LinkNode<T>::insertLeft(_LinkNode<T> && val) noexcept
// {
//     auto pval = std::make_shared<_LinkNode<T>>(val);
//     auto pleft =left.lock();
//     if(pleft!=nullptr){
//         pleft->right = pval;
//         val.left = left;
//     }
//     left = pval;
//     val.right = std::make_shared<_LinkNode<T>>(this);
//     return std::move(val);
// }

// template<typename T>
// _LinkNode<T>  _LinkNode<T>::insertUp(_LinkNode<T> && val) noexcept
// {
//     auto pval = std::make_shared<_LinkNode<T>>(val);
//     auto pup = up.lock();
//     if(pup!=nullptr){
//         pup->down=pval;
//         val.up = up;
//     }
//     up = pval;
//     val.down = std::make_shared<_LinkNode<T>>(this);
//     return std::move(val);
// }

// template<typename T>
// _LinkNode<T>  _LinkNode<T>::insertDown(_LinkNode<T> && val) noexcept
// {
//     auto pval = std::make_shared<_LinkNode<T>>(val);
//     if(down!=nullptr){
//         down->up=pval;
//         val.down = down;
//     }
//     down = pval;
//     val.up = std::make_shared<_LinkNode<T>>(this);
//     return std::move(val);
// }

template<typename T>
void DancingLink<T>::push_state() 
{ 
    _state_stack.push(*p_origin); 
} //Incorrect, need impl

template<typename T>
bool DancingLink<T>::pop_state()
{
    p_origin = _state_stack.top();
    _state_stack.pop();
} //Incorrect, need impl