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
    std::string value;
    bool operator == (const parameter & other) {
        if (type == other.type) return true;
        return false;
    }
};

struct tid {
public:

    tid() {}
    tid(tid * other) {
        vec = other->vec;
    }
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
        for (parameter& i : vec) {
            if (i.id == x) {
                return i.type;
            }
        }
        // throw std::invalid_argument("such variable does not exist");
        return "";
    }
    std::string get_val(std::string x) {
        for (parameter& i : vec) {
            if (i.id == x) {
                return i.value;
            }
        }
        return "";
    }
    bool set_val(std::string x, const std::string& val) {
       // std::cout << "here" << std::endl;
        for (parameter& i : vec) {
           // std::cout << i.id << " " << x << std::endl;
            if (i.id == x) {
                i.value = val;
                return 1;
            }
        }
        return 0;
    }
    void get_cur () {
        for (auto u: vec) {
            std::cout << u.id << " ";
        }
        std::cout << std::endl;
    }
private:
    std::vector<parameter> vec;
};


struct tree_tid {
public:
    tree_tid() {}
    tree_tid(const tree_tid& other) : me_(other.me_) {
        for (const auto& child : other.child_) {
            auto child_copy = std::make_unique<tree_tid>(*child);
            child_copy->father_ = this;
            child_.emplace_back(std::move(child_copy));
        }
        current_scope_ = this;
    }
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
    std::string check_id_me(std::string x) {
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

    std::string check_id(const std::string& x) {
        tree_tid* scope = current_scope_;
        while (scope) {
            std::string result = scope->me_.check_id(const_cast<std::string&>(x));
            if (!result.empty()) {
                return result;
            }
            scope = scope->father_;
        }
        return "";
    }
    std::string get_value(const std::string& x) {
        tree_tid* scope = current_scope_;
        while (scope) {
            std::string result = scope->me_.get_val(const_cast<std::string&>(x));
            if (!result.empty()) {
                return result;
            }
            scope = scope->father_;
        }
        return "";
    }
    void set_value(const std::string& x, const std::string& val) {
        tree_tid* scope = current_scope_;
        while (scope) {
            if (scope->me_.set_val(x, val)) {
                break;
            }
            scope = scope->father_;
        }
    }
    void get_cur() {
        current_scope_->me_.get_cur();
    }
    tree_tid* current_scope_ = this;
private:
    tid me_;
    tree_tid* father_ = nullptr;
    std::vector<std::unique_ptr<tree_tid>> child_;

};