#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

#include "bor.hpp"

bor Bor;
typedef std::tuple<int, std::string, int> iter;
std::vector<iter> lexems;

class invalid_argument : public std::exception {
public:
    // Конструктор с инициализацией
    invalid_argument(int num_line, const std::string& line)
        : num_line_(std::to_string(num_line)), line_(line) {
        ans_ = "Invalid symbols \"" + line_ + "\" in line: " + num_line_;
    }

    // Переопределение метода what()
    const char* what() const noexcept override {
        return ans_.c_str();
    }

private:
    std::string num_line_;
    std::string line_;
    std::string ans_;
};

bool is_int(std::string s) {
    for (char i : s) {
        if (!isdigit(i)) {
            return false;
        }
    }
    return true;
}

bool is_operation(std::string s) {
    for (std::string i : {"+", "-", "*", "/", ">", "<", ">=", "<=", "=", "++", "--", "+=", "-=", "&"}) {
        if (i == s) {
            return true;
        }
    }
    return false;
}

bool is_unary(std::string s) {
    if (!(s == "+" || s == "-" || s == "&" || s == "*")) return 0;
    int id = std::get<0>(lexems[lexems.size() - 1]);
    if (id == 2 || id == 3) {
        return 0;
    }
    return 1;
}

bool is_punctuation(std::string s) {
    for (std::string i: {";", "(", ")", "{", "}"}) {
        if (i == s) {
            return true;
        }
    }
    return false;
}

bool try_size(std::string s) {
    if (s == "++" || s == "--" || s == "+=" || s == "-=") {
        return true;
    }
    return false;
}

bool is_variable (std::string s) {
    for (char i : s) {
        if (!((i >= 'a' && i <= 'z') || (i >='A' && i <= 'Z')
            || (i >= '0' && i <= '9') || i == '_')) {
            return false;
        }
    }
    return true;
}

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
            || ispunct(ch) && currentWord.size() == 1 && ispunct(currentWord[0]) && try_size(currentWord + ch)
            || currentWord[0] == '"' && ch != '"')
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

void creat_bor() {
    std::string line;
    std::ifstream file("/Users/damir/CLionProjects/programming-language_main/wordsList.txt");
    while (std::getline(file, line)) {
        Bor.add(line);
    }
}

void solve() {
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
                if (is_unary(i) || first) {
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

int main() {
    creat_bor();
    try {
        solve();
    } catch (const std::pair<int, std::string> & e) {
        // std::cout << "Invalid symbols in line " << e.first << " : " << e.second << std::endl;
        throw invalid_argument(e.first, e.second);
        //qwerty
    }
    for (auto i : lexems) {
        std::cout << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << '\n';
    }
}
