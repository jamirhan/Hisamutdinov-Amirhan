#include <iostream>
#include <vector>
#include <cmath>



using matrix = std::vector<std::vector<long long>>;

std::vector<matrix> pows(52);
int p = 999999937;

bool has_bit(long long mask, long long bit) {
    return (mask >> bit) & 1;
}


matrix multiply(matrix A, matrix B) {
    int n = A.size();
    int m = A[0].size(); // suppose equal to B.size()
    int k = B[0].size();
    matrix result(n, std::vector<long long>(k));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int el = 0; el < m; ++el) {
                result[i][j] = (result[i][j] + (A[i][el] * B[el][j]) % p) % p;
            }
        }
    }
    return result;
}

matrix identity(int size) {
    matrix ans(size, std::vector<long long>(size, 0));
    for (int i = 0; i < size; ++i)
        ans[i][i] = 1;
    return ans;
}


void fast_charge(matrix& A, long long power) { // for square matrix only
    int max_p = log2(power);

    A = pows[0];
    for (int bit = 0; bit < max_p + 1; ++bit) {
        if (has_bit(power, bit)) {
            A = multiply(A, pows[bit + 1]);
        }
    }

}


int main() {
    long long n = 1;
    matrix base = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 0, 1, 0},
        {1, 1, 0, 1, 0}
    };
    matrix initial = {{1},
                      {1},
                      {1},
                      {1},
                      {1}};

    pows[0] = identity(5);
    pows[1] = base;
    for (int i = 2; i < 52; ++i) {
        pows[i] = multiply(pows[i-1], pows[i-1]);
    }
    matrix A;
    matrix ans;
    long long sum;
    while (n != 0) {
        std::cin >> n;
        A = initial;
        fast_charge(A, n - 1);
        ans = multiply(A, initial);
        sum = 0;
        for (int i = 0; i < 5; ++i)
            sum = (sum + ans[i][0]) % p;
        std::cout << sum << '\n';
    }
}
