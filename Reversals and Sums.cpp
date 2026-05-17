#include <bits/stdc++.h>
#define ll long long
using namespace std;
 
const int nmax=2e5;
 
struct Treap {
 
    int n,root;
 
    struct Node{
        int pri;
        int val,subtree;
        ll sum;
        int flip;
        int l,r;
    }v[nmax+1];
 
    void init() {
        n=0;
        root=0;
        v[0]={0,0,0,0,0,0,0};
    }
 
    void get_subtree(int t) {
        if (t) {
            v[t].subtree=v[v[t].l].subtree+v[v[t].r].subtree+1;
            v[t].sum=v[v[t].l].sum+v[v[t].r].sum+v[t].val;
        }
    }
 
    void push(int t) {
       if (!v[t].flip)
           return;
        v[v[t].l].flip^=1;
        v[v[t].r].flip^=1;
        swap(v[t].l,v[t].r);
        v[t].flip=0;
    }
 
    void split(int t,int pos,int &l,int &r) {
        push(t);
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
        push(t);
        if (!t)
            t=n;
        else if (v[t].pri>v[n].pri) {
            split(t,pos-1,v[n].l,v[n].r);
            t=n;
        }else if (v[v[t].l].subtree+1>=pos)
            insert(v[t].l,elem,pos);
        else insert(v[t].r,elem,pos-v[v[t].l].subtree-1);
        get_subtree(t);
    }
 
    void insert(int pos,int val) {
        v[++n]={rand(),val,1,val,0,0,0};
        insert(root,n,pos);
    }
 
    void merge(int& t,int l,int r) {
        push(t);
        if (!l)
            t=r;
        else if (!r)
            t=l;
        else if (v[l].pri>v[r].pri) {
            push(l);
            merge(v[l].r,v[l].r,r);
            t=l;
        }
        else {
            push(r);
            merge(v[r].l,l,v[r].l);
            t=r;
        }
        get_subtree(t);
    }
 
    void reverse(int st,int dr) {
        int x,y,z;
        split(root,st-1,x,y);
        split(y,dr-st+1,y,z);
        v[y].flip^=1;
        merge(x,x,y);
        merge(root,x,z);
    }
 
    void show(int t) {
        push(t);
        if (v[t].l)
            show(v[t].l);
        cout<<v[t].val<<' ';
        if (v[t].r)
            show(v[t].r);
    }
 
    ll sum(int st,int dr) {
        int x,y,z;
        ll sum;
        split(root,st-1,x,y);
        split(y,dr-st+1,y,z);
        sum=v[y].sum;
        merge(x,x,y);
        merge(root,x,z);
        return sum;
    }
 
 
}treap;
 
 
int main(){
    treap.init();
    int n,q;
    cin>>n>>q;
    for (int i=1;i<=n;i++) {
        int x;
        cin>>x;
        treap.insert(i,x);
    }
    while (q--) {
        int st,dr,t;
        cin>>t>>st>>dr;
        if (t==2)
            cout<<treap.sum(st,dr)<<'\n';
        else treap.reverse(st,dr);
    }
}
