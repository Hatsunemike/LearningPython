#include <cstdio>
#include <cstdlib>
#include <time.h>

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    srand(unsigned(time(0)));
    FILE* output = fopen("1.in","w");
    int n=100000;
    fprintf(output, "%d %d\n", n, n);
    for(int i=1;i<=n;++i) {
        fprintf(output, "%d ", rand()*rand());
    }
    fputs("", output);
    int opt, l, r, x;
    for(int i=1;i<=n;++i) {
        opt = rand()%2+1;
        l = rand()%n+1;
        r = rand()%n+1;
        if(l>r)swap(l,r);
        if(opt == 1) {
            fprintf(output, "%d %d %d %d\n", opt, l, r, rand());
        }
        else if(opt == 2) {
            fprintf(output, "%d %d %d\n", opt, l, r);
        }
    }
    return 0;
}