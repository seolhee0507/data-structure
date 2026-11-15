
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef struct { int type, value; } Operation; /* 0=insert, 1=delete */
typedef struct { int *a; int n, cap; unsigned long long cmp, moves; size_t mem; } Array;
typedef struct Node {
    int key, count, height;
    struct Node *left, *right;
} Node;
typedef struct { Node *root; unsigned long long cmp, moves; size_t mem; } AVL;

static size_t peak_array_mem, peak_avl_mem;
static size_t quick_stack_peak;
static int quick_depth;

static double now_ms(void) {
    return (double)clock() * 1000.0 / CLOCKS_PER_SEC;
}
static int max2(int a,int b){return a>b?a:b;}
static int height(Node *p){return p?p->height:0;}
static void fix(Node *p){p->height=1+max2(height(p->left),height(p->right));}
static int balance(Node *p){return height(p->left)-height(p->right);}
static Node *rotate_right(Node *y){
    Node *x=y->left, *t=x->right; x->right=y; y->left=t; fix(y); fix(x); return x;
}
static Node *rotate_left(Node *x){
    Node *y=x->right, *t=y->left; y->left=x; x->right=t; fix(x); fix(y); return y;
}
static Node *avl_insert(Node *p,int key,AVL *s){
    if(!p){
        Node *q=(Node*)malloc(sizeof(Node)); if(!q){perror("malloc");exit(1);}
        q->key=key; q->count=1; q->height=1; q->left=q->right=NULL;
        s->mem+=sizeof(Node); if(s->mem>peak_avl_mem)peak_avl_mem=s->mem;
        s->moves++; return q;
    }
    s->cmp++;
    if(key<p->key) p->left=avl_insert(p->left,key,s);
    else {
        s->cmp++;
        if(key>p->key) p->right=avl_insert(p->right,key,s);
        else { p->count++; return p; }
    }
    fix(p);
    if(balance(p)>1){s->cmp++; if(key<p->left->key) return rotate_right(p);}
    if(balance(p)<-1){s->cmp++; if(key>p->right->key) return rotate_left(p);}
    if(balance(p)>1){s->cmp++; if(key>p->left->key){p->left=rotate_left(p->left);return rotate_right(p);}}
    if(balance(p)<-1){s->cmp++; if(key<p->right->key){p->right=rotate_right(p->right);return rotate_left(p);}}
    return p;
}
static Node *min_node(Node *p){while(p->left)p=p->left;return p;}
static Node *avl_delete(Node *p,int key,AVL *s,int *deleted){
    if(!p)return NULL;
    s->cmp++;
    if(key<p->key) p->left=avl_delete(p->left,key,s,deleted);
    else {
        s->cmp++;
        if(key>p->key) p->right=avl_delete(p->right,key,s,deleted);
        else {
            *deleted=1;
            if(p->count>1){p->count--;return p;}
            if(!p->left || !p->right){
                Node *child=p->left?p->left:p->right;
                free(p); s->mem-=sizeof(Node);
                return child;
            }
            Node *succ=min_node(p->right);
            p->key=succ->key; p->count=succ->count; s->moves++;
            /* remove the successor node completely, including its multiplicity */
            int sk=succ->key;
            /* helper below removes the whole successor node */
            extern Node *delete_whole(Node*,int,AVL*);
            p->right=delete_whole(p->right,sk,s);
        }
    }
    if(!p)return NULL;
    fix(p);
    if(balance(p)>1 && balance(p->left)>=0)return rotate_right(p);
    if(balance(p)>1 && balance(p->left)<0){p->left=rotate_left(p->left);return rotate_right(p);}
    if(balance(p)<-1 && balance(p->right)<=0)return rotate_left(p);
    if(balance(p)<-1 && balance(p->right)>0){p->right=rotate_right(p->right);return rotate_left(p);}
    return p;
}
Node *delete_whole(Node *p,int key,AVL *s){
    if(!p)return NULL;
    s->cmp++;
    if(key<p->key)p->left=delete_whole(p->left,key,s);
    else {s->cmp++; if(key>p->key)p->right=delete_whole(p->right,key,s); else {
        if(!p->left||!p->right){Node *c=p->left?p->left:p->right;free(p);s->mem-=sizeof(Node);return c;}
        Node *q=min_node(p->right); p->key=q->key;p->count=q->count;s->moves++;
        p->right=delete_whole(p->right,q->key,s);
    }}
    fix(p);
    if(balance(p)>1 && balance(p->left)>=0)return rotate_right(p);
    if(balance(p)>1 && balance(p->left)<0){p->left=rotate_left(p->left);return rotate_right(p);}
    if(balance(p)<-1 && balance(p->right)<=0)return rotate_left(p);
    if(balance(p)<-1 && balance(p->right)>0){p->right=rotate_right(p->right);return rotate_left(p);}
    return p;
}
static void free_tree(Node *p){if(p){free_tree(p->left);free_tree(p->right);free(p);}}
static void inorder(Node *p,int *out,int *idx){if(p){inorder(p->left,out,idx);for(int i=0;i<p->count;i++)out[(*idx)++]=p->key;inorder(p->right,out,idx);}}
static int tree_height(Node *p){return p?1+max2(tree_height(p->left),tree_height(p->right)):0;}

