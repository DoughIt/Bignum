//
// Created by oops on 18-6-15.
//

#ifndef BIGNUM_UTIL_H
#define BIGNUM_UTIL_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * 去除首尾空格
 * @param s 字符串
 * @return 去除首尾空格后的字符串
 */
string trim(string &s);

/**
 * 以任意字符集合切割字符串
 * @param s 待切割字符串
 * @param v 切割后的子串集合
 * @param c 分隔符集合
 */
void splitString(string &s, vector<string> &v, string &c);


/**
 * 判断op是否属于四种操作符+、-、*、/
 * @param op
 * @return
 */
bool isOperator(string op);

#endif //BIGNUM_UTIL_H
