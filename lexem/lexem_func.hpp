#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

#include "bor.hpp"
#include "class_error.hpp"
#include "creat_lexem.hpp"

bor Bor;

// struct inf_lexem {
//     int num;
//     std::string word;
//     int num_len;
// };
std::vector<inf_lexem> lexems;

std::vector<inf_lexem> lexer() {
    creat_bor(Bor);
    try {
        lexical_analysis(lexems, Bor);
    } catch (const std::pair<int, std::string> & e) {
        throw invalid_argument(e.first, e.second);
    }
    std::cout << lexems.size() << std::endl;
    for (auto i : lexems) {
        std::cout << i.type << " " << i.word << " " << i.num_len << '\n';
    }
    return lexems;
}