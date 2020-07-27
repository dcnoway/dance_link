#ifndef __DANCING_LINK
#define __DANCING_LINK

#include <iostream>
#include <memory>
#include <stack>
#include <optional>
#include <vector>
#include "ptr_util.hpp"

namespace dlx
{
    /*
    Rules for smart pointers in dancing link nodes
    1. sp_right sp_down using shared_ptr
    2. wp_left wp_up using weak_pter
    3. wp_col using weak_pter
    4. NO circle link with std::shared_ptr
    */
    template <typename T>
    class node //: public std::enable_shared_from_this<node<T>>
    {
    public:
        T val;
        unsigned row, col;
        std::shared_ptr<node<T>> sp_right;
        std::shared_ptr<node<T>> sp_down;
        std::weak_ptr<node<T>> wp_up;
        std::weak_ptr<node<T>> wp_left;
        std::weak_ptr<node<T>> wp_col;

        node() : val(){};
        node(const T &v) : val(v), row(0), col(0),
                           sp_right(nullptr), sp_down(nullptr){};
        node(const T &&v) : val(v), row(0), col(0),
                            sp_right(nullptr), sp_down(nullptr){};

        node(node<T> &&rval) : val(rval.val), row(0), col(0),
                               sp_right(rval.sp_right), sp_down(rval.sp_down)
        {
            rval.sp_right = rval.sp_down = nullptr;
            rval.wp_left.reset();
            rval.wp_up.reset();
            rval.wp_col.reset();
        }
        friend std::ostream &operator<<(std::ostream &os, const node<T> &val)
        {
            os << "node<T>{col=" << val.col << ", row=" << val.row << ", val=" << val.val << ", sp_right=" << val.sp_right.get() << ", sp_down=" << val.sp_down.get();
            os << ", wp_up=" << (val.wp_up.expired() ? nullptr : val.wp_up.lock().get());
            os << ", wp_left=" << (val.wp_left.expired() ? nullptr : val.wp_left.lock().get());
            os << ", wp_col=" << (val.wp_col.expired() ? nullptr : val.wp_col.lock().get());
            os << "}" << std::endl;
            return os;
        }

        //using std::enable_shared_from_this<node<T>>::shared_from_this;
    };

    template <typename T>
    using pnode_t = typename std::shared_ptr<node<T>>;

    /*
    TODO:Next version, 
        1,use a STL dynamic container to store shared_ptrs of all the nodes
        2,use raw ptr as up,down,left,right and col
        because I think smart pointer is designed for RAII and prevent dangling pinter 
    */
    template <typename T>
    class link
    {
    private:
        pnode_t<T> sp_head, sp_tail;
        std::stack<pnode_t<T>> _answers;

        // return tuple is <up,down,left,right>
        // nullptr means that the node with given coordinate is the edge node on that direction
        std::tuple<pnode_t<T>, pnode_t<T>, pnode_t<T>, pnode_t<T>> find_neighbour(const unsigned col, const unsigned row);
        pnode_t<T> find_col_head(const unsigned col);

    protected:
        const pnode_t<T> clone_all() noexcept;
        bool _solve();

    public:
        //Disable copy
        link(const link &) = delete;
        link &operator=(const link &) = delete;

        //Constractors
        link() : _answers()
        {
            // std::cout << "link constractor link()" << std::endl;
            sp_tail = sp_head = std::make_shared<node<T>>(0);
            sp_head->wp_left = sp_head->wp_up = sp_head;
        }

        link(const unsigned num_of_cols) : _answers()
        {
            // std::cout << "link constractor link()" << std::endl;
            sp_tail = sp_head = std::make_shared<node<T>>(0);
            sp_head->wp_left = sp_head->wp_up = sp_head;
            init_cols(num_of_cols);
        }

        //column initlizer
        void init_cols(const unsigned num_of_cols);

        //link state functions
        void delete_all();
        bool empty() { return (sp_head->sp_right == nullptr); }

