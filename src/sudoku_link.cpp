#include <iostream>
#include "sudoku_link.hpp"

using namespace std;
using namespace dlx;

unsigned sudoku_link::rule_cell_not_empty(const cell_t & cell)
{
    return cell.y * 9 + cell.x;
}

unsigned sudoku_link::rule_row_has_num(const cell_t & cell)
{
    unsigned res = cell.y *9 +cell.num;
    return res + 81;
}

unsigned sudoku_link::rule_col_has_num(const cell_t &cell)
{
    unsigned res = cell.x *9 +cell.num;
    return res+81+81;
}

unsigned sudoku_link::rule_block_has_num(const cell_t &cell)
{
    unsigned res = cell.y/3*3 + cell.x/3 + cell.num;
    return res+81+81+81;
}

void sudoku_link::build_link(const board99_t &board)
{
    delete_all();
    init_cols(81*4);
    for(unsigned i =0;i<81;++i){
        if(board[i]!=0){
            cell_t cell{i%9,i/9,board[i]};
            unsigned col_for_rule1 = rule_cell_not_empty(cell);
            unsigned col_for_rule2 = rule_row_has_num(cell);
            unsigned col_for_rule3 = rule_col_has_num(cell);
            unsigned col_for_rule4 = rule_block_has_num(cell);
            append_row({col_for_rule1,col_for_rule2,col_for_rule3,col_for_rule4},{board[i],board[i],board[i],board[i]});
        }
        else{
            for(unsigned val =1;val<10;++val){
                cell_t cell{i%9,i/9,val};
                unsigned col_for_rule1 = rule_cell_not_empty(cell);
                unsigned col_for_rule2 = rule_row_has_num(cell);
                unsigned col_for_rule3 = rule_col_has_num(cell);
                unsigned col_for_rule4 = rule_block_has_num(cell);
                append_row({col_for_rule1,col_for_rule2,col_for_rule3,col_for_rule4},{val,val,val,val});
            }
        }
    }
}

sudoku_link::cell_t sudoku_link::parse_solution_row(const pnode_t<unsigned> & row_of_solution)
{
    cell_t result;
    for(pnode_t<unsigned> p = row_of_solution; p != nullptr; p=p->sp_right){
        unsigned s = p->col / 81;
        switch (s)
        {
        case 0: //rule 1 cell not empty
            result.y = p->col / 9;
            result.x = p->col % 9;
            result.num = p->val;
            break;
        case 1: //rule 2 row has num
            result.y = (p->col-81)/9;
            if(p->val != (p->col-81)%9)cerr<<"rule2 borken!val != parsed val";
            result.num = p->val;
            break;
        case 2: //rule 3 col has num
            result.x = (p->col-81-81)/9;
            if(p->val != (p->col-81-81)%9)cerr<<"rule3 broken!p->val != parsed val";
            result.num = p->val;
            break;
        case 3: //rule 4 block has num

            break;
        default:
            break;
        }
    }
    return result;
}

sudoku_link::board99_t sudoku_link::parse_solution(const vector<pnode_t<unsigned>> &&solution)
{
    board99_t result{};
    for(auto prow:solution){
        cell_t cell = parse_solution_row(prow);
        result[cell.y*9+cell.x]=cell.num;
    }
    return result;
}

optional<sudoku_link::board99_t> sudoku_link::solve_sudoku(const board99_t &sudoku)
{
    auto solution = solve();
    if (solution.has_value())
    {
        return parse_solution(std::move(*solution));
    }
    else
        return nullopt;
}

ostream &operator<<(ostream &os, const sudoku_link::board99_t &val)
{

    os << "Sudoku:" << endl;
    for (int row = 0; row < 9; ++row)
    {
        bool first = true;
        for (int col = 0; col < 9; ++col)
        {
            if (first)
                first = false;
            else
                os << ',';
            os << val[row * 9 + col];
        }
        os << endl;
    }
    return os;
}