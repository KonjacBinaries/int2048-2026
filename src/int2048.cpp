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
int2048::int2048(long long origin_number) {
    negative_ = false;
    if (origin_number == 0) {
        return;
    }
    unsigned long long number = static_cast<unsigned long long>(origin_number);
    if (origin_number < 0) {
        negative_ = true;
        // 如果origin_number < 0，那么此时number里面存储的应该是 ULL_MAX + origin_number
        number = 0ull - number;
        // ULL_MAX - (ULL_MAX + origin_number) = -origin_number
        // 这样是为了防止number_ = -LL_MAX，然后因为补码还是自己直接UB
    }
    while (number) {
        if (number < BASE_) {
            digits_.push_back(number);
            break;
        }
        long long res = number / BASE_;
        digits_.push_back(static_cast<unsigned int>(number - res * BASE_));
        number = res;
    }
    normalize();
    return;
}
int2048::int2048(const std::string &number) {
    int number_size = number.size();
    if (!number_size) {
        negative_ = false;
        return;
    }
    negative_ = (number[0] == '-');
    int non_negative = (negative_ ^ 1);
    int temp, upperbound, base_size = BASE_SIZE_;
    for (int i = number_size - 1; i >= negative_; i -= base_size) {
        if (i + non_negative < base_size) {
            upperbound = i + non_negative - 1;
        } else {
            upperbound = base_size - 1;
        }
        temp = 0;
        for (int j = upperbound; j >= 0; --j) {
            temp = (temp << 3) + (temp << 1) + number[i - j] - '0';
        }
        digits_.push_back(temp);
    }
    normalize();
    return;
}
int2048::int2048(const int2048 &) = default; // 采用默认拷贝构造
int2048 &int2048::operator=(const int2048 &) = default; // 采用默认拷贝赋值
int2048 &int2048::operator=(const long long &origin_number) {
    digits_.clear();
    negative_ = false;
    if (origin_number == 0) {
        return *this;
    }
    unsigned long long number = static_cast<unsigned long long>(origin_number);
    if (origin_number < 0) {
        negative_ = true;
        // 如果origin_number < 0，那么此时number里面存储的应该是 ULL_MAX + origin_number
        number = 0ull - number;
        // ULL_MAX - (ULL_MAX + origin_number) = -origin_number
        // 这样是为了防止number_ = -LL_MAX，然后因为补码还是自己直接UB
    }
    while (number) {
        if (number < BASE_) {
            digits_.push_back(number);
            break;
        }
        long long res = number / BASE_;
        digits_.push_back(static_cast<unsigned int>(number - res * BASE_));
        number = res;
    }
    normalize();
    return *this;
}

