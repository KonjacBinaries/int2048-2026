#include "include/int2048.h" // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！
                             // 写到最后的时候记得把include/删了！！！

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
int2048::int2048(const int2048 &) = default; // 采用默认的拷贝构造
int2048 &int2048::operator=(const int2048 &) = default; // 采用默认拷贝赋值

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
        negative_ ^= true;
        return *this;
    } else { // 同号
        // 符号不变
        bool carrier = false;
        if (digits_.size() > value.digits_.size()) { // 当前数位数更大
            for (int i = 0; i < value.digits_.size(); ++i) {
                digits_[i] += value.digits_[i];
                if (carrier) {
                    ++digits_[i];
                    carrier = false;
                }
                if (digits_[i] >= BASE_) {
                    digits_[i] -= BASE_;
                    carrier = true;
                }
            } // 处理重叠部分加法
            for (int i = value.digits_.size(); i < digits_.size(); ++i) {
                if (carrier) {
                    ++digits_[i];
                    carrier = false;
                } else {
                    break;
                }
                if (digits_[i] >= BASE_) {
                    digits_[i] -= BASE_;
                    carrier = true;
                }
            }
            if (carrier) {
                digits_.push_back(1);
            } // 已经处理完最高位，仍然存在一个进位
            // 加法不会新增前导0， 不必标准化，下同
        } else {
            digits_.resize(value.digits_.size()); // 位数对齐
            for (int i = 0; i < value.digits_.size(); ++i) {
                digits_[i] += value.digits_[i];
                if (carrier) {
                    ++digits_[i];
                    carrier = false;
                }
                if (digits_[i] >= BASE_) {
                    digits_[i] -= BASE_;
                    carrier = true;
                }
            } // 处理加法
            if (carrier) {
                digits_.push_back(1);
            } // 已经处理完最高位，仍然存在一个进位
        }
    }
    return *this;
}
int2048 add(int2048 A, const int2048 &B) {
    return A.add(B);
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
        bool borrow;
        unsigned int temp;
        if (value.digits_.size() < digits_.size()) { // 此时，一定是this的绝对值更大
            // 符号与this一致，不改变
            borrow = false;
            for (int i = 0; i < value.digits_.size(); ++i) {
                temp = value.digits_[i] + borrow; // 要减去的数字
                if (temp > digits_[i]) {
                    digits_[i] = (digits_[i] + BASE_) - temp;
                    borrow = true;
                } else {
                    digits_[i] -= temp;
                    borrow = false;
                }
            } // 处理重叠部分
            for (int i = value.digits_.size(); i < digits_.size(); ++i) {
                if (borrow) {
                    if (digits_[i]) {
                        digits_[i] -= 1;
                        break; // 借位终止
                    } else {
                        digits_[i] = BASE_ - 1;
                        borrow = true;
                    }
                } else {
                    break;
                }
            } // 处理剩余借位
            normalize(); // 标准化
        } else if (value.digits_.size() > digits_.size()) { // 此时，value绝对值更大
            negative_ = value.negative_ ^ true;
            borrow = false;
            digits_.resize(value.digits_.size());
            for (int i = 0; i < value.digits_.size(); ++i) {
                temp = digits_[i] + borrow;
                if (temp > value.digits_[i]) {
                    digits_[i] = (value.digits_[i] + BASE_) - temp;
                    borrow = true;
                } else {
                    digits_[i] = value.digits_[i] - temp;
                    borrow = false;
                }
            }
            normalize(); // 标准化
        } else {
            int value_pointer = value.digits_.size() - 1;
            while ((!digits_.empty()) 
                   && (digits_.back() == value.digits_[value_pointer])) {
                digits_.pop_back();
                --value_pointer;
            } // 删除高位的相同数块
            if (digits_.empty()) {
                negative_ = false;
                return *this;
            } // 两数相等
            if (digits_.back() > value.digits_[value_pointer]) { // this更大
                // 符号与this一致
                borrow = false;
                for (int i = 0; i <= value_pointer; ++i) { // 之所以这么写，是因为value_pointer = digits_.size() - 1
                    temp = value.digits_[i] + borrow;
                    if (temp > digits_[i]) {
                        digits_[i] = (digits_[i] + BASE_) - temp;
                        borrow = true;
                    } else {
                        digits_[i] -= temp;
                        borrow = false;
                    }
                }
                normalize();
            } else { // value更大
                negative_ = value.negative_ ^ true;
                borrow = false;
                for (int i = 0; i <= value_pointer; ++i) { // 之所以这么写，是因为value_pointer = digits_.size() - 1
                    temp = digits_[i] + borrow;
                    if (temp > value.digits_[i]) {
                        digits_[i] = (value.digits_[i] + BASE_) - temp;
                        borrow = true;
                    } else {
                        digits_[i] = value.digits_[i] - temp;
                        borrow = false;
                    }
                }
                normalize();
            }
        } // 数块大小一致
    }
    return *this;
}
int2048 minus(int2048 A, const int2048 &B) {
    return A.minus(B);
}

} // namespace sjtu
