/* 
 * File:   test_priceT.cpp
 * Author: zineb
 *
 * Created on 20 septembre 2017, 14:28
 */

#include <cstdlib>
#include "parser.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    char *infile = argv[1];
    Param *P = new Parser(infile);
    PnlMat *past;
    //P->extract("past", (100, 40));
    pnl_mat_print(past);
    return 0;
}