#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

#define re register
#define ll long long
#define bool char
#define N 100000

int n,m;
ll a[N+10];
struct node{
	int l,r;
	ll sum,lazy;
}tree[N*4+10];

inline void up(int t){tree[t].sum=tree[t<<1].sum+tree[t<<1|1].sum;}

inline void build(int t,int l,int r)
{
	tree[t].l=l;tree[t].r=r;tree[t].lazy=0;
	if(l==r){tree[t].sum=a[l];return;}
	int mid=l+r>>1;
	build(t<<1,l,mid);build(t<<1|1,mid+1,r);
	up(t);
}

inline void add(int t,ll val)
{
	tree[t].sum+=val*(tree[t].r-tree[t].l+1);tree[t].lazy+=val;
}

inline void down(int t)
{
	if(tree[t].lazy)
	{
		add(t<<1,tree[t].lazy);add(t<<1|1,tree[t].lazy);
		tree[t].lazy=0;
	}
}

inline void update(int t,int l,int r,ll val)
{
	if(tree[t].l>r||tree[t].r<l)return;
	if(l<=tree[t].l&&tree[t].r<=r)
	{
		add(t,val);
		return;
	}
	down(t);
	update(t<<1,l,r,val);update(t<<1|1,l,r,val);
	up(t);
}

inline ll query_sum(int t,int l,int r)
{
	if(tree[t].l>r||tree[t].r<l)return 0;
	if(l<=tree[t].l&&tree[t].r<=r)return tree[t].sum;
	down(t);
	return query_sum(t<<1,l,r)+query_sum(t<<1|1,l,r);
}

int main()
{
    freopen("1.in","r",stdin);
    freopen("std.out","w",stdout);
	scanf("%d%d",&n,&m);
	for(re int i=1;i<=n;++i)scanf("%lld",&a[i]);
	build(1,1,n);
	int opt,x,y;ll z;
	while(m--)
	{
		scanf("%d",&opt);
		if(opt==1)
		{
			scanf("%d%d%lld",&x,&y,&z);
			update(1,x,y,z);
		}
		else if(opt==2)
		{
			scanf("%d%d",&x,&y);
			printf("%lld\n",query_sum(1,x,y));
		}
	}
	return 0;
}