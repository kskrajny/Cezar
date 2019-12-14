#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int N, M, H, CIN, currLeft, ID;
long long lastOutcome;

#define CONST 1000000007

class span;
class node;
void change(node *Node);

class span {
public:
    long long RR = 0;
    long long RG = 0;
    long long GR = 0;
    long long GG = 0;
    long long sum() {
        return (RR + GR + RG + GG) % CONST;
    }
} ;



class node
{
public:
    int id = -1;
    int leftSize = -1;
    int rightSize = -1;
    node *left = nullptr;
    node *right = nullptr;
    span *bigSpan = nullptr;
    span *s = nullptr;
    bool toRotate = false;
    void rotate(){
        if(toRotate){
            node *help = this->left;
            this->left = this->right;
            this->right = help;
            change(this->right);
            change(this->left);
            int helpSize = leftSize;
            leftSize = rightSize;
            rightSize = helpSize;
            toRotate = false;
            long long o = bigSpan->RG;
            bigSpan->RG = bigSpan->GR;
            bigSpan->GR = o;
        }
    }
};

void change(node *Node){
    if(Node == nullptr) return;
    if(Node->leftSize+Node->rightSize == 0) {Node->toRotate = false; return;}
    Node->toRotate = !Node->toRotate;
}

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

void count(span *all, span *l, span *r){
    if(all == nullptr) return;
    all->RR = fRR(l, r);
    all->GG = fGG(l, r);
    all->RG = fRG(l, r);
    all->GR = fGR(l, r);
}

void count(span *all, span *l, span *m, span *r){
    span help = span();
    count(&help, l, m);
    count(all, &help, r);
}

node *rightRotate(node *x)
{
    node *y = x->left;
    if(y == nullptr) return y;
    y->rotate();
    x->left = y->right;
    y->right = x;

    int T2 = y->rightSize;
    int T3 = x->rightSize;
    x->rightSize = T3;
    x->leftSize = T2;
    y->rightSize = T2+T3+1;

    span t1 = span();
    span t2 = span();
    span t3 = span();
    if(y->left != nullptr){
        y->left->rotate(); t1 = *y->left->bigSpan;
    }
    if(x->left != nullptr){
        x->left->rotate(); t2 = *x->left->bigSpan;
    }
    if(x->right != nullptr){
        x->right->rotate(); t3 = *x->right->bigSpan;
    }
    count(x->bigSpan,&t2,x->s,&t3);
    count(y->bigSpan,&t1,y->s,x->bigSpan);
    return y;
}

node *leftRotate(node *x)
{
    node *y = x->right;
    if(y == nullptr) return y;
    y->rotate();
    x->right = y->left;
    y->left = x;

    int T1 = x->leftSize;
    int T2 = y->leftSize;
    x->rightSize = T2;
    x->leftSize = T1;
    y->leftSize = T1+T2+1;

    span t1 = span();
    span t2 = span();
    span t3 = span();
    if(y->right != nullptr){
        y->right->rotate(); t3 = *y->right->bigSpan;
    }
    if(x->left != nullptr){
        x->left->rotate(); t1 = *x->left->bigSpan;
    }
    if(x->right != nullptr){
        x->right->rotate(); t2 = *x->right->bigSpan;
    }
    count(x->bigSpan,&t1,x->s,&t2);
    count(y->bigSpan,x->bigSpan,y->s,&t3);

    return y;
}

// elem with key becomes a root
// key depends on leftSize and key is changing during function
node *splay(node *root, int key)
{
    if (root == nullptr)
        return root;

    // updates a lot of nodes below
    root->rotate();
    if(root->left != nullptr) {
        root->left->rotate();
        if(root->left->left != nullptr) root->left->left->rotate();
        if(root->left->right != nullptr) root->left->right->rotate();
    }
    if(root->right != nullptr) {
        root->right->rotate();
        if(root->right->left != nullptr) root->right->left->rotate();
        if(root->right->right != nullptr) root->right->right->rotate();
    }
    if (root->leftSize+1 == key)
        return root;

    if (root->leftSize+1 > key) // go to left subtree
    {
        //no need to update key

        // Zig-Zig (Left Left)
        if (root->left->leftSize+1 > key)
        {
            // brings key to root->left->left
            root->left->left = splay(root->left->left, key);

            // Do first rotation for root,
            // second rotation is later
            root = rightRotate(root);
        }
        else if (root->left->leftSize+1 < key) // Zig-Zag (Left Right)
        {
            // brings the key to root->left->right
            root->left->right = splay(root->left->right, key-root->left->leftSize-1);

            // Do first rotation for root->left
            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }

        // Do second rotation for root
        return (root->left == nullptr) ? root : rightRotate(root);
    }
    else // go to right subtree
    {
        // need to update key
        key = key-root->leftSize-1;

        if (root->right->leftSize+1 > key)
        {
            root->right->left = splay(root->right->left, key);

            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }
        else if (root->right->leftSize+1 < key)
        {
            root->right->right = splay(root->right->right, key-root->right->leftSize-1);
            root = leftRotate(root);
        }

        // Do second rotation for root
        return (root->right == nullptr) ? root : leftRotate(root);
    }
}

