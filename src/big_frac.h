#pragma once
#include "big_int.h"


class BigFrac {
    BigInt float_part;
    BigInt int_part;
    BigInt period;
    bool isNegative;
    int radix;
    bool has_period;
    bool has_float_part;

    BigInt num;
    BigInt den;
    public:
        // default constructor
        BigFrac() {
            // 0.0(0)_10
            float_part = BigInt();
            int_part = BigInt();
            period = BigInt();
            isNegative = false;
            has_period = false;
            has_float_part = false;
            radix = 10;
            num = BigInt();
            den = BigInt(1);
        }
        // data constructor
        BigFrac(BigInt int_part, BigInt float_part, BigInt period, bool isNegative, bool has_period, bool has_float_part, int radix) : int_part(int_part), float_part(float_part), period(period), isNegative(isNegative), radix(radix), has_period(has_period), has_float_part(has_float_part) {
            // std::cout << "data constructor" << std::endl;
            BigInt bi_radix = BigInt(radix, radix);
            // now working with float part
            BigInt float_part_num = float_part;
            BigInt float_part_den = bi_radix.pow(float_part.digits.size());
            BigInt period_part_num = period;
            BigInt period_part_den = bi_radix.pow(float_part.digits.size()) * (bi_radix.pow(period.digits.size()) - BigInt(1, radix));

            float_part_num.gcd(float_part_den);
            std::cout << "got gcd" << std::endl;

            period_part_num.gcd(period_part_den);
            std::cout << "got gcd" << std::endl;

            // now we need to sum float_part_num / float_part_den and period_part_num / period_part_den
            BigInt new_num = has_period ? float_part_num * period_part_den + period_part_num * float_part_den : float_part_num;
            BigInt new_den = has_period ? float_part_den * period_part_den : float_part_den;
            // new_num.print();
            // std::cout << "/";
            // new_den.print();
            // std::cout << std::endl;
            // reduce the fractions on gcd() of them
            new_num.gcd(new_den);
            std::cout << "got gcd" << std::endl;
            this->num = new_num + int_part * new_den;
            this->den = new_den;
            if (new_den == BigInt(1, new_den.radix)) {
                this->has_float_part = 0;
            }
            if (period_part_den == BigInt(1, new_den.radix)) {
                this->has_period = 0;
            }
        }
        // constructor without radix
        BigFrac(BigInt int_part, BigInt float_part, BigInt period, bool isNegative) : int_part(int_part), float_part(float_part), period(period), isNegative(isNegative), radix(10), has_period(true), has_float_part(true) {}
        // constructor without radix and isNegative
        BigFrac(BigInt int_part, BigInt float_part, BigInt period) : int_part(int_part), float_part(float_part), period(period), isNegative(false), radix(10), has_period(true), has_float_part(true) {}
        // constructor without radix, isNegative and period
        BigFrac(BigInt int_part, BigInt float_part) {
            this->int_part = int_part;
            this->float_part = float_part;
            this->period = BigInt();
            this->isNegative = false;
            this->radix = 10;
        }
        // constructor with BigInt
        BigFrac(BigInt int_part) {
            this->int_part = int_part;
            this->float_part = BigInt();
            this->period = BigInt();
            this->isNegative = false;
            this->radix = 10;
        }
        // constructor with string
        BigFrac(std::string s) {
            bool isNegative = false; // (без токса)

            if (s[0] == '-') {
                isNegative = true;
                s = s.substr(1);
            }
            bool has_period = false;
            bool has_float_part = false;
            // find custom radix
            size_t pos_radix = s.find('_');
            int radix_int = 10;
            if (pos_radix != std::string::npos) {
                // number in format XXX_radix
                std::string radix_str = s.substr(pos_radix + 1);
                if (!isNumber(radix_str)) throw InvalidNumberException();
                radix_int = stoi(radix_str);
                s = s.substr(0, pos_radix);
            }

            // find float point
            size_t pos_float_point = s.find('.');
            BigInt int_part(0, radix_int);
            BigInt period(0, radix_int);
            BigInt float_part(0, radix_int);
            if (pos_float_point != std::string::npos) {
                has_float_part = true;
                // number in format XXX_radix
                // 0.1234(5678) -> 1234(5678)
                std::string float_part_str = s.substr(pos_float_point + 1);
                
                // todo: parse float part and period
                size_t pos_period_start = float_part_str.find('(');
                if (pos_period_start != std::string::npos) {
                    // number with period
                    has_period = true;

                    // check if period is valid
                    size_t pos_period_stop = float_part_str.find(')');
                    if (pos_period_stop == std::string::npos) throw InvalidNumberException();
                    if (pos_period_stop < pos_period_start) throw InvalidNumberException();
                    if (pos_period_stop - pos_period_start == 1) throw InvalidNumberException();
                    // check if last symbol in s is not ")"
                    if (pos_period_stop != float_part_str.size() - 1) throw InvalidNumberException();

                    std::string period_str = float_part_str.substr(pos_period_start + 1, pos_period_stop - pos_period_start - 1);
                    period = BigInt(stringToVectorInt(period_str, radix_int), radix_int);
                    float_part_str = float_part_str.substr(0, pos_period_start);
                }
                float_part = BigInt(stringToVectorInt(float_part_str, radix_int), radix_int);
                s = s.substr(0, pos_float_point);
            }
            int_part = BigInt(stringToVectorInt(s, radix_int), radix_int);

            *this = BigFrac(int_part, float_part, period, isNegative, has_period, has_float_part, radix_int);
        }

        friend std::ostream& operator<<(std::ostream& os, const BigFrac& bf) {
            if (bf.isNegative) os << "-";
            os << bf.int_part;
            if (bf.has_period || bf.has_float_part) os << ".";
            if (bf.has_float_part) os << bf.float_part;
            if (bf.has_period) os << "(" << bf.period << ")";
            os << " (radix: " << bf.radix << ") ";
            return os;
        }

        friend std::istream& operator>>(std::istream& is, BigFrac& bf) {
            /*
            Input BigInt in formats:
            - 123[13]ABCD - in 10 radix (by default)
            - 123[13]ABCD_16 - in 16 radix
            */
            std::string s;
            is >> s;
            bf = BigFrac(s);
            return is;
        }
        void print_num_den() {
            if (isNegative) std::cout << "-";
            std::cout << num << " / " << den << std::endl;
        }
        void translate(int q) {
            this->num = num.translate(q);
            this->den = den.translate(q);
            this->radix = q;
        }
        
        // write std::string toString() method
        std::string toString() {
            BigInt bi_radix = BigInt(radix, radix);
            std::string s = "";
            if (isNegative) s += "-";
            s += (num / den).toString();
            if (!(has_period || has_float_part)) return s;
            s += ".";
            BigInt remainder = num % den;
            // if (!has_period) return s + remainder.toString();
            // s += "(";
            std::map<BigInt, int> remainderMap; // Map to store remainders and their positions

            while (remainder != BigInt(0, remainder.radix) && remainderMap.find(remainder) == remainderMap.end()) {
                // Mark the current remainder and its position
                remainderMap[remainder] = s.length();

                remainder = remainder * bi_radix;
                s += (remainder / den).toString();
                // std::cout << s << std::endl;
                remainder = remainder % den;
            }
            if (remainder != BigInt(0, remainder.radix)) {
                s.insert(remainderMap[remainder], 1, '(');
                s += ")";
            }
            return s;
        }
};