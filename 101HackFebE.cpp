//https://www.hackerrank.com/contests/101hack34/challenges/beautiful-segments
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <utility>
#include <memory.h>
#include <iterator>
#include <iomanip>
#include <queue>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define pb push_back
#define mp make_pair
#define F first
#define S second

const int mod = (int)1e9 + 7;
const long long longMod = (long long)1e9 + 7LL;

const int N = 40500;
const int Q = 100500;
const int MAX_LOG = 18; 

char mem[300000000];
int used_mem = 0;

void * operator new(size_t sz) {
    void * res = mem + used_mem;
    used_mem += sz;
    return res;
}

void operator delete (void * p) {}

struct Event {
    int x, y, tin, num;
    Event () {};
    Event (int x, int y, int tin, int num) : x(x), y(y), tin(tin), num(num) {};
    bool operator < (const Event &e) const {
        if (tin != e.tin) {
            return tin > e.tin;
        }
        return num < e.num;
    }
};

int modRand = (1 << 16);

struct node {
    node * left, * right;
    int key, prior;
    int cnt;
    int sum_val;
    int sum_cnt;
    node () {
        left = right = NULL;
        key = prior = sum_cnt = sum_val = 0;
    }
    node (node * left, node * right) {
        this -> left = left;
        this -> right = right;
        key = prior = sum_cnt = sum_val = 0;
    }
    node (int key) {
        cnt = 1;
        left = right = NULL;
        this -> key = key;
        this -> sum_val = key;
        this -> sum_cnt = 1;
        prior = (((rand() % modRand) << 15) + rand() % modRand);
    }
};

void add(int &x, int y) {
    x += y;
    if (x >= mod) {
        x -= mod;
    }
    if (x < 0) {
        x += mod;
    }
}

int get_cnt(node * ver) {
    if (!ver) {
        return 0;
    }
    return ver -> sum_cnt;
}

int get_sum(node * ver) {
    if (!ver) {
        return 0;
    }
    return ver -> sum_val;
}

void update_cnt(node * ver) {
    if (!ver) return;
    ver -> sum_cnt = ver -> cnt + get_cnt(ver -> left) + get_cnt(ver -> right);
}

void update_sum(node * ver) {
    if (!ver) return;
    ver -> sum_val = ver -> key * ver -> cnt;
    add(ver -> sum_val, get_sum(ver -> left));
    add(ver -> sum_val, get_sum(ver -> right));
}

void split(node * ver, node * &left, node * &right, int key) {
    if (!ver) {
        left = right = NULL;
        return;
    }
    if (ver -> key <= key) {
        split(ver -> right, ver -> right, right, key);
        left = ver;
    } else {
        split(ver -> left, left, ver -> left, key);
        right = ver;
    }
    update_cnt(left);
    update_cnt(right);
    update_sum(left);
    update_sum(right);
}

void merge(node * &ver, node * left, node * right) {
    if (!left || !right) {
        ver = left;
        if (!ver) {
            ver = right;
        }
        update_sum(ver);
        update_cnt(ver);
        return;
    }
    if (left -> prior > right -> prior) {
        merge(left -> right, left -> right, right);
        ver = left;
    } else {
        merge(right -> left, left, right -> left);
        ver = right;
    }
    update_sum(ver);
    update_cnt(ver);
}

node * contains(node * ver, int key) {
    if (!ver) return NULL;
    if (ver -> key == key) return ver;
    if (key > ver -> key) {
        return contains(ver -> right, key);
    } else {
        return contains(ver -> left, key);
    }
}

void fix(node * ver, int key) {
    if (!ver) return;
    if (ver -> key == key) {
        update_cnt(ver);
        update_sum(ver);
    }
    if (key > ver -> key) {
        fix(ver -> right, key);
        update_sum(ver);
        update_cnt(ver);
    } else {
        fix(ver -> left, key);
        update_cnt(ver);
        update_sum(ver);
    }
}

void insert(node * &ver, node * what) {
    if (!ver) {
        ver = what;
        return;
    }
    if (what -> prior > ver -> prior) {
        split(ver, what -> left, what -> right, what -> key);
        ver = what;
        update_sum(ver);
        update_cnt(ver);
        return;
    }
    if (what -> key > ver -> key) {
        insert(ver -> right, what);
    } else {
        insert(ver -> left, what);
    }
    update_sum(ver);
    update_cnt(ver);
}

int n, q;
int a[N];
int T[N];
int numOfNode[N];
int parent[4 * N];
int two[22];
int answer[Q];
int f_size = 0;
int psum[MAX_LOG][N];
node * tree[4 * N];
vector< pair<int, int> > have[N];

void build(int ver, int l, int r) {
    parent[ver] = (ver / 2);
    if (l == r) {
        numOfNode[l] = ver;
        return;
    }
    int mid = (l + r) >> 1;
    build(ver + ver, l, mid);
    build(ver + ver + 1, mid + 1, r);
}

