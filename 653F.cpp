//http://codeforces.com/contest/653/problem/F
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <utility>
#include <memory.h>
#include <cassert>
#include <iterator>
#include <bitset>
#include <iomanip>
#include <complex>
#include <queue>
#include <ctime>
#include <deque>
#include <stack>
#include <set>
#include <map>

using namespace std;

#define pb push_back
#define mp make_pair
#define F first
#define S second

const int A = 333;
const int LG = 20;
const int N = 500500;

int n;
char s[N];
int lg[N];
int bal[N];
int idx[N];
int perm[N];
bool used[A];
int order[A];
int max_go[N];
int sa[LG][N];
int table[LG][N];
vector<int> g[N];
pair<int, int> a[N];
pair<int, int> have[N];

bool cmp(int x, int y) {
	return have[x] < have[y];
}

int get(int line, int x) {
	if (g[line][0] > x) {
		return 0;
	}
	int le = 0;
	int ri = (int)g[line].size();
	while (ri - le > 1) {
		int mid = (le + ri) >> 1;
		if (g[line][mid] <= x) {
			le = mid;
		} else {
			ri = mid;
		}
	}
	return le + 1;
}

void buildSA() {
	memset(used, false, sizeof(used));
	for (int i = 1; i <= n; i++) {
		used[s[i]] = true;
	}
	int diff = 0;
	for (int i = 0; i < A; i++) {
		if (used[i]) {
			order[i] = ++diff;
		}
	}
	for (int i = 1; i <= n; i++) {
		sa[0][i] = order[s[i]];
	}
	for (int i = 1; i <= n; i++) {
		perm[i] = i;
	}
	for (int lvl = 1; lvl <= 19; lvl++) {
		for (int i = 1; i <= n; i++) {
			int le = sa[lvl - 1][i];
			int ri = i + (1 << (lvl - 1));
			if (ri > n) {
				ri = -1;
			} else {
				ri = sa[lvl - 1][ri];
			}
			have[i] = mp(le, ri);
		}
		sort(perm + 1, perm + 1 + n, cmp);
		int classes = 0;
		for (int i = 1; i <= n; i++) {
			if (i == 1 || have[perm[i]] != have[perm[i - 1]]) {
				++classes;
			}
			sa[lvl][perm[i]] = classes;
		}
	}
	for (int i = 1; i <= n; i++) {
		perm[sa[19][i]] = i;
	}
}

int getMin(int le, int ri) {
	int lvl = lg[ri - le];
	int ret = min(table[lvl][le], table[lvl][ri - (1 << lvl) + 1]);
	return ret;
}

void buildSparse() {
	lg[1] = 0;
	for (int i = 2; i <= n + 2; i++) {
		lg[i] = lg[i >> 1] + 1;
	}
	for (int i = 1; i <= n; i++) {
		table[0][i] = bal[i];
	}
	for (int lvl = 1; lvl <= 19; lvl++) {
		for (int i = 1; i <= n; i++) {
			int le = table[lvl - 1][i];
			int ri = i + (1 << (lvl - 1));
			if (ri > n) {
				ri = n;
			}
			ri = table[lvl - 1][ri];
			table[lvl][i] = min(le, ri);
		}
	}
}

int getLCP(int x, int y) {
	int ret = 0;
	for (int i = 19; i >= 0; --i) {
		if (sa[i][x] == sa[i][y]) {
			ret += (1 << i);
			x += (1 << i);
			y += (1 << i);
		}
	}
	return ret;
}

int main() {
	scanf("%d\n", &n);
	gets(s + 1);
	bal[0] = 0;
	a[0] = mp(0, 0);
	for (int i = 1; i <= n; i++) {
		if (s[i] == '(') {
			bal[i] = bal[i - 1] + 1;
		} else {
			bal[i] = bal[i - 1] - 1;
		}
		a[i] = mp(bal[i], i);
	}
	sort(a + 0, a + 1 + n);
	int ptr = 0;
	for (int i = 0; i <= n; i++) {
		if (i == 0 || a[i - 1].F != a[i].F) {
			++ptr;
		}
		idx[a[i].S] = ptr;
	}
	buildSA();
	buildSparse();
	g[idx[0]].pb(0);
	for (int i = 1; i <= n; i++) {
		int le = i - 1;
		int ri = n + 1;
		while (ri - le > 1) {
			int mid = (le + ri) >> 1;
			if (getMin(i, mid) < bal[i - 1]) {
				ri = mid;
			} else {
				le = mid;
			}
		}
		max_go[i] = le;
		g[idx[i]].pb(i);
	}
	long long ans = 0LL;
	for (int i = 1; i <= n; i++) {
		ans = ans + 1LL * get(idx[i - 1], max_go[i]) - 1LL * get(idx[i - 1], i - 1);
	}
	long long dec;
	for (int i = 1; i < n; i++) {
		int le = perm[i];
		int ri = perm[i + 1];
		int lcp = getLCP(le, ri);
		lcp = min(lcp, max_go[le] - le + 1);
		lcp = min(lcp, max_go[ri] - ri + 1);
		lcp = le + lcp - 1;
		lcp = max(lcp, le - 1);
		dec = 1LL * get(idx[le - 1], lcp) - 1LL * get(idx[le - 1], le - 1);
		ans -= dec;
	}
	cout << ans; puts("");
	return 0;
}
