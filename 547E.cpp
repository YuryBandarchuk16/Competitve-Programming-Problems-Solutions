//http://codeforces.com/problemset/problem/547/E
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

int n, sz;
int lg[N];
int len[N];
char s[N];
char ps[N];
int own[N];
int lcp[N];
int start[N];
int order[A];
bool used[A];
int perm[N];
int sa[LG][N];
int max_le[N];
int max_ri[N];
int posInSA[N];
int table[LG][N];
pair<int, int> have[N];

bool cmp(int x, int y) {
	return have[x] < have[y];
}

struct node {
	node * l, * r;
	int sum;
	node () {
		l = r = NULL;
		sum = 0;
	}
	node (int sum) : sum(sum) {
		l = r = NULL;
	}
	node (node * l, node * r) : l(l), r(r) {
		this -> sum = l -> sum + r -> sum;
	}
};

void buildSA() {
	memset(used, false, sizeof(used));
	for (int i = 1; i <= sz; i++) {
		used[s[i]] = true;
	}
	int diff = 0;
	for (int i = 0; i < A; i++) {
		if (used[i]) {
			order[i] = ++diff;
		}
	}
	for (int i = 1; i <= sz; i++) {
		sa[0][i] = order[s[i]];
		perm[i] = i;
	}
	for (int lvl = 1; lvl <= 19; lvl++) {
		for (int i = 1; i <= sz; i++) {
			int le = sa[lvl - 1][i];
			int ri = i + (1 << (lvl - 1));
			if (ri > sz) {
				ri = -1;
			} else {
				ri = sa[lvl - 1][ri];
			}
			have[i] = mp(le, ri);
		}
		sort(perm + 1, perm + 1 + sz, cmp);
		int classes = 0;
		for (int i = 1; i <= sz; i++) {
			if (i == 1 || have[perm[i]] != have[perm[i - 1]]) {
				++classes;
			}
			sa[lvl][perm[i]] = classes;
		}
	}
	for (int i = 1; i <= sz; i++) {
		perm[sa[19][i]] = i;
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

int getLCPOnSegment(int l, int r) {
	--r;
	int lvl = lg[r - l];
	int ret = min(table[lvl][l], table[lvl][r - (1 << lvl) + 1]);
	return ret;
}

void buildSparse() {
	lg[1] = 0;
	for (int i = 2; i <= sz; i++) {
		lg[i] = lg[i >> 1] + 1;
	}
	for (int i = 1; i < sz; i++) {
		table[0][i] = lcp[i];
	}
	for (int lvl = 1; lvl <= 19; lvl++) {
		for (int i = 1; i < sz; i++) {
			int le = table[lvl - 1][i];
			int ri = i + (1 << (lvl - 1));
			if (ri >= sz) {
				ri = sz - 1;
			}
			ri = table[lvl - 1][ri];
			table[lvl][i] = min(le, ri);
		}
	}
}

node * build(int l, int r) {
	if (l == r) return new node(0);
	int mid = (l + r) >> 1;
	return new node(build(l, mid), build(mid + 1, r));
}

int getSum(node * ver, int l, int r, int pl, int pr) {
	if (pl > pr) return 0;
	if (l == pl && r == pr) return ver -> sum;
	int mid = (l + r) >> 1;
	return getSum(ver -> l, l, mid, pl, min(pr, mid)) + getSum(ver -> r, mid + 1, r, max(pl, mid + 1), pr);
}

node * update(node * ver, int l, int r, int pos) {
	if (l == r) return new node(ver -> sum + 1);
	int mid = (l + r) >> 1;
	if (pos <= mid) return new node(update(ver -> l, l, mid, pos), ver -> r);
	else return new node(ver -> l, update(ver -> r, mid + 1, r, pos));
}

int main() {
	int q;
	sz = 0;
	scanf("%d%d\n", &n, &q);
	for (int i = 1; i <= n; i++) {
		gets(ps + 1);
		len[i] = (int)strlen(ps + 1);
		start[i] = sz + 1;
		for (int j = 1; j <= len[i]; j++) {
			s[++sz] = ps[j];
			own[sz] = i;
		}
		if (i < n) {
			s[++sz] = '$';
			own[sz] = -1;
		}
	}
	buildSA();
	for (int i = 1; i <= sz; i++) {
		posInSA[perm[i]] = i;
	}
	for (int i = 1; i < sz; i++) {
		lcp[i] = getLCP(perm[i], perm[i + 1]);
	}
	buildSparse();
	for (int i = 1; i <= n; i++) {
		int id = start[i];
		id = posInSA[id];
		int le = 0;
		int ri = id;
		while (ri - le > 1) {
			int mid = (le + ri) >> 1;
			int p_id = id - mid;
			if (getLCPOnSegment(p_id, id) >= len[i]) {
				le = mid;
			} else {
				ri = mid;
			}
		}
		max_le[i] = id - le;
		max_le[i] = max(max_le[i], 1);
		le = 0;
		ri = sz - id + 1;
		while (ri - le > 1) {
			int mid = (le + ri) >> 1;
			int p_id = id + mid;
			if (getLCPOnSegment(id, p_id) >= len[i]) {
				le = mid;
			} else {
				ri = mid;
			}
		}
		max_ri[i] = id + le;
	}
	vector<node*> roots;
	roots.pb(build(1, n));
	for (int i = 1; i <= sz; i++) {
		int owner = own[perm[i]];
		if (owner == -1) {
			roots.pb(roots.back());
			continue;
		}
		roots.pb(update(roots.back(), 1, n, owner));
	}
	int le, ri, k;
	for (int i = 1; i <= q; i++) {
		scanf("%d%d%d", &le, &ri, &k);
		printf("%d\n", getSum(roots[max_ri[k]], 1, n, le, ri) - getSum(roots[max_le[k] - 1], 1, n, le, ri));
	}
	return 0;
}
