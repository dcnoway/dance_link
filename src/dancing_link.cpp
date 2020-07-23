#include "../include/dancing_link.hpp"

// template<typename T>
// _LinkNode<T>::_LinkNode(const _LinkNode<T> & v) noexcept
// {
//     std::cout<<" Copy constraction function called!" <<std::endl;
//     val = v.val;
//     sp_left = v.sp_left;
//     sp_right = v.sp_right;
//     sp_up = v.sp_up;
//     sp_down = v.sp_down;
// }

// template<typename T>
// _LinkNode<T> & _LinkNode<T>::operator=(const _LinkNode<T>& v) noexcept
// {
//     std::cout<<"Copy operator = function called!" <<std::endl;
//     val = v.val;
//     sp_left = v.sp_left;
//     sp_right = v.sp_right;
//     sp_up = v.sp_up;
//     sp_down = v.sp_down;
//     return *this;
// }

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

namespace DancingLink
{

    template <typename T>
    void DancingLink<T>::push_state()
    {
        _state_stack.push(*sp_origin);
    } //Incorrect, need impl

    template <typename T>
    bool DancingLink<T>::pop_state()
    {
        if (_state_stack.emplace())
            return false;
        sp_origin = _state_stack.top();
        _state_stack.pop();
        return true;
    } //Incorrect, need impl

    //        const std::shared_ptr<_LinkNode<T>> clone_all() const noexcept;
    // template <typename T>
    // const std::shared_ptr<_LinkNode<T>> DancingLink<T>::clone_all() const noexcept
    // {
    // }

    // template <typename T>
    // using pnode = typename std::shared_ptr<_LinkNode<T>>;

    template <typename T>
    const pnode_t<T> DancingLink<T>::clone_all()  noexcept
    {
        return nullptr;
    }

} // namespace DancingLink