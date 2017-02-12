// AVL Tree Implementation

#include <iostream>
#include <random>
#include <set>

using namespace std;

struct avl_item {
   int key;
   int key_right;
   bool is_leaf;
   int height;
   avl_item * l, * r;
   avl_item() {};
   avl_item(int key) : key(key), l (NULL), r (NULL), is_leaf(true), key_right(key) {};
   avl_item(int key, avl_item * l, avl_item * r) : key(key), l(l), r(r), is_leaf(false) {};
};

typedef avl_item * pitem;

bool contains(pitem t, int val) {
   if (!t)
      return false;
   if (t->key == val)
      return true;
   if (t->key > val)
      return contains(t->l, val);
   return contains(t->r, val);
}

int get_height(pitem t) {
   if (!t)
      return 0;
   if (t->is_leaf)
      return 1;
   return t->height;
}

int get_diff(pitem t) {
   if (!t || t->is_leaf)
      return 0;
   return get_height(t->l) - get_height(t->r);
}

void update_vertex(pitem & t) {
   if (!t)
      return;
   if (t->is_leaf) {
      return;
   }
   if (!t->l || !t->r) {
      if (!t->l) {
         t = t->r;
      } else {
         t = t->l;
      }
   } else {
      int left_height = get_height(t->l);
      int right_height = get_height(t->r);
      if (abs(left_height - right_height) <= 1) {
         t->height = max(left_height, right_height) + 1;
         t->key = t->l->key_right;
         t->key_right = t->r->key_right;
      } else {
         if (get_diff(t) == -2) {
            if (get_diff(t->r) <= 0) {
               // small left rotate
               pitem a = t;
               pitem b = t->r;
               a->r = b->l;
               b->l = a;
               t = b;
               update_vertex(t->l);
               update_vertex(t);
            } else {
               // big left rotate
               pitem a = t;
               pitem b = t->r;
               pitem c = b->l;
               a->r = c->l;
               b->l = c->r;
               c->l = a;
               c->r = b;
               t = c;
               update_vertex(a);
               update_vertex(b);
               update_vertex(t);
            }
         } else {
            if (get_diff(t->l) >= 0) {
               // small right rotate
               pitem a = t;
               pitem b = t->l;
               a->l = b->r;
               b->r = a;
               t = b;
               update_vertex(t->r);
               update_vertex(t);
            } else {
               // big right rotate
               pitem a = t;
               pitem b = t->l;
               pitem c = b->r;
               a->l = c->r;
               b->r = c->l;
               c->r = a;
               c->l = b;
               t = c;
               update_vertex(a);
               update_vertex(b);
               update_vertex(t);
            }
         }
      }
   }
   
}

void insert(pitem & t, pitem new_item, bool check = true) {
   if (check && contains(t, new_item->key))
      return;
   if (!t) {
      t = new_item;
   } else {
      if (t->is_leaf) {
         if (t->key > new_item->key) {
            t = new avl_item(new_item->key, new_item, t);
         } else {
            t = new avl_item(t->key, t, new_item);
         }
      } else {
         if (t->key > new_item->key) {
            insert(t->l, new_item, false);
         } else {
            insert(t->r, new_item, false);
         }
      }
   }
   update_vertex(t);
}

void remove(pitem & t, int val, bool check = true) {
   if (check && !contains(t, val))
      return;
   if (t->is_leaf) {
      delete t;
      t = NULL;
   } else {
      if (t->key >= val) {
         remove(t->l, val, false);
      } else {
         remove(t->r, val, false);
      }
      update_vertex(t);
   }
}

bool test() {
   pitem root = NULL;
   set<int> std_set;
   
   for (int i = 0; i < 100; i++) {
      if (i % 2 == 0) {
         std_set.insert(i);
         insert(root, new avl_item(i));
      }
   }
   for (int i =0; i < 100; i++) {
      bool exist_my_algo = contains(root, i);
      bool exist_std_set = (std_set.find(i) != std_set.end());
      if (exist_my_algo != exist_std_set)
         return false;
   }
   
   
   for (int i = 0; i < 1000000; i++) {
      int val = rand() % 1000;
      //int val = i % 2 == 0 ? -i : i;
      if (rand() % 2 <= 0) {
         std_set.insert(val);
         insert(root, new avl_item(val));
      } else {
         if (rand() % 2 == 0) {
            // remove
            std_set.erase(val);
            remove(root, val);
         } else {
            // check
            bool exist_my_algo = contains(root, val);
            bool exist_std_set = (std_set.find(val) != std_set.end());
            if (exist_my_algo != exist_std_set) {
               return false;
            }
         }
      }

   }

   return true;
}


int main() {
   //srand(time(0));
   if (!test()) {
      cout << "fail" << endl;
   } else {
      cout << "all tests passed" << endl;
   }
}
