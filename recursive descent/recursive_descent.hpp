#include "../lexem/lexem_func.hpp"
#include "TID.hpp"
#include <stack>
#include <utility>
#include <map>
struct Parser {
public:
    Parser(std::vector<inf_lexem> lexems_) : lexems(std::move(lexems_)), iter(0) {
    }

    void pars() {
        start();
    }
    void generation () {
        func * curFunc;
        for (auto u: functions) {
            if ("main" == u->name) {
                curFunc  = u;
            }
        }
        std::vector <std::string> params;
        std::string res = get_function_call(curFunc, params);
        std::cout << "result of " << curFunc->name<< " function: " << res << std::endl;
    }

private:
    std::vector<inf_lexem> lexems;
    int iter = 0;
    enum class type_lexem {
        adress_variable,
        adress,
        variable,
        operation,
        func_call,
        literal,
        perehod
    };
    struct func {
        func(std::string type_, std::string name_) : type_answer(std::move(type_)), name(std::move(name_)) {
        }
        func(func * other)
                : name(other->name),
                  type_answer(other->type_answer),
                  poliz(other->poliz),
                  TID(nullptr)
        {
            parameters.reserve(other->parameters.size());
            for (const auto& param_ptr : other->parameters) {
                if (param_ptr) {
                    parameters.push_back(new parameter(*param_ptr));
                } else {
                    parameters.push_back(nullptr);
                }
            }
            if (other->TID) {
                TID = new tree_tid(*other->TID);
            }
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
        std::vector <std::pair<std::string, type_lexem>> poliz;
        tree_tid * TID;
    };
    std::vector<func *> functions;
    tree_tid Tree;
    struct general_stack {
        general_stack () {
            priority[","]  = 1;
            priority["="]  = 2;
            priority["+="]  = 2;
            priority["-="]  = 2;
            priority["||"]  = 3;
            priority["&&"]  = 4;
            priority["|"]  = 5;
            priority["&"]  = 6;
            priority["<"]  = 7;
            priority[">"]  = 7;
            priority["<="]  = 7;
            priority[">="]  = 7;
            priority["=="]  = 7;
            priority["!="]  = 7;
            priority["+"]  = 8;
            priority["-"]  = 8;
            priority["*"]  = 9;
            priority["/"]  = 9;
            priority["%"]  = 9;
            priority["++"]  = 10;
            priority["--"]  = 10;
        }
        std::stack <std::string> gstack;
        std::vector <std::pair<std::string, type_lexem>> poliz;
        std::map <std::string, int> priority;
        void push_literal (std::string& s, type_lexem type) {
            //std::cout << "PERFECT";
            poliz.push_back(std::make_pair(s, type));
        }
        void push_operation (std::string& s) {
            if (s == ",") {
                while (!gstack.empty()) {
                    poliz.push_back(std::make_pair(gstack.top(), type_lexem::operation));
                    gstack.pop();
                }
            }
            if (!gstack.empty()) {
                std::string top = gstack.top();
                while (priority[top] > priority[s] && top != "(") {
                    gstack.pop();
                    poliz.push_back(std::make_pair(top, type_lexem::operation));
                    if (!gstack.empty()) {
                        top = gstack.top();
                    } else break;
                }
            }
            gstack.push(s);
        }

        void push_other(std::string& s) {
            if (s == "(") gstack.push(s);
            if (s == ",") {
                while (!gstack.empty()) {
                    poliz.push_back(std::make_pair(gstack.top(), type_lexem::operation));
                    gstack.pop();
                }
                gstack.push(s);
            }
            if (s == "r") {
                clear_stack();
                gstack.push(s);
            }
            if (s == ")") {
                while (!gstack.empty()) {
                    if (gstack.top() == "(") {
                        gstack.pop();
                        return;
                    }
                    poliz.push_back(std::make_pair(gstack.top(), type_lexem::operation));
                    gstack.pop();
                }
            }
        }
        void print () {
            while (!gstack.empty()) {
                poliz.push_back(std::make_pair(gstack.top(), type_lexem::operation));
                gstack.pop();
            }
            for (auto u: poliz) {
                std::cout << u.first << " ";
            }
            std::cout << std::endl;
        }
        void clear_stack () {
            while (!gstack.empty()) {
                poliz.push_back(std::make_pair(gstack.top(), type_lexem::operation));
                gstack.pop();
            }
        }
        void clear() {
            while (!poliz.empty()) {
                poliz.pop_back();
            }
        }
        int len_poliz() {
            return poliz.size();
        }

