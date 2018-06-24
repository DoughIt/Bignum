//
// Created by oops on 18-6-15.
//
#include "../Header/Bignum.h"
#include "../Header/Util.h"

/**
 * 去除首尾空格
 * @param s 字符串
 * @return 去除首尾空格后的字符串
 */
string trim(string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
}

/**
 * 以任意字符集合切割字符串
 * @param s 待切割字符串
 * @param v 切割后的子串集合
 * @param c 分隔符集合
 */
void splitString(string &s, vector<string> &v, string &c) {   //将字符串按指定字符集分割
    string::size_type pos1, pos2;
    pos2 = s.find_first_of(c);
    pos1 = 0;
    while (string::npos != pos2) {
        if ((pos2 - pos1) != 0u)
            v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = s.find_first_of(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

/**
 * 判断op是否属于四种操作符+、-、*、/
 * @param op
 * @return
 */
bool isOperator(string op) {
    op = trim(op);
    return op == "+" || op == "-" || op == "*" || op == "/";
}

