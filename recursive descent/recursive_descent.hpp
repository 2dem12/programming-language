#include "../lexem/lexem_func.hpp"

struct Parser {
public:
    Parser(std::vector<inf_lexem> lexems_) : lexems(std::move(lexems_)), iter(0) {}

    void pars() {
        start();
    }
private:
    std::vector<inf_lexem> lexems;
    int iter = 0;

    void error() {
        // throw std::runtime_error("Unexpected token: " + lexems[iter].word);
        // throw invalid_argument(lexems[iter].num_len, lexems[iter].word);
        throw invalid_argument(lexems[iter-1].num_len, lexems[iter-1].word);
    }

    void start() {
        while (iter < lexems.size()) {
            if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "char") {
                ++iter;
                id();
                if (lexems[iter].word == "(") {
                    iter -= 2;
                    function();
                } else {
                    --iter;
                    defining_variables();
                    checkPoint();
                }

            }
        }
    }

    void type() {
        if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "char") {
            iter++;
        } else {
            error();
        }
    }

    bool type(bool chang) {
        if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "char") {
            return true;
        } else {
            return false;
        }
    }

    void id() {
        if (lexems[iter].type == 2) {
            iter++;
        } else {
            error();
        }
    }

    void expression() {
        L12();
    }
    void L12 () {
        L11();
        while (iter < lexems.size()) {
            if (lexems[iter].word == ",") {
                ++iter;
                L11();
            } else {
                break;
            }
        }
    }
