#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char a[MAX];
    int top;
} CStack;

typedef struct {
    int a[MAX];
    int top;
} IStack;

void cp(CStack *s,char x){s->a[++s->top]=x;}
char co(CStack *s){return s->a[s->top--];}
void ip(IStack *s,int x){s->a[++s->top]=x;}
int io(IStack *s){return s->a[s->top--];}

int main(){
    char s[MAX], child[MAX];
    CStack ns={.top=-1};
    IStack ds={.top=-1}, cs={.top=-1};

    int i,n=0,leaf=0,nonleaf=0,maxdeg=0,depth=0,maxdepth=0;
    char parentC=0;
    int cchild=0, balance=0, valid=1;

    scanf("%99s",s);

    /* 입력 검사 */
    if(s[0]!='A') valid=0;

    for(i=0;s[i]&&valid;i++){
        char x=s[i];

        if(isupper(x)){
            if(i>0 && isupper(s[i-1])) valid=0;
            if(x!='A'+n) valid=0;
            n++;
        }
        else if(x=='('){
            balance++;
            if(i==0 || !isupper(s[i-1]) || !isupper(s[i+1]))
                valid=0;
            nonleaf++;
            depth++;
            ip(&cs,0);
        }
        else if(x==','){
            if(i==0 || !isupper(s[i-1]) || !isupper(s[i+1]))
                valid=0;
        }
        else if(x==')'){
            if(balance--<=0 || i==0 || !isupper(s[i-1]))
                valid=0;
            depth--;
            if(cs.top>=0) io(&cs);
        }
        else valid=0;
    }

    if(balance!=0 || !isupper(s[strlen(s)-1])) valid=0;

    if(!valid){
        printf("ERROR\n");
        return 0;
    }

    /* 다시 순차 탐색하며 스택으로 계산 */
    depth=0;

    for(i=0;s[i];i++){
        char x=s[i];

        if(isupper(x)){
            int d=0;

            if(ns.top>=0){
                d=io(&ds)+1;
                if(d>maxdeg) maxdeg=d;
                ip(&ds,d);

                if(ns.a[ns.top]=='C')
                    child[cchild++]=x;
            }
            else ip(&ds,0);

            if(x=='C' && ns.top>=0)
                parentC=ns.a[ns.top];

            cp(&ns,x);

            if(depth>maxdepth) maxdepth=depth;
        }
        else if(x=='('){
            depth++;
            ip(&cs,0);
        }
        else if(x==')'){
            int d=io(&ds);
            if(d>maxdeg) maxdeg=d;
            co(&ns);
            depth--;
            if(cs.top>=0) io(&cs);
        }
    }

    leaf=n-nonleaf;
    child[cchild]='\0';

    printf("전체 노드의 수 : %d\n",n);
    printf("단말 노드의 수 : %d\n",leaf);
    printf("비단말 노드의 수 : %d\n",nonleaf);
    printf("트리의 높이 : %d\n",maxdepth+1);
    printf("트리의 차수 : %d\n",maxdeg);

    printf("노드 C의 부모 : %c\n",parentC?parentC:'-');

    printf("노드 C의 자식 : ");
    if(cchild==0) printf("없음");
    else for(i=0;i<cchild;i++) printf("%c ",child[i]);
    printf("\n");

    /* 트리 출력 */
    printf("\n%s\n",s);

    return 0;
}