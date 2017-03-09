//初始内存大小
#define INIT_RAM 2
//定义一些事
typedef long long vector_type;
typedef void (*v_FUNC)(vector_type);
//临时测试函数PRINT
void v_PRINT(vector_type a){printf("%I64d ",a);}
//定义向量类
typedef struct vector{
    vector_type *_head; Rank _size,_ram;
}vector;
//向量构造函数
vector *new_vector(){
    vector *V=(vector*)malloc(sizeof(vector));
    V->_size=0;V->_ram=INIT_RAM;
    V->_head=(vector_type*)malloc(sizeof(vector_type)*V->_ram);
    return V;
}
//向量相关内存操作函数
void to_big(Rank i,vector *V){
    if(V->_size+i<=V->_ram)return;
    V->_ram=2*(V->_size+i);
    vector_type *new_head=\
    (vector_type*)malloc(sizeof(vector_type)*V->_ram);
    for(i=0;i<V->_size;i++)new_head[i]=V->_head[i];
    free(V->_head);V->_head=new_head;
}
void to_little(vector *V){
    if(V->_ram/2<=V->_size)return;
    V->_ram=V->_ram/2<INIT_RAM?INIT_RAM:V->_ram/2;
    vector_type *new_head=\
    (vector_type*)malloc(sizeof(vector_type)*V->_ram);
    Rank i;for(i=0;i<V->_size;i++)new_head[i]=V->_head[i];
    free(V->_head);V->_head=new_head;
}
//在向量末尾添加节点
void push_back(vector_type a,vector *V){
    to_big(1,V);
    V->_head[V->_size++]=a;
}
//在向量末尾删除节点
void pop_back(vector *V){
    if(!V->_size)return; V->_size--;
    to_little(V);
}
//清除向量所有节点
void clear(vector *V){
    V->_size=0;V->_ram=INIT_RAM;
    free(V->_head);
    V->_head=(vector_type*)malloc(sizeof(vector_type)*V->_ram);
}
//遍历向量所有节点
void for_each(vector *V,v_FUNC visit){
    Rank i;for(i=0;i<V->_size;i++)visit(V->_head[i]);
}
