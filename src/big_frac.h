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
        }
        // full constructor
        BigFrac(BigInt int_part, BigInt float_part, BigInt period, bool isNegative, bool has_period, bool has_float_part, int radix) : int_part(int_part), float_part(float_part), period(period), isNegative(isNegative), radix(radix), has_period(has_period), has_float_part(has_float_part) {}
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

            this->int_part = int_part;
            this->float_part = float_part;
            this->period = period;
            this->isNegative = isNegative;
            this->radix = radix_int;
            this->has_period = has_period;
            this->has_float_part = has_float_part;
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
};