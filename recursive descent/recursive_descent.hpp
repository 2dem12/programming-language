#include "../lexem/lexem_func.hpp"
#include "TID.hpp"
#include <stack>
#include <utility>

struct Parser {
public:
    Parser(std::vector<inf_lexem> lexems_) : lexems(std::move(lexems_)), iter(0) {
    }

    void pars() {
        start();
    }

private:
    std::vector<inf_lexem> lexems;
    int iter = 0;

    struct func {
        func(std::string type_, std::string name_) : type_answer(std::move(type_)), name(std::move(name_)) {
        }

        bool operator==(const func &function) {
            if (name == function.name
                && parameters.size() == function.parameters.size()) {
                for (int i = 0; i < parameters.size(); ++i) {
                    if (*parameters[i] == *function.parameters[i]) {
                        continue;
                    }
                    return false;
                }
                return true;
            }
            return false;
        }

        bool equals(const func &function) {
            if (parameters.size() == function.parameters.size()) {
                for (int i = 0; i < parameters.size(); ++i) {
                    if (*parameters[i] == *function.parameters[i]) {
                        continue;
                    }
                    return false;
                }
                return true;
            }
            return false;
        }

        std::string name;
        std::string type_answer;
        std::vector<parameter *> parameters;
    };

    std::vector<func *> functions;
    tree_tid Tree;

    struct stck {
        std::stack<std::string> types;
        std::stack<std::string> operations;
        std::string pr[3] = {"int", "float", "bool"};
        std::string sc[2] = {"string", "array"};

        void pushOp(std::string op) {
            operations.push(op);
        }

        void pushT(std::string type) {
            types.push(type);
        }

        bool type1(std::string type) {
            for (int i = 0; i < 3; ++i) {
                if (type == pr[i]) {
                    return 1;
                }
            }
            return 0;
        }

        void clear() {
            while (!operations.empty()) {
                operations.pop();
            }
            while (!types.empty()) {
                types.pop();
            }
        }
    };

    stck stack;

    void check_bin() {
        std::string lhs = stack.types.top();
        stack.types.pop();
        std::string rhs = stack.types.top();
        stack.types.pop();
        std::string ops = stack.operations.top();
        stack.operations.pop();
        std::cout << lhs << " " << ops << " " << rhs << std::endl;
        if (stack.type1(lhs) && stack.type1(rhs)) {
            if (lhs == "int" || rhs == "int") {
                stack.types.push("int");
            } else if (lhs == "float" || rhs == "float") {
                stack.types.push("float");
            } else {
                stack.types.push("bool");
            }
        } else if (lhs == "string" && rhs == "string") {
            std::cout << "here";
            if (ops == "+" || ops == "==" || ops == "!=") {
                stack.types.push("string");
                return;
            }
            error(" incorrect operation to string ");
            //error(line, " incorrect operation to string: ", ops);
        } else {
            error(" incorrect types");
        }
    }

    void check_unary() {
        std::string type = stack.types.top();
        stack.types.pop();
        std::string ops = stack.operations.top();
        stack.operations.pop();
        if (stack.type1(type)) {
            if ((ops == "++" || ops == "--") && type == "bool") {
                error(" impossible to do this operation to type bool: ");
            }
            stack.types.push(type);
        } else {
            if (ops == "*" || ops == "&") {
                stack.types.push(type);
            } else error(" incorrect operation to " + type);
        }
    }

    void check_bool() {
        std::string type = stack.types.top();
        stack.types.pop();
        if (type == "int" || type == "bool" || type == "float") {
        } else error(" not a boollean expression, type is: " + type);
    }


    void error() {
        // throw std::runtime_error("Unexpected token: " + lexems[iter].word);
        // throw invalid_argument(lexems[iter].num_len, lexems[iter].word);
        throw invalid_argument(lexems[iter - 1].num_len, lexems[iter - 1].word);
    }

    void error(int line, std::string msg) {
        std::string s = "in line " + std::to_string(line);
        s += msg;
        throw std::runtime_error(s);
    }

    void error(int line, std::string msg, std::string op) {
        std::string s = "in line " + std::to_string(line);
        s += msg + op;
        throw std::runtime_error(s);
    }

