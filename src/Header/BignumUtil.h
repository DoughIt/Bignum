//
// Created by oops on 18-6-23.
//

#ifndef BIGNUM_BIGNUMUTIL_H
#define BIGNUM_BIGNUMUTIL_H

#include <string>
#include <map>
#include "Bignum.h"
#include "Util.h"

class BignumUtil {
    /**
    *
    * @param line 表达式
    * @param res 存储操作数和操作符
    */
    static void parse(string line, vector<string> &res);

public:
    /**
     * 异常处理类
     */
    class Over {
        const string message;   //异常信息
    public:
        Over(const string msg = 0) : message(msg) {
        };

        ~Over();

        string getMessage();
    };

    /**
    *
    * @param a 操作数a
    * @param b 操作数b
    * @param op 操作符
    * @return Bignum
    */
    static Bignum calculate(const Bignum &a, const Bignum &b, const char &op);

    /**
    *
    * @param line 表达式
    * @param max_length 大整数的最大有效长度
    * @return Bignum
    */
    static Bignum execute(string &line, long max_length = -1);
};


#endif //BIGNUM_BIGNUMUTIL_H
