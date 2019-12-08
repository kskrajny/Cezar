#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;

#define CONST 1000000007

int N; // 2*N is size of tree
int H; // heigth of tree
int R;

class span {
public:
    long long RR = 0;
    long long RG = 0;
    long long GR = 0;
    long long GG = 0;
    bool ready = true;
    int start;
    int stop;
    span *l = NULL;
    span *r = NULL;
    span *f = NULL;
    long long sum(){
        return (RR+GR+RG+GG)%CONST;
    }
    span(int l, int p){
        start = l;
        stop = p;
    }
} ;

long long fRR(span *s){
    return (s->l->RR+
            s->r->RR+
            s->l->RR*s->r->RR+
            s->l->RR*s->r->GR+
            s->l->RG*s->r->RR
            )%CONST;
}

long long fGG(span *s){
    return (s->l->GG+
            s->r->GG+
            s->l->GG*s->r->RG+
            s->l->GR*s->r->GG+
            s->l->GR*s->r->RG
            )%CONST;
}

long long fGR(span *s){
    return (s->l->GR+
            s->r->GR+
            s->l->GG*s->r->RR+
            s->l->GR*s->r->GR+
            s->l->GR*s->r->RR
           )%CONST;
}

long long fRG(span *s){
    return (s->l->RG+
            s->r->RG+
            s->l->RR*s->r->GG+
            s->l->RG*s->r->RG+
            s->l->RR*s->r->RG
           )%CONST;
}

void count(span *s){
    s->RR = fRR(s);
    s->GG = fGG(s);
    s->RG = fRG(s);
    s->GR = fGR(s);
}

void update(span *s) {
    if(s == NULL || !s->ready) return;
    span *u = s->l;
    s->l = s->r;
    s->r = u;
    long long o = s->RG;
    s->RG = s->GR;
    s->GR = o;
    s->ready = true;
    s->l->ready = false;
    s->r->ready = false;
}

void build(int stage, span *s){
    if(stage == H) {
        if(R == 0) return;
        char c;
        cin >> c;
        if(c == 'R') s->RR = 1;
        if(c == 'G') s->GG = 1;
        R--;
    } else {
        span *left = new span(s->start, (s->start+s->stop)/2);
        span *rigth = new span(1+(s->start+s->stop)/2, s->stop);
        left->f = s;
        rigth->f = s;
        s->l = left;
        s->r = rigth;
        build(stage + 1, left);
        build(stage + 1, rigth);
        count(s);
    }
}

// find biggest span that starts with wanted
span* find(int l, int p, span *s){
    int c = (s->start+s->stop)/2;
    while(s->start != l && s->stop > p){
        if(l == s->start) {
            s = s->l;
        } else {
            if (l <= c) {
                s = s->l;
            } else {
                s = s->r;
            }
        }
        update(s);
    }
    return s;
}

long long query(int l, int p, span *tree){
    int sum = 0;
    while(l < p){
        span *s = find(l, p, tree);
        while(s->stop > p){
            s = s->l;
            update(s);
            assert(s != NULL);
        }
        sum += s->sum();
        l = s->stop+1;
    }
    return sum;
}

void rotate(int l, int p){
}

int main() {
    int n, m;
    ios::sync_with_stdio(false);
    cin >> n >> m;
    H = ceil(log2(n));
    N = pow(2,H);
    R = n;
    span *tree = new span(1, N);
    build(0, tree);
    for(int i=0;i<m;i++) {
        char c;
        int start, finish;
        cin >> c >> start >> finish;
        if (c == '?') {
            cout << query(start, finish, tree) << endl;
        } else {
            rotate(start, finish);
        }
    }
    return 0;
}

