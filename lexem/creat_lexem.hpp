#include <string>
#include <utility>
#include "check_func.hpp"

bool longComment = 0;

std::vector<std::string> splitIntoWordsAndPunctuation(const std::string& text) {
    std::vector<std::string> result;
    std::string currentWord;
    for (size_t i = 0; i < text.length(); ++i) {
        char ch = text[i];
        // if (ch == '#') {
        //     if (longComment) longComment = false;
        //     else {
        //         longComment = true;
        //         return result;
        //     }
        // }
        // if (longComment) return result;
        if (currentWord.empty() && ch != ' ') {
            currentWord += ch;
            continue;
        }
        if (currentWord == "/" && ch == '/') return result;
        if ((isalnum(ch) && (currentWord.empty() || !currentWord.empty() && isalnum(currentWord[0]))
            || currentWord.size() == 1 && ispunct(currentWord[0] && ispunct(ch))
            || currentWord[0] == '"' && ch != '"'
            || is_operation(currentWord+ch)
            || is_int(currentWord+ch))
            && (!(isdigit(currentWord[0]) && isalnum(ch)) || isdigit(currentWord[0]) && isdigit(ch))) {
            currentWord += ch;
        } else {
            if (!currentWord.empty() && currentWord[0] == '"') {
                currentWord += ch;
                result.push_back(currentWord);
                currentWord.clear();
            } else {
                if (!currentWord.empty()) {
                    result.push_back(currentWord);
                    currentWord.clear();
                }
                if (!std::isspace(ch)) {
                    currentWord += ch;
                }
            }
        }

        if (currentWord.size() == 2 && ispunct(currentWord[0]) && ispunct(currentWord[1])) {
            result.push_back(currentWord);
            currentWord.clear();
        }
    }

    if (!currentWord.empty()) {
        result.push_back(currentWord);
    }

    return result;
}

void lexical_analysis(std::vector<inf_lexem>& lexems, bor& Bor) {
    std::ifstream file(CODE, std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    buffer[size] = '\0';
    file.close();
    int cnt_line = 1;
    char* line = strtok(buffer, "\n");
    while (line != nullptr) {
        bool first = 1;
        for (auto i : splitIntoWordsAndPunctuation(line)) {
            if (i == "#" && longComment == false) {
                longComment = true;
                continue;
            } else if (i == "#" && longComment == true) {
                longComment = false;
                continue;
            }
            if (longComment == true) continue;

            if (Bor.exists(i)) {
                lexems.emplace_back(1, i, cnt_line);
            } else if (is_int(i) || i[0] == '"') {
                lexems.emplace_back(3, i, cnt_line);
            } else if (is_operation(i)) {
                if (is_unary(i, lexems) || (first && ((i == "+" || i == "-" || i == "&" || i == "*")))) {
                    lexems.emplace_back(8, i, cnt_line);
                } else {
                    lexems.emplace_back(4, i, cnt_line);
                }
            } else if (is_punctuation(i)) {
                lexems.emplace_back(5, i, cnt_line);
            } else if (i == ".") {
                lexems.emplace_back(6, i, cnt_line);
            } else if (i == ",") {
                lexems.emplace_back(7, i, cnt_line);
            } else if (is_variable(i)) {
                lexems.emplace_back(2, i, cnt_line);
            } else {
                throw std::make_pair(cnt_line, i);
            }
            first = 0;
        }
        cnt_line++;
        line = strtok(nullptr, "\n");
    }
    delete[] buffer;
}