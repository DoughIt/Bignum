#include <iostream>
#include <fstream>
#include "../Header/BignumUtil.h"


using namespace std;


int main() {
//    Bignum num1("123456789123456789123456789"), num2("123456789123456789"), num3("987654321");
//    Bignum num4("123456789123456789"), num5("987654321987654321"), num6("23456789123456789");
//    Bignum num7("12345678901234567890"), num8("1234567890");
//    Bignum num9("121932631356500531347203169112635269"), num10("2");
//
//    cout << num1 + num2 - num3 << endl;
//    cout << num7 - num8 << endl;
//    cout << num4 * num5 << endl;
//    cout << num4 * num5 / num6 << endl;
//    cout << num9 / num6 << endl;

    string in_filename("sample.in");
    string out_filename("sample.out");
    ifstream in(in_filename);
    ofstream out(out_filename);
    string line;
    getline(in, line);
    int row = atoi(line.c_str());
    while (row-- > 0) {
        if (getline(in, line)) {
            try {
                Bignum res = BignumUtil::execute(line, 100);    //执行表达式line，限制大整数最大长度为100
                out << res << endl;
            } catch (BignumUtil::Over &e) {
                out << e.getMessage() << endl;
            }
        }
    }
    int times;
    cout << "请输入测试次数：";
    cin >> times;
    cout << endl;
    while (times > 0) {
        Bignum a, b, c;
        cout << "测试整数运算，请输入三个大整数（以空格分隔）：";
        cin >> a >> b >> c;
        cout << a << " + " << b << " + " << c << " = " << a + b + c << endl;
        cout << a << " + " << b << " - " << c << " = " << a + b - c << endl;
        cout << a << " * " << b << " * " << c << " = " << a * b * c << endl;
        cout << a << " * " << b << " / " << c << " = " << a * b / c << endl;
        cout << "(" << a << " + " << b << ")" << " * " << c << " = " << (a + b) * c << endl;
        cout << "(" << a << " + " << b << ")" << " / " << c << " = " << (a + b) / c << endl;
        times--;
    }
    return 0;
}