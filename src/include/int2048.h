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
  /**
   * @param OPTIMIZATION_LEVEL_: 选择压位优化激进程度
   *  OPTIMIZATION_LEVEL_ = 0 : 进行5位压位，粗略估计有效位数在「四千万位」以上，
   *                            主要瓶颈在于NTT模数的原根阶数，但是效率较低
   *  OPTIMIZATION_LEVEL_ = 1 : 进行6位压位，粗略估计有效位数只有「两百万位」左右，
   *                            主要瓶颈在于CRT合并时的值域上限，相对稳妥
   *  OPTIMIZATION_LEVEL_ = 2 : 进行7位压位，粗略估计有效位数只有「五万位」左右，
   *                            主要瓶颈仍然在于CRT合并时的值域上限，不宜采用
   * 
   *  进行5位压位时，瓶颈已经在于NTT模数的原根阶数，再设置更低的优化挡位没有意义。
   * 
   *  进行8位压位时，粗略估计有效位数已经骤减至「两千位」左右，而进行9位压位时，
   *  有效位数已经不足「一百位」，所以并未设置8位及以上的优化挡位，事实上，即便
   *  是7位压位也不建议采用，因为「五万位」的意义太过局限，其作为大整数是不合格的。
   */
  static constexpr unsigned int OPTIMIZATION_LEVEL_ = 1;
  static constexpr unsigned int CHOOSE_BASE_[3] = {100000, 1000000, 10000000}{5, 6, 7};
  static constexpr unsigned int CHOOSE_BASE_SIZE_[3] = {5, 6, 7};
  static constexpr unsigned int BASE_ = CHOOSE_BASE_[OPTIMIZATION_LEVEL_]; // 选择BASE_进制
  static constexpr unsigned int BASE_SIZE_ = CHOOSE_BASE_SIZE_[OPTIMIZATION_LEVEL_]; // 一个BASE_的长度
  static constexpr unsigned long long MODULE[2] = {998244353ull, 469762049ull}; // 模数
  static constexpr unsigned long long PRIMITIVE_ROOT = 3ull; // 两个模数共同的原根
  static constexpr unsigned long long INVERSE_OF_MODULE_0_MOD_MODULE_1 = 208783132ull; // MODULE_0在模MODULE_1意义下的逆元
  static constexpr unsigned int TRIVIAL_RECIPROCAL = 200;
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
  unsigned int &operator[](const unsigned int &);
  const unsigned int &operator[](const unsigned int &) const;
  const unsigned int size() const;
  const bool empty() const;
  void push_back(const unsigned int &);
  void resize(const unsigned int &, const unsigned int &);
public:
  // 空构造函数
  int2048();
  // long long 构造函数
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);
  // 最低位补count个0块
  void expand(int);
  // 最低的count个块全部舍弃
  void shrink(int);
  // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
  // 如果需要，可以自行增加其他所需的函数
  // ===================================
  // Integer1
  // ===================================

  // 读入一个大整数
  void read(const std::string &);
  void read(long long);
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
  /**
   * 特判简单情况
   */
  friend bool divide_trivial(const int2048 &, const int2048 &, int2048 &, int2048 &);
  /**
   * [Knuth D]
   * 使用朴素估商办法（Knuth D）来计算小规模除法
   * 需要保证A和B全部非负，且保证B非零
   * 计算出A = Q * B + R
   * 其中0 <= R < B
   * Q和R写入对应位置
   */
  friend void divide_basic(const int2048 &, const int2048 &, int2048 &, int2048 &);
  /**
   * 传入一个完成归一化的分母B > 0
   * 假设B有n位，则BASE_ ^ n / 2 <= B < BASE_ ^ n
   * 那么取S = BASE_ ^ (2 * n)
   * 目标是返回floor(S / B)
   */
  friend void compute_reciprocal(const int2048 &, int2048 &);
  /**
   * [Newton's method]
   * 使用牛顿迭代法来计算大规模除法
   * 需要保证A和B全部非负，且保证B非零
   * 计算出A = Q * B + R
   * 其中0 <= R < B
   * Q和R写入对应位置
   */
  friend void divide_advanced(int2048, int2048, int2048 &, int2048 &);
  /**
   * 最终除法接口
   * 不做任何要求
   */
  friend void divide(const int2048 &, const int2048 &, int2048 &, int2048 &);
  // 选择商块大小
  friend int choose_k(const int2048 &, const int2048 &);
  int2048 &operator/=(const int2048);
  friend int2048 operator/(const int2048 &, const int2048 &);
  int2048 &operator%=(const int2048);
  friend int2048 operator%(const int2048 &, const int2048 &);

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
