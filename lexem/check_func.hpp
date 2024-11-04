struct inf_lexem {
    inf_lexem(int num_, std::string word_, int num_len_) : type(num_), word(std::move(word_)), num_len(num_len_) {}
    int type;
    std::string word;
    int num_len;
};


bool is_int(std::string s) {
    if (s[0] == '.') return false;
    int cnt_dot = 0;
    for (char i : s) {
        if (i == '.') cnt_dot++;
        if (!isdigit(i) && i != '.' || cnt_dot > 1) {
            return false;
        }
    }
    return true;
}

bool is_operation(std::string s) {
    for (std::string i : {"+", "-", "*", "/", "%", ">", "<", ">=", "<=", "=", "==", "!=", "++", "--", "+=", "-=", "&"}) {
        if (i == s) {
            return true;
        }
    }
    return false;
}

bool is_unary(std::string s, std::vector<inf_lexem>& lexems) {
    if (!(s == "+" || s == "-" || s == "&" || s == "*")) return 0;
    int id = (lexems[lexems.size() - 1].type);
    if (id == 2 || id == 3) {
        return 0;
    }
    return 1;
}

bool is_punctuation(std::string s) {
    for (std::string i: {";", "(", ")", "{", "}", "[", "]"}) {
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