        void replase_adres(int index, int adres) {
            poliz[index].first = std::to_string(adres);
        }

        void push_adres(int adres) {
            poliz.push_back({std::to_string(adres), type_lexem::adress});
        }

        void push_perehod(std::string perehod) {
            poliz.push_back({perehod, type_lexem::perehod});
        }
    };
    general_stack gen_stack;
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
    std::pair <std::string, type_lexem> get_hs (std::stack <std::pair<std::string, type_lexem>>& operands) {
        std::pair <std::string, type_lexem> ans = operands.top();
        operands.pop();
        return ans;
    }
    std::string get_function_call(func * function, std::vector <std::string>& parameters) {
        for (int i = 0; i < function->parameters.size(); i++) {
            function->TID->set_value(function->parameters[i]->id, parameters[i]);
        }
        std::stack <std::pair<std::string, type_lexem>> operands;
        for (auto u: function->poliz) {
            std::cout << u.first << " ";
        }
        std::cout << std::endl;
        // for (auto u: function->poliz) {
        for (int i = 0; i < function->poliz.size(); i++) {
            auto u = function->poliz[i];
            if (u.second == type_lexem::func_call) {
                func * actualFunc;
                int size;
                for (auto g: functions) {
                    if (u.first == g->name) {
                        actualFunc  = g;
                        size = g->parameters.size();
                    }
                }
                func * curFunc = new func(actualFunc);
                std::vector <std::string> params;
                while (size--) {
                    std::pair <std::string, type_lexem> hs_ = get_hs(operands);
                    if (hs_.second == type_lexem::variable || hs_.second == type_lexem::adress_variable) {
                        hs_.first = function->TID->get_value(hs_.first);
                    }
                    params.push_back(hs_.first);
                }
                std::reverse(params.begin(), params.end());
                std::string res = get_function_call(curFunc, params);
                std::cout << "result of " << curFunc->name<< " function: " << res << std::endl;
                operands.push(std::make_pair(res, type_lexem::literal));
            } else if (u.second == type_lexem::adress) {
                int adres = stoi(u.first);
                i++;
                std::string perehod = function->poliz[i].first;
                if (perehod == "!") {
                    i = adres - 1;
                } else if (perehod == "!F") {
                    if (operands.top().first == "0") {
                        i = adres - 1;
                    }
                }
            } else if (u.second != type_lexem::operation) {
                operands.push(u);
            } else if (u.first == "r") {
                std::pair <std::string, type_lexem> hs_ = get_hs(operands);
                if (hs_.second == type_lexem::variable || hs_.second == type_lexem::adress_variable) {
                    hs_.first = function->TID->get_value(hs_.first);
                }
                return hs_.first;
            } else if (u.first == ",") {
            } else if (u.first == "=") {
                std::pair <std::string, type_lexem> rhs_ = get_hs(operands), lhs_ = get_hs(operands);
                if (rhs_.second == type_lexem::variable || rhs_.second == type_lexem::adress_variable) {
                    rhs_.first = function->TID->get_value(rhs_.first);
                }
                function->TID->set_value(lhs_.first, rhs_.first);
            } else if (u.first == "+=") {
                std::pair <std::string, type_lexem> rhs_ = get_hs(operands), lhs_ = get_hs(operands);
                if (rhs_.second == type_lexem::variable || rhs_.second == type_lexem::adress_variable) {
                    rhs_.first = function->TID->get_value(rhs_.first);
                }
                std::string val = function->TID->get_value(lhs_.first);
                int lhs  = std::stoi(val), rhs = std::stoi(rhs_.first);
                int result_ = lhs + rhs;
                std::string result = std::to_string(result_);
                function->TID->set_value(lhs_.first, result);
            } else if (u.first == "-=") {
                std::pair <std::string, type_lexem> rhs_ = get_hs(operands), lhs_ = get_hs(operands);
                if (rhs_.second == type_lexem::variable || rhs_.second == type_lexem::adress_variable) {
                    rhs_.first = function->TID->get_value(rhs_.first);
                }
                std::string val = function->TID->get_value(lhs_.first);
                int lhs  = std::stoi(val), rhs = std::stoi(rhs_.first);
                int result_ = lhs - rhs;
                std::string result = std::to_string(result_);
                function->TID->set_value(lhs_.first, result);
            } else if (u.first == "++") {
                std::pair <std::string, type_lexem> hs_ = get_hs(operands);
                std::string val = function->TID->get_value(hs_.first);
                int lhs  = std::stoi(val);
                int result_ = lhs + 1;
                std::string result = std::to_string(result_);
                function->TID->set_value(hs_.first, result);
            } else if (u.first == "--") {
                std::pair <std::string, type_lexem> hs_ = get_hs(operands);
                std::string val = function->TID->get_value(hs_.first);
                int lhs  = std::stoi(val);
                int result_ = lhs - 1;
                std::string result = std::to_string(result_);
                function->TID->set_value(hs_.first, result);
            } else {
                std::pair <std::string, type_lexem> rhs_ = get_hs(operands), lhs_ = get_hs(operands);
                if (lhs_.second == type_lexem::variable || lhs_.second == type_lexem::adress_variable) {
                    lhs_.first = function->TID->get_value(lhs_.first);
                }
                if (rhs_.second == type_lexem::variable || rhs_.second == type_lexem::adress_variable) {
                    rhs_.first = function->TID->get_value(rhs_.first);
                }
                int lhs  = std::stoi(lhs_.first), rhs = std::stoi(rhs_.first);
                int result_;
                if (u.first == "+") {
                    result_ = lhs + rhs;
                } else if (u.first == "-") {
                    result_ = lhs - rhs;
                } else if (u.first == "*") {
                    result_ = lhs * rhs;
                } else if (u.first == "/") {
                    result_ = lhs / rhs;
                } else if (u.first == "%") {
                    result_ = lhs % rhs;
                    // result_ = rhs % lhs;
                } else if (u.first == "||") {
                    result_ = lhs || rhs;
                } else if (u.first == "&&") {
                    result_ = lhs * rhs;
                } else if (u.first == "|") {
                    result_ = lhs * rhs;
                } else if (u.first == "&") {
                    result_ = lhs * rhs;
                } else if (u.first == "<") {
                    result_ = lhs < rhs;
                } else if (u.first == "<=") {
                    result_ = lhs <= rhs;
                } else if (u.first == ">") {
                    result_ = lhs > rhs;
                } else if (u.first == ">=") {
                    result_ = lhs >= rhs;
                } else if (u.first == "==") {
                    result_ = lhs == rhs;
                } else if (u.first == "!=") {
                    result_ = lhs != rhs;
                }

                std::string result = std::to_string(result_);
                operands.push(std::make_pair(result, type_lexem::literal));
            }
        }
        if (operands.top().second == type_lexem::variable || operands.top().second == type_lexem::adress_variable) {
            operands.top().first = function->TID->get_value(operands.top().first);
        }
        return operands.top().first;
    }

