#include <bits/stdc++.h>
#include <sys/time.h>
#define ll long long
#define pii pair<int,int>

using namespace std;

ifstream fin("clasament.in");
ofstream fout("clasament.out");

const int nmax=3e5;

const int BASE=50067;
ll powers[nmax+1];

int last_update[nmax+1],last_key[nmax+1];

unordered_map<ll,int>vap;

struct Treap {

    int n,root;

    struct Node {
        int pri;
        pii key;
        ll hash,id,subtree;
        int l,r;
    }v[nmax+1];

    void init() {
        n=0;
        root=0;
        v[0]={0,{0,0},0,0,0,0,0};
    }

    void get_info(int t) {
        if (t) {
            v[t].hash=v[v[t].l].hash+v[t].id*powers[v[v[t].l].subtree]+v[v[t].r].hash*powers[v[v[t].l].subtree+1];
            v[t].subtree=v[v[t].l].subtree+v[v[t].r].subtree+1;
        }
    }

    void split(int t,pii key,int& l,int &r) {
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
        get_info(t);
    }

    void insert(int& t,int elem) {
        if (!t)
            t=elem;
        else if (v[elem].pri>v[t].pri) {
            split(t,v[elem].key,v[elem].l,v[elem].r);
            t=elem;
        }
        else if (v[elem].key<v[t].key)
            insert(v[t].l,elem);
        else insert(v[t].r,elem);
        get_info(t);
    }

    void insert(pii key,int id) {
        v[++n]={rand(),key,id,id,1,0,0};
        insert(root,n);
    }

    void merge(int& t,int l,int r) {
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
        get_info(l);
        get_info(r);
    }

    void erase(int& t,pii key) {
        if (v[t].key==key)
            merge(t,v[t].l,v[t].r);
        else if (key<v[t].key)
            erase(v[t].l,key);
        else erase(v[t].r,key);
        get_info(t);
    }

    ll get_hash() {
        return v[root].hash;
    }

}treap;

void precalc_powers() {
    powers[0]=1;
    for (int i=1;i<=nmax;i++)
        powers[i]=powers[i-1]*BASE;
}

int n,q;

void read(){
    fin>>n>>q;
    for (int i=1;i<=n;i++) {
        int x;
        fin>>x;
        x=-x;
        last_update[i]=i;
        last_key[i]=x;
        pii key={last_key[i],last_update[i]};
        treap.insert(key,i);
    }

}

void process_queries(){
    for (int i=n+1;i<=n+q;i++){
        int x,y;
        fin>>x>>y;
        y=-y;
        pii key={last_key[x],last_update[x]};
        last_update[x]=i;
        treap.erase(treap.root,key);
        last_update[x]=i;
        last_key[x]=y;
        key={last_key[x],last_update[x]};
        treap.insert(key,x);
        fout<<vap[treap.get_hash()]<<'\n';
        vap[treap.get_hash()]++;
    }
}

void randomize(){
    struct timeval time;
    gettimeofday(&time,NULL);
    int seed=time.tv_sec*1000000+time.tv_usec;
    srand(seed);
}

int main(){
    int n,q;
    treap.init();
    randomize();
    precalc_powers();
    read();
    vap[treap.get_hash()]++;
    process_queries();
    return 0;
}
