#include "../include/dancing_link.hpp"

template<typename T>
LinkNode<T>::LinkNode(const LinkNode<T> & v) noexcept
{
    std::cout<<" Copy constraction function called!" <<std::endl;
    val = v.val;
    left = v.left;
    right = v.right;
    up = v.up;
    down = v.down;
}

template<typename T>
LinkNode<T> & LinkNode<T>::operator=(const LinkNode<T>& v) noexcept
{
    std::cout<<"Copy operator = function called!" <<std::endl;
    val = v.val;
    left = v.left;
    right = v.right;
    up = v.up;
    down = v.down;
    return *this;
}

    //insert LinkNode to given direction, return a reference to the inserted node
template<typename T>
LinkNode<T>  LinkNode<T>::insertRight(LinkNode<T> && val) noexcept
{
    auto pval = std::make_shared<LinkNode<T>>(val);
    if(right!=nullptr){
        //auto temp = right;
        right->left = pval;
        val.right = right;
    }
    right = pval;
    val.left = std::make_shared<LinkNode<T>>(this);
    return std::move(val);
}

template<typename T>
LinkNode<T> LinkNode<T>::insertLeft(LinkNode<T> && val) noexcept
{
    auto pval = std::make_shared<LinkNode<T>>(val);
    auto pleft =left.lock();
    if(pleft!=nullptr){
        pleft->right = pval;
        val.left = left;
    }
    left = pval;
    val.right = std::make_shared<LinkNode<T>>(this);
    return std::move(val);
}

template<typename T>
LinkNode<T>  LinkNode<T>::insertUp(LinkNode<T> && val) noexcept
{
    auto pval = std::make_shared<LinkNode<T>>(val);
    auto pup = up.lock();
    if(pup!=nullptr){
        pup->down=pval;
        val.up = up;
    }
    up = pval;
    val.down = std::make_shared<LinkNode<T>>(this);
    return std::move(val);
}

template<typename T>
LinkNode<T>  LinkNode<T>::insertDown(LinkNode<T> && val) noexcept
{
    auto pval = std::make_shared<LinkNode<T>>(val);
    if(down!=nullptr){
        down->up=pval;
        val.down = down;
    }
    down = pval;
    val.up = std::make_shared<LinkNode<T>>(this);
    return std::move(val);
}