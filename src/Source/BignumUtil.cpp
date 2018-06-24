//
// Created by oops on 18-6-23.
//

#include "../Header/BignumUtil.h"

/**
 *
 * @param a 操作数a
 * @param b 操作数b
 * @param op 操作符
 * @return Bignum
 */
Bignum BignumUtil::calculate(const Bignum &a, const Bignum &b, const char &op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            break;
    }
    return Bignum();
}

/**
 *
 * @param line 表达式
 * @param max_length 大整数的最大有效长度
 * @return Bignum
 */
Bignum BignumUtil::execute(string &line, long max_length) {
    vector<string> ops;
    vector<Bignum> operands;
    vector<string> operators;
    parse(line, ops);
    for (const string &op:ops) {
        if (isOperator(op)) {
            operators.push_back(op);
        } else {
            Bignum num(op);
            if (num.getPureNum().length() < max_length || max_length < 0)
                operands.emplace_back(num);
            else
                // 大整数有效长度超过max_length则抛出异常
                throw Over("输入整数过大！有效长度（例如：-001123有效长度为4）应小于" + to_string(max_length));
        }
    }
    while (!operators.empty()) {
        Bignum tmp = calculate(operands[0], operands[1], operators[0][0]);
        if (tmp.getPureNum().length() < max_length || max_length < 0) {
            operands.erase(operands.begin());
            operands.erase(operands.begin());
            operands.insert(operands.begin(), tmp);
            operators.erase(operators.begin());
        } else
            // 大整数有效长度超过max_length则抛出异常
            throw Over("生成的整数过大！有效长度（例如：-001123有效长度为4）应小于" + to_string(max_length));
    }
    return *operands.begin().base();
}

/**
 *
 * @param line 表达式
 * @param res 存储操作数和操作符
 */
void BignumUtil::parse(string line, vector<string> &res) {
    string sign = " \n\t\f\r";
    splitString(line, res, sign);
}


BignumUtil::Over::~Over() = default;

/**
 *
 * @return 获取异常信息
 */
string BignumUtil::Over::getMessage() {
    return this->message;
}
