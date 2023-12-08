#pragma once
#include "big_int.h"


class BigFrac {
    BigInt float_part;
    BigInt int_part;
    BigInt period;
    bool isNegative;
    int radix;
    public:
        // default constructor
        BigFrac() {
            // 0.0(0)_10
            float_part = BigInt();
            int_part = BigInt();
            period = BigInt();
            isNegative = false;
            radix = 10;
        }
        // full constructor
        BigFrac(BigInt int_part, BigInt float_part, BigInt period, bool isNegative, int radix) : int_part(int_part), float_part(float_part), period(period), isNegative(isNegative), radix(radix) {}
        // constructor without radix
        BigFrac(BigInt int_part, BigInt float_part, BigInt period, bool isNegative) : int_part(int_part), float_part(float_part), period(period), isNegative(isNegative), radix(10) {}
        // constructor without radix and isNegative
        BigFrac(BigInt int_part, BigInt float_part, BigInt period) {
            this->int_part = int_part;
            this->float_part = float_part;
            this->period = period;
            this->isNegative = false;
            this->radix = 10;
        }
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
};