#include<iostream>
#include<vector>

#define CONST 1000000007
using namespace std;

class span {
public:
    long long RR = 0;
    long long RG = 0;
    long long GR = 0;
    long long GG = 0;
    long long sum() {
        return (RR + GR + RG + GG) % CONST;
    }
    void str(){
        cout << RR << " " << GG << " " << RG << " " << GR << endl;
    }
} ;

long long fRR(span *l, span *r){
    return (l->RR+
            r->RR+
            (l->RR*r->RR)%CONST+
            (l->RR*r->GR)%CONST+
            (l->RG*r->RR)%CONST
           )%CONST;
}

long long fGG(span *l, span *r){
    return (l->GG+
            r->GG+
            (l->GG*r->RG)%CONST+
            (l->GR*r->GG)%CONST+
            (l->GR*r->RG)%CONST
           )%CONST;
}

long long fGR(span *l, span *r){
    return (l->GR+
            r->GR+
            (l->GG*r->RR)%CONST+
            (l->GR*r->GR)%CONST+
            (l->GR*r->RR)%CONST
           )%CONST;
}

long long fRG(span *l, span *r){
    return (l->RG+
            r->RG+
            (l->RR*r->GG)%CONST+
            (l->RG*r->RG)%CONST+
            (l->RR*r->RG)%CONST
           )%CONST;
}

void count(span *s, span *l, span *r){
    if(s == nullptr) return;
    s->RR = fRR(l, r);
    s->GG = fGG(l, r);
    s->RG = fRG(l, r);
    s->GR = fGR(l, r);
}

long long query(vector<char> &znaki, int l, int p){
    vector<span> v;    
    for(int i=l;i<p+1;i++){
        v.push_back(span());        
        if(znaki[i] == 'R'){
            v[i-l].RR = 1;
        } else {
            v[i-l].GG = 1;
        }
    }
    vector<span> w;
    while(v.size() > 1){
        //for(int i=0;i<v.size();i++) v[i].str(); cout << endl;
        int i = 0;
        while(i<v.size()-1){
            w.emplace_back(span());
            count(&w[i/2], &v[i], &v[i+1]);
            i += 2;
        }
        if(i == v.size()-1) w.push_back(v[i]);
        v = move(w);
        w.clear();
    }
    return v[0].sum();
}

void rotate(vector<char> &znaki, vector<int> &liczby, int l, int p){
    while(l < p){
        char c = znaki[l];
        znaki[l] = znaki[p];
        znaki[p] = c;
        int o = liczby[l];
        liczby[l] = liczby[p];
        liczby[p] = o; 
        l++;
        p--;
    }
}

int main(){
    int n; int m;
    cin >> n >> m;
    vector<char> v;
    vector<int> liczby;
    for(int i=0;i<n;i++){
        char c;
        cin >> c;
        v.push_back(c);
        liczby.push_back(i+1);
    }
    //for(int j=0;j<n;j++){
      //  cout << liczby[j] << " ";
    //} cout << endl;
    for(int i=0;i<m;i++){
        char c; int l, p;
        cin >> c >> l >> p;
        l--; p--;
        if(c == '?'){
            cout << query(v,l,p) << endl;
        } else {
            rotate(v,liczby,l,p);
            for(int j=0;j<n;j++){
                //cout << liczby[j] << " ";
            } //cout << endl;
        }
    }
    return 0;
}
