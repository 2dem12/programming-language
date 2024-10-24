#include <string>
#include "check_func.hpp"

std::vector<std::string> splitIntoWordsAndPunctuation(const std::string& text) {
    std::vector<std::string> result;
    std::string currentWord;

    for (size_t i = 0; i < text.length(); ++i) {
        char ch = text[i];

        if (currentWord.empty() && ch != ' ') {
            currentWord += ch;
            continue;
        }
        if ((isalnum(ch) && (currentWord.empty() || !currentWord.empty() && isalnum(currentWord[0]))
            || currentWord.size() == 1 && ispunct(currentWord[0] && ispunct(ch))
            // || ispunct(ch) && currentWord.size() == 1 && ispunct(currentWord[0]) && try_size(currentWord + ch)
            || currentWord[0] == '"' && ch != '"'
            || is_operation(currentWord+ch))
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

void solve(std::vector<std::tuple<int, std::string, int>>& lexems, bor& Bor) {
    std::ifstream file("/Users/damir/CLionProjects/programming-language_main/code.txt", std::ios::binary);
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
            if (Bor.exists(i)) {
                lexems.emplace_back(1, i, cnt_line);
            } else if (is_int(i) || i[0] == '"') {
                lexems.emplace_back(3, i, cnt_line);
            } else if (is_operation(i)) {
                if (is_unary(i, lexems) || first) {
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