        //insert operation
        pnode_t<T> insert(const unsigned col, const unsigned row, const T val);

        //remove operation
        pnode_t<T> remove(const unsigned col, const unsigned row);
        pnode_t<T> remove(const pnode_t<T>);

        //restore operation
        pnode_t<T> restore(const pnode_t<T>);

        //remove col
        pnode_t<T> remove_col(const pnode_t<T>);
        //restore col
        pnode_t<T> restore_col(const pnode_t<T>);

        //solve problem function
        void append_row(const std::vector<unsigned> &cols, const std::vector<unsigned> &vals);
        std::optional<std::vector<pnode_t<T>>> solve();
    };

    template <typename T>
    const pnode_t<T> link<T>::clone_all() noexcept
    {
        std::cout << "Not implemented yet!" << std::endl;
        return nullptr;
    }

    template <typename T>
    void link<T>::init_cols(const unsigned num_of_cols)
    {
        if (empty())
        {
            //auto sp_curr = sp_head;
            for (unsigned i = 1; i <= num_of_cols; ++i)
            {
                //new a node
                auto pnew = std::make_shared<node<T>>(0);
                //insert node into column nodes
                sp_tail->sp_right = pnew;
                pnew->wp_left = sp_tail;

                pnew->wp_up = pnew;

                //init row and col variants
                pnew->row = 0;
                pnew->col = i;
                //move cursor to the new created node
                sp_head->wp_left = sp_tail = pnew;
            }
        }
        else
        {
            throw "Call link<T>::init_cols() while link is NOT empty!";
        }
    }

    template <typename T>
    void link<T>::delete_all()
    {
        if (sp_head != nullptr)
        {
            sp_head->wp_left.reset();
            sp_tail = sp_head;
            sp_head->sp_right = nullptr;
        }
    }

    template <typename T>
    pnode_t<T> link<T>::insert(const unsigned col, const unsigned row, const T val)
    {
        if (empty())
            throw "Call insert(col,row,val) on a empty link object";
        if (sp_tail->col < col)
            throw "insert a node with a col greater than max col of link";

        auto pnew = std::make_shared<node<T>>(val);
        pnew->col = col;
        pnew->row = row;

        auto [pup, pdown, pleft, pright] = find_neighbour(col, row);
        pnew->wp_up = pup;
        pnew->sp_down = pdown;
        pnew->wp_left = pleft;
        pnew->sp_right = pright;
        auto pcol = pup->wp_col.lock();
        pnew->wp_col = pcol;

        //pdown == nullptr: this node is the last node in the col
        if (pdown == nullptr)
            pcol->wp_up = pnew; //set col header ->wp_up to this new node

        //pright == nullptr: this node is the last node in the row
        if (pright == nullptr)
        {
            //set this row's first node->wp_left to this new node

            //seek to the first node in this row
            pnode_t<T> pl;
            for (pl = pnew; pl->wp_left.lock() != nullptr; pl = pl->wp_left.lock())
                ;
            pl->wp_left = pnew;
        }
        return pnew;

        // //find the col header node
        // auto p = sp_head;
        // while (p != nullptr)
        // {
        //     if (p->col == col)
        //     {
        //         pnew->wp_col = p;
        //         break;
        //     }
        //     p = p->sp_right;
        // }

        // //find the last row node that has a row is the greatest of the all the node in the col nodes link which row is less than p
        // //in the 1st iter, p must be the col header that has a ZERO row variant
        // while (p->sp_down != nullptr)
        // {
        //     if (row < p->sp_down->row)
        //         break;
        //     else if (row == p->sp_down->row)
        //     {
        //         std::cerr << "link<T>::insert(col,row) insert a node in a exists node with same col and row!" << std::endl;
        //         break;
        //     }
        //     p = p->sp_down;
        // }

        // //the p var MUST NOT nullptr
        // if (p == nullptr)
        //     return nullptr;

        // pnew->wp_up = p;
        // pnew->sp_down = p->sp_down;

        // p->sp_down = pnew;
        // if (pnew->sp_down != nullptr)
        //     pnew->sp_down->wp_up = pnew;
        // else pnew->wp_col.lock()->wp_up = pnew;

        // //find the left and right node in the same row
        // //find left node in the same row
        // pnode_t<T> pleft = nullptr;
        // p = pnew->wp_col.lock();
        // while (nullptr != (p = p->wp_left.lock()))
        // {
        //     if (pleft != nullptr)
        //         break;
        //     while (p->sp_down != nullptr)
        //     {
        //         p = p->sp_down;
        //         if (p->row == row)
        //         {
        //             pleft = p;
        //             break;
        //         }
        //         else if (p->row > row)
        //             break;
        //     }
        // }
        // if (pleft != nullptr)
        // {
        //     pnew->wp_left = pleft;
        //     pleft->sp_right = pnew;
        // }
        // else{
        //     //find the rightest node in the same row
        //     //TODO:
        // }
        // //find right node in the same row
        // pnode_t<T> pright = nullptr;
        // p = pnew->wp_col.lock();
        // while (nullptr != p->sp_right)
        // {
        //     if (pright != nullptr)
        //         break;
        //     p = p->sp_right;
        //     while (p->sp_down != nullptr)
        //     {
        //         p = p->sp_down;
        //         if (p->row == row)
        //         {
        //             pright = p;
        //             break;
        //         }
        //         else if (p->row > row)
        //             break;
        //     }
        // }
        // if (pright != nullptr)
        // {
        //     pnew->sp_right = pright;
        //     pright->wp_left = pnew;
        // }

        // return pnew;
    }

