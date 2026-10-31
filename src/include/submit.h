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
class int2048 {
private:
  static constexpr unsigned int BASE_ = 100000; // 选择BASE_进制
  static constexpr unsigned int BASE_SIZE_ = 5; // 一个BASE_的长度
  
  std::vector<unsigned int> digits_; // 数位
  bool negative_; // 是否为负
  
public:
  // 空构造函数
  int2048();
  // long long 构造函数
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);
  int2048(int2048 &&);

  // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
  // 如果需要，可以自行增加其他所需的函数
  // ===================================
  // Integer1
  // ===================================

  // 判断是否为0
  bool is_zero() const;

  /**
   * 标准化，原则
   * 1. 无前导0
   * 2. 不存在负0
   */
  void normalize();

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
  int2048 &operator=(int2048 &&);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
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

namespace sjtu {

bool int2048::is_zero() const{
    return digits_.empty();
}
void int2048::normalize() {
    while ((!digits_.empty()) && (digits_.back() == 0)) {
        digits_.pop_back();
    }
    if (digits_.empty()) {
      negative_ = false;
    }
    return;
  }
int2048::int2048() {
    negative_ = false;
    return;
} // default的默认初始化不保证把数值都清零. 此处手动清零negative_
int2048::int2048(long long origin_value) {
    negative_ = false;
    if (origin_value == 0) {
        return;
    }
    unsigned long long value = static_cast<unsigned long long>(origin_value);
    if (origin_value < 0) {
        negative_ = true;
        // 如果origin_value < 0，那么此时value里面存储的应该是 ULL_MAX + origin_value
        value = 0ull - value;
        // ULL_MAX - (ULL_MAX + origin_value) = -origin_value
        // 这样是为了防止value_ = -LL_MAX，然后因为补码还是自己直接UB
    }
    while (value) {
        if (value < BASE_) {
            digits_.push_back(value);
            break;
        }
        long long res = value / BASE_;
        digits_.push_back(static_cast<unsigned int>(value - res * BASE_));
        value = res;
    }
    normalize();
    return;
}
int2048::int2048(const std::string &value) {
    int value_size = value.size();
    if (!value_size) {
        negative_ = false;
        return;
    }
    negative_ = (value[0] == '-');
    int non_negative = (negative_ ^ 1);
    int temp, upperbound, base_size = BASE_SIZE_;
    for (int i = value_size - 1; i >= negative_; i -= base_size) {
        if (i + non_negative < base_size) {
            upperbound = i + non_negative - 1;
        } else {
            upperbound = base_size - 1;
        }
        temp = 0;
        for (int j = upperbound; j >= 0; --j) {
            temp = (temp << 3) + (temp << 1) + value[i - j] - '0';
        }
        digits_.push_back(temp);
    }
    normalize();
    return;
}
int2048::int2048(const int2048 &) = default; // 采用默认拷贝构造
int2048::int2048(int2048 &&) = default; // 采用默认移动构造
int2048 &int2048::operator=(const int2048 &) = default; // 采用默认拷贝赋值
int2048 &int2048::operator=(int2048 &&) = default; // 采用默认移动赋值
void int2048::read(const std::string &value) {
    int value_size = value.size();
    if (!value_size) {
        digits_.clear();
        negative_ = false;
        return;
    }
    digits_.clear();
    negative_ = (value[0] == '-');
    int non_negative = (negative_ ^ 1);
    int temp, upperbound;
    for (int i = value_size - 1; i >= negative_; i -= BASE_SIZE_) {
        if (i + non_negative < BASE_SIZE_) {
            upperbound = i + non_negative - 1;
        } else {
            upperbound = BASE_SIZE_ - 1;
        }
        temp = 0;
        for (int j = upperbound; j >= 0; --j) {
            temp = (temp << 3) + (temp << 1) + value[i - j] - '0';
        }
        digits_.push_back(temp);
    }
    normalize();
    return;
}

// 输出最高位数值
void write_highest_digit(const int &digit) {
    if (digit < 10) {
        putchar(digit + '0');
    } else {
        int temp = digit / 10;
        write_highest_digit(temp);
        putchar((digit - ((temp << 3) + (temp << 1))) + '0');
    }
    return;
}
// 输出中间位数值
void write_interval_digit(const int &digit, int count) {
    if (digit < 10) {
        while (--count) {
            putchar('0');
        }
        putchar(digit + '0');
    } else {
        int temp = digit / 10;
        write_interval_digit(temp, count - 1);
        putchar((digit - ((temp << 3) + (temp << 1))) + '0');
    }
    return;
}

void int2048::print() const{
    if (digits_.empty()) {
        putchar('0');
        return;
    }
    if (negative_) {
        putchar('-');
    }
    int size = digits_.size();
    write_highest_digit(digits_[size - 1]);
    for (int i = size - 2; i >= 0; --i) {
        write_interval_digit(digits_[i], BASE_SIZE_);
    }
    return;
} // 调用时必须保证已经normalize

int2048 &int2048::add(const int2048 &value) {
    if (value.is_zero()) {// 0不会导致任何变化
        return *this;
    } 
    if (this->is_zero()) { // 自己为0，直接拷贝value
        *this = value;
        return *this;
    }
    // 现在，保证两数非零
    if (negative_ ^ value.negative_) { // 异号
        // A + B
        // -((-A) - B) = A + B
        negative_ ^= true;
        this->minus(value); // 调用减法
        if (digits_.empty()) {
            return *this;
        } // 拦截0，防止反转成-0
        negative_ ^= true;
        return *this;
    } else { // 同号
        // 符号不变
        bool carry = false;
        unsigned int pointer = 0, temp = 0;
        if (digits_.size() > value.digits_.size()) { // 当前数位数更大
            while (pointer < value.digits_.size()) {
                digits_[pointer] += value.digits_[pointer] + carry;
                carry = digits_[pointer] >= BASE_;
                digits_[pointer] -= (carry ? BASE_ : 0u);
                ++pointer;
            } // 处理重叠部分加法
            while (carry && pointer < digits_.size()) {
                ++digits_[pointer];
                carry = digits_[pointer] >= BASE_;
                digits_[pointer] -= (carry ? BASE_ : 0u);   
                ++pointer;          
            }
            if (carry) {
                digits_.push_back(1u);
            } // 已经处理完最高位，仍然存在一个进位
            // 加法不会新增前导0， 不必标准化，下同
        } else {
            unsigned int previous_size = digits_.size();
            digits_.resize(value.digits_.size()); // 位数对齐
            while (pointer < previous_size) {
                digits_[pointer] += value.digits_[pointer] + carry;
                carry = digits_[pointer] >= BASE_;
                digits_[pointer] -= (carry ? BASE_ : 0u);
                ++pointer;
            } // 处理重叠部分加法
            while (carry && pointer < digits_.size()) {
                temp = value.digits_[pointer] + 1u;
                carry = temp >= BASE_;
                digits_[pointer] = (carry ? 0u : temp);
                ++pointer;
            }
            while (pointer < digits_.size()) {
                digits_[pointer] = value.digits_[pointer];
                ++pointer;
            }
            if (carry) {
                digits_.push_back(1u);
            } // 已经处理完最高位，仍然存在一个进位
        }
    }
    return *this;
}
int2048 add(int2048 A, const int2048 &B) {
    A.add(B);
    return A;
}

int2048 &int2048::minus(const int2048 &value) {
    if (value.is_zero()) {// 0不会导致任何变化
        return *this;
    } 
    if (this->is_zero()) { // 自己为0，直接拷贝负的value
        *this = value;
        negative_ ^= 1;
        return *this;
    }
    // 现在，保证两数非零
    if (negative_ ^ value.negative_) { // 异号
        // A - B
        // -((-A) + B) = A - B
        negative_ ^= true;
        this->add(value);
        negative_ ^= true;
    } else { // 同号
        bool borrow = false;
        unsigned int temp, pointer = 0;
        if (value.digits_.size() < digits_.size()) { // 此时，一定是this的绝对值更大
            // 符号与this一致，不改变
            while (pointer < value.digits_.size()) {
                temp = value.digits_[pointer] + borrow; // 要减去的数字
                borrow = temp > digits_[pointer];
                digits_[pointer] = (digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                ++pointer;
            } // 处理重叠部分
            while (borrow && pointer < digits_.size()) {
                borrow = digits_[pointer] == 0u;
                digits_[pointer] = (borrow ? BASE_ - 1u : digits_[pointer] - 1u);
                ++pointer;
            } // 处理剩余借位
            normalize(); // 标准化
        } else if (value.digits_.size() > digits_.size()) { // 此时，value绝对值更大
            negative_ = value.negative_ ^ true;
            unsigned int previous_size = digits_.size();
            digits_.resize(value.digits_.size());
            while (pointer < previous_size) {
                temp = digits_[pointer] + borrow;
                borrow = temp > value.digits_[pointer];
                digits_[pointer] = (value.digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                ++pointer;
            }
            while (borrow && pointer < digits_.size()) {
                borrow = value.digits_[pointer] == 0u;
                digits_[pointer] = (borrow ? BASE_ - 1u : value.digits_[pointer] - 1u);
                ++pointer;
            }
            while (pointer < digits_.size()) {
                digits_[pointer] = value.digits_[pointer];
                ++pointer;
            }
            normalize(); // 标准化
        } else {
            unsigned int size = digits_.size();
            while ((size) 
                   && (digits_[size - 1u] == value.digits_[size - 1u])) {
                --size;
            } // 删除高位的相同数块
            digits_.resize(size);
            if (digits_.empty()) {
                negative_ = false;
                return *this;
            } // 两数相等
            unsigned int value_pointer = size - 1u;
            if (digits_.back() > value.digits_[value_pointer]) { // this更大
                // 符号与this一致
                while (pointer <= value_pointer) { // 之所以这么写，是因为value_pointer = digits_.size() - 1
                    temp = value.digits_[pointer] + borrow;
                    borrow = temp > digits_[pointer];
                    digits_[pointer] = (digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                    ++pointer;
                }
                normalize();
            } else { // value更大
                negative_ = value.negative_ ^ true;
                while (pointer <= value_pointer) { // 之所以这么写，是因为value_pointer = digits_.size() - 1
                    temp = digits_[pointer] + borrow;
                    borrow = temp > value.digits_[pointer];
                    digits_[pointer] = (value.digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                    ++pointer;
                }
                normalize();
            }
        } // 数块大小一致
    }
    return *this;
}
int2048 minus(int2048 A, const int2048 &B) {
    A.minus(B);
    return A;
}

} // namespace sjtu

#endif
