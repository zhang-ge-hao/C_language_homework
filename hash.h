#include "vector.h"
#define HASH_SIZE 11
#define bit1  54
#define bit2  10
//定义一些事
typedef enum posi_mode{Empty,Fill,Del}posi_mode;
typedef enum SL_mode{save,load}SL_mode;
//定义散列表节点
typedef struct _data{
    vector *data;
    posi_mode state;
    char *_str;
}_data;
//定义散列表类
typedef struct hash{
    _data *hash_list;
}hash;
//散列表构造函数（伪
hash *new_hash(){
    hash *h=(hash*)malloc(sizeof(hash));Rank i;
    h->hash_list=(_data*)malloc(sizeof(_data)*HASH_SIZE);
    for(i=0;i<HASH_SIZE;i++){
        h->hash_list[i].state=Empty;
        h->hash_list[i].data=new_vector();}
    return h;
}
//散列表编码获取（子函数
Rank _code(char *c,Rank old_code,Rank times,SL_mode m,hash *H)
{
    Rank new_code=old_code+((times&1?-1:1)*(times/2)*(times/2));
    new_code%=HASH_SIZE;if(new_code<0)new_code+=HASH_SIZE;
    if(m==load&&(HASH_SIZE<times||H->hash_list[new_code].state==Empty))
        return -1;
    if(H->hash_list[new_code].state==Empty)
        return new_code;
    else if(H->hash_list[new_code].state==Fill){
        return str_equal(c,H->hash_list[new_code]._str)?\
        new_code:_code(c,old_code,times+1,m,H);}
    else {if(m==load)return _code(c,old_code,times+1,m,H);
        else return new_code;}
}
//散列表编码获取（主函数
Rank hash_code(char *c,SL_mode m,hash *H){
    Rank h_code=0;
    Rank i;for(i=0;c[i]!='\0';i++){
        h_code+=c[i];h_code=(h_code>>bit1)^(h_code<<bit2);}
    h_code=h_code%HASH_SIZE;if(h_code<0)h_code+=HASH_SIZE;
    h_code=_code(c,h_code,1,m,H);
    return h_code;
}
//存入数据函数
void hash_save(char *c,vector_type v,hash *H){
    Rank h_code=hash_code(c,save,H);
    if(H->hash_list[h_code].state==Del){
        free(H->hash_list[h_code]._str);
        clear(H->hash_list[h_code].data);}
    H->hash_list[h_code]._str=str_copy(c);
    H->hash_list[h_code].state=Fill;
    push_back(v,H->hash_list[h_code].data);
}
//搜索数据函数
vector *hash_load(char *c,hash *H){
    Rank h_code=hash_code(c,load,H);
    return h_code<0?(vector*)NULL:H->hash_list[h_code].data;
}
//删除数据函数（估计并没有用
void hash_del(char *c,hash *H){
    Rank h_code=hash_code(c,load,H);
    if(h_code<0)return;
    H->hash_list[h_code].state=Del;
}
