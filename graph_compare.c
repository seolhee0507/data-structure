#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 100
#define SE 100
#define DE 4000

typedef struct Node { int v; struct Node *next; } Node;
typedef struct { int a[V][V]; } Matrix;
typedef struct { Node *head[V]; size_t nodes; } List;
typedef struct { int u,v; } Edge;
typedef struct { long long ins,del,conn,adj; size_t mem; } Result;

static int has_edge(const Edge e[], int n, int u, int v) {
    for(int i=0;i<n;i++)
        if((e[i].u==u && e[i].v==v)||(e[i].u==v && e[i].v==u)) return 1;
    return 0;
}
static void generate(Edge e[], int n) {
    int k=0;
    while(k<n) {
        int u=rand()%V, v=rand()%V, t;
        if(u==v) continue;
        if(u>v){t=u;u=v;v=t;}
        if(!has_edge(e,k,u,v)){e[k].u=u;e[k].v=v;k++;}
    }
}
static void matrix_build(Matrix *g,const Edge e[],int n) {
    for(int i=0;i<V;i++) for(int j=0;j<V;j++) g->a[i][j]=0;
    for(int i=0;i<n;i++) g->a[e[i].u][e[i].v]=g->a[e[i].v][e[i].u]=1;
}
static void list_init(List *g) {
    g->nodes=0; for(int i=0;i<V;i++) g->head[i]=NULL;
}
static void list_build(List *g,const Edge e[],int n) {
    list_init(g);
    for(int i=0;i<n;i++) {
        Node *a=malloc(sizeof(Node)),*b=malloc(sizeof(Node));
        if(!a||!b){fprintf(stderr,"malloc failed\n");exit(1);}
        a->v=e[i].v;a->next=g->head[e[i].u];g->head[e[i].u]=a;
        b->v=e[i].u;b->next=g->head[e[i].v];g->head[e[i].v]=b;
        g->nodes+=2;
    }
}
static long long contains(List *g,int u,int v) {
    long long c=0; Node *p=g->head[u];
    while(p){c++;if(p->v==v)break;p=p->next;}
    return c;
}
static int list_has(List *g,int u,int v,long long *c) {
    Node *p=g->head[u];
    while(p){(*c)++;if(p->v==v)return 1;p=p->next;}
    return 0;
}
static void list_insert_one(List *g,int u,int v) {
    Node *p=malloc(sizeof(Node)); if(!p){fprintf(stderr,"malloc failed\n");exit(1);}
    p->v=v;p->next=g->head[u];g->head[u]=p;g->nodes++;
}
static void list_insert(List *g,int u,int v) {list_insert_one(g,u,v);list_insert_one(g,v,u);}
static int list_delete_one(List *g,int u,int v,long long *c) {
    Node *p=g->head[u],*prev=NULL;
    while(p){(*c)++;if(p->v==v){if(prev)prev->next=p->next;else g->head[u]=p->next;free(p);g->nodes--;return 1;}prev=p;p=p->next;}
    return 0;
}
static void list_delete(List *g,int u,int v,long long *c) {list_delete_one(g,u,v,c);list_delete_one(g,v,u,c);}
static void free_list(List *g) {
    for(int i=0;i<V;i++){Node*p=g->head[i];while(p){Node*n=p->next;free(p);p=n;}g->head[i]=NULL;}
}
static void choose(const Edge e[],int n,int *iu,int *iv,int *du,int *dv,int *cu,int *cv,int *q) {
    int u,v,t;
    do{u=rand()%V;v=rand()%V;if(u>v){t=u;u=v;v=t;}}while(u==v||has_edge(e,n,u,v));
    *iu=u;*iv=v;
    int k=rand()%n;*du=e[k].u;*dv=e[k].v;
    k=rand()%n;*cu=e[k].u;*cv=e[k].v;
    *q=rand()%V;
}
static void print_adj_m(Matrix*g,int u,long long*c) {
    printf("Adjacent vertices of %d: ",u);int first=1;
    for(int v=0;v<V;v++){(*c)++;if(g->a[u][v]){printf("%s%d",first?"":", ",v);first=0;}}
    puts(first?"(none)":"");
}
static void print_adj_l(List*g,int u,long long*c) {
    printf("Adjacent vertices of %d: ",u);int first=1;
    for(Node*p=g->head[u];p;p=p->next){(*c)++;printf("%s%d",first?"":", ",p->v);first=0;}
    puts(first?"(none)":"");
}
static void show(const char *name,Result r) {
    printf("\n%s\nMemory                         : %zu Bytes\nEdge insertion comparisons     : %lld\nEdge deletion comparisons      : %lld\nConnectivity check comparisons : %lld\nAdjacent vertices comparisons  : %lld\n",
           name,r.mem,r.ins,r.del,r.conn,r.adj);
}
static void run_matrix(const char*name,const Edge e[],int n,int iu,int iv,int du,int dv,int cu,int cv,int q) {
    Matrix g;matrix_build(&g,e,n);Result r={0,0,0,0,sizeof(g.a)};
    r.ins++;if(!g.a[iu][iv])g.a[iu][iv]=g.a[iv][iu]=1;
    r.del++;if(g.a[du][dv])g.a[du][dv]=g.a[dv][du]=0;
    r.conn++;printf("Connectivity (%d,%d): %s\n",cu,cv,g.a[cu][cv]?"connected":"not connected");
    print_adj_m(&g,q,&r.adj);show(name,r);
}
static void run_list(const char*name,const Edge e[],int n,int iu,int iv,int du,int dv,int cu,int cv,int q) {
    List g;list_build(&g,e,n);Result r={0,0,0,0,0};
    (void)list_has(&g,iu,iv,&r.ins);list_insert(&g,iu,iv);
    list_delete(&g,du,dv,&r.del);
    r.conn=contains(&g,cu,cv);printf("Connectivity (%d,%d): %s\n",cu,cv,list_has(&g,cu,cv,&(long long){0})?"connected":"not connected");
    print_adj_l(&g,q,&r.adj);
    r.mem=sizeof(g.head)+sizeof(g.nodes)+g.nodes*sizeof(Node);
    show(name,r);free_list(&g);
}
int main(int argc,char**argv) {
    unsigned seed=argc>1?(unsigned)strtoul(argv[1],NULL,10):(unsigned)time(NULL);srand(seed);
    Edge sparse[SE],dense[DE];generate(sparse,SE);generate(dense,DE);
    int siu,siv,sdu,sdv,scu,scv,sq,diu,div,ddu,ddv,dcu,dcv,dq;
    choose(sparse,SE,&siu,&siv,&sdu,&sdv,&scu,&scv,&sq);
    choose(dense,DE,&diu,&div,&ddu,&ddv,&dcu,&dcv,&dq);
    printf("Seed: %u | Vertices: %d | Sparse edges: %d | Dense edges: %d\n",seed,V,SE,DE);
    printf("Sparse tests: insert=(%d,%d), delete=(%d,%d), check=(%d,%d), vertex=%d\n",siu,siv,sdu,sdv,scu,scv,sq);
    printf("Dense tests : insert=(%d,%d), delete=(%d,%d), check=(%d,%d), vertex=%d\n",diu,div,ddu,ddv,dcu,dcv,dq);
    run_matrix("Case 1: Sparse Graph - Adjacency Matrix",sparse,SE,siu,siv,sdu,sdv,scu,scv,sq);
    run_list("Case 2: Sparse Graph - Adjacency List",sparse,SE,siu,siv,sdu,sdv,scu,scv,sq);
    run_matrix("Case 3: Dense Graph - Adjacency Matrix",dense,DE,diu,div,ddu,ddv,dcu,dcv,dq);
    run_list("Case 4: Dense Graph - Adjacency List",dense,DE,diu,div,ddu,ddv,dcu,dcv,dq);
    puts("\nMemory excludes malloc allocator metadata and temporary generation arrays.");
    return 0;
}
