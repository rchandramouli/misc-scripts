#include <iostream>
#include <algorithm>
#include <set>

#include <cstdint>
#include <cinttypes>
#include <cmath>

int main (int argc, char *argv[])
{
    std::set<uint64_t> s_vals;
    uint64_t nr, i, j, m, mx3, max_l, v, old_sz;
    long double x3 = 1.0/3.0, u;

    std::cin >> max_l;

    mx3 = 1+std::pow(static_cast<long double>(max_l)/4.0, x3);

    std::cout << "MX3 = " << mx3 << std::endl;

    nr = 0;
    for (i = 2; i <= mx3; i++) {
	j = i*i*i;
	u = (long double)max_l / (long double)j;
	v = std::sqrt(u);
	nr += v-1;

	// std::cout << "\t---- I: " << i << ", V: " << v << ", NR: " << nr << std::endl;
	// std::cout << "\t---- MAX: " << (j*v*v) << std::endl;

	for (m = 2; m <= v; m++) {
	    s_vals.insert(j*m*m);
	}
    }

    std::cout << "Total elements: " << s_vals.size() << std::endl;
    
    return 0;
}
