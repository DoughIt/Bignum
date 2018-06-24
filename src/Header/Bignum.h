//
// Created by oops on 18-6-13.
//

#ifndef BIGNUM_H
#define BIGNUM_H

#include <string>
#include <iostream>
#include <regex>

using namespace std;

class Bignum {
public:
    Bignum();

    explicit Bignum(long long num_int);

    Bignum(string num_str);

    Bignum(const Bignum &num);

    virtual ~Bignum();

    const Bignum negate() const;

    const Bignum operator+(const Bignum &in) const;

    const Bignum operator-(const Bignum &in) const;

    const Bignum operator*(const Bignum &in) const;

    const Bignum operator/(const Bignum &divisor) const;

    friend istream &operator>>(istream &in, Bignum &num);

    friend ostream &operator<<(ostream &out, const Bignum &num);

    friend bool operator>(Bignum a, Bignum b);

    friend bool operator<(Bignum a, Bignum b);

    friend bool operator==(Bignum a, Bignum b);

    const int compare(Bignum other) const;

    const bool isPositive() const;

    const string getPureNum() const;

private:
    bool positive = true;  //true 代表整数或0，false代表负数
    string pure_num_str; //大整数的数字部分


};


#endif //BIGNUM_H
