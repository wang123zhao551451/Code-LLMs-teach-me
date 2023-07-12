/* 请帮我实现一个浮点数累计和的c++类CZLPHighAddition，包含如下成员函数：
   plus(double), 将输入的浮点数按照Neumaier算法累和；clear() 将和清空；
   get()得到当前的累和；包含如下成员变量：result : 当前的累和 error：当前的误差 */
class CZLPHighAddition {
private:
    double result; // 当前的累和
    double error;  // 当前的误差

public:
    CZLPHighAddition() {
        result = 0.0;
        error = 0.0;
    }

    // 将输入的浮点数按照Neumaier算法累加
    void plus(double x) {
        double t = result + x;
        if (fabs(result) >= fabs(x)) {
            error += (result - t) + x;
        } else {
            error += (x - t) + result;
        }
        result = t + error;
        error -= (result - t);
    }

    // 将和清空
    void clear() {
        result = 0.0;
        error = 0.0;
    }

    // 得到当前的累和
    double get() {
        return result;
    }
};
