#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
struct omega_table;
class int2048 {
private:
  static constexpr unsigned int BASE_ = 100000; // 选择BASE_进制
  static constexpr unsigned int BASE_SIZE_ = 5; // 一个BASE_的长度
  static constexpr unsigned long long MODULE[2] = {998244353ull, 469762049ull}; // 模数
  static constexpr unsigned long long PRIMITIVE_ROOT = 3ull; // 两个模数共同的原根
  static constexpr unsigned long long INVERSE_OF_MODULE_0_MOD_MODULE_1 = 208783132ull; // MODULE_0在模MODULE_1意义下的逆元
  friend constexpr omega_table make_omega_table();
  protected:
  std::vector<unsigned int> digits_; // 数位
  bool negative_; // 是否为负
  // 判断是否为0
  bool is_zero() const;
  /**
   * 标准化，原则
   * 1. 无前导0
   * 2. 不存在负0
   */
  void normalize();
public:
  // 空构造函数
  int2048();
  // long long 构造函数
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);

  // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
  // 如果需要，可以自行增加其他所需的函数
  // ===================================
  // Integer1
  // ===================================

  // 读入一个大整数
  void read(const std::string &);
  // 输出储存的大整数，无需换行
  void print() const;

  // 加上一个大整数
  int2048 &add(const int2048 &);
  // 返回两个大整数之和
  friend int2048 add(int2048, const int2048 &);

  // 减去一个大整数
  int2048 &minus(const int2048 &);
  // 返回两个大整数之差
  friend int2048 minus(int2048, const int2048 &);

  // ===================================
  // Integer2
  // ===================================

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);
  int2048 &operator=(const long long &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);
  int2048 &operator+=(const long long &);
  friend int2048 operator+(int2048, const long long &);
  friend int2048 operator+(const long long &, int2048);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);
  int2048 &operator-=(const long long &);
  friend int2048 operator-(int2048, const long long &);
  friend int2048 operator-(const long long &, int2048);

  template <bool use_module>
  friend void NTT(
    std::vector<unsigned int> &,
    const unsigned int &,
    const unsigned int &,
    const bool &);
  int2048 &operator*=(const int2048 &);
  /**
   * 将A和B在模MODULE意义下相乘
   * 结果写回A
   */
  template <bool use_module>
  friend void multiply_single_module(int2048 &, int2048);
  /**
   * 将(*this)和B相乘
   * 结果写回(*this)
   */
  int2048 &multiply(int2048);
  friend void merge(unsigned int, unsigned int, unsigned long long&);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};
} // namespace sjtu

#endif
