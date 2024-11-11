#pragma once

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
        for (var i: vec) {
            if (i == x) {
                return false;
            }
        }
        vec.push_back(x);
        return true;
    }

    std::string check_id(var& x) {
        for (var i : vec) {
            if (i == x) {
                return i.get_tupe();
            }
        }
        return "";
    }

private:
    std::vector<var> vec;
};
