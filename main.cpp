#include <bits/stdc++.h>

using namespace std;

ifstream fin("sortare.in");
ofstream fout("sortare.out");

///treap implicit

const int nmax=1e6;

struct Treap {

    struct Nod {
        int pri;
        int val,subtree;
        int l,r;
    }v[nmax+1];

    int n,root;

    void init() {
        n=0;
        root=0;
        v[0]={0,0,0,0,0};
    }

    void get_subtree(int t) {
        if (t)
            v[t].subtree=v[v[t].l].subtree+v[v[t].r].subtree+1;
    }

    void split(int t,int pos,int& l,int& r) {
        if (!t)
            l=0,r=0;
        else if (v[v[t].l].subtree+1<=pos) {
            split(v[t].r,pos-v[v[t].l].subtree-1,v[t].r,r);
            l=t;
        }
        else {
            split(v[t].l,pos,l,v[t].l);
            r=t;
        }
        get_subtree(t);
    }

    void insert(int &t,int elem,int pos) {
        if (!t)
            t=n;
        else if (v[t].pri>v[elem].pri) {
            split(t,pos-1,v[elem].l,v[elem].r);
            t=elem;
        } else if (v[v[t].l].subtree+1>=pos)
            insert(v[t].l,elem,pos);
        else insert(v[t].r,elem,pos-v[v[t].l].subtree-1);
        get_subtree(t);
    }

    void insert(int val,int pos) {
        v[++n]={rand(),val,1,0,0};
        insert(root,n,pos);
    }

    void merge(int&t,int l,int r) {
        if (!l)
            t=r;
        else if (!r)
            t=l;
        else if (v[l].pri>v[r].pri) {
            merge(v[l].r,v[l].r,r);
            t=l;
        }
        else {
            merge(v[r].l,l,v[r].l);
            t=r;
        }
        get_subtree(t);
    }

    void erase(int &t,int pos) {
        if (v[v[t].l].subtree+1==pos)
            merge(t,v[t].l,v[t].r);
        else if (pos<=v[v[t].l].subtree+1)
                erase(v[t].l,pos);
        else erase(v[t].r,pos-v[v[t].l].subtree-1);
        get_subtree(t);
    }

    void show(int t) {
        if (v[t].l)
            show(v[t].l);
        if (v[t].r)
            show(v[t].r);
    }

    void update(int st,int dr) {
        int x=0,y=0,z=0;
        split(root,st-1,x,y);
        split(y,dr-st+1,y,z);
        merge(x,x,z);
        merge(root,x,y);
    }


}treap;


int main(){
    int n,q;
    cin>>n>>q;
    treap.init();
    string s;
    cin>>s;
    for (int i=0;i<n;i++)
        treap.insert(s[i],i+1);
    while (q--) {
        int st,dr;
        cin>>st>>dr;
        treap.update(st,dr);
    }
    treap.show(treap.root);
    return 0;
}
