#include <iostream>
#include <vector>
#include <complex>


std::vector<int> rev;
int k;
using ll = int64_t;
ll fft_mod = 998'244'353;
ll w = 31;
std::vector<ll> w_pows(24);
std::vector<ll> w_pows_inv(24);


void find_rev() {
    int oldest = -1;
    for (int mask = 1; mask < (1 << k); ++mask) {
        if (!(mask&(mask - 1))) ++oldest;
        rev[mask] = rev[mask^(1 << oldest)] | (1 << (k - oldest - 1));
    }
}


ll gcd (ll a, ll b, ll & x, ll & y) {
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}

	ll x1, y1;
	ll d = gcd (b%a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}


ll find_inverse(ll num, ll mod) {
    ll x, y;
    ll gg = gcd(num, mod, x, y);
    return (x + mod) % mod;
}


void find_vals(std::vector<ll>& polynom, int sign) {
    std::vector<ll> new_pol(polynom.size());
    for (int i = 0; i < polynom.size(); ++i) {
        new_pol[rev[i]] = polynom[i];
    }

    ll n = (1 << k);

    for (ll len_p=1; len_p<=k; ++len_p) {
        ll wlen;
        if (sign == 1)
            wlen = w_pows[23 - len_p];
        else
            wlen = w_pows_inv[23 - len_p];
        
		for (int i=0; i<n; i+=(1 << len_p)) {
            ll w_cp = 1;
			for (int j=0; j<(1 << (len_p-1)); ++j) {
				ll u = new_pol[i+j];
                ll v = (new_pol[i+j+(1 << (len_p-1))] * w_cp) % fft_mod;
				new_pol[i+j] = (u + v) % fft_mod;
				new_pol[i+j+(1 << (len_p-1))] = (fft_mod + u - v) % fft_mod;
				w_cp *= wlen;
                w_cp %= fft_mod;
			}
		}
	}
    polynom = new_pol;
}


std::vector<ll> fft(const std::vector<ll>& p, const std::vector<ll>& q) {

    int n = p.size() + q.size() - 1;
    k = std::ceil(std::log2(n));
    n = std::pow(2, k);
    rev.resize(n);
    find_rev();

    std::vector<ll> pc(n);
    std::vector<ll> qc(n);
    for (int i = 0; i < p.size(); ++i) pc[i] = p[i];
    for (int i = 0; i < q.size(); ++i) qc[i] = q[i];
    find_vals(pc, 1);
    find_vals(qc, 1);

    for (int i = 0; i < n; ++i) pc[i] *= qc[i], pc[i] %= fft_mod;

    find_vals(pc, -1);

    std::vector<ll> ans(n);

    ll n_inv = find_inverse(n, fft_mod);

    for (int i = 0; i < n; ++i) ans[i] = (pc[i] * n_inv) % fft_mod;

    return ans;

}


void fft_precompute() {
    w_pows[0] = w;
    for (int i = 1; i <= 23; ++i) w_pows[i] = (w_pows[i - 1] * w_pows[i - 1]) % fft_mod;
    for (int i = 0; i <= 23; ++i) w_pows_inv[i] = find_inverse(w_pows[i], fft_mod);
}


int main() {
    fft_precompute();
    int n, m;
    std::vector<ll> pol1;
    std::vector<ll> pol2;

    std::cin >> n;
    ++n;
    pol1.resize(n);
    for (int i = 0; i < n; ++i) { 
        std::cin >> pol1[n - i - 1];
        pol1[n - i - 1] += fft_mod;
        pol1[n - i - 1] %= fft_mod;
    }
    std::cin >> m;
    ++m;
    pol2.resize(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> pol2[m - i - 1];
        pol2[m - i - 1] += fft_mod;
        pol2[m - i - 1] %= fft_mod;
    }

    auto res = fft(pol1, pol2);
    std::vector<int> ans(res.size());

    for (int i = 0; i < res.size(); ++i) {
        if (res[i] >= 0)
            ans[i] = int(res[i] + 0.1);
        else 
            ans[i] = int(res[i] - 0.1);
    }

    int ctr = 0;
    for (int i = ans.size() - 1; i >= 0; --i) {
        if (ans[i] != 0) {
            ctr = i;
            break;
        }
    }

    std::cout << ctr << " ";

    for (int i = ctr; i >= 0; --i) {
        std::cout << ans[i] << " ";
    }
    std::cout << '\n';
}
