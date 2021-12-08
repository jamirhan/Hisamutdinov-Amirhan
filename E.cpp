#include <iostream>
#include <vector>
using ll = int64_t;
ll MaxN = 200000;
ll mod = 1'000'000'007;
ll max_log = 20;
std::vector<ll> facts(MaxN + 1);
std::vector<ll> facts_inv(MaxN + 1);
std::vector<std::vector<ll>> power(MaxN + 1, std::vector<ll>(max_log + 1));


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


ll find_power(ll j, ll n) {
    ll cur_power = 0;
    ll ans = 1;
    while (n > 0) {
        if (n & 1ll) {
            ans *= power[j][cur_power];
            ans %= mod;
        }
        n >>= 1ll;
        ++cur_power;
    }
    return ans;
}

ll c(ll n, ll k) {
    if (k > n) return 0;
    ll ans = facts[n];
    ll div1 = facts_inv[k];
    ll div2 = facts_inv[n - k];
    ans *= div1;
    ans %= mod;
    ans *= div2;
    ans %= mod;
    return ans;
}


ll f(ll n, ll k) {
    ll ans = facts_inv[k];

    if (k % 2) {
        ans *= (mod - 1);
        ans %= mod;
    }

    ll s = 0;

    for (ll j = 0; j <= k; ++j) {
        ll cur_ans = 1;
        if (j % 2) cur_ans = mod - 1;
        cur_ans *= c(k, j);
        cur_ans %= mod;
        cur_ans *= find_power(j, n);
        cur_ans %= mod;
        s += cur_ans;
        if (s >= mod) s -= mod;
    }

    s %= mod;

    ans *= s;
    ans %= mod;

    return ans;
    
}


int main() {
    facts[0] = 1;
    for (ll i = 1; i <= MaxN; ++i) facts[i] = (facts[i - 1] * i) % mod;
    for (ll i = 0; i <= MaxN; ++i) facts_inv[i] = find_inverse(facts[i], mod);
    for (ll j = 0; j <= MaxN; ++j) {
        power[j][0] = j;
        for (ll lg = 1; lg <= max_log; ++lg) power[j][lg] = (power[j][lg - 1]*power[j][lg - 1]) % mod;
    }

    ll n, k;
    std::cin >> n >> k;

    ll s = 0;
    for (ll i = 0; i < n; ++i) {
        ll x;
        std::cin >> x;
        s += x;
        if (s >= mod) s -= mod;
    }

    s %= mod;

    ll f1 = f(n, k);
    ll f2 = f(n - 1, k);

    f2 *= n - 1;

    f2 %= mod;

    f1 += f2;

    f1 %= mod;

    f1 *= s;

    f1 %= mod;

    std::cout << f1 << '\n';

}
