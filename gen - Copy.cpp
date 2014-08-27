#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<algorithm>
using namespace std;

#define ii int

int kas=1,n=500,m=2;

ii theta[10];

int main(){

    ii i,j,k,l,test,t=1,i1;

    freopen("in.txt","w",stdout);

    srand(time(0));

    //printf("%d\n",kas);

    theta[0]=1;
    theta[1]=23;
    theta[2]=41;
    theta[3]=2;
    theta[4]=4;

    ii v,v1;
    ii last=0;
    for(t=1;t<=kas;t++){
        printf("%d %d\n",n,m);
        for(i=1;i<=n;i++){
            for(j=1;j<=m;j++){
                printf("%d ",i);
            }
            k=rand()%500;
            l=rand()%3;
            if(!l)l=-1;
            ii val=last+(k*l);
            if(val<0) val=1;
            printf("%d\n",val);
            last=val;
        }
        puts("");

        continue;
    }

    return 0;
}
