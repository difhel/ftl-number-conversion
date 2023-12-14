#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "algo.h"

class BigInt {
    public:
        std::vector<int> digits;
        int radix;
        ~BigInt() = default;
        // default constructor
        BigInt() : digits({0}), radix(10) {};
        // copy constructor
        BigInt(const BigInt& other) : digits(other.digits), radix(other.radix) {}
        // constructor with digits
        BigInt(std::vector<int> digits) : radix(10), digits(digits) {}
        // constructor with digits and radix
        BigInt(std::vector<int> digits, int radix) : radix(radix), digits(digits) {}
        // constructor with string
        BigInt(std::string str) : radix(10) {
            digits = stringToVectorInt(str);
        }
        // constructor with string and radix
        BigInt(std::string str, int radix) : radix(radix) {
            digits = stringToVectorInt(str, radix);
        }
        // constructor with int
        BigInt(int64_t n) : radix(10) {
            while (n) {
                digits.push_back(n % 10);
                n /= 10;
            }
            if (digits.size() == 0) digits.push_back(0);
            std::reverse(digits.begin(), digits.end());
        }
        // constructor with int and radix
        BigInt(int64_t n, int radix) : radix(radix) {
            this->digits = {};
            while (n) {
                digits.push_back(n % radix);
                n /= radix;
            }
            if (digits.size() == 0) digits.push_back(0);
            std::reverse(digits.begin(), digits.end());
        }

        BigInt operator<<(int n) const {
            std::vector<int> res(digits);
            res.insert(res.end(), n, 0);
            return BigInt(res, radix);
        }

        BigInt operator>>(int n) const {
            n = std::min(n, static_cast<int>(digits.size()));
            std::vector<int> res(digits);
            res.erase(res.end() - n, res.end());
            return BigInt(res, radix);
        }

        friend std::ostream& operator<<(std::ostream& os, const BigInt& bi) {
            for (int i = 0; i < bi.digits.size(); i++) {
                os << bi.digits[i] << " ";
            }
            os << " (radix: " << bi.radix << ") ";
            return os;
        }

        friend std::istream& operator>>(std::istream& is, BigInt& bi) {
            /*
            Input BigInt in formats:
            - 123[13]ABCD - in 10 radix (by default)
            - 123[13]ABCD_16 - in 16 radix
            */
            std::string s;
            is >> s;
            size_t pos = s.find('_');
            int radix_int = 10;
            if (pos != std::string::npos) {
                // number in format XXX_radix
                std::string radix_str = s.substr(pos + 1);
                if (!isNumber(radix_str)) throw InvalidNumberException();
                radix_int = stoi(radix_str);
                s = s.substr(0, pos);
            }
            bi = BigInt(s, radix_int);
            return is;
        }

        // function to fill the bigint size to n by adding leading zeroes
        void fillToSize(int n) {
            if (digits.size() < n) {
                std::vector<int> tmp(n - digits.size(), 0);
                tmp.insert(tmp.end(), digits.begin(), digits.end());
                digits = tmp;
            }
        }

        // write operator+
        BigInt operator+(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            std::vector<int> res;
            int carry = 0;
            int i = digits.size() - 1;
            int j = other.digits.size() - 1;
            while (i >= 0 || j >= 0) {
                int sum = carry;
                if (i >= 0) sum += digits[i];
                if (j >= 0) sum += other.digits[j];
                res.push_back(sum % radix);
                carry = sum / radix;
                i--;
                j--;
            }
            if (carry) res.push_back(carry);
            std::reverse(res.begin(), res.end());
            BigInt answer(res, radix);
            answer.remove_leading_zeroes();
            return answer;
        }
        // write operator-
        BigInt operator-(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            std::vector<int> res;
            int carry = 0;
            int i = digits.size() - 1;
            int j = other.digits.size() - 1;
            while (i >= 0 || j >= 0) {
                int sum = carry;
                if (i >= 0) sum += digits[i];
                if (j >= 0) sum -= other.digits[j];
                if (sum < 0) {
                    sum += radix;
                    carry = -1;
                } else {
                    carry = 0;
                }
                res.push_back(sum);
                i--;
                j--;
            }
            std::reverse(res.begin(), res.end());
            BigInt answer(res, radix);
            answer.normalize();
            answer.remove_leading_zeroes();
            return answer;
        }
        // write function normalize that does Number normalization - bringing each digit into conformity with the number system.

        void normalize() {
            int carry = 0;
            for (int i = digits.size() - 1; i >= 0; i--) {
                digits[i] += carry;
                carry = digits[i] / radix;
                digits[i] %= radix;
            }
            while (carry) {
                digits.insert(digits.begin(), carry % radix);
                carry /= radix;
            }
        }

