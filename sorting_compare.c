#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZES 6
static const int sizes[SIZES]={1000,2000,4000,8000,16000,20000};
typedef unsigned long long U64;
typedef struct { U64 comparisons; int max_depth; } Stats;
typedef struct { int left,right,mid,depth; int *a,*tmp; } MergeFrame;
typedef struct { int low,high,pivot,depth; int *a; } QuickFrame;
typedef enum { FIRST,RANDOM,MEDIAN3 } Pivot;
static double millis(void){struct timespec t; timespec_get(&t,TIME_UTC); return t.tv_sec*1000.0+t.tv_nsec/1000000.0;}
static int *random_data(int n){int *a=malloc((size_t)n*sizeof *a); if(!a){perror("malloc");exit(1);} for(int i=0;i<n;i++)a[i]=rand()%100001;return a;}
static int sorted(const int*a,int n){for(int i=1;i<n;i++)if(a[i-1]>a[i])return 0;return 1;}
static void merge(int*a,int*t,int l,int m,int r,Stats*s){int i=l,j=m+1,k=l;while(i<=m&&j<=r){s->comparisons++;if(a[i]<=a[j])t[k++]=a[i++];else t[k++]=a[j++];}while(i<=m)t[k++]=a[i++];while(j<=r)t[k++]=a[j++];for(i=l;i<=r;i++)a[i]=t[i];}
static void mrec(int*a,int*t,int l,int r,int d,Stats*s){if(d>s->max_depth)s->max_depth=d;if(l>=r)return;int m=l+(r-l)/2;mrec(a,t,l,m,d+1,s);mrec(a,t,m+1,r,d+1,s);merge(a,t,l,m,r,s);}
static void merge_sort(int*a,int n,Stats*s){int*t=malloc((size_t)n*sizeof *t);if(!t){perror("merge temp");exit(1);}mrec(a,t,0,n-1,1,s);free(t);}
static void swap(int*x,int*y){int t=*x;*x=*y;*y=t;}
static int median3(int*a,int x,int y,int z,Stats*s){int A=a[x],B=a[y],C=a[z];s->comparisons++;if(A<B){s->comparisons++;if(B<C)return y;s->comparisons++;return A<C?z:x;}s->comparisons++;if(A<C)return x;s->comparisons++;return B<C?z:y;}
static int partition(int*a,int l,int h,Stats*s){int p=a[h],i=l-1;for(int j=l;j<h;j++){s->comparisons++;if(a[j]<=p){i++;swap(&a[i],&a[j]);}}swap(&a[i+1],&a[h]);return i+1;}
static void qrec(int*a,int l,int h,Pivot type,int d,Stats*s){if(d>s->max_depth)s->max_depth=d;if(l>=h)return;int pi;if(type==FIRST)pi=l;else if(type==RANDOM)pi=l+rand()%(h-l+1);else pi=median3(a,l,l+(h-l)/2,h,s);swap(&a[pi],&a[h]);int p=partition(a,l,h,s);qrec(a,l,p-1,type,d+1,s);qrec(a,p+1,h,type,d+1,s);}
static void quick_sort(int*a,int n,Pivot p,Stats*s){qrec(a,0,n-1,p,1,s);}
static size_t merge_bytes(int n,Stats*s){return (size_t)n*sizeof(int)+(size_t)s->max_depth*sizeof(MergeFrame);}
static size_t quick_bytes(Stats*s){return (size_t)s->max_depth*sizeof(QuickFrame);}
static void output(const char*name,Stats*s,double ms,size_t mem,int ok){printf("%-16s %-15llu %-12.3f %-16zu %s\n",name,s->comparisons,ms,mem,ok?"OK":"FAIL");}
int main(void){srand((unsigned)time(NULL));for(int x=0;x<SIZES;x++){int n=sizes[x];int*src=random_data(n),*work=malloc((size_t)n*sizeof *work);if(!work){perror("work");free(src);return 1;}printf("\nData Size : %d\n",n);printf("%-16s %-15s %-12s %-16s %s\n","Algorithm","Comparisons","Time(ms)","Memory(Bytes)","Check");puts("--------------------------------------------------------------------------");Stats s={0,0};memcpy(work,src,(size_t)n*sizeof *work);double t=millis();merge_sort(work,n,&s);double dt=millis()-t;output("Merge Sort",&s,dt,merge_bytes(n,&s),sorted(work,n));Pivot p[3]={FIRST,RANDOM,MEDIAN3};const char*names[3]={"Quick (First)","Quick (Random)","Quick (Median3)"};for(int k=0;k<3;k++){Stats q={0,0};memcpy(work,src,(size_t)n*sizeof *work);t=millis();quick_sort(work,n,p[k],&q);dt=millis()-t;output(names[k],&q,dt,quick_bytes(&q),sorted(work,n));}free(work);free(src);}return 0;}
