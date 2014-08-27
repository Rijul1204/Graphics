#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<algorithm>
using namespace std;

#define ii int

int kas=1,n=50,m=2;

ii theta[10],x[10];

int main(){

    ii i,j,k,l,test,t=1,i1;

    freopen("in.txt","w",stdout);

    srand(time(0));

    //printf("%d\n",kas);

    theta[0]=1;
    theta[1]=1.0;
    theta[2]=41;
    theta[3]=2;
    theta[4]=4;
    theta[5]=5;

    ii v,v1;
    ii last=0;
    for(t=1;t<=kas;t++){
        printf("%d %d\n",n,m);
        for(i=1;i<=n;i++){
            ii val=theta[0];
            for(j=1;j<=m;j++){
                x[j]=rand()%1000;
                printf("%d ",x[j]);
                val+=(x[j]*theta[j]);
                val+=rand()%100;
            }
            //val/=scale;
            printf("%d\n",val);
        }
        puts("");

        continue;
    }

    return 0;
}
