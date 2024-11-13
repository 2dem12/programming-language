#include <string>
#include <vector>
#include <memory>
#include <tuple>

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
};

struct tid {
public:
    bool push_id(var& x) {
        for (var& i : vec) {
            if (i == x) {
                return false;
            }
        }
        vec.push_back(x);
        return true;
    }

    std::string check_id(var& x) {
        for (var& i : vec) {
            if (i == x) {
                return i.get_tupe();
            }
        }
        return "";
    }

private:
    std::vector<var> vec;
};

struct tree_tid {
public:
    void create_scope(const var& x) {
        auto new_scope = std::make_unique<tree_tid>();
        new_scope->father_ = this;
        new_scope->me_.push_id(const_cast<var&>(x));

        child_.push_back(std::move(new_scope));
        current_scope_ = child_.back().get();
    }

    void exit_scope() {
        if (current_scope_ && current_scope_->father_) {
            current_scope_ = current_scope_->father_;
        }
    }

    std::string check_id(const var& x) {
        tree_tid* scope = current_scope_;
        while (scope) {
            std::string result = scope->me_.check_id(const_cast<var&>(x));
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
