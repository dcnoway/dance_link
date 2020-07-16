#include <iostream>
#include <memory>

class DanceLink
{
    private:
    protected:
    public:
};

template<typename T>
class LinkNode
{
    private:
    T val;
    std::shared_ptr<LinkNode> right;
    std::shared_ptr<LinkNode> down;
    std::weak_ptr<LinkNode> up;
    std::weak_ptr<LinkNode> left;
    public:
    // Do NOT disable copy, we need push obj to stack
    // Disable temprorily to check rvalue functions is working or not.
    LinkNode(const LinkNode & a)=delete;
    LinkNode& operator=(const LinkNode & a)=delete;

    LinkNode():val(){};
    LinkNode(const T && v):val(v){};
    LinkNode(LinkNode && rval):right(rval.right),left(rval.left),up(rval.up),down(rval.down){
        rval.up=rval.down=rval.left=rval.right=nullptr;
    }

    //insert LinkNode to given direction, return a reference to the inserted node
    LinkNode && insertRight(LinkNode && val) noexcept;
    LinkNode && insertLeft(LinkNode && val) noexcept;
    LinkNode && insertUp(LinkNode && val) noexcept;
    LinkNode && insertDown(LinkNode && val) noexcept;

    //delete LinkNode on the given direction, return a reference to the deleted node
    LinkNode && removeRight();
    LinkNode && removeLeft();
    LinkNode && removeUp();
    LinkNode && removeDown();
};

