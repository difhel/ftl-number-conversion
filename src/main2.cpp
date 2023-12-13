#include <bits/stdc++.h>
#include "big_frac.h"
#pragma GCC optimize("O3,fast-math")
#pragma GCC target("avx,avx2,fma")
using namespace std;
int main() {
    BigFrac x;
    int y;
    cin >> x >> y;
    x.print_num_den();
    cout << "\n";
    x.translate(y);
    x.print_num_den();
    cout << "\n";
    cout << x.toString();
    // z.print();
}