// *
    void L11 () {
        L10();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "=" || lexems[iter].word == "+=" || lexems[iter].word == "-=") {
                ++iter;
                L10();
            } else {
                break;
            }
        }
    }
    void L10 () {
        L9();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "||") {
                ++iter;
                L9();
            } else {
                break;
            }
        }
    }
    void L9 () {
        L8();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "&&") {
                ++iter;
                L8();
            } else {
                break;
            }
        }
    }
    void L8 () {
        L7();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "|") {
                ++iter;
                L7();
            } else {
                break;
            }
        }
    }
    void L7 () {
        L6();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "&") {
                ++iter;
                L6();
            } else {
                break;
            }
        }
    }
    void L6 () {
        L4();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "<=" || lexems[iter].word == ">=" || lexems[iter].word == "==" || lexems[iter].word == ">" || lexems[iter].word == "<" || lexems[iter].word == "!=") {
                ++iter;
                L4();
            } else {
                break;
            }
        }
    }
    void L4() {
        L3();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "+" || lexems[iter].word == "-") {
                ++iter;
                L3();
            } else {
                break;
            }
        }
    }

    void L3() {
        L23();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "*" || lexems[iter].word == "/" || lexems[iter].word == "%") {
                ++iter;
                L23();
            } else {
                break;
            }
        }
    }

    void L23 () {
        if (lexems[iter].type == 8) {
            ++iter;
            L2();
        } else L2();
    }

    void L2() {
        if (lexems[iter].type == 3 || lexems[iter].type == 2) {
            L1();
        } else if (iter < lexems.size() && lexems[iter].word == "(") {
            ++iter;
            expression();
            if (iter < lexems.size() && lexems[iter].word == ")") {
                ++iter;
            } else {
                error();
            }
        } else if (lexems[iter].word == "++") {
            ++iter;
            L1();
        } else error();
    }

    void L1() {
        if (lexems[iter].type == 3) {
            ++iter;
        } else if (lexems[iter].type == 2) {
            if (lexems[iter + 1].word == "(") {
                ++iter;
                function_call();
            } else ++iter;
        } else error();
    }

    void function_call () {
        if (lexems[iter].word == "(") {
            ++iter;
        } else error();
        expression();
        while (lexems[iter].word == ",") {
            ++iter;
            expression();
        }
        if (lexems[iter].word == ")") {
            ++iter;
        } else error();
    }

    void literal() {
        if (lexems[iter].type == 3 || lexems[iter].type == 2) {
            ++iter;
        } else error();
    }


    void defining_variables() {
        id();
        if (lexems[iter].word == "=") {
            ++iter;
            expression();
        }  else if (lexems[iter].word == "[") {
            ++iter;
            expression();
            if (lexems[iter].word == "]") {
                ++iter;
            } else error();
            if (lexems[iter].word == "=") {
                ++iter;
                if (lexems[iter].word == "{") {
                    ++iter;
                    literal();
                    while (lexems[iter].word == ",") {
                        ++iter;
                        literal();
                    }
                    if (lexems[iter].word == "}") {
                        ++iter;
                    } else error();
                } else error();
            }
        }

    }

    void input () {
        if (lexems[iter].word == "(") {
            ++iter;
            id();
            if (lexems[iter].word == ")") {
                ++iter;
            } else error();
        } else error();
    }
    void print () {
        if (lexems[iter].word == "(") {
            ++iter;
            if (lexems[iter].type == 3 || lexems[iter].type == 2) {
                ++iter;
            } else {
                expression();
            }
            if (lexems[iter].word == ")") {
                ++iter;
            } else error();
        } else error();
    }


    void func_else() {
        iter++; // "else" dont ++, becos we ++ now
        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();
    }

    void func_elif() {
        iter++; // "elif" dont ++, becos we ++ now
        if (lexems[iter++].word != "(") error();

        expression();

        if (lexems[iter++].word != ")") error();
        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();

        if (lexems[iter].word == "elif") func_elif();
    }

    void func_if() {
        iter++; // "if" dont ++, becos we ++ now
        if (lexems[iter++].word != "(") error();

        expression();

        if (lexems[iter++].word != ")") error();
        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();

        if (lexems[iter].word == "elif") func_elif();
        if (lexems[iter].word == "else") func_else();
    }

    void func_for() {
        // Damir
        iter++;
        if (lexems[iter++].word != "(") error();
        if (lexems[iter].word != ";") {
            expression();
            if (lexems[iter++].word != ";") error();
        } else {
            iter++;
        }
        if (lexems[iter].word != ";") {
            expression();
            if (lexems[iter++].word != ";") error();
        } else {
            iter++;
        }
        if (lexems[iter].word != ")") {
            expression();
            if (lexems[iter++].word != ")") error();
        } else {
            iter++;
        }

        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();
    }

    void func_while() {
        // Damir
        iter++;
        if (lexems[iter++].word != "(") error();
        expression();
        if (lexems[iter++].word != ")") error();

        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();
    }

    void func_case() {
        iter++;
        expression();

        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();
    }

    void func_switch() {
        iter++;

        if (lexems[iter++].word != "(") error();

        expression();

        if (lexems[iter++].word != ")") error();

        if (lexems[iter++].word != "{") error();

        while (lexems[iter].word == "case") {
            func_case();
        }

        if (lexems[iter].word == "default") {
            iter++;
            if (lexems[iter++].word != "{") error();
            body();
            if (lexems[iter++].word != "}") error();
        }

        if (lexems[iter++].word != "}") error();
    }

    void checkPoint () {
        std::cout << lexems[iter].word << std::endl;
        if (lexems[iter].word == ";") {
            ++iter;
        } else error();
    }

    void return_block () {
        if (lexems[iter].word == ";") {
            ++iter;
        } else {
            expression();
            checkPoint();
        }
    }

    void command_block() {
        if (lexems[iter].word == "int" || lexems[iter].word == "double" || lexems[iter].word == "string" || lexems[iter].word == "bool") {
            ++iter;
            defining_variables();
            checkPoint();
        } else if (lexems[iter].word == "if") {
            func_if();
        } else if (lexems[iter].word == "for") {
            func_for();
        } else if (lexems[iter].word == "while") {
            func_while();
        } else if (lexems[iter].word == "switch") {
            func_switch();
        }
        else if (lexems[iter].word == "break") {
            iter++;
            checkPoint();
        }
        else if (lexems[iter].word == "continue") {
            iter++;
            checkPoint();
        }
        else if (lexems[iter].word == "return") {
            iter++;
            return_block();
        } else if (lexems[iter].word == "input") {
            ++iter;
            input();
            checkPoint();
        } else if (lexems[iter].word == "print") {
            ++iter;
            print();
            checkPoint();
        } else {
            if (lexems[iter].word == "}") {
                return;
            } else {
                expression();
                checkPoint();
            }
        }
        command_block();
    }

    void body() {
        command_block();
    }

    void function() {
        type();
        id();
        if (lexems[iter++].word != "(") error();

        if (type(true)) {
            type();
            id();
        }
        while (lexems[iter].type == 7) {
            iter++;
            type();
            id();
        }
        if (lexems[iter++].word != ")") error();

        if (lexems[iter++].word != "{") error();

        body();

        if (lexems[iter++].word != "}") error();
    }
};

void solve() {
    Parser parser(lexer());

    try {
        parser.pars();
        std::cout << "Parsing succeeded.\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Parsing error: " << e.what() << '\n';
    }
}