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
    for (std::string i : {"+", "-", "*", "/", ">", "<", ">=", "<=", "=", "==", "++", "--", "+=", "-=", "&"}) {
        if (i == s) {
            return true;
        }
    }
    return false;
}

bool is_unary(std::string s, std::vector<std::tuple<int, std::string, int>>& lexems) {
    if (!(s == "+" || s == "-" || s == "&" || s == "*")) return 0;
    int id = std::get<0>(lexems[lexems.size() - 1]);
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