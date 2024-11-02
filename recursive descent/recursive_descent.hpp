#include "/Users/damir/CLionProjects/programming-language_main/lexem/lexem_func.hpp"

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
        throw std::runtime_error("Unexpected token: " + lexems[iter].word);
        // throw invalid_argument(lexems[iter].num_len, lexems[iter].word);
    }

    void start() {
        function();
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
        // Polina
    }

    void defining_variables() {
        // Polina
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
    }

    void func_while() {
        // Damir
    }

    void func_switch() {
        // Damir
    }

    void command_block() {
        expression();
        if (lexems[iter].word == "int" || lexems[iter].word == "double" || lexems[iter].word == "string") {
            expression();
        } else if (lexems[iter].word == "if") {
            func_if();
        } else if (lexems[iter].word == "for") {
            func_for();
        } else if (lexems[iter].word == "while") {
            func_while();
        } else if (lexems[iter].word == "switch") {
            func_switch();
        }
        else if (lexems[iter].word == "break") {iter++;}
        else if (lexems[iter].word == "continue") {iter++;}
        else if (lexems[iter].word == "return") {iter++;}
        else {
            if (lexems[iter].word == "}") {
                return;
            } else {
                error();
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
        if (lexems[iter].word == "(") {
            iter++;
        } else {
            error();
        }
        if (type(1)) {
            type();
            id();
        }
        while (lexems[iter].type == 7) {
            iter++;
            type();
            id();
        }
        if (lexems[iter].word == ")") {
            iter++;
        } else {
            error();
        }

        if (lexems[iter].word == "{") {
            iter++;
        } else {
            error();
        }

        body();

        if (lexems[iter].word == "}") {
            iter++;
        } else {
            error();
        }
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