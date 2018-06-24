//
// Created by oops on 18-6-13.
//

#include "../Header/Bignum.h"
#include "../Header/Util.h"
#include <algorithm>

using namespace std;

Bignum::Bignum() : Bignum(0) {
};

Bignum::~Bignum() = default;

Bignum::Bignum(long long num_int) {
    this->positive = num_int >= 0;
    this->pure_num_str = to_string(abs(num_int));
}

/**
 * 若输入的字符串不是数字，则返回0
 * 简单地处理下科学计数法
 * @param num_str
 */
Bignum::Bignum(string num_str) : Bignum(0) {
    string tmp = trim(num_str); //去除首尾空格
    regex pattern("^[-]?[0-9]+$");  //纯数字
    regex pattern_e("^[-]?[0-9]+.?[0-9]*e[0-9]+$"); //科学计数法
    if (regex_match(tmp, pattern)) {
        this->positive = tmp[0] != '-'; // true: >= 0; false: < 0
        unsigned long idx(tmp.find_first_not_of('0', positive ? 0 : 1));
        if (idx != string::npos) {
            this->pure_num_str = tmp.substr(idx); // 截取大整数的有效部分，如：-0000123 => 123
        } else
            this->pure_num_str = "0";
    } else if (regex_match(tmp, pattern_e)) {  //简单处理科学计数法
        vector<string> tokens;
        string splitSign = "e";
        splitString(tmp, tokens, splitSign);
        this->positive = tokens[0][0] != '-'; // true: >= 0; false: < 0
        unsigned long idx(tokens[0].find_first_not_of('0', positive ? 0 : 1));
        unsigned long point_idx(tokens[0].find_first_of('.'));
        if (idx != string::npos) {
            // 截取大整数的有效部分，如：-0000123 => 123
            this->pure_num_str = tokens[0]
                    .substr(idx, point_idx != string::npos ? point_idx - idx : tokens[0].length() - idx);
            Bignum e(tokens[1]);
            Bignum zero(0), one(1);
            if (e < Bignum(0)) {
                this->pure_num_str = "0";
            } else {
                do {
                    this->pure_num_str.append("0");
                    e = e - one;
                } while (e > zero);
            }
        } else
            this->pure_num_str = "0";
    }
}

Bignum::Bignum(const Bignum &num) {
    this->positive = num.positive;
    this->pure_num_str = num.pure_num_str;
}

const Bignum Bignum::negate() const {
    Bignum res_num(*this);
    res_num.positive = !this->positive;
    return res_num;
}

const Bignum Bignum::operator+(const Bignum &in) const {
    Bignum big = this->compare(in) >= 0 ? *this : in;
    Bignum small = *this > in ? in : *this;
    if (!this->isPositive() && !in.isPositive()) { //同时为负数
        return (this->negate() + in.negate()).negate();
    }
    if (!this->isPositive()) {
        return in - this->negate();
    }
    if (!in.isPositive()) { //一个是非负数，一个是负数（小的）
        return *this - in.negate();
    }
    /*
     * 两个大整数皆为非负数
     */
    string tmp_big = big.getPureNum();
    string tmp_small = small.getPureNum();
    string num_str;
    int carry(0), tmp;
    while (!tmp_small.empty()) {
        tmp = atoi((tmp_big.end() - 1).base()) + atoi((tmp_small.end() - 1).base()) + carry;
        tmp_big.pop_back(), tmp_small.pop_back();
        num_str.insert(0, to_string(tmp % 10));
        carry = tmp / 10;
    }
    while (!tmp_big.empty()) {
        tmp = atoi((tmp_big.end() - 1).base()) + carry;
        tmp_big.pop_back();
        num_str.insert(0, to_string(tmp % 10));
        carry = tmp / 10;
    }
    num_str.insert(0, to_string(carry));
    return Bignum(num_str);
}

const Bignum Bignum::operator-(const Bignum &in) const {
    Bignum big = this->compare(in) >= 0 ? *this : in;
    Bignum small = *this > in ? in : *this;
    if (!this->isPositive() && !in.isPositive()) { //同时为负数
        return (this->negate() - in.negate()).negate();
    }
    if (!this->isPositive()) {
        return (this->negate() + in).negate();
    }
    if (!in.isPositive()) { //一个是非负数，一个是负数（小的）
        return *this + in.negate();
    }
    /*
      * 两个大整数皆为非负数
      */
    string tmp_big = big.getPureNum();
    string tmp_small = small.getPureNum();
    string num_str;
    int borrow(0), tmp, big_digit, small_digit;
    while (!tmp_small.empty()) {
        big_digit = atoi((tmp_big.end() - 1).base()) - borrow;
        small_digit = atoi((tmp_small.end() - 1).base());
        borrow = big_digit >= small_digit ? 0 : 1;
        tmp = big_digit - small_digit + borrow * 10;
        tmp_big.pop_back(), tmp_small.pop_back();
        num_str.insert(0, to_string(tmp % 10));
    }
    while (!tmp_big.empty()) {
        tmp = atoi((tmp_big.end() - 1).base()) - borrow;
        tmp_big.pop_back();
        borrow = tmp >= 0 ? 0 : 1;
        num_str.insert(0, to_string((tmp + 10) % 10));
    }
    if (*this < in) {
        num_str.insert(0, "-");
    }
    return Bignum(num_str);
}