    template <typename T>
    pnode_t<T> link<T>::remove(const unsigned col, const unsigned row)
    {
        //the head node can not be removed
        if ((0 == col) && (0 == row))
            return nullptr;

        auto p = sp_head;
        while (p != nullptr)
        {
            if (p->col == col)
            {
                while (p != nullptr)
                {
                    if (p->row == row)
                        return remove(p);
                    p = p->sp_down;
                }
            }
            p = p->sp_right;
        }
        return nullptr;
    }

    template <typename T>
    pnode_t<T> link<T>::remove(const pnode_t<T> p)
    {
        if (0 == p->row)
        {
            std::cerr << "remove of col header is NOT supported!" << std::endl;
            return nullptr;
        }

        //Check p validation
        if (!is_uninitialized<node<T>>(p->wp_up) && p->wp_up.expired())
        {
            std::cerr << "link<T>::mark_remove() on a node that has a expired weak_ptr to up node" << std::endl;
            return nullptr;
        }
        if (!is_uninitialized<node<T>>(p->wp_left) && p->wp_left.expired())
        {
            std::cerr << "link<T>::mark_remove() on a node that has a expired weak_ptr to left" << std::endl;
            return nullptr;
        }

        if (auto ptr = p->wp_up.lock())
            ptr->sp_down = p->sp_down;

        if (auto ptr = p->wp_left.lock())
            ptr->sp_right = p->sp_right;

        //p is NOT the last node in the col
        if (p->sp_down != nullptr)
            p->sp_down->wp_up = p->wp_up.lock();
        else //p is the last node in the col, set p_up of col header to the up node of p
            p->wp_col.lock()->wp_up = p->wp_up.lock();

        if (p->sp_right != nullptr)
            p->sp_right->wp_left = p->wp_left.lock();
        else //p is the last node in the row, set this row's first node->p_left to the left node of p
        {
            //seek to the first node of this row
            //first node ->p_left == p
            pnode_t<T> pl;
            for (pl = p; pl->wp_left.lock() != p; pl = pl->wp_left.lock())
                ;
            pl->wp_left = p->wp_left.lock();
        }

        return p;
    }