    void check_bin() {
        std::string lhs = stack.types.top();
        stack.types.pop();
        std::string rhs = stack.types.top();
        stack.types.pop();
        std::string ops = stack.operations.top();
        stack.operations.pop();
        //std::cout << lhs << " " << ops << " " << rhs << std::endl;
        if (ops == ",") {
            stack.types.push(lhs);
            return;
        }
        if (stack.type1(lhs) && stack.type1(rhs)) {
            if (lhs == "int" || rhs == "int") {
                stack.types.push("int");
            } else if (lhs == "float" || rhs == "float") {
                stack.types.push("float");
            } else {
                stack.types.push("bool");
            }
        } else if (lhs == "string" && rhs == "string") {
            if (ops == "+" || ops == "==" || ops == "!=" || ops == "=") {
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
            //std::cout << ops << std::endl;
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
                std::string op = lexems[iter].word;
                ++iter;
                L11();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L10();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L9();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L8();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L7();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L6();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L4();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L3();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                L23();
                check_bin();
                gen_stack.push_operation(op);
            } else {
                break;
            }
        }
    }

    void L23() {
        if (lexems[iter].type == 8) {
            stack.pushOp(lexems[iter].word);
            std::string op = lexems[iter].word;
            ++iter;
            L2();
            check_unary();
            gen_stack.push_operation(op);
        } else L2();
    }

