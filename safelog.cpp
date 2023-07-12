/*
在c++11版本中，如何实现如下功能的打印日志函数：可以接受" the value is %5f" (value) 形式的参数
如果要支持任意格式的格式化字符串呢？除了printf函数，C++11还有哪些字符串格式化的函数可用？
支持任意多个参数，类型可以为整型，浮点型，和字符串. 你可以把log函数整理为一个类
*/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>
#include <mutex>

class Logger {
public:
    template<typename... Args>
    void log(const std::string& format, Args... args) {
        std::ostringstream ss(get_buffer(), get_buffer_size());
        int i = 0;
        int n = format.size();
        while (i < n) {
            char c = format[i];
            if (c == '{' && i + 1 < n && format[i + 1] == '}') {
                ss << std::forward<decltype(args)>(args)...;
                i += 2;
            } else {
                ss << c;
                i++;
            }
        }
        std::cout << ss.str();
        ss.clear();
    }

private:
    static std::array<char, 1024>& get_buffer() {
        static std::array<char, 1024> buffer;
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        return buffer;
    }

    static std::size_t get_buffer_size() {
        return 1024;
    }
};

int main() {
    Logger logger;
    int i = 42;
    double d = 3.14159;
    std::string s = "hello";
    logger.log("i={}, d={}, s={}\n", i, d, s);
    return 0;
}