    template <typename T>
    pnode_t<T> link<T>::restore(const pnode_t<T> p)
    {
        if (!is_uninitialized<node<T>>(p->wp_up) &&
            p->wp_up.expired())
            return nullptr;
        if (!is_uninitialized<node<T>>(p->wp_left) &&
            p->wp_left.expired())
            return nullptr;
        pnode_t<T> ptr;
        if (nullptr != (ptr = p->wp_up.lock()))
        {
            ptr->sp_down = p;
        }

        if (nullptr != (ptr = p->wp_left.lock()))
        {
            ptr->sp_right = p;
        }

        if (nullptr != p->sp_right)
        {
            p->sp_right->wp_left = p;
        }
        else
        {
            for (ptr = p; ptr->wp_left.lock()->sp_right != nullptr; ptr = ptr->wp_left.lock())
                ;
            ptr->wp_left = p;
        }

        if (nullptr != p->sp_down)
        {
            p->sp_down->wp_up = p;
        }
        else
        {
            p->wp_col.lock()->wp_up = p;
        }

        return p;
    }

    //remove all node under col header p, and all node that have the same row with these
    template <typename T>
    pnode_t<T> link<T>::remove_col(const pnode_t<T> col)
    {
        if (col->row != 0)
            return nullptr;

        //remove the col header col
        if (auto ptr = col->wp_left.lock())
            ptr->sp_right = col->sp_right;
        if (col->sp_right != nullptr)
            col->sp_right->wp_left = col->wp_left.lock();
        else
        {
            //this col header is the last col header
            sp_head->wp_left = col->wp_left.lock();
        }

        //iter all row nodes under col
        for (pnode_t<T> row = col->sp_down; row != nullptr; row = row->sp_down)
        {
            //For a given row node
            //remove all node on the left side
            for (pnode_t<T> p = row->wp_left.lock(); p != nullptr; p = p->wp_left.lock())
            {
                if (nullptr != p->sp_down)
                    p->sp_down->wp_up = p->wp_up.lock();
                else
                {
                    p->wp_col.lock()->wp_up = p->wp_up.lock();
                }

                if (auto ptr = p->wp_up.lock())
                    ptr->sp_down = p->sp_down;
            }
            //remove all node on the right side
            for (pnode_t<T> p = row->sp_right; p != nullptr; p = p->sp_right)
            {
                if (nullptr != p->sp_down)
                    p->sp_down->wp_up = p->wp_up.lock();
                else
                {
                    p->wp_col.lock()->wp_up = p->wp_up.lock();
                }

                if (auto ptr = p->wp_up.lock())
                    ptr->sp_down = p->sp_down;
            }
        }

        return col;
    }

    //restore a given col header and all attached nodes
    template <typename T>
    pnode_t<T> link<T>::restore_col(const pnode_t<T> col)
    {
        if (col->row != 0)
            return nullptr;

        pnode_t<T> row;
        //iter to the last row node
        for (row = col; row->sp_down != nullptr; row = row->sp_down)
            ;

        for (; (nullptr != row) && (row != col) && !is_uninitialized(row->wp_up); row = row->wp_up.lock())
        {
            pnode_t<T> p;
            //find the last node on the right side
            for (p = row; p->sp_right != nullptr; p = p->sp_right)
                ;
            //reverse restore the nodes
            for (; (p != row) && (!p->wp_left.expired()); p = p->wp_left.lock())
            {
                if (nullptr != p->sp_down)
                    p->sp_down->wp_up = p;
                else
                {
                    p->wp_col.lock()->wp_up = p;
                }

                if (auto ptr = p->wp_up.lock())
                    ptr->sp_down = p;
            }

            //find the last node on the left side
            for (p = row; !is_uninitialized(p->wp_left) && !(p->wp_left.expired()); p = p->wp_left.lock())
                ;
            //reverse restore the nodes
            for (; (p != row) && (p->sp_right != nullptr); p = p->sp_right)
            {
                if (nullptr != p->sp_down)
                    p->sp_down->wp_up = p;
                else
                {
                    p->wp_col.lock()->wp_up = p;
                }

                if (auto ptr = p->wp_up.lock())
                    ptr->sp_down = p;
            }
        }
        //restore the col header
        if (nullptr != col->sp_right)
            col->sp_right->wp_left = col;
        else
            sp_head->wp_left = col;

        if (auto ptr = col->wp_left.lock())
            ptr->sp_right = col;

        return col;
    }

