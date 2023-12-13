#include <bits/stdc++.h>
#include "big_frac.h"
using namespace std;
int main() {
    BigFrac x;
    int y;
    cin >> x >> y;
    x.print_num_den();
    cout << "\n";
    x.translate(y);
    x.print_num_den();
    // z.print();
}