node * temp;

pair<int, int> CCC;

void setValue(int pos, int val) {
    int ver = numOfNode[pos];
    while (ver > 0) {
        temp = contains(tree[ver], val);
        if (temp == NULL) {
            insert(tree[ver], new node(val));
        } else {
            add(temp -> sum_val, val);
            temp -> sum_cnt++;
            temp -> cnt++;
            fix(tree[ver], val);
        }
        ver = parent[ver];
    }
}

void deleteValue(int pos, int val) {
    int ver = numOfNode[pos];
    while (ver > 0) {
        temp = contains(tree[ver], val);
        if (temp == NULL) {
            puts("FAIL");
            exit(228);
        } else {
            add(temp -> sum_val, -val);
            temp -> sum_cnt--;
            temp -> cnt--;
            fix(tree[ver], val);
        }
        ver = parent[ver];
    }
}

pair<int, int> make_go(int pos, int le, int ch) {
    int ri, mid, its;
    ri = n;
    while (le < ri) {
        mid = (le + ri) >> 1;
        its = 0;
        for (int i = 0; i <= 17; i++) {
            if (psum[i][mid] - psum[i][pos - 1] == mid - pos + 1) {
                its += two[i];
            }
        }
        if (its == ch) {
            le = mid + 1;
        } else {
            ri = mid;
        }
    }
    le = min(le, n);
    its = 0;
    for (int i = 0; i <= 17; i++) {
        if (psum[i][le] - psum[i][pos - 1] == le - pos + 1) {
            its += two[i];
        }
    }
    if (its == ch) {
        return mp(n + 1, 0);
    } else {
        return mp(le, its);
    }
}

int L, R;

node * t1, * t2;

void go(int ver, int l, int r, int pl, int pr) {
    if (pl > pr) return;
    if (l == pl && r == pr) {
        split(tree[ver], t1, t2, R);
        CCC.F += t1 -> sum_cnt;
        add(CCC.S, t1 -> sum_val);
        merge(tree[ver], t1, t2);
        return;
    }
    int mid = (l + r) >> 1;
    go(ver + ver, l, mid, pl, min(pr, mid));
    go(ver + ver + 1, mid + 1, r, max(pl, mid + 1), pr);
}

void make_query(int l, int r) {
    L = l;
    R = r;
    go(1, 1, n, l, r);
}

int main() {
    //freopen("input.txt","r",stdin);
    //freopen("o1","w",stdout);
    //double h = clock();
    scanf("%d%d", &n, &q);
    for (int i = 0; i < 17; i++) {
        psum[i][0] = 0;
        two[i] = (1 << i);
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        for (int j = 0; j <= 17; j++) {
            psum[j][i] = psum[j][i - 1];
            if ((a[i] & (1 << j)) != 0) {
                ++psum[j][i];
            }
        }
    }
    build(1, 1, n);
    pair<int, int> now, then;
    for (int i = 1; i <= n; i++) {
        now = mp(i, a[i]);
        have[i].pb(mp(i, a[i]));
        while (true) {
            then = make_go(i, now.F, now.S);
            have[i].pb(mp(then.F, then.S));
            if (then.F == n + 1) {
                break;
            }
            now = then;
        }
    }
    pair<int, pair<int, int> > data;
    priority_queue< pair<int, pair<int, int> > > pq;
    int e_size = 0;
    int le, ri, val;
    vector<Event> events;
    for (int i = 1; i <= q; i++) {
        scanf("%d%d%d", &le, &ri, &val);
        events.pb(Event(le, ri, val, i));
    }
    for (int i = 1; i <= n; i++) {
        T[i] = i;
        setValue(i, i);
        pq.push(mp(a[i], mp(0, i)));
    }
    sort(events.begin(), events.end());
    for (int i = 0; i < q; i++) {
        val = events[i].tin;
        while (true) {
            data = pq.top();
            if (data.F <= val) {
                break;
            }
            pq.pop();
            le = have[data.S.S][data.S.F].F;
            deleteValue(data.S.S, T[data.S.S]);
            while (true) {
                data.S.F++;
                if (have[data.S.S][data.S.F].S <= val) {
                    data.F = have[data.S.S][data.S.F].S;
                    break;
                } else continue;
            }
            le = have[data.S.S][data.S.F].F;
            T[data.S.S] = le;
            setValue(data.S.S, T[data.S.S]);
            pq.push(data);
        }
        val = events[i].num;
        CCC = mp(0, 0);
        make_query(events[i].x, events[i].y);
        answer[val] = ((CCC.F * (events[i].y + 1) - CCC.S));
    }
    for (int i = 1; i <= q; i++) {
        printf("%d\n", answer[i]);
    }
    //cout << f_size << endl;
    //cout << (clock() - h) / CLOCKS_PER_SEC << "sec" << endl;
    return 0;
}