        void remove_leading_zeroes() {
            while (digits.size() > 1 && digits[0] == 0) digits.erase(digits.begin());
            if (digits.size() == 0) digits.push_back(0);
        }
        // write simple operator* algorithm (for small numbers)
        BigInt simple_multiply(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            return BigInt(static_cast<int64_t>(this->toInt()) * other.toInt(), this->radix);
        }
        // write operator* using Karatsuba algorithm
        BigInt operator*(const BigInt& other) const {
            // std::cout << "operator *" << std::endl;
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            // std::vector<int> res;
            BigInt bi_a(*this);
            BigInt bi_b(other);
            BigInt INT32_MAX_VALUE = BigInt(INT32_MAX, bi_a.radix);
            if (bi_a <= INT32_MAX_VALUE && bi_b <= INT32_MAX_VALUE) {
                return this->simple_multiply(other);
            }
            // bi_a.normalize();
            // bi_b.normalize();
            // bi_a.remove_leading_zeroes();
            // bi_b.remove_leading_zeroes();
            int n = std::max(bi_a.digits.size(), bi_b.digits.size());
            if (n == 0) return BigInt(0, radix);
            if (n == 1) {
                BigInt answer(std::vector<int>{bi_a.digits[0] * bi_b.digits[0]}, radix);
                answer.normalize();
                return answer;
            }
            if (n % 2 == 1) n++;
            bi_a.fillToSize(n);
            bi_b.fillToSize(n);
            int k = n / 2;

            std::vector<int> vec_a0(bi_a.digits.begin(), bi_a.digits.begin() + k);
            BigInt a0(vec_a0, radix);
            std::vector<int> vec_a1(bi_a.digits.begin() + k, bi_a.digits.end());
            BigInt a1(vec_a1, radix);
            BigInt b0(std::vector<int>(bi_b.digits.begin(), bi_b.digits.begin() + k), radix);
            BigInt b1(std::vector<int>(bi_b.digits.begin() + k, bi_b.digits.end()), radix);
            BigInt a0b0 = a0 * b0;
            BigInt a1b1 = a1 * b1;

            BigInt temp = (a0 + a1) * (b0 + b1);

            BigInt res1 = a0b0 << n;
            BigInt res2 = (temp - a1b1 - a0b0) << k; 
            BigInt res3 = a1b1;

            // vector<int> product()
            BigInt res = res1 + res2 + res3;
            res.normalize();
            res.remove_leading_zeroes();
            return res;
        }

