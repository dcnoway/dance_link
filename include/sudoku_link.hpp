#ifndef __SUDOKU_LINK
#define __SUDOKU_LINK
#include "dancing_link.hpp"

using namespace dlx;
using namespace std;



class sudoku_link : public link<unsigned>
{
public:
    using board99_t = std::array<unsigned, 81>;
    struct cell_t
    {
        unsigned x;
        unsigned y;
        unsigned num;
    };

protected:
    unsigned rule_cell_not_empty(const cell_t &);
    unsigned rule_row_has_num(const cell_t &);
    unsigned rule_col_has_num(const cell_t &);
    unsigned rule_block_has_num(const cell_t &);
    void build_link(const board99_t &);

    cell_t parse_solution_row(const pnode_t<unsigned> & row_of_solution);
    board99_t parse_solution(const vector<pnode_t<unsigned>> &&solution);

public:
    using link::link;
    sudoku_link()=default;

    optional<board99_t> solve_sudoku(const board99_t& );
};
ostream &operator<<(ostream &os, const sudoku_link::board99_t &val);
#endif