node* read(node *root){
    char c;
    cin >> c;
    root->id = ID;
    ID++;
    root->s = new span();
    if(c == 'R'){
        root->s->RR = 1;
    }
    if(c == 'G'){
        root->s->GG = 1;
    }
    return root;
}

// builds a tree, gives id to each node
node* build (int stage) {
    if(CIN == 0 || stage > H){
        return nullptr;
    }
    node *Node = new node();
    CIN--;
    Node->left = build(stage+1);
    read(Node);
    Node->right = build(stage+1);
    int l = 0;
    int r = 0;
    span t1 = span();
    span t2 = span();
    Node->bigSpan = new span();
    if(Node->left != nullptr) {
        l = Node->left->leftSize+Node->left->rightSize+1;
        t1 = *Node->left->bigSpan;
    }
    if(Node->right != nullptr) {
        r = Node->right->leftSize+Node->right->rightSize+1;
        t2 = *Node->right->bigSpan;
    }
    count(Node->bigSpan,&t1,Node->s,&t2);
    Node->rightSize = r;
    Node->leftSize = l;
    return Node;
}

node* rotate(node *root, int left, int right){
    if(left == right) return root;
    root = splay(root, left);
    currLeft = root->id;
    root = splay(root, right);
    if(root->left->id != currLeft){
        if(root->left->left != nullptr && root->left->left->id == currLeft)
            root->left = rightRotate(root->left);
        if(root->left->right != nullptr && root->left->right->id == currLeft)
            root->left = leftRotate(root->left);
    }
    node *l = root->left;
    node *r = root;
    node *toRotate = l->right;
    node *leftChild = l->left;
    node *rightChild = r->right;
    l->left = r;
    l->right = rightChild;
    r->left = leftChild;
    r->right = toRotate;
    change(toRotate);
    if(toRotate != nullptr) toRotate->rotate();
    int T1 = l->leftSize;
    int T2 = l->rightSize;
    int T3 = r->rightSize;
    l->rightSize = T3;
    l->leftSize = T1+T2+1;
    r->leftSize = T1;
    r->rightSize = T2;
    return l;
}

void deleteTree(node *root)
{
    if (root != nullptr)
    {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root->s;
        delete root->bigSpan;
        delete root;
    }
}

node* query(node *root, int left, int right){
    lastOutcome = 1;
    if(left == right) return root;
    root = splay(root, left);
    currLeft = root->id;
    root = splay(root, right);
    if(root->left->id != currLeft){
        if(root->left->left != nullptr && root->left->left->id == currLeft)
            root->left = rightRotate(root->left);
        if(root->left->right != nullptr && root->left->right->id == currLeft)
            root->left = leftRotate(root->left);
    }
    span mid = span();
    if(root->left->right != nullptr) mid = *root->left->right->bigSpan;
    span all = span();
    count(&all,root->left->s,&mid,root->s);
    lastOutcome = all.sum();
    return root;
}

int main()
{
    std::ios::sync_with_stdio(false);
    cin >> N >> M;
    CIN = N;
    ID = 1;
    H = ceil(log2(N+1));
    node *root = build(1);

    for(int i=0;i<M;i++){
        char c;
        int l, r;
        cin >> c >> l >> r;
        if(c == '?') {
            root = query(root, l, r);
            cout << lastOutcome << endl;
        } else {
            root = rotate(root, l, r);
        }
    }
    deleteTree(root);
    return 0;
}
