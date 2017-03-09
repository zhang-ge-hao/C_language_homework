#include "lib.h"
#define black 1
#define red 0
typedef book _type;
//重载小于号（伪
int less(_type a,_type b){return a.book_ID<b.book_ID;}
//重载等于号（伪
int equal(_type a,_type b){return a.book_ID==b.book_ID;}
typedef void (*FUNC)(_type);
//定义二叉树节点类
typedef struct btn{
    struct btn *parent,*left_son,*right_son;
    int color; _type data;
}btn;
//二叉树节点构造函数（伪
btn *new_btn(_type d,btn *pr,int c){
    btn *p=(btn*)malloc(sizeof(btn));
    p->data=d;p->parent=pr;p->color=c;
    p->left_son=p->right_son=NULL;
    return p;
}
//根据红黑树定义返回节点颜色
int _color(btn *p){
    return p?p->color:black;
}
//判断节点是否为左孩子
int is_left(btn *p){
    if(!p)return 3;if(!p->parent)return 2;
    if(p==p->parent->left_son)return 1;
    else return 0;
}
//在节点有右孩子的情况下返回节点的中序遍历后继
btn *succ(btn *p){
    if(!p)return (btn*)NULL;
    btn *q=p->right_son;
    while(q&&q->left_son)q=q->left_son;
    return q;
}
//交换两data值，和上面那个函数一样只用了一次（笑
void swap(_type *a,_type *b){
    _type t=*a;*a=*b;*b=t;
}
//返回节点的兄弟节点
btn *bro(btn *n){
    return is_left(n)?n->parent->right_son:n->parent->left_son;
}
//若该节点有红色孩子，返回该红色孩子
btn *red_son(btn *n){
    if(!n)return (btn*)NULL;
    btn *s=n->left_son;
    if(!_color(n->right_son))s=n->right_son;
    return s;
}
//红黑树类
typedef struct RB_tree{
    btn *_root,*_hot;
}RB_tree;
//红黑树构造函数（伪
RB_tree *new_tree(){
    RB_tree *p=(RB_tree*)malloc(sizeof(RB_tree));
    p->_root=p->_hot=NULL;
    return p;
}
//中序遍历函数（其实我会写迭代版的，但我懒
void in_order(btn *n,FUNC visit){
    if(!n)return;
    in_order(n->left_son,visit);
    visit(n->data);//printf("(%d) ",n->color);
    in_order(n->right_son,visit);
}
//先序遍历函数
void pre_order(btn *n,FUNC visit){
    if(!n)return;
    visit(n->data);
    pre_order(n->left_son,visit);
    pre_order(n->right_son,visit);
}
//3+4重构
btn *updata34(btn *p1,btn *p2,btn *p3,\
              btn *s1,btn *s2,btn *s3,btn *s4){
    p2->left_son=p1;p1->parent=p2;
    p2->right_son=p3;p3->parent=p2;
    p1->left_son=s1;if(s1)s1->parent=p1;
    p1->right_son=s2;if(s2)s2->parent=p1;
    p3->left_son=s3;if(s3)s3->parent=p3;
    p3->right_son=s4;if(s4)s4->parent=p3;
    return p2;
}
//判断重构类型并处理子树根节点的函数
btn *updata_balance(btn *g,btn *p,btn *n,RB_tree *T){
    int mode=is_left(g),Case=is_left(p)*2+is_left(n);
    btn *P,*G=g->parent;
    switch(Case){
    case 0:P=updata34(g,p,n,\
        g->left_son,p->left_son,n->left_son,n->right_son);
        break;
    case 1:P=updata34(g,n,p,\
        g->left_son,n->left_son,n->right_son,p->right_son);
        break;
    case 2:P=updata34(p,n,g,\
        p->left_son,n->left_son,n->right_son,g->right_son);
        break;
    case 3:P=updata34(n,p,g,\
        n->left_son,n->right_son,p->right_son,g->right_son);
        break;}
    if(mode==0)G->right_son=P;
    else if(mode==1)G->left_son=P;
    else if(mode==2)T->_root=P;
    P->parent=G;
    return P;
}
//对子树进行……ZigOrZag……
void zig_or_zag(btn *p,btn *n,RB_tree *T){
    btn *g=p->parent,*s1,*s2,*s3; int mode=is_left(p);
    if(less(p->data,n->data)){
        s1=p->left_son;s2=n->left_son;s3=n->right_son;
        n->left_son=p;p->parent=n;
        n->right_son=s3;if(s3)s3->parent=n;
        p->left_son=s1;if(s1)s1->parent=p;
        p->right_son=s2;if(s2)s2->parent=p;}
    else {
        s1=n->left_son;s2=n->right_son;s3=p->right_son;
        n->left_son=s1;if(s1)s1->parent=n;
        n->right_son=p;p->parent=n;
        p->left_son=s2;if(s2)s2->parent=p;
        p->right_son=s3;if(s3)s3->parent=p;}
    n->parent=g;
    if(g){if(mode)g->left_son=n;else g->right_son=n;}
    else T->_root=n;
    n->color=black;p->color=red;
}
//处理双红缺陷（子函数，含颜色处理
void double_red_1(btn *g,btn *p,btn *n,RB_tree *T){
    btn *P=updata_balance(g,p,n,T);
    P->color=black;P->left_son->color=P->right_son->color=red;
}
//处理双红缺陷（主函数
void double_red(btn *g,btn *p,btn *n,RB_tree *T){
    if(is_left(p)?_color(g->right_son):_color(g->left_son))
    {double_red_1(g,p,n,T);return;}
    g->color=g==T->_root?black:red;
    g->left_son->color=g->right_son->color=black;
    if(g->parent&&g->parent->color==red){
        double_red(g->parent->parent,g->parent,g,T);}
}
//处理双黑缺陷（子函数，含颜色处理
void double_black_1(btn *g,btn *p,btn *n,RB_tree *T){
    int c=_color(g);
    btn *G=updata_balance(g,p,n,T);
    G->color=c;
    G->left_son->color=G->right_son->color=black;
}
//处理双黑缺陷（主函数
void double_black(btn *n,RB_tree *T){
    btn *b=(T->_hot->left_son==n)?(T->_hot->right_son):(T->_hot->left_son);
    btn *p=T->_hot,*rs=red_son(b);
    if(!_color(b)){zig_or_zag(p,b,T);
        double_black(n,T);return;}
    else if(_color(b->left_son)&&_color(b->right_son)){
        if(_color(p)){b->color=red;
            T->_hot=p->parent;if(T->_hot)double_black(p,T);}
        else {
            T->_hot->color=black;b->color=red;}
    }
    else double_black_1(p,b,rs,T);
}
//二叉搜索树三大支柱函数（笑
btn *search(_type a,RB_tree *T){
    btn *p=T->_root; T->_hot=NULL;
    while(p&&!equal(p->data,a)){
        T->_hot=p;
        p=less(a,p->data)?p->left_son:p->right_son;}
    return p;
}
void insert(_type a,RB_tree *T){
    btn *p=search(a,T); if(p)return;
    if(!T->_hot){T->_root=new_btn(a,0,black);return;}
    if(less(a,T->_hot->data))T->_hot->left_son=p=new_btn(a,T->_hot,red);
    else T->_hot->right_son=p=new_btn(a,T->_hot,red);
    if(T->_hot->color==black)return;
    double_red(T->_hot->parent,T->_hot,p,T);
}
void erase(_type a,RB_tree *T){
    btn *p=search(a,T),*pr,*son,*q=succ(p);
    int mode; if(!p)return;
    if(p->left_son&&p->right_son){swap(&p->data,&q->data);p=q;}
    son=p->left_son?p->left_son:p->right_son;
    pr=T->_hot=p->parent;
    mode=is_left(p);
    if(son)son->parent=pr;
    if(!pr){T->_root=son;if(son)son->color=black;}
    else {
    if(mode)pr->left_son=son;else pr->right_son=son;
    if(_color(p)&&_color(son))double_black(son,T);
    if(son)son->color=black;}
    free(p);
}
