#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

const int nmax=1e6;

ifstream fin("sortare.in");
ofstream fout("sortare.out");

struct Treap {

    struct Nod {
        int key,pri,l,r;
    }v[nmax+1];

    int root,n;

    void init() {
        root=0;
        v[0]={0,0,0,0};
    }

    void split(int t,int key,int& l,int& r) {
        if (!t)
            l=0,r=0;
        else if (v[t].key<key) {
            split(v[t].r,key,v[t].r,r);
            l=t;
        }
        else {
            split(v[t].l,key,l,v[t].l);
            r=t;
        }
    }

    void insert(int &t,int elem) {
        if (!t)
            t=elem;
        else if (v[t].pri<v[elem].pri) {
            split(t,v[elem].key,v[elem].l,v[elem].r);
            t=elem;
        }
        else if (v[elem].key<v[t].key)
            insert(v[t].l,elem);
        else insert(v[t].r,elem);
    }

    void insert(int key) {
        v[++n]={key,rand(),0,0};
        insert(root,n);
    }

    void show(int t) {
        if (v[t].l)
            show(v[t].l);
        fout<<v[t].key<<' ';
        if (v[t].r)
            show(v[t].r);
    }
}treap;

int main() {
    struct timeval time;
    gettimeofday(&time,NULL);
    int seed=time.tv_sec*1000000+time.tv_usec;
    srand(seed);

    int n;
    fin>>n;
    for (int i=1;i<=n;i++) {
        int x;
        fin>>x;
        treap.insert(x);
    }
    treap.show(treap.root);
    return 0;
}