        // write operator==
        bool operator==(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            if (std::max(static_cast<int>(digits.size()), 1) != std::max(static_cast<int>(other.digits.size()), 1)) return false;
            for (int i = 0; i < digits.size(); i++) {
                if (digits[i] != other.digits[i]) return false;
            }
            return true;
        }
        bool operator!=(const BigInt& other) const {
            return !operator==(other);
        }
        // write operator<
        bool operator<(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            if (digits.size() != other.digits.size()) return digits.size() < other.digits.size();
            for (int i = 0; i < digits.size(); i++) {
                if (digits[i] != other.digits[i]) return digits[i] < other.digits[i];
            }
            return false;
        }
        // write operator>
        bool operator>(const BigInt& other) const {
            return !(operator==(other) || operator<(other));
        }
        bool operator>=(const BigInt& other) const {
            return operator>(other) || operator==(other);
        }
        bool operator<=(const BigInt& other) const {
            return operator<(other) || operator==(other);
        }
        // write operator% using operator/ and operator-
        BigInt operator%(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            BigInt bi_a(*this);
            BigInt bi_b(other);
            // bi_a.normalize();
            // bi_b.normalize();
            // bi_a.remove_leading_zeroes();
            // bi_b.remove_leading_zeroes();
            if (bi_b == BigInt(0, radix)) throw std::invalid_argument("division by zero");
            if (bi_a < bi_b) return bi_a;
            return bi_a - (bi_a / bi_b) * bi_b;
        }
        // write operator/ for int using long division
        BigInt operator/(int n) const {
            // std::cout << "operator / (int)" << std::endl;
            if (n == 0) throw std::invalid_argument("division by zero");
            BigInt bi_a(*this);
            // bi_a.normalize();
            // bi_a.remove_leading_zeroes();
            BigInt res({}, bi_a.radix);
            int carry = 0;
            for (int i = 0; i < bi_a.digits.size(); i++) {
                int cur = bi_a.digits[i] + carry * bi_a.radix;
                res.digits.push_back(cur / n);
                carry = cur % n;
            }
            res.normalize();
            res.remove_leading_zeroes();
            return res;
        }
        BigInt simple_divide(const BigInt& other) const {
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            if (other.toInt() == 0) throw std::invalid_argument("division by zero");
            return BigInt(static_cast<int64_t>(this->toInt()) / other.toInt(), this->radix);
        }
        // write operator/ (binary search for answer using operator*)
        BigInt operator/(const BigInt& other) const {
            // std::cout << "operator /" << std::endl;
            if (radix != other.radix) throw std::invalid_argument("radixes are not equal");
            BigInt bi_a(*this);
            BigInt bi_b(other);
            BigInt INT32_MAX_VALUE = BigInt(INT32_MAX, bi_a.radix);
            if (bi_a < INT32_MAX_VALUE && bi_b < INT32_MAX_VALUE) {
                return simple_divide(bi_b);
            }
            bi_a.normalize();
            bi_b.normalize();
            bi_a.remove_leading_zeroes();
            bi_b.remove_leading_zeroes();
            if (bi_b == BigInt(0, radix)) throw std::invalid_argument("division by zero");
            if (bi_a < bi_b) return BigInt(0, radix);
            BigInt l(0, radix);
            BigInt r = BigInt(bi_a) + BigInt(1, radix);
            BigInt m;
            BigInt one(1, radix);
            while (r - l > one) {
                m = (l + r) / 2;
                if (m * bi_b <= bi_a) {
                    l = m;
                } else {
                    r = m;
                }
            }
            return l;
        }
        int toInt() const {
            // WARN!!! This may cause overflow
            int res = 0;
            for (int i = 0; i < digits.size(); i++) {
                res *= radix;
                res += digits[i];
            }
            return res;
        }
        // write function to translate to another radix
        BigInt translate_from_10(int new_radix) const {
            BigInt bi_a(*this);
            bi_a.normalize();
            bi_a.remove_leading_zeroes();
            if (new_radix == bi_a.radix) return bi_a;
            std::vector<int> new_digits;
            BigInt radix(new_radix, bi_a.radix);
            BigInt zero(0, bi_a.radix);
            while (bi_a > zero) {
                BigInt cur = bi_a % radix;
                // std::cout << cur << std::endl;
                new_digits.insert(new_digits.begin(), cur.toInt());
                bi_a = bi_a / radix;
            }
            BigInt res(new_digits, new_radix);
            res.normalize();
            res.remove_leading_zeroes();
            return res;
        }
        BigInt pow(int n) const {
            BigInt bi_a(*this);
            bi_a.normalize();
            bi_a.remove_leading_zeroes();
            BigInt res(1, bi_a.radix);
            while (n) {
                if (n & 1) {
                    res = res * bi_a;
                }
                bi_a = bi_a * bi_a;
                n >>= 1;
            }
            res.normalize();
            res.remove_leading_zeroes();
            return res;
        }
        BigInt translate_to_10() const {
            BigInt bi_a(*this);
            bi_a.normalize();
            bi_a.remove_leading_zeroes();
            if (bi_a.radix == 10) return bi_a;
            BigInt res(0, 10);
            BigInt radix(bi_a.radix, 10);
            for (int i = 0; i < bi_a.digits.size(); i++) {
                BigInt powResult = radix.pow(bi_a.digits.size() - i - 1);
                res = res + BigInt(bi_a.digits[i], 10) * powResult;
            }
            res.normalize();
            res.remove_leading_zeroes();
            return res;
        }
        BigInt translate(int new_radix) const {
            BigInt bi_a(*this);
            bi_a.normalize();
            bi_a.remove_leading_zeroes();
            if (bi_a.radix == new_radix) return bi_a;
            if (bi_a.radix == 10) return bi_a.translate_from_10(new_radix);
            if (new_radix == 10) return bi_a.translate_to_10();
            return bi_a.translate_to_10().translate_from_10(new_radix);

        }
        void print() {
            for (int i = 0; i < digits.size(); i++) {
                // write code to transcript digits in int to letters (10 -> A)
                if (digits[i] >= 10) {
                    std::cout << (char)('A' + digits[i] - 10);
                } else std::cout << digits[i];
            }
        }
        BigInt gcd(BigInt &other) {
            // std::cout << "gcd(" << *this << ", " << other << ")" << std::endl;
            BigInt bi_a = *this;
            BigInt bi_b = other;
            while (bi_b != BigInt(0, bi_a.radix)) {
                BigInt temp = bi_b;
                bi_b = bi_a % bi_b;
                bi_a = temp;
            }
            *this = *this / bi_a;
            other = other / bi_a;
            return bi_a;
            // if (b == 0) return a
            // return gcd(b, a % b)
        }
        std::string toString() const {
            // std::cout << "to string on bigint" << std::endl;
            std::string res = "";
            for (int i = 0; i < digits.size(); i++) {
                // write code to transcript digits in int to letters (10 -> A)
                if (digits[i] >= 10 && digits[i] <= 35) {
                    res += (char)('A' + digits[i] - 10);
                } else if (digits[i] >= 10) {
                    res += '[' + std::to_string(digits[i]) + ']';
                } else res += std::to_string(digits[i]);
            }
            return res;
        }
};