const Bignum Bignum::operator*(const Bignum &in) const {
    vector<vector<int>> tmp_vec;
    Bignum big = this->compare(in) >= 0 ? *this : in;
    Bignum small = *this > in ? in : *this;
    if (!big.isPositive() && !small.isPositive()) { //同时为负数
        return big.negate() * small.negate();
    }
    if (!small.isPositive()) { //一个是非负数，一个是负数（小的）
        return (big * small.negate()).negate();
    }
    /*
      * 两个大整数皆为非负数
      */
    string tmp_big = big.getPureNum();
    string tmp_small = small.getPureNum();
    unsigned long big_len = tmp_big.length();
    int offset = 0;
    while (!tmp_small.empty()) {
        vector<int> tmp;
        int small_bit = atoi((tmp_small.end() - 1).base());
        for (int i = 0; i < big_len; ++i) {
            string big_bit = to_string(tmp_big[i] - '0');
            tmp.push_back(atoi(big_bit.c_str()) * small_bit);
        }
        for (int j = 0; j < offset; ++j) {
            tmp.push_back(0);
        }
        reverse(tmp.begin(), tmp.end());
        tmp_vec.push_back(tmp);
        tmp_small.pop_back();
        offset++;
    }
    auto last = tmp_vec.end() - 1;
    for (auto it = tmp_vec.begin(); it != last; it++) {
        for (int i = 0; i < it.base()->size(); ++i) {
            (*last.base())[i] += (*it.base())[i];
        }
    }
    int carry(0), tmp;
    string num_str;
    for (int k : *last.base()) {
        tmp = k + carry;
        num_str.insert(0, to_string(tmp % 10));
        carry = tmp / 10;
    }
    num_str.insert(0, to_string(carry));
    return Bignum(num_str);
}

const Bignum Bignum::operator/(const Bignum &divisor) const {
    Bignum zero(0);
    if (divisor == zero) {
        cout << "除数不能为0" << endl;
        return Bignum(-1);
    }
    Bignum dividend = *this;
    if (!dividend.isPositive() && !divisor.isPositive()) { //同时为负数
        return dividend.negate() / divisor.negate();
    }
    if (!dividend.isPositive()) {
        return (dividend.negate() / divisor).negate();
    }
    if (!divisor.isPositive()) {
        return (dividend / divisor.negate()).negate();
    }
    /*
     * 两个大整数皆为非负数
     */
    if (dividend < divisor) {
        return zero;
    }
    string tmp_dividend = dividend.getPureNum();
    string tmp_divisor = divisor.getPureNum();
    long len = tmp_dividend.size() - tmp_divisor.size();
    if (len < 0) {
        return zero;
    }
    if (dividend == divisor) {
        return Bignum(1);
    }
    string num_str;
    Bignum nine(9), num1(tmp_dividend.substr(0, tmp_divisor.size())), num2(tmp_divisor);
    if (num1 < num2) {
        len--;
    }
    while (len-- >= 0) {
        while (tmp_dividend.length() > (tmp_divisor.length())) {
            tmp_divisor += "0";
        }
        num1 = tmp_dividend;
        num2 = tmp_divisor;
        if (num1 < num2) {
            tmp_dividend += "0";
            num1 = tmp_dividend;
        }

        if (num1 == num2 && num_str.empty()) {
            num_str = "1";
        }
        for (int i = 9; i > 0; i--) {
            Bignum num3 = num2 * Bignum(i);
            if (num3 < num1 || num3 == num1) {
                num1 = num1 - num3;
                num_str += to_string(i);
                break;
            }
            if (i == 1) {
                num_str += "0";
            }
        }
        tmp_dividend = num1.getPureNum();
        if (num1 == zero)
            break;
    }
    return Bignum(num_str);
}

istream &operator>>(istream &in, Bignum &num) {
    string num_str;
    in >> num_str;
    num = num_str;
    return in;
}

ostream &operator<<(ostream &out, const Bignum &num) {
    out << (num.isPositive() ? "" : "-") << num.getPureNum();
    return out;
}

bool operator>(Bignum a, Bignum b) {
    return a.compare(b) > 0;
}

bool operator<(Bignum a, Bignum b) {
    return a.compare(b) < 0;
}

bool operator==(Bignum a, Bignum b) {
    return a.compare(b) == 0;
}

const int Bignum::compare(Bignum other) const {
    if (this->positive && !other.positive)
        return 1;
    if (!this->positive && other.positive)
        return -1;
    if (this->positive && other.positive) {
        if (this->pure_num_str.length() > other.pure_num_str.length())
            return 1;
        if (this->pure_num_str.length() < other.pure_num_str.length())
            return -1;
        unsigned long length = this->pure_num_str.length();
        for (int i = 0; i < length; ++i) {
            if (this->pure_num_str[i] > other.pure_num_str[i])
                return 1;
            else if (this->pure_num_str[i] < other.pure_num_str[i])
                return -1;
        }
        return 0;
    }
    Bignum tmp_a = *this, tmp_b = other;
    return tmp_b.negate().compare(tmp_a.negate());
}

const bool Bignum::isPositive() const {
    return positive;
}

const string Bignum::getPureNum() const {
    return pure_num_str;
}