#include <math.h>
#include <iostream>
#include <ctime>
using std::cout;
using std::endl;

typedef long long int ll;

void dfs(int now, int rest_step, ll& cnt) {
    if(!rest_step) {
        if(!now)++cnt;
        return;
    }
    dfs((now+1)%3, rest_step-1, cnt);
    dfs((now+2)%3, rest_step-1, cnt);
}

int main()
{
    double start = clock();
    for(int i=0;i<20;++i) {
        ll cnt=0;
        dfs(0, i, cnt);
        printf("%.10f\n", cnt*(pow(2, -i)));
    }
    double end = clock();
    printf("runtime: %.3f\n", (end-start)/1000);
    return 0;
}