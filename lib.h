#include <stdlib.h>
#include <stdio.h>
typedef long long Rank; typedef double money;
//一些关于字符串的函数
int str_equal(char *a,char *b){
    return *a==*b?(*a=='\0'?1:str_equal(a+1,b+1)):0;}
Rank str_length(char *c){
    return *c=='\0'?0:1+str_length(c+1);}
void _str_copy(char *a,char *b){
    *a=*b;if(*b=='\0')return;_str_copy(a+1,b+1);}
char *str_copy(char *c){
    char *cc=(char*)malloc(sizeof(char)*str_length(c));
    _str_copy(cc,c);
    return cc;}
//定义书本类
typedef struct book{
    Rank book_ID; money price;
    char *book_name,*book_author;
}book;
//书本类构造函数（伪
book Book(Rank ID,money pri,char *name,char *aut){
    book B; B.book_ID=ID; B.price=pri;
    B.book_name=str_copy(name); B.book_author=str_copy(aut);
    return B;
}
//输出书本信息（供函数指针用
void b_PRINT(book B){
    printf("%04I64d\t%s\t%s\t%.2lf\n",\
           B.book_ID,B.book_name,B.book_author,B.price);
}
//向文件中输出书本信息（供函数指针用
void put_book_in_file(book B){
    FILE *fl=fopen("lib","ab");
    fprintf(fl,"%04I64d %s %s %.2lf\n",\
            B.book_ID,B.book_name,B.book_author,B.price);
    fclose(fl);
}
