//http://codeforces.com/contest/641/problem/E
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

const int N = 2000500;

int n;
int fwSz;
int fw[N];
int legal[N];
int type[N], t[N], v[N], p[N];

bool cmp(int x, int y) {
	if (v[x] != v[y]) {
		return v[x] < v[y];
	}
	return x < y;
}

void update(int i, int add) {
	while (i <= fwSz) {
		fw[i] += add;
		i |= (i + 1);
	}
}

int get(int r) {
	int ret = 0;
	while (r > 0) {
		ret += fw[r];
		r = (r & (r + 1)) - 1;
	}
	return ret;
}

int idx;
int ans[N];
pair<int, int> ar[N];

void solve(int l, int r) {
	idx = 0;
	for (int i = l; i <= r; i++) {
		ar[++idx] = mp(t[p[i]], i);
	}
	sort(ar + 1, ar + 1 + idx);
	int dif = 1;
	for (int i = 1; i <= idx; i++) {
		if (i > 1 && ar[i - 1].F != ar[i].F) {
			++dif;
		}
		legal[p[ar[i].S]] = dif;
	}
	fwSz = dif + 1;
	for (int i = 0; i <= fwSz; i++) {
		fw[i] = 0;
	}
	for (int i = l; i <= r; i++) {
		if (type[p[i]] == 1) {
			update(legal[p[i]], 1);
		} else if (type[p[i]] == 2) {
			update(legal[p[i]], -1);
		} else {
			ans[p[i]] = get(legal[p[i]]);
		}
	}
}

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d%d%d", &type[i], &t[i], &v[i]);
		p[i] = i;
	}
	sort(p + 1, p + 1 + n, cmp);
	int was = 1;
	for (int i = 2; i <= n; i++) {
		if (v[p[i]] != v[p[i - 1]]) {
			solve(was, i - 1);
			was = i;
		}
	}
	solve(was, n);
	for (int i = 1; i <= n; i++) {
		if (type[i] == 3) {
			printf("%d\n", ans[i]);
		}
	}
	return 0;
}