    void error(std::string msg) {
        throw std::runtime_error(msg);
    }


    void start() {
        while (iter < lexems.size()) {
            if (lexems[iter].word == "}") error();
            if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "float" || lexems[
                    iter].word == "bool" || lexems[iter].word == "void") {
                ++iter;
                id();
                if (lexems[iter].word == "(") {
                    iter -= 2;
                    function();
                } else {
                    --iter;
                    //defining_variables();
                    many_variables();
                    checkPoint();
                }
            }
        }
    }

    void type() {
        if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "char" || lexems[iter].
            word == "bool" || lexems[iter].word == "float") {
            iter++;
        } else {
            error();
        }
    }

    bool type(bool chang) {
        if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "char" || lexems[iter].
            word == "bool" || lexems[iter].word == "float") {
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

    void L12() {
        L11();
        while (iter < lexems.size()) {
            if (lexems[iter].word == ",") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L11();
                check_bin();
            } else {
                break;
            }
        }
    }

    // *
    void L11() {
        L10();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "=" || lexems[iter].word == "+=" || lexems[iter].word == "-=") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L10();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L10() {
        L9();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "||") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L9();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L9() {
        L8();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "&&") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L8();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L8() {
        L7();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "|") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L7();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L7() {
        L6();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "&") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L6();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L6() {
        L4();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "<=" || lexems[iter].word == ">=" || lexems[iter].word == "==" || lexems[iter].word
                == ">" || lexems[iter].word == "<" || lexems[iter].word == "!=") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L4();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L4() {
        L3();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "+" || lexems[iter].word == "-") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L3();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L3() {
        L23();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "*" || lexems[iter].word == "/" || lexems[iter].word == "%") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                L23();
                check_bin();
            } else {
                break;
            }
        }
    }

    void L23() {
        if (lexems[iter].type == 8) {
            stack.pushOp(lexems[iter].word);
            ++iter;
            L2();
            check_unary();
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
            stack.pushOp(lexems[iter].word);
            ++iter;
            L1();
            check_unary();
        } else error();
    }

    void L1() {
        if (lexems[iter].type == 3) {
            if (lexems[iter].word[0] == '"') {
                stack.pushT("string");
            } else {
                bool fl = 0;
                for (auto u: lexems[iter].word) {
                    if (u == '.') {
                        fl = 1;
                        break;
                    }
                }
                if (fl) stack.pushT("float");
                else stack.pushT("int");
            }
            ++iter;
        } else if (lexems[iter].type == 2) {
            if (lexems[iter + 1].word == "(") {
                function_call();
            } else {
                std::string type = Tree.check_id(lexems[iter].word);
                if (type == "") throw std::runtime_error("Such variable does not exist: " + lexems[iter].word);
                // std::cout << type << " " << lexems[iter].word << std::endl;
                stack.pushT(type);
                ++iter;
            }
        } else error();
    }

    void check_expression(std::string function, int line) {
        for (func *f: functions) {
            if (f->name == function) {
                return;
            }
        }
        std::string s = "in line " + std::to_string(line);
        s += " function does not exist : ";
        throw std::runtime_error(s + function);
    }

    std::string check_func(func *function) {
        for (auto u: functions) {
            if (function->equals(*u)) {
                return u->type_answer;
            }
        }
        error("function call is incorrect: function does not exist");
    }

    void function_call() {
        id();
        check_expression(lexems[iter - 1].word, lexems[iter - 1].num_len);
        if (lexems[iter].word == "(") {
            ++iter;
        } else error();
        func *functionCall = new func(" ", " ");
        expression1();
        functionCall->parameters.push_back(new parameter(stack.types.top()));
        stack.types.pop();
        while (lexems[iter].word == ",") {
            ++iter;
            expression();
            functionCall->parameters.push_back(new parameter(stack.types.top()));
            stack.types.pop();
        }
        if (lexems[iter].word == ")") {
            ++iter;
        } else error();

        stack.pushT(check_func(functionCall));
    }

    void literal() {
        if (lexems[iter].type == 3 || lexems[iter].type == 2) {
            ++iter;
        } else error();
    }

    void many_variables() {
        std::string type = lexems[iter - 1].word;
        defining_variables(type);
        while (lexems[iter].word == ",") {
            ++iter;
            defining_variables(type);
        }
    }

    void defining_variables(std::string type_) {
        id();
        std::string name = lexems[iter - 1].word;
        parameter param(type_, name);
        // Tree.push_id(param);
        if (lexems[iter].word == "=") {
            ++iter;
            expression1();
        } else if (lexems[iter].word == "[") {
            ++iter;
            expression();
            if (lexems[iter].word == "]") {
                ++iter;
            } else error();
            //parameter param(type_ + " array", name);
            //Tree.push_id(param);
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
            return;
        }
        Tree.push_id(param);
        //parameter param(type_, name);
        //Tree.push_id(param);
    }

    void input() {
        if (lexems[iter].word == "(") {
            ++iter;
            id();
            if (lexems[iter].word == ")") {
                ++iter;
            } else error();
        } else error();
    }

    void print() {
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
        check_bool();

        if (lexems[iter++].word != ")") error();
        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();

        if (lexems[iter].word == "elif") func_elif();
    }

    void func_if() {
        iter++; // "if" dont ++, becos we ++ now
        if (lexems[iter++].word != "(") error();

        expression();
        check_bool();

        if (lexems[iter++].word != ")") error();
        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();

        if (lexems[iter].word == "elif") func_elif();
        if (lexems[iter].word == "else") func_else();
    }

    void func_for() {
        // Damir
        iter++;
        if (lexems[iter++].word != "(") error();
        if (lexems[iter].word != ";") {
            expression();
            stack.clear();
            if (lexems[iter++].word != ";") error();
        } else {
            iter++;
        }
        if (lexems[iter].word != ";") {
            expression();
            check_bool();
            if (lexems[iter++].word != ";") error();
        } else {
            iter++;
        }
        if (lexems[iter].word != ")") {
            expression();
            stack.clear();
            if (lexems[iter++].word != ")") error();
        } else {
            iter++;
        }

        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();
    }

    void func_while() {
        // Damir
        iter++;
        if (lexems[iter++].word != "(") error();
        expression();
        check_bool();
        if (lexems[iter++].word != ")") error();

        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();
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

    void checkPoint() {
        std::cout << lexems[iter].word << std::endl;
        if (lexems[iter].word == ";") {
            ++iter;
        } else error();
    }


    void command_block() {
        if (lexems[iter].word == "int" || lexems[iter].word == "float" || lexems[iter].word == "string" || lexems[iter].
            word == "bool") {
            ++iter;
            //defining_variables();
            many_variables();
            checkPoint();
        } else if (lexems[iter].word == "if") {
            func_if();
        } else if (lexems[iter].word == "for") {
            func_for();
        } else if (lexems[iter].word == "while") {
            func_while();
        } else if (lexems[iter].word == "switch") {
            func_switch();
        } else if (lexems[iter].word == "break") {
            iter++;
            checkPoint();
        } else if (lexems[iter].word == "continue") {
            iter++;
            checkPoint();
        } else if (lexems[iter].word == "input") {
            ++iter;
            input();
            checkPoint();
        } else if (lexems[iter].word == "print") {
            ++iter;
            print();
            checkPoint();
        } else {
            if (lexems[iter].word == "}" || lexems[iter].word == "return") {
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

    void push_function(func *function, int line) {
        for (func *f: functions) {
            if (*function == *f) {
                std::string s = "in line " + std::to_string(line);
                s += " the same function name already exists : ";
                throw std::runtime_error(s + f->name);
            }
        }
        functions.push_back(function);
    }

    void check_return (func * cur_func) {
        if (lexems[iter].word == "return") {
            ++iter;
        } else throw std::runtime_error("function does not contain a return");
        if (cur_func->type_answer == "void") {
            checkPoint();
            return;
        }
        expression1();
        std::string res = stack.types.top();
        stack.types.pop();
        if (res == cur_func->type_answer) {
            checkPoint();
        } else error("return does not match function parameters");
    }

    void function() {
        std::string s_type = lexems[iter].word;
        if (lexems[iter].word == "int" || lexems[iter].word == "string" || lexems[iter].word == "char" || lexems[iter].
            word == "bool" || lexems[iter].word == "void" || lexems[iter].word == "float") {
            iter++;
        } else {
            error();
        }
        //std::cout << "here";
        std::string s_id = lexems[iter].word;
        int line = lexems[iter].num_len;
        id();
        func *cur_func = new func(s_type, s_id);
        if (lexems[iter++].word != "(") error();
        Tree.create_scope();
        std::string p_type, p_id;
        if (type(true)) {
            p_type = lexems[iter].word;
            type();
            p_id = lexems[iter].word;
            id();
            //cur_func->parameters.push_back(new parameter(p_type, p_id));
            parameter * x = new parameter(p_type, p_id);
            cur_func->parameters.push_back(x);
            Tree.push_id(*x);
        }
        while (lexems[iter].type == 7) {
            iter++;
            p_type = lexems[iter].word;
            type();
            p_id = lexems[iter].word;
            id();
            parameter * x = new parameter(p_type, p_id);
            cur_func->parameters.push_back(x);
            Tree.push_id(*x);
        }

        if (lexems[iter++].word != ")") error();
        push_function(cur_func, line);
        //create_scope
        //push_id

        if (lexems[iter++].word != "{") error();
        body();
        std::cout << lexems[iter].word << std::endl;
        check_return(cur_func);
        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();
    }

    void expression1() {
        R11();
    }

    void R11() {
        R10();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "=" || lexems[iter].word == "+=" || lexems[iter].word == "-=") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R10();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R10() {
        R9();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "||") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R9();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R9() {
        R8();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "&&") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R8();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R8() {
        R7();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "|") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R7();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R7() {
        R6();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "&") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R6();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R6() {
        R4();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "<=" || lexems[iter].word == ">=" || lexems[iter].word == "==" || lexems[iter].word
                == ">" || lexems[iter].word == "<" || lexems[iter].word == "!=") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R4();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R4() {
        R3();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "+" || lexems[iter].word == "-") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R3();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R3() {
        R23();
        while (iter < lexems.size()) {
            if (lexems[iter].word == "*" || lexems[iter].word == "/" || lexems[iter].word == "%") {
                stack.pushOp(lexems[iter].word);
                ++iter;
                R23();
                check_bin();
            } else {
                break;
            }
        }
    }

    void R23() {
        if (lexems[iter].type == 8) {
            stack.pushOp(lexems[iter].word);
            ++iter;
            R2();
            check_unary();
        } else R2();
    }

    void R2() {
        if (lexems[iter].type == 3 || lexems[iter].type == 2) {
            R1();
        } else if (iter < lexems.size() && lexems[iter].word == "(") {
            ++iter;
            expression();
            if (iter < lexems.size() && lexems[iter].word == ")") {
                ++iter;
            } else {
                error();
            }
        } else if (lexems[iter].word == "++") {
            stack.pushOp(lexems[iter].word);
            ++iter;
            R1();
            check_unary();
        } else error();
    }

    void R1() {
        if (lexems[iter].type == 3) {
            if (lexems[iter].word[0] == '"') {
                stack.pushT("string");
            } else {
                bool fl = 0;
                for (auto u: lexems[iter].word) {
                    if (u == '.') {
                        fl = 1;
                        break;
                    }
                }
                if (fl) stack.pushT("float");
                else stack.pushT("int");
            }
            ++iter;
        } else if (lexems[iter].type == 2) {
            if (lexems[iter + 1].word == "(") {
                function_call();
            } else {
                std::string type = Tree.check_id(lexems[iter].word);
                if (type == "") throw std::runtime_error("Such variable does not exist: " + lexems[iter].word);
                stack.pushT(type);
                ++iter;
            }
        } else error();
    }
};

void solve() {
    Parser parser(lexer());
    try {
        parser.pars();
        std::cout << "Parsing succeeded.\n";
    } catch (const std::runtime_error &e) {
        std::cerr << "Parsing error: " << e.what() << '\n';
    }
}