static void quick_sort(int *a,int lo,int hi,Array *s){
    if(lo>=hi)return;
    quick_depth++;
    size_t stack=(size_t)quick_depth*64; /* approximate active recursive frame */
    if(stack>quick_stack_peak)quick_stack_peak=stack;
    int mid=lo+(hi-lo)/2;
    /* median-of-three pivot selection; count only data-value comparisons */
    int x=a[lo],y=a[mid],z=a[hi],pivot;
    s->cmp++;
    if(x<y){s->cmp++; if(y<z)pivot=y;else {s->cmp++;pivot=(x<z)?z:x;}}
    else {s->cmp++; if(x<z)pivot=x;else {s->cmp++;pivot=(y<z)?z:y;}}
    int i=lo,j=hi;
    while(i<=j){
        while(1){s->cmp++; if(!(a[i]<pivot))break; i++;}
        while(1){s->cmp++; if(!(a[j]>pivot))break; j--;}
        if(i<=j){
            if(i!=j){int t=a[i];a[i]=a[j];a[j]=t;s->moves+=3;}
            i++;j--;
        }
    }
    if(lo<j)quick_sort(a,lo,j,s);
    if(i<hi)quick_sort(a,i,hi,s);
    quick_depth--;
}
static void arr_insert(Array *s,int v){
    int pos=0;
    while(pos<s->n){s->cmp++; if(s->a[pos]>=v)break; pos++;}
    if(s->n==s->cap){int nc=s->cap*2;int *p=realloc(s->a,(size_t)nc*sizeof(int));if(!p){perror("realloc");exit(1);}s->a=p;s->cap=nc;s->mem=(size_t)s->cap*sizeof(int);}
    for(int i=s->n;i>pos;i--){s->a[i]=s->a[i-1];s->moves++;}
    s->a[pos]=v;s->moves++;s->n++;
}
static void arr_delete(Array *s,int v){
    int pos=0;
    while(pos<s->n){s->cmp++;if(s->a[pos]==v)break;pos++;}
    if(pos==s->n)return;
    for(int i=pos;i<s->n-1;i++){s->a[i]=s->a[i+1];s->moves++;}
    s->n--;
}

int main(void){
    const int sizes[]={1000,2000,4000,8000,16000,20000};
    const int ns=6;
    srand(20261115);
    for(int si=0;si<ns;si++){
        int n=sizes[si], opsn=n/5;
        int *original=(int*)malloc((size_t)n*sizeof(int));
        Operation *ops=(Operation*)malloc((size_t)opsn*sizeof(Operation));
        if(!original||!ops){perror("malloc");return 1;}
        for(int i=0;i<n;i++)original[i]=rand()%100001-50000;
        for(int i=0;i<opsn;i++){ops[i].type=rand()%2;ops[i].value=rand()%100001-50000;}
        Array ar={0}; ar.cap=n+opsn+1; ar.n=n; ar.a=(int*)malloc((size_t)ar.cap*sizeof(int));
        memcpy(ar.a,original,(size_t)n*sizeof(int)); ar.mem=(size_t)ar.cap*sizeof(int);
        peak_array_mem=ar.mem+(size_t)opsn*sizeof(Operation); quick_stack_peak=0; quick_depth=0;
        double at=now_ms(); quick_sort(ar.a,0,n-1,&ar);
        for(int i=0;i<opsn;i++){if(ops[i].type==0)arr_insert(&ar,ops[i].value);else arr_delete(&ar,ops[i].value);}
        double ams=now_ms()-at;
        size_t array_memory=ar.mem+(size_t)opsn*sizeof(Operation)+quick_stack_peak;

        AVL av={0}; av.mem=0; peak_avl_mem=(size_t)opsn*sizeof(Operation);
        double vt=now_ms();
        for(int i=0;i<n;i++)av.root=avl_insert(av.root,original[i],&av);
        for(int i=0;i<opsn;i++){
            if(ops[i].type==0)av.root=avl_insert(av.root,ops[i].value,&av);
            else {int deleted=0;av.root=avl_delete(av.root,ops[i].value,&av,&deleted);}
        }
        double vms=now_ms()-vt;
        size_t avl_memory=peak_avl_mem+(size_t)opsn*sizeof(Operation)+(size_t)tree_height(av.root)*64;

        int *check=(int*)malloc((size_t)(n+opsn+1)*sizeof(int));
        int idx=0;inorder(av.root,check,&idx);
        int equal=(idx==ar.n);
        if(equal)for(int i=0;i<idx;i++)if(check[i]!=ar.a[i]){equal=0;break;}
        printf("Data Size : %d  Operations: %d\n",n,opsn);
        printf("%-20s %-16s %-16s\n","Metric","Quick Sort","AVL Tree");
        printf("%-20s %-16.3f %-16.3f\n","Total Time (ms)",ams,vms);
        printf("%-20s %-16llu %-16llu\n","Comparisons",ar.cmp,av.cmp);
        printf("%-20s %-16llu %-16llu\n","Data Movements",ar.moves,av.moves);
        printf("%-20s %-16zu %-16zu\n","Memory Usage (B)",array_memory,avl_memory);
        printf("%-20s %-16d %-16d\n","Final Data Count",ar.n,idx);
        printf("Final Data Match: %s\n\n",equal?"PASS":"FAIL");
        free(check);free_tree(av.root);free(ar.a);free(original);free(ops);
    }
    return 0;
}
