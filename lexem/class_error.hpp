#include <string>


class invalid_argument : public std::exception {
public:
    // Конструктор с инициализацией
    invalid_argument(int num_line, std::string& line)
        : num_line_(std::to_string(num_line)), line_(line) {
        ans_ = "Invalid symbols \"" + line_ + "\" in line: " + num_line_;
    }

    // Переопределение метода what()
    const char* what() const noexcept override {
        return ans_.c_str();
    }

private:
    std::string num_line_;
    std::string line_;
    std::string ans_;
};