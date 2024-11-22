#include <string>
#include <vector>
#include <memory>
#include <tuple>
/*
struct var {
public:
    var(std::string name_, std::string tupe_, std::string value_)
        : name(std::move(name_)), tupe(std::move(tupe_)), value(std::move(value_)) {}

    std::string get_name() {
        return name;
    }

    std::string get_tupe() {
        return tupe;
    }

    std::string get_value() {
        return value;
    }

    bool operator==(var& other) {
        return std::tie(name, tupe, value) == std::tie(other.name, other.tupe, other.value);
    }

private:
    std::string name;
    std::string tupe;
    std::string value;
};*/

struct parameter {
    parameter(std::string type_, std::string id_): type(type_), id(id_) {}
    parameter(std::string type_): type(type_), id(" ") {}
    std::string type;
    std::string id;
    bool operator == (const parameter & other) {
        if (type == other.type) return true;
        return false;
    }
};

struct tid {
public:
    bool push_id(parameter& x) {
        for (parameter& i : vec) {
            if (i.id == x.id && i.type == x.type) {
                throw std::invalid_argument("the same variable already exists");
                return false;
            }
        }
        vec.push_back(x);
        return true;
    }

    std::string check_id(parameter& x) {
        for (parameter& i : vec) {
            if (i == x) {
                return i.type;
            }
        }
        return "";
    }

    std::string check_id(std::string x) {
        //std::cout << x << " HERE"<< std::endl;
        for (parameter& i : vec) {
            std::cout << i.id << std::endl;
            if (i.id == x) {
                return i.type;
            }
        }
        throw std::invalid_argument("such variable does not exist");
        //return "";
    }

private:
    std::vector<parameter> vec;
};


struct tree_tid {
public:
    void create_scope() {
        auto new_scope = std::make_unique<tree_tid>();
        new_scope->father_ = current_scope_;
        //new_scope->me_.push_id(const_cast<parameter&>(x));
        current_scope_->child_.push_back(std::move(new_scope));
        current_scope_ = current_scope_->child_.back().get();
    }

    void exit_scope() {
        if (current_scope_ && current_scope_->father_) {
            current_scope_ = current_scope_->father_;
        }
    }

    void push_id(parameter& x) {
        current_scope_->me_.push_id(x);
        // me_.push_id(x);
    }
    std::string check_id(std::string x) {
        return current_scope_->me_.check_id(x);
    }

    std::string check_id(const parameter& x) {
        tree_tid* scope = current_scope_;
        while (scope) {
            std::string result = scope->me_.check_id(const_cast<parameter&>(x));
            if (!result.empty()) {
                return result;
            }
            scope = scope->father_;
        }
        return "";
    }

private:
    tid me_;
    tree_tid* father_ = nullptr;
    std::vector<std::unique_ptr<tree_tid>> child_;
    tree_tid* current_scope_ = this;
};