    void L2() {
        if (lexems[iter].type == 3 || lexems[iter].type == 2) {
            L1();
        } else if (iter < lexems.size() && lexems[iter].word == "(") {
            gen_stack.push_other(lexems[iter].word);
            ++iter;
            expression();
            if (iter < lexems.size() && lexems[iter].word == ")") {
                gen_stack.push_other(lexems[iter].word);
                ++iter;
            } else {
                error();
            }
        } else if (lexems[iter].word == "++") {
            stack.pushOp(lexems[iter].word);
            std::string op = lexems[iter].word;
            ++iter;
            L1();
            check_unary();
            gen_stack.push_operation(op);
        } else if (lexems[iter].word == "--") {
            stack.pushOp(lexems[iter].word);
            std::string op = lexems[iter].word;
            ++iter;
            L1();
            check_unary();
            gen_stack.push_operation(op);
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
            gen_stack.push_literal(lexems[iter].word, type_lexem::literal);
            ++iter;
        } else if (lexems[iter].type == 2) {
            if (lexems[iter + 1].word == "(") {
                std::string s = lexems[iter].word;
                function_call();
                gen_stack.push_literal(s, type_lexem::func_call);
            } else {
                std::string type = Tree.check_id(lexems[iter].word);
                if (type == "") throw std::runtime_error("Such variable does not exist: " + lexems[iter].word);
                if (lexems[iter + 1].word == "=") {
                    gen_stack.push_literal(lexems[iter].word, type_lexem::adress_variable);
                } else {
                    gen_stack.push_literal(lexems[iter].word, type_lexem::variable);
                }

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
        std::string Name = lexems[iter].word;
        id();
        check_expression(lexems[iter - 1].word, lexems[iter - 1].num_len);
        if (lexems[iter].word == "(") {
            gen_stack.push_other(lexems[iter].word);
            ++iter;
        } else error();
        func *functionCall = new func(" ", " ");
        expression1();
        functionCall->parameters.push_back(new parameter(stack.types.top()));
        stack.types.pop();
        while (lexems[iter].word == ",") {
            ++iter;
            expression1();
            functionCall->parameters.push_back(new parameter(stack.types.top()));
            stack.types.pop();
        }
        if (lexems[iter].word == ")") {
            gen_stack.push_other(lexems[iter].word);
            ++iter;
        } else error();
        stack.pushT(check_func(functionCall));
        /*
        func * curFunc;
        for (auto u: functions) {
            if (Name == u->name && functionCall->parameters.size() == u->parameters.size()) {
                curFunc  = u;
            }
        }
        std::vector <std::string> params;
        for (auto u: functionCall->parameters) {
            std::cout << u->id << std::endl;
            params.push_back(u->id);
        }
        std::string res = get_function_call(curFunc, params);
        std::cout << "RES " << res;*/
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
            gen_stack.push_literal(name, type_lexem::variable);
            gen_stack.push_operation(lexems[iter].word);
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

    void func_elif(std::vector<int>& adress) {
        iter++; // "elif" dont ++, becos we ++ now
        if (lexems[iter++].word != "(") error();

        expression();
        check_bool();

        gen_stack.clear_stack();

        int perehod_po_lzy = gen_stack.len_poliz();
        gen_stack.push_adres(0);
        gen_stack.push_perehod("!F");


        if (lexems[iter++].word != ")") error();
        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();


        adress.push_back(gen_stack.len_poliz());
        gen_stack.push_adres(0);
        gen_stack.push_perehod("!");

        gen_stack.replase_adres(perehod_po_lzy, gen_stack.len_poliz());

        if (lexems[iter].word == "elif") func_elif(adress);
    }

    void func_if() {
        iter++; // "if" dont ++, becos we ++ now
        if (lexems[iter++].word != "(") error();

        expression();
        check_bool();
        gen_stack.clear_stack();

        int perehod_po_lzy = gen_stack.len_poliz();
        gen_stack.push_adres(0);
        gen_stack.push_perehod("!F");

        if (lexems[iter++].word != ")") error();
        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();

        int exit = gen_stack.len_poliz();
        gen_stack.push_adres(0);
        gen_stack.push_perehod("!");

        gen_stack.replase_adres(perehod_po_lzy, gen_stack.len_poliz());
        std::vector<int> adress;

        if (lexems[iter].word == "elif") func_elif(adress);
        if (lexems[iter].word == "else") func_else();

        for (int i : adress) {
            gen_stack.replase_adres(i, gen_stack.len_poliz());
        }
        gen_stack.replase_adres(exit, gen_stack.len_poliz());
    }

    void func_for() {
        // Damir
        iter++;
        if (lexems[iter++].word != "(") error();
        if (lexems[iter].word != ";") {
            expression();
            stack.clear();
            gen_stack.clear_stack();
            if (lexems[iter++].word != ";") error();
        } else {
            iter++;
        }

        int begin_prowerka = gen_stack.len_poliz();

        if (lexems[iter].word != ";") {
            expression();
            check_bool();
            gen_stack.clear_stack();
            if (lexems[iter++].word != ";") error();
        } else {
            iter++;
        }

        gen_stack.push_adres(0);
        int perehod_po_lzy = gen_stack.len_poliz() - 1;
        gen_stack.push_perehod("!F");
        gen_stack.push_adres(0);
        int perehod_posle_prov = gen_stack.len_poliz() - 1;
        gen_stack.push_perehod("!");
        int perehod_plas_plas = gen_stack.len_poliz();

        if (lexems[iter].word != ")") {
            expression();
            stack.clear();
            gen_stack.clear_stack();
            if (lexems[iter++].word != ")") error();
        } else {
            iter++;
        }
        gen_stack.push_adres(begin_prowerka);
        gen_stack.push_perehod("!");

        if (lexems[iter++].word != "{") error();

        gen_stack.replase_adres(perehod_posle_prov, gen_stack.len_poliz());

        Tree.create_scope();
        stack.clear();
        body();

        if (lexems[iter++].word != "}") error();
        Tree.exit_scope();
        gen_stack.push_adres(perehod_plas_plas);
        gen_stack.push_perehod("!");
        gen_stack.replase_adres(perehod_po_lzy, gen_stack.len_poliz());
    }

    void func_while() {
        // Damir
        iter++;
        if (lexems[iter++].word != "(") error();
        int perehod_proverka = gen_stack.len_poliz();
        expression();
        check_bool();
        gen_stack.clear_stack();
        gen_stack.push_adres(0);
        int perehod_po_lzy = gen_stack.len_poliz() - 1;
        gen_stack.push_perehod("!F");
        if (lexems[iter++].word != ")") error();

        if (lexems[iter++].word != "{") error();
        Tree.create_scope();
        stack.clear();
        body();
        gen_stack.push_adres(perehod_proverka);
        gen_stack.push_perehod("!");
        if (lexems[iter++].word != "}") error();
        gen_stack.clear_stack();
        gen_stack.replase_adres(perehod_po_lzy, gen_stack.len_poliz());
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
        //std::cout << lexems[iter].word << std::endl;
        if (lexems[iter].word == ";") {
            gen_stack.clear_stack();
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
        } else if (lexems[iter].word == "return") {
            check_return1();
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
                //gen_stack.print();
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
    void check_return1 () {
        if (lexems[iter].word == "return") {
            ++iter;
        } else throw std::runtime_error("function does not contain a return");
        if (lexems[iter].word == ";") {
            checkPoint();
            return;
        }
        expression1();
        //std::string res = stack.types.top();
        //stack.types.pop();
        std::string s = "r";
        gen_stack.push_other(s);
        checkPoint();
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
        //create_scope
        //push_id
        push_function(cur_func, line);
        if (lexems[iter++].word != "{") error();
        body();
        //std::cout << lexems[iter].word << std::endl;
        //check_return(cur_func);
        if (lexems[iter++].word != "}") error();
        gen_stack.clear_stack();
        cur_func->poliz = gen_stack.poliz;
        gen_stack.clear();
        cur_func->TID = Tree.current_scope_;
        //push_function(cur_func, line);
        //std::cout << "HERE";
        //cur_func->TID->get_cur();
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
                std::string op = lexems[iter].word;
                ++iter;
                R10();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R9();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R8();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R7();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R6();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R4();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R3();
                check_bin();
                gen_stack.push_operation(op);
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
                std::string op = lexems[iter].word;
                ++iter;
                R23();
                check_bin();
                gen_stack.push_operation(op);
            } else {
                break;
            }
        }
    }
    //#
    void R23() {
        if (lexems[iter].type == 8) {
            stack.pushOp(lexems[iter].word);
            std::string op = lexems[iter].word;
            ++iter;
            R2();
            check_unary();
            gen_stack.push_operation(op);
        } else R2();
    }

    void R2() {
        if (lexems[iter].type == 3 || lexems[iter].type == 2) {
            R1();
        } else if (iter < lexems.size() && lexems[iter].word == "(") {
            gen_stack.push_other(lexems[iter].word);
            ++iter;
            expression();
            if (iter < lexems.size() && lexems[iter].word == ")") {
                gen_stack.push_other(lexems[iter].word);
                ++iter;
            } else {
                error();
            }
        } else if (lexems[iter].word == "++" || lexems[iter].word == "--") {
            stack.pushOp(lexems[iter].word);
            std::string op = lexems[iter].word;
            //gen_stack.push_operation(lexems[iter].word);
            ++iter;
            R1();
            check_unary();
            gen_stack.push_operation(op);
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
            gen_stack.push_literal(lexems[iter].word, type_lexem::literal);
            ++iter;
        } else if (lexems[iter].type == 2) {
            if (lexems[iter + 1].word == "(") {
                std::string s = lexems[iter].word;
                function_call();
                gen_stack.push_literal(s, type_lexem::func_call);
            } else {
                std::string type = Tree.check_id(lexems[iter].word);
                if (type == "") throw std::runtime_error("Such variable does not exist: " + lexems[iter].word);
                stack.pushT(type);
                if (lexems[iter + 1].word == "=") {
                    gen_stack.push_literal(lexems[iter].word, type_lexem::adress_variable);
                } else {
                    gen_stack.push_literal(lexems[iter].word, type_lexem::variable);
                }
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
    } catch (const std::invalid_argument &e) {
        std::cerr << "Parsing error: " << e.what() << '\n';
    }
    parser.generation();
}