#include <iostream>
#include <memory>
#include <stack>

namespace DancingLink
{
    template <typename T>
    class _LinkNode //: public std::enable_shared_from_this<_LinkNode<T>>
    {
    public:
        T val;
        std::shared_ptr<_LinkNode<T>> sp_right;
        std::shared_ptr<_LinkNode<T>> sp_down;
        std::weak_ptr<_LinkNode<T>> sp_up;
        std::weak_ptr<_LinkNode<T>> sp_left;
        // Do NOT disable copy, we need push obj to stack
        // Disable temprorily to check rvalue functions is working or not.
        // _LinkNode(const _LinkNode<T> & a) noexcept;//=delete;
        // _LinkNode<T>& operator=(const _LinkNode<T> & a) noexcept;//=delete;

        _LinkNode() : val(){};
        _LinkNode(const T &&v) : val(v){};
        _LinkNode(_LinkNode<T> &&rval) : sp_right(rval.sp_right), sp_left(rval.sp_left), sp_up(rval.sp_up), sp_down(rval.sp_down)
        {
            rval.sp_right = rval.sp_down = nullptr;
            rval.sp_left.reset();
            rval.sp_up.reset();
        }

        //using std::enable_shared_from_this<_LinkNode<T>>::shared_from_this;
    };

    template<typename T>
    using pnode_t = typename std::shared_ptr<_LinkNode<T>>;

    template <typename T>
    class DancingLink
    {
    private:
        // node_t _origin;
        std::shared_ptr<_LinkNode<T>> sp_origin, sp_curr;
        std::stack<std::shared_ptr<_LinkNode<T>>> _state_stack;

    protected:
        //template<typename T>
        //using pnode = typename std::shared_ptr<_LinkNode<T>>;
        const pnode_t<T> clone_all()  noexcept;
        // const std::shared_ptr<_LinkNode<T>> clone_all() const noexcept;

    public:
        DancingLink() : _state_stack() {std::cout<<"DancingLink constractor"<<std::endl;}

        //Disable copy
        DancingLink(const DancingLink &) = delete;
        DancingLink &operator=(const DancingLink &) = delete;

        void push_state();
        bool pop_state();
    };

} // namespace DancingLink