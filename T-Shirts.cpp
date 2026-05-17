#include <bits/stdc++.h>
#include <sys/time.h>
#define pii pair<int,int>
#define ll long long
using namespace std;

const int nmax=2e5;

pii tshirts[nmax+1];

bool tshirts_comp(pii a,pii b) {
    if (a.first!=b.first)
        return a.first>b.first;
    return a.second<b.second;
}

int answer[nmax+1];

struct Treap {

    struct Node {
        int pri,key;
        int answer,lazy_answer,lazy_key,index;
        int l,r;
    }v[nmax+1];

    int n,root;

    void apply(int t,int key_value,int answer_value) {
        v[t].answer+=answer_value;
        v[t].key+=key_value;
        v[t].lazy_answer+=answer_value;
        v[t].lazy_key+=key_value;
    }

    void push(int t) {
        if (!v[t].lazy_answer)
            return;
        if (v[t].l)
            apply(v[t].l,v[t].lazy_key,v[t].lazy_answer);
        if (v[t].r)
            apply(v[t].r,v[t].lazy_key,v[t].lazy_answer);
        v[t].lazy_answer=v[t].lazy_key=0;
    }

    Treap() {
        v[0]={0,0,0,0,0,0,0,0};
        n=0;
        root=0;
    }

    void split(int t,int key,int& l,int& r) {
        push(t);
        if (!t)
            l=r=0;
        else if (v[t].key<key) {
            split(v[t].r,key,v[t].r,r);
            l=t;
        }
        else {
            split(v[t].l,key,l,v[t].l);
            r=t;
        }
    }

    void merge(int& t,int l,int r) {
        push(l);
        push(r);
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
    }

    void insert(int& t,int elem) {
        push(t);
        if (!t)
            t=elem;
        else if (v[t].pri<v[elem].pri) {
            split(t,v[elem].key,v[elem].l,v[elem].r);
            t=elem;
        }
        else if (v[elem].key<=v[t].key)
            insert(v[t].l,elem);
        else insert(v[t].r,elem);
    }

    void insert(int val,int answer,int index,int& dest) {
        v[++n]={(int)rand()*50000+rand(),val,answer,0,0,index,0,0};
        insert(dest,n);
    }

    void absorb(int t,int& dest) {
        if (!t)
            return;
        push(t);
        if (v[t].l)
            absorb(v[t].l,dest);
        if (v[t].r)
            absorb(v[t].r,dest);
        v[t].l=v[t].r=0;
        insert(dest,t);
    }

    void buy(int price) {
        int l,r,poor,rich;
        split(root,price,l,r);
        if (r)
            apply(r,-price,1);
        split(r,price,poor,rich);

        absorb(poor,l);
        merge(root,l,rich);
    }

    void collect_answers(int t) {
        push(t);
        if (v[t].l)
            collect_answers(v[t].l);
        answer[v[t].index]=v[t].answer;
        if (v[t].r)
            collect_answers(v[t].r);
    }

}treap;

int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    struct timeval time;
    gettimeofday(&time,NULL);
    int seed=time.tv_sec*1000000+time.tv_usec;
    srand(seed);

    int t,n;
    cin>>t;
    for (int i=1;i<=t;i++)
        cin>>tshirts[i].second>>tshirts[i].first;
    sort(tshirts+1,tshirts+t+1,tshirts_comp);
    cin>>n;
    for (int i=1;i<=n;i++) {
        int x;
        cin>>x;
        treap.insert(x,0,i,treap.root);
    }

    for (int i=1;i<=t;i++)
        treap.buy(tshirts[i].second);

    treap.collect_answers(treap.root);

    for (int i=1;i<=n;i++)
        cout<<answer[i]<<' ';

    return 0;
}
