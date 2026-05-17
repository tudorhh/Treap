#include <bits/stdc++.h>
#include <sys/time.h>
#define ll long long

using namespace std;

const int nmax=1e5;

struct Treap{
    int root,n;
    struct Node{
        int pri;
        ll val,subtree,sum;
        ll lazy_add,lazy_set;
        int flip;
        int l,r;
    }v[3*nmax+1];

    void apply_set(int t,ll val){
        v[t].lazy_add=0;
        v[t].lazy_set=val;
        v[t].val=val;
        v[t].sum=v[t].subtree*val;
    }

    void apply_add(int t,ll val){
        v[t].lazy_add+=val;
        v[t].val+=val;
        v[t].sum+=v[t].subtree*val;
    }

    void push(int t){
        if(v[t].flip){
            swap(v[t].l,v[t].r);
            v[v[t].l].flip^=1;
            v[v[t].r].flip^=1;
            v[t].flip=0;
        }
        if(v[t].lazy_set!=0){
            apply_set(v[t].l,v[t].lazy_set);
            apply_set(v[t].r,v[t].lazy_set);
            v[t].lazy_set=0;
        }
        if(v[t].lazy_add!=0){
            apply_add(v[t].l,v[t].lazy_add);
            apply_add(v[t].r,v[t].lazy_add);
            v[t].lazy_add=0;
        }
    }

    void get_info(int t){
        if(t){
            v[t].sum=v[v[t].l].sum+v[v[t].r].sum+v[t].val;
            v[t].subtree=v[v[t].l].subtree+v[v[t].r].subtree+1;
        }
    }

    void init(){
        n=0;
        v[0]={0,0,0,0,0,0,0,0};
    }

    void split(int t,int pos,int& l,int& r){
        push(t);
        if(!t)
            l=0,r=0;
        else if(pos>=v[v[t].l].subtree+1){
            split(v[t].r,pos-v[v[t].l].subtree-1,v[t].r,r);
            l=t;
        }
        else{
            split(v[t].l,pos,l,v[t].l);
            r=t;
        }
        get_info(t);
    }

    void insert(int& t,int elem,int pos){
        push(t);
        if(!t)
            t=elem;
        else if(v[elem].pri>v[t].pri){
            split(t,pos-1,v[elem].l,v[elem].r);
            t=elem;
        }
        else if(pos<=v[v[t].l].subtree+1)
            insert(v[t].l,elem,pos);
        else insert(v[t].r,elem,pos-v[v[t].l].subtree-1);
        get_info(t);
    }

    void merge(int& t,int l,int r){
        push(l);
        push(r);
        if(!r)
            t=l;
        else if(!l)
            t=r;
        else if(v[l].pri>v[r].pri){
            merge(v[l].r,v[l].r,r);
            t=l;
        }
        else{
            merge(v[r].l,l,v[r].l);
            t=r;
        }
        get_info(t);
    }

    void erase(int& t,int pos){
        push(t);
        if(pos==v[v[t].l].subtree+1)
            merge(t,v[t].l,v[t].r);
        else if(pos<v[v[t].l].subtree+1)
            erase(v[t].l,pos);
        else erase(v[t].r,pos-v[v[t].l].subtree-1);
        get_info(t);
    }

    void insert(int val,int pos){
        v[++n]={rand(),val,1,val,0,0,0,0};
        insert(root,n,pos);
    }

    void interval_split(int& x,int& y,int& z,int st,int dr){
        split(root,st-1,x,y);
        split(y,dr-st+1,y,z);
    }

    void interval_merge(int x,int y,int z){
        merge(x,x,y);
        merge(root,x,z);
    }

    void lazy_add(int st,int dr,int val){
        int x,y,z;
        interval_split(x,y,z,st,dr);
        v[y].lazy_add+=val;
        v[y].sum+=v[y].subtree*val;
        v[y].val+=val;
        interval_merge(x,y,z);
    }

    void lazy_set(int st,int dr,int val){
        int x,y,z;
        interval_split(x,y,z,st,dr);
        v[y].lazy_set=val;
        v[y].val=val;
        v[y].lazy_add=0;
        v[y].sum=v[y].subtree*val;
        interval_merge(x,y,z);
    }

    void reverse(int st,int dr){
        int x,y,z;
        interval_split(x,y,z,st,dr);
        v[y].flip^=1;
        interval_merge(x,y,z);
    }

    ll sum(int st,int dr){
        int x,y,z;
        interval_split(x,y,z,st,dr);
        ll sum=v[y].sum;
        interval_merge(x,y,z);
        return sum;
    }

    void show(int t){
        push(t);
        if(v[t].l)
            show(v[t].l);
        cout<<v[t].val<<' ';
        if(v[t].r)
            show(v[t].r);
    }
}treap;

int main()
{
    struct timeval time;
    gettimeofday(&time,NULL);
    int seed=time.tv_sec*1000000+time.tv_usec;
    srand(seed);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    treap.init();
    int n,q;
    cin>>n>>q;
    for(int i=1;i<=n;i++){
        int x;
        cin>>x;
        treap.insert(x,i);
    }
    while(q--){
        char type;
        int st,dr,val;
        cin>>type;
        if(type=='Q'){
            cin>>st>>dr;
            cout<<treap.sum(st,dr)<<'\n';
        }
        else if(type=='+'){
            cin>>st>>dr>>val;
            treap.lazy_add(st,dr,val);
        }
        else if(type=='='){
            cin>>st>>dr>>val;
            treap.lazy_set(st,dr,val);
        }
        else if(type=='D'){
            cin>>st;
            treap.erase(treap.root,st);
        }
        else if(type=='I'){
            cin>>st>>val;
            treap.insert(val,st);
        }
        else if(type=='R'){
            cin>>st>>dr;
            treap.reverse(st,dr);
        }
    }
    treap.show(treap.root);
    return 0;
}