void int2048::read(const std::string &number) {
    int number_size = number.size();
    if (!number_size) {
        digits_.clear();
        negative_ = false;
        return;
    }
    digits_.clear();
    negative_ = (number[0] == '-');
    int non_negative = (negative_ ^ 1);
    int temp, upperbound;
    for (int i = number_size - 1; i >= negative_; i -= BASE_SIZE_) {
        if (i + non_negative < BASE_SIZE_) {
            upperbound = i + non_negative - 1;
        } else {
            upperbound = BASE_SIZE_ - 1;
        }
        temp = 0;
        for (int j = upperbound; j >= 0; --j) {
            temp = (temp << 3) + (temp << 1) + number[i - j] - '0';
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

int2048 &int2048::add(const int2048 &number) {
    if (number.is_zero()) {// 0不会导致任何变化
        return *this;
    } 
    if (this->is_zero()) { // 自己为0，直接拷贝number
        *this = number;
        return *this;
    }
    // 现在，保证两数非零
    if (negative_ ^ number.negative_) { // 异号
        // A + B
        // -((-A) - B) = A + B
        negative_ ^= true;
        this->minus(number); // 调用减法
        if (digits_.empty()) {
            return *this;
        } // 拦截0，防止反转成-0
        negative_ ^= true;
        return *this;
    } else { // 同号
        // 符号不变
        bool carry = false;
        unsigned int pointer = 0, temp = 0;
        if (digits_.size() > number.digits_.size()) { // 当前数位数更大
            while (pointer < number.digits_.size()) {
                digits_[pointer] += number.digits_[pointer] + carry;
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
            digits_.resize(number.digits_.size()); // 位数对齐
            while (pointer < previous_size) {
                digits_[pointer] += number.digits_[pointer] + carry;
                carry = digits_[pointer] >= BASE_;
                digits_[pointer] -= (carry ? BASE_ : 0u);
                ++pointer;
            } // 处理重叠部分加法
            while (carry && pointer < digits_.size()) {
                temp = number.digits_[pointer] + 1u;
                carry = temp >= BASE_;
                digits_[pointer] = (carry ? 0u : temp);
                ++pointer;
            }
            while (pointer < digits_.size()) {
                digits_[pointer] = number.digits_[pointer];
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
    return A.add(B);
}
int2048 &int2048::minus(const int2048 &number) {
    if (number.is_zero()) {// 0不会导致任何变化
        return *this;
    } 
    if (this->is_zero()) { // 自己为0，直接拷贝负的number
        *this = number;
        negative_ ^= 1;
        return *this;
    }
    // 现在，保证两数非零
    if (negative_ ^ number.negative_) { // 异号
        // A - B
        // -((-A) + B) = A - B
        negative_ ^= true;
        this->add(number);
        negative_ ^= true;
    } else { // 同号
        bool borrow = false;
        unsigned int temp, pointer = 0;
        if (number.digits_.size() < digits_.size()) { // 此时，一定是this的绝对值更大
            // 符号与this一致，不改变
            while (pointer < number.digits_.size()) {
                temp = number.digits_[pointer] + borrow; // 要减去的数字
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
        } else if (number.digits_.size() > digits_.size()) { // 此时，number绝对值更大
            negative_ = number.negative_ ^ true;
            unsigned int previous_size = digits_.size();
            digits_.resize(number.digits_.size());
            while (pointer < previous_size) {
                temp = digits_[pointer] + borrow;
                borrow = temp > number.digits_[pointer];
                digits_[pointer] = (number.digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                ++pointer;
            }
            while (borrow && pointer < digits_.size()) {
                borrow = number.digits_[pointer] == 0u;
                digits_[pointer] = (borrow ? BASE_ - 1u : number.digits_[pointer] - 1u);
                ++pointer;
            }
            while (pointer < digits_.size()) {
                digits_[pointer] = number.digits_[pointer];
                ++pointer;
            }
            normalize(); // 标准化
        } else {
            unsigned int size = digits_.size();
            while ((size) 
                   && (digits_[size - 1u] == number.digits_[size - 1u])) {
                --size;
            } // 删除高位的相同数块
            digits_.resize(size);
            if (digits_.empty()) {
                negative_ = false;
                return *this;
            } // 两数相等
            unsigned int number_pointer = size - 1u;
            if (digits_.back() > number.digits_[number_pointer]) { // this更大
                // 符号与this一致
                while (pointer <= number_pointer) { // 之所以这么写，是因为number_pointer = digits_.size() - 1
                    temp = number.digits_[pointer] + borrow;
                    borrow = temp > digits_[pointer];
                    digits_[pointer] = (digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                    ++pointer;
                }
                normalize();
            } else { // number更大
                negative_ = number.negative_ ^ true;
                while (pointer <= number_pointer) { // 之所以这么写，是因为number_pointer = digits_.size() - 1
                    temp = digits_[pointer] + borrow;
                    borrow = temp > number.digits_[pointer];
                    digits_[pointer] = (number.digits_[pointer] + (borrow ? BASE_ : 0u)) - temp;
                    ++pointer;
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
int2048 int2048::operator+() const {
    return *this;
}
int2048 int2048::operator-() const {
    if (is_zero()) {
        return *this;
    }
    // 现在，保证非零
    int2048 temp(*this);
    temp.negative_ ^= true;
    return temp;
}
int2048 &int2048::operator+=(const long long &number) {
    this->add(int2048(number));
    return *this;
}
int2048 &int2048::operator-=(const long long &number) {
    this->minus(int2048(number));
    return *this;
}
int2048 &int2048::operator+=(const int2048 &number) {
    this->add(number);
    return *this;
}
int2048 operator+(int2048 A, const long long &B) {
    return A.add(int2048(B));
}
int2048 operator+(const long long &A, int2048 B) {
    return B.add(int2048(A));
}
int2048 operator-(int2048 A, const long long &B) {
    return A.minus(int2048(B));
}
int2048 operator-(const long long &A, int2048 B) {
    return int2048(A).minus(B);
}
int2048 &int2048::operator-=(const int2048 &number) {
    this->minus(number);
    return *this;
}
int2048 operator+(int2048 A, const int2048 &B) {
    return A.add(B);
}
int2048 operator-(int2048 A, const int2048 &B) {
    return A.minus(B);
}

std::istream &operator>>(std::istream &istr, int2048 &number) {
    std::string temp;
    istr >> temp;
    number.read(temp);
    return istr;
}
// 输出最高位数值
void write_highest_digit_ostr(std::ostream &ostr, const int &digit) {
    if (digit < 10) {
        ostr << static_cast<char>(digit + '0');
    } else {
        int temp = digit / 10;
        write_highest_digit_ostr(ostr, temp);
        ostr << static_cast<char>((digit - ((temp << 3) + (temp << 1))) + '0');
    }
    return;
}
// 输出中间位数值
void write_interval_digit_ostr(std::ostream &ostr, const int &digit, int count) {
    if (digit < 10) {
        while (--count) {
            ostr << static_cast<char>('0');
        }
        ostr << static_cast<char>(digit + '0');
    } else {
        int temp = digit / 10;
        write_interval_digit_ostr(ostr, temp, count - 1);
        ostr << static_cast<char>((digit - ((temp << 3) + (temp << 1))) + '0');
    }
    return;
}
std::ostream &operator<<(std::ostream &ostr, const int2048 &number) {
    if (number.digits_.empty()) {
        ostr << '0';
        return ostr;
    }
    if (number.negative_) {
        ostr << '-';
    }
    int size = number.digits_.size();
    write_highest_digit_ostr(ostr, number.digits_[size - 1]);
    for (int i = size - 2; i >= 0; --i) {
        write_interval_digit_ostr(ostr, number.digits_[i], int2048::BASE_SIZE_);
    }
    return ostr;
}

bool operator==(const int2048 &A, const int2048 &B) {
    if (A.negative_ ^ B.negative_) {
        return false;
    }
    if (A.digits_.size() != B.digits_.size()) {
        return false;
    }
    for (unsigned int i = 0; i < A.digits_.size(); ++i) {
        if (A.digits_[i] != B.digits_[i]) {
            return false;
        }
    }
    return true;
}
bool operator!=(const int2048 &A, const int2048 &B) {
    if (A.negative_ ^ B.negative_) {
        return true;
    }
    if (A.digits_.size() != B.digits_.size()) {
        return true;
    }
    for (unsigned int i = 0; i < A.digits_.size(); ++i) {
        if (A.digits_[i] != B.digits_[i]) {
            return true;
        }
    }
    return false;
}
bool operator<(const int2048 &A, const int2048 &B) {
    if (A.negative_ ^ B.negative_) {
        return A.negative_;
    }
    bool negative = A.negative_;
    if (A.digits_.size() != B.digits_.size()) {
        return (A.digits_.size() < B.digits_.size()) ^ negative;
    }
    for (int i = static_cast<int>(A.digits_.size()) - 1; i >= 0; --i) {
        if (A.digits_[i] != B.digits_[i]) {
            return (A.digits_[i] < B.digits_[i]) ^ negative;
        }
    }
    return false; // 说明 A == B
}
bool operator>(const int2048 &A, const int2048 &B) {
    if (A.negative_ ^ B.negative_) {
        return B.negative_;
    }
    bool negative = A.negative_;
    if (A.digits_.size() != B.digits_.size()) {
        return (A.digits_.size() > B.digits_.size()) ^ negative;
    }
    for (int i = static_cast<int>(A.digits_.size()) - 1; i >= 0; --i) {
        if (A.digits_[i] != B.digits_[i]) {
            return (A.digits_[i] > B.digits_[i]) ^ negative;
        }
    }
    return false; // 说明 A == B
}
bool operator<=(const int2048 &A, const int2048 &B) {
    if (A.negative_ ^ B.negative_) {
        return A.negative_;
    }
    bool negative = A.negative_;
    if (A.digits_.size() != B.digits_.size()) {
        return (A.digits_.size() < B.digits_.size()) ^ negative;
    }
    for (int i = static_cast<int>(A.digits_.size()) - 1; i >= 0; --i) {
        if (A.digits_[i] != B.digits_[i]) {
            return (A.digits_[i] < B.digits_[i]) ^ negative;
        }
    }
    return true; // 说明 A == B
}
bool operator>=(const int2048 &A, const int2048 &B) {
    if (A.negative_ ^ B.negative_) {
        return B.negative_;
    }
    bool negative = A.negative_;
    if (A.digits_.size() != B.digits_.size()) {
        return (A.digits_.size() > B.digits_.size()) ^ negative;
    }
    for (int i = static_cast<int>(A.digits_.size()) - 1; i >= 0; --i) {
        if (A.digits_[i] != B.digits_[i]) {
            return (A.digits_[i] > B.digits_[i]) ^ negative;
        }
    }
    return true; // 说明 A == B
}
// 计划采用CRT提升对中间值的分析能力
// 假设双模NTT过后，得到的值为a和b，原本应该是x
// 那么有x mod MODULE[0] = a, x mod MODULE[1] = b
// 鉴于MODULE_0 = 119 * 2 ^ 23 + 1, MODULE[1] = 7 * 2 ^ 26 + 1
// 所以，如果想要声称有效原根，被NTT的多项式的长度不得超过2 ^ 23
// 那么，出于安全考虑，参与乘法的多项式的长度不得达到或超过2 ^ 23 = 8388608，折合成十进制大概四千万位
// 进行卷积后（尚未进位），粗略估计单位最大值不会达到或超过 2 ^ 23 * (BASE_ - 1) ^ 2 = 83884402286788608
// 而CRT可以保证的有效值域为 0 ~ MODULE[0] * MODULE[1] - 1 = 468937312667959296
// 468937312667959297 - 83884402286788608 = 385052910381170689，遥遥领先
// 这就确保了CRT必然可以还原出x
// 设x = k * MODULE[0] + a
// 带入第二个式子，k * MODULE[0] + a = b (mod MODULE[1])
// 移项，k * MODULE[0] = b - a (mod MODULE[1])
// 求出c = b - a (mod MODULE[1])
// i.e. c = (b > a) ? b - a : b + MODULE[1] - a
// 乘逆元，k = INVERSE_OF_MODULE_0_MOD_MODULE_1 * c (mod MODULE[1])
// 记 d = INVERSE_OF_MODULE_0_MOD_MODULE_1 * c (mod MODULE[1])
// 从而k = d + t * MODULE[1]
// x = (d + t * MODULE[1]) * MODULE[0] + a + l * MODULE[0] * MODULE[1]
// i.e. x = d * MODULE[0] + a + s * MODULE[0] * MODULE[1]
// 鉴于d * MODULE[0] + a >= 0，总和上述值域分析，s不得大于0
// 现在考虑d * MODULE[0] + a
// 由于d是对MODULE_1取模的结果，a是对MODULE_0取模的结果
// 就有d * MODULE[0] + a <= (MODULE[1] - 1) * MODULE[0] + (MODULE[0] - 1) < MODULE[0] * MODULE[1]
// i.e. 0 <= d * MODULE[0] + a < MODULE[0] * MODULE[1]
// i.e. x = d * MODULE[0] + a
// 所以，流程大致如下：
// (0) 计算 a_mod_MODULE_1 =  a % MODULE_1，准备进行(1)的计算
// (1) c = (b >= a_mod_MODULE_1) ? b - a_mod_MODULE_1 : b + MODULE[1] - a_mod_MODULE_1
// (2) d = INVERSE_OF_MODULE_0_MOD_MODULE_1 * c % MODULE[1]
// (3) x = d * MODULE[0] + a
// 其中，乘法会炸long long，想个办法处理一下
// 分析一下值域，假设x自己产生的进位为carry
// (1) 是完全安全的，得到的c: 0 ~ 469762048
// (2) 中，208783132 * 469762048 = 98078391676174336
// (3) 中 d * MODULE_0最多469762048 * 998244353 = 468937311669714944
// 呃呃，long long的上界似乎是9223372036854775807 > 98078391676174336
// 还有，9223372036854775807 > 468937311669714944
// 我的天哪long long大人
// 好了，就用long long
// 这一位新产生的carry最多也绝对不会超过是98078391676174336 / 100000，大概980783916761吧
// 所以上一位进过来的carry最多也对不会超过
// 980783916761 + 980783916761 / 100000 + 980783916761/ 10000000000 + 980783916761 / 1000000000000000
// 总之绝对不可能超过9223372036854775807
// 可以安全地用long long进行处理
// 将a(mod MODULE[0])和b(mod MODULE[1])用CRT合并，结果写入x
void merge(unsigned int a, unsigned int b, unsigned long long &x) {
    unsigned long long a_mod_MODULE_1 = a % int2048::MODULE[1];
    unsigned long long c = (b >= a_mod_MODULE_1)
                           ? b - a_mod_MODULE_1 
                           : b + int2048::MODULE[1] - a_mod_MODULE_1;
    unsigned long long d = int2048::INVERSE_OF_MODULE_0_MOD_MODULE_1
                           * c % int2048::MODULE[1];
    x = d * int2048::MODULE[0] + a;
    return;
} // CRT

constexpr unsigned long long fast_pow(unsigned long long value,
                                      unsigned int exponent,
                                      unsigned long long MODULE) {
    unsigned long long result = 1ull;
    value %= MODULE;
    while (exponent) {
        if (exponent & 1u) {
            result = result * value % MODULE;
        }
        value = value * value % MODULE;
        exponent >>= 1u;
    }
    return result;
}
constexpr unsigned int MAX_OMEGA(23);
struct omega_table {
    unsigned long long value_[MAX_OMEGA + 1][2][2];
    // omega(exponent, is_inverse, use_module): 2 ^ exponent阶单位根，is_inverse指示是否为逆元，use_module指示使用哪个模数
    constexpr unsigned long long operator()(
        unsigned int exponent,
        bool is_inverse,
        bool use_module) const {
        return value_[exponent][is_inverse][use_module];
    }
};
constexpr omega_table make_omega_table() {
    omega_table temp{};
    for (int i = 0; i <= MAX_OMEGA; ++i) {
        temp.value_[i][0][0] = fast_pow(int2048::PRIMITIVE_ROOT,
                                  (int2048::MODULE[0] - 1ull) / (1ull << i),
                                  int2048::MODULE[0]);
        temp.value_[i][0][1] = fast_pow(int2048::PRIMITIVE_ROOT,
                                  (int2048::MODULE[1] - 1ull) / (1ull << i),
                                  int2048::MODULE[1]);
        temp.value_[i][1][0] = fast_pow(int2048::PRIMITIVE_ROOT,
                                  (int2048::MODULE[0] - 1ull) - (int2048::MODULE[0] - 1ull) / (1ull << i),
                                  int2048::MODULE[0]);
        temp.value_[i][1][1] = fast_pow(int2048::PRIMITIVE_ROOT,
                                  (int2048::MODULE[1] - 1ull) - (int2048::MODULE[1] - 1ull) / (1ull << i),
                                  int2048::MODULE[1]);
    }
    return temp;
}
constexpr omega_table omega = make_omega_table(); // 编译期计算各阶单位根及其逆
static unsigned int reverse_position[(1 << 23) + 1]; // 蝶形变换
static unsigned long long omega_pow[(1 << 23) + 1]; // 指定阶的omega的指数
template <bool use_module>
void NTT(
    std::vector<unsigned int> &polynomial,
    const unsigned int &N,
    const unsigned int &k,
    const bool &is_inverse) {
    constexpr unsigned int MODULE = int2048::MODULE[use_module];
    for (int i = 0; i < N; ++i) {
        if (i < reverse_position[i]) {
            int temp = polynomial[i];
            polynomial[i] = polynomial[reverse_position[i]];
            polynomial[reverse_position[i]] = temp;
        } // 由于构成双射，可以这样直接蝶形变换
    }
    // 自下而上地合并
    for (int half = 0; half < k; ++half) {
        // 将两个规模为2 ^ half的子问题合并为规模为2 * 2 ^ half的子问题
        unsigned long long omega_2half = omega(half + 1, is_inverse, use_module); // 获取结果子问题的单位根
        int sub_len = (1 << half); int aim_len = (sub_len << 1); // 获取当前子问题和结果子问题的长度
        /**
         * 结果子问题是求所有的A(ω ^ t), 0 <= t < 2 ^ (half + 1)
         * 分成奇偶两部分，A1表示偶次项的和，A2表示奇次项的和
         * A(ω ^ t) = A1(ω ^ (2 * t)) + (ω ^ t) * A2(ω ^ (2 * t))
         * 如果t < 2 ^ half，那么可以把ω ^ 2看成2 ^ half阶单位根，这样A1 A2直接是子问题的对应位置结果
         * 如果t >= 2 ^ half，则考虑
         *  A1(ω ^ (2 * t)) = A1(ω ^ (2 * (t - 2 ^ half) + 2 * 2 ^ half)) = A1(ω ^ (2 * (t - 2 ^ half)))
         *  A2(ω ^ (2 * t)) = A2(ω ^ (2 * (t - 2 ^ half) + 2 * 2 ^ half)) = A2(ω ^ (2 * (t - 2 ^ half)))
         * 这也可以被看作子问题
         * 如果只枚举0 ~ 2 ^ half - 1，那么另一半的(ω ^ t)得相应地变成-(ω ^ (t - 2 ^ half))
         */
        omega_pow[0] = 1ull;
        // 计算所有ω ^ t，0 <= t < 2 ^ half
        for (int exp = 1; exp < sub_len; ++exp) {
            omega_pow[exp] = (omega_pow[exp - 1] * omega_2half) % MODULE;
        }
        for (int begin_position = 0; begin_position + aim_len <= N; begin_position += aim_len) {
            for (int t = 0; t < sub_len; ++t) {
                unsigned int A1 = polynomial[begin_position + t],
                                   A2 = polynomial[begin_position + sub_len + t]
                                        * omega_pow[t]
                                        % MODULE;
                unsigned int add = A1 + A2 - MODULE; add += (MODULE & (0u - (add >> 31u)));
                // 将A1 + A2 - MODULE的二进制编码直接写入usigned int的add
                // 如果A1 + A2 - MODULE < 0，那它将会是A1 + A2 - MODULE的补码形式，值为UINT_MAX + A1 + A2 - MODULE
                // 所以，最高位(add >> 31u)将会是1u，0u - 1u发生自然溢出，变成全1，正好形成一个mask，使得add += MODULE
                // 这时候，发生自然溢出，得到A1 + A2
                // 否则，(0u - (add >> 31u)) = 0u，add不变
                unsigned int sub = A1 - A2; sub += (MODULE & (0u - (sub >> 31u)));
                // A1 - A2的二进制编码直接写入unsigned int的sub
                // 如果A1 < A2，那就是个补码，值为UINT_MAX + A1 - A2
                // 同样用(0u - (sub >> 31u))生成这个由符号决定的mask
                // 如果mask为1-mask，就补一个MODULE，自然溢出回A1 - A2 + MODULE
                // 否则，正好什么也不做
                polynomial[begin_position + t] = add;
                polynomial[begin_position + sub_len + t] = sub;
            }
        } // 按块合并
    }
    if (is_inverse) {
        unsigned long long inv_N = fast_pow(N, 
                                            MODULE - 2ull,
                                            MODULE);
        for (int i = 0; i < N; ++i) {
            polynomial[i] = static_cast<unsigned long long>(polynomial[i]) * inv_N % MODULE;
        }
    }
    return;
}
template <bool use_module>
void multiply_single_module(int2048 &A, int2048 B) {
    constexpr unsigned int MODULE = int2048::MODULE[use_module];
    A.negative_ = A.negative_ ^ B.negative_; // 定号
    std::vector<unsigned int> &A_absolute = A.digits_;
    std::vector<unsigned int> &B_absolute = B.digits_;
    unsigned int N = 1, k = 0; // 最终结果长为N = 2 ^ k
    unsigned int min_length = (static_cast<int>(A.digits_.size()) - 1)
                              + (static_cast<int>(B.digits_.size()) - 1)
                              + 1;
    while (N < min_length) {
        N <<= 1u;
        ++k;
    } // 求出扩充后的长度
    A_absolute.resize(N, 0u);
    B_absolute.resize(N, 0u);
    /**
     * 不难发现，NTT的时候，会根据系数的奇偶性把系数分到两侧
     * 如果规定每次分割的时候，奇数分到右侧，偶数分到左侧
     * 就会发现，如果当前最低位是1，那么相当于变换到最后，最高位是1
     * 其他位以此类推，就会发现，实际上就是一个翻转
     * 从最高层到最底层，一共有k + 1层
     * 不过实际上只有0 ~ k - 1层存在划分
     * 所以，分析可得，其实就是翻转了0 ~ k位
     */
    for (int i = 1; i < N; ++i) {
        // 翻转 [...)x :
        // 先 >> 1，得到0[...)
        // 然后获得它的翻转(...]0
        // 再 >> 1，得到0(...]
        // 最后把x = (i & 1)左移(k - 1)位，得到x(...]
        // 由于[...) < [...)x （前提是[...)x不等于0）
        // 这样一定可以求出正确的reverse_position
        reverse_position[i] = (reverse_position[i >> 1] >> 1) | ((i & 1) << (k - 1));
    } // 计算蝶形变换数组
    NTT<use_module>(A_absolute, N, k, false);
    NTT<use_module>(B_absolute, N, k, false);
    for (int i = 0; i < N; ++i) {
        A_absolute[i] = 1ull * A_absolute[i] * B_absolute[i] % MODULE;
    }
    NTT<use_module>(A_absolute, N, k, true);
    return;
}
int2048 &int2048::multiply(int2048 B) {
    if (is_zero() || B.is_zero()) {
        digits_.clear();
        negative_ = false;
        return *this;
    }
    int2048 &Result_0 = *this;
    int2048 Result_1 = *this; // 拷贝一份
    multiply_single_module<0>(Result_0, B);
    multiply_single_module<1>(Result_1, B); // 这里要对B进行两次拷贝，产生两次拷贝开销
    // 合并
    unsigned long long carry = 0, temp;
    for (int i = 0; i < Result_0.digits_.size(); ++i) {
        unsigned long long x = 0;
        merge(Result_0.digits_[i], Result_1.digits_[i], x); // 顺序非常重要
        x += carry;
        carry = x / BASE_;
        Result_0.digits_[i] = x - carry * BASE_;
    } // 注意，Result_0就是*this
    while (carry) {
        temp = carry;
        carry /= BASE_;
        Result_0.digits_.push_back(temp - carry * BASE_);
    } // 已经填满了Result_0的全部数位，仍然存在进位，只能是继续填充高位
    normalize();
    return *this;
}
int2048 &int2048::operator*=(const int2048 &B) {
    return multiply(B);
}
int2048 operator*(int2048 A, const int2048 &B) {
    return A.multiply(B);
}

} // namespace sjtu
