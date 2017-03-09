#include "RB_tree.h"
#include "hash.h"
#include <time.h>
//提示信息打印函数
void message_print(char *message){
    system("cls");
    puts("**************************************************************");
    puts("* 1. Add new book into the library.                          *");
    puts("* 2. Remove book from the library.                           *");
    puts("* 3. Save changes.                                           *");
    puts("* 4. Print all book's message.                               *");
    puts("* 5. Search books by book's ID or keyword.                   *");
    puts("* 6. Exit this system after save changes.                    *");
    puts("* 7. Exit this system without save changes.                  *");
    puts("**************************************************************");
    printf("  ");puts(message);
    puts("**************************************************************");
}
//插入书本
void insert_book(RB_tree *Library,hash *Search_Engines,\
                 char *name,char *aut,Rank ID,money pri){
    insert(Book(ID,pri,name,aut),Library);
    hash_save(name,ID,Search_Engines);
    hash_save(aut,ID,Search_Engines);
}
//删除书本
void erase_book(RB_tree *Library){
    Rank ID;
    scanf("%I64d",&ID);
    erase(Book(ID,0,"NULL","NULL"),Library);
}
//打印全部书本信息
void print_lib(RB_tree *Library){
    puts("ID\tNAME\tAUTHOR\tPRICE");
    in_order(Library->_root,b_PRINT);
}
//循书号搜索书本
void search_book_by_ID(RB_tree *Library,hash *Search_Engines){
    Rank ID; scanf("%I64d",&ID);
    message_print("Press Enter to continue.");
    btn *ans=search(Book(ID,0,"NULL","NULL"),Library);
    if(!ans){printf("Nothing\n");return;}
    puts("ID\tNAME\tAUTHOR\tPRICE");
    b_PRINT(ans->data);
}
//循关键词搜索书本
void search_book_by_str(RB_tree *Library,hash *Search_Engines){
    fflush(stdin);btn *ans;
    char word[30]; scanf("%s",word);
    message_print("Press Enter to continue.");
    vector *V=hash_load(word,Search_Engines);
    if(!V){printf("Nothing\n");return;}
    puts("ID\tNAME\tAUTHOR\tPRICE");
    Rank i,j;for(i=j=0;i<V->_size;i++)
    {
        ans=search(Book(V->_head[i],0,"NULL","NULL"),Library);
        if(ans){b_PRINT(ans->data);j++;}
    }
    if(!j)printf("Nothing\n");
}
//从键盘中读入书本信息以供插入
void insert_from_key(RB_tree *Library,hash *Search_Engines){
    fflush(stdin);
    char name[30],aut[30]; Rank ID; money pri;
    scanf("%I64d %s %s %lf",&ID,name,aut,&pri);
    insert_book(Library,Search_Engines,name,aut,ID,pri);
}
//从文件中读入书本信息以供插入
void insert_from_file(RB_tree *Library,hash *Search_Engines){
    FILE *fl=fopen("lib","rb");
    char name[30],aut[30]; Rank ID; money pri;
    if(!fl)fl=fopen("lib","wb");
    else while(fscanf(fl,"%I64d %s %s %lf",&ID,name,aut,&pri)==4)
        insert_book(Library,Search_Engines,name,aut,ID,pri);
    fclose(fl);
}
//更新覆盖文件信息
void updata_file(RB_tree *Library,hash *Search_Engines){
    FILE *fl=fopen("lib","wb"); fclose(fl);
    in_order(Library->_root,put_book_in_file);
}
int main()
{
    hash *Search_Engines=new_hash();
    RB_tree *Library=new_tree();
    Rank order;
    insert_from_file(Library,Search_Engines);
    while(1){
        message_print("Enter number to select the options above :");
        fflush(stdin);if(scanf("%I64d",&order)!=1)break;
        switch (order){
        case 1:
            message_print("How many books do you want to add?");
            scanf("%I64d",&order);
            message_print("Please enter the message of the books.");
            puts("Enter the book's message in the format \"ID NAME AUTHOR PRICE\".");
            puts("Separated with the space.");
            for(;order;order--)insert_from_key(Library,Search_Engines);
            break;
        case 2:
            message_print("How many books do you want to remove?");
            scanf("%I64d",&order);
            message_print("Please enter the ID of the books.");
            for(;order;order--)erase_book(Library);
            break;
        case 3:
            updata_file(Library,Search_Engines);
            break;
        case 4:
            message_print("Press Enter to continue.");
            print_lib(Library);fflush(stdin);
            while(1)if(getchar()=='\n')break;
            break;
        case 5:
            message_print("Press 1 if search by ID,2 if search by keyword.");
            scanf("%I64d",&order);
            if(order==1){
                message_print("Please enter the book's ID.");
                search_book_by_ID(Library,Search_Engines);}
            else if(order==2){
                message_print("Please enter the keyword.");
                search_book_by_str(Library,Search_Engines);}
            fflush(stdin);while(1)if(getchar()=='\n')break;
            break;
        case 6:
            updata_file(Library,Search_Engines); exit(0);
        case 7: exit(0);
        }
    }
    return 0;
}
