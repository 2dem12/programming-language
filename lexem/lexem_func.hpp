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
typedef std::tuple<int, std::string, int> iter;
std::vector<iter> lexems;

std::vector<iter> lexer() {
    creat_bor(Bor);
    try {
        lexical_analysis(lexems, Bor);
    } catch (const std::pair<int, std::string> & e) {
        throw invalid_argument(e.first, e.second);
    }
    for (auto i : lexems) {
        std::cout << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << '\n';
    }
    return lexems;
}