    template <typename T>
    bool link<T>::_solve()
    {
        //TODO:
        return false;
    }
    template <typename T>
    std::optional<std::vector<pnode_t<T>>> link<T>::solve()
    {
        //clear answer stack
        while (!_answers.empty())
            _answers.pop();

        if (_solve())
        {
            std::vector<pnode_t<T>> result;
            while (!_answers.empty())
            {
                auto ele = _answers.top();
                _answers.pop();
                result.push_back(ele);
            }
            return result;
        }
        else
            return std::nullopt;
    }

    template <typename T>
    std::tuple<pnode_t<T>, pnode_t<T>, pnode_t<T>, pnode_t<T>> link<T>::find_neighbour(const unsigned col, const unsigned row)
    {
        pnode_t<T> res_up, res_down, res_left, res_right;

        if (col > sp_head->wp_left.lock()->col)
            return {nullptr, nullptr, nullptr, nullptr};
        auto pcol = sp_head;
        while (pcol->sp_right != nullptr)
        {
            //move to next col header node
            pcol = pcol->sp_right;

            //get the node with the MAX row in this col
            auto p = pcol;
            do
            {
                p = p->wp_up.lock();
                if (p->col < col)
                {
                    if (p->row == row)
                        res_left = p;
                }
                else if (p->col == col)
                {
                    if (p->row > row)
                        continue;
                    else if (p->row == row)
                    {
                        res_up = p->wp_up.lock();
                        res_down = p->sp_down;
                        std::cerr << "Something wrong!" << std::endl;
                        break;
                    }
                    else if (p->row < row)
                    {
                        res_up = p;
                        res_down = p->sp_down;
                        break;
                    }
                }
                else
                { //p->col > col
                    if (p->row == row)
                    {
                        res_right = p;
                        return std::make_tuple(res_up, res_down, res_left, res_right);
                    }
                }
            } while (p != pcol);
        }
        return {nullptr, nullptr, nullptr, nullptr};
    }

    template <typename T>
    pnode_t<T> link<T>::find_col_head(const unsigned col)
    {
        for (auto pcol = sp_head; pcol != nullptr; pcol = pcol->sp_right)
        {
            if (pcol->col == col)
                return pcol;
        }
        return nullptr;
    }

    template <typename T>
    void link<T>::append_row(const std::vector<unsigned> &cols, const std::vector<unsigned> &vals)
    {
        //TODO: sort cols

        if ((vals.size() != 0) && (cols.size() != vals.size()))
        {
            std::cerr << "append_row(), cols.size != vals.size" << std::endl;
            return;
        }

        if (cols.size() > sp_head->wp_left.lock()->col)
        {
            std::cerr << "col exceed max limit of column header" << std::endl;
            return;
        }

        pnode_t<T> pfirst, pprior;
        for (int i = 0; i < cols.size(); ++i)
        {
            unsigned col = cols[i];
            pnode_t<T> pcol = find_col_head(col);
            pnode_t<T> plast = pcol->wp_up.lock();

            pnode_t<T> pnode;
            if (vals.size() > i)
                pnode = std::make_shared<node<T>>(vals[i]);
            else
                pnode = std::make_shared<node<T>>(0);
            if (i == 0)
                pfirst = pnode;
            pprior = pnode;
            pnode->wp_up = plast;
            pnode->sp_down = nullptr;
            pnode->wp_left = pprior;
            pnode->sp_right = nullptr;

            plast->sp_down = pnode;
            pcol->wp_up = pnode;
            if (pprior != pnode)
                pprior->sp_right = pnode;
            if (pfirst != pnode)
                pfirst->wp_left = pnode;
        }
    }
} // namespace dlx

#endif