#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;
#define FILE_NAME "books.txt"  //存储图书的文件名
enum information {name=1,version,author,press,ISBN,publish_time};  //枚举书本信息，便于修改信息
#define LIST_INIT_SIZE 100  //线性表存储空间的初始分配量
#define LISTINCREMENT 10
typedef struct 
{
    char name[50];  //书名
    int version;    //版本
    char author[10];    //作者
    char press[20];     //出版社
    char ISBN[30];      //ISBN
    char publish_time[20];  //出版时间
}Book;
typedef struct 
{
    Book *elem; //存储空间基址
    int length; //当前长度
    int listsize;   //当前分配的存储容量
}Library;
Status InitLibrary(Library *L); //线性表初始化
Status ReadFile(Library *L,FILE **fp);  //读取文件内容并保存到library中
Status ListIncrease(Library *L); //扩大线性表
Status AddBook(Library *L,FILE **fp); //增加一本图书并存储到文件中
Status ChangeInformation(Library *L,FILE **fp); //修改一本图书的信息
int SearchBook(Library *L); //根据书的某项信息来查找书籍并返回其在线性表中的位置
Status StrFullCpy(char s1[],char s2[]);//字符串覆盖复制
Status BookRefresh(Library *L,FILE **fp);  //更新整个文件
Status DeleteBooK(Library *L,FILE **fp);  //删除一本书
int main(void)
{
    Library library;
    FILE *fp;
    int type=0;
    int i;
    InitLibrary(&library);   //初始化线性表
    //读取文件内容并保存到library中
    if ((fp=fopen(FILE_NAME,"r+"))==NULL)
    {
        printf("file open error\n");
        exit(0);
    }
    else
    {
        ReadFile(&library,&fp);
    }
    printf("请选择您要使用的功能，按回车确认\n1.录入  2.修改  3.删除  4.统计总数  5.查询图书  0.退出\n");
    scanf("%d",&type);
    while (type)
    {
        switch (type)
        {
        case 1:
            AddBook(&library,&fp);
            break;
        case 2:
            ChangeInformation(&library,&fp);
            break;
        case 3:
            DeleteBooK(&library,&fp);
            break;
        case 4:
            printf("总计%d本图书\n",library.length);
            break;
        case 5:
            i=SearchBook(&library);
            if(i>=0)
                printf("%s %d %s %s %s %s\n",library.elem[i].name,library.elem[i].version,library.elem[i].author,library.elem[i].press,library.elem[i].ISBN,library.elem[i].publish_time);
            break;
        default:
            break;
        }
        printf("请选择您要使用的功能，按回车确认\n1.录入  2.修改  3.删除  4.统计总数  5.查询图书  0.退出\n");
        scanf("%d",&type);
    }
    fclose(fp); //关闭文件
    free(library.elem);//释放library
    system("pause");
    return 0;
}
Status InitLibrary(Library *L)
{
    L->elem=(Book*)malloc(LIST_INIT_SIZE*sizeof(Book));
    if (!L->elem)exit(OVERFLOW);    //存储分配失败
    L->length=0;                    //空表长度为0
    L->listsize=LIST_INIT_SIZE;     //初始存储容量
    return OK;
}
Status ListIncrease(Library *L)
{
    Book *newbase=(Book*)realloc(L->elem,(L->listsize+LISTINCREMENT)*sizeof(Book));
    if (!newbase)exit(OVERFLOW);  //存储分配失败
    L->elem=newbase;
    L->listsize+=LISTINCREMENT;
    return OK;
}
Status ReadFile(Library *L,FILE **fp)
{
    int i=0;
    while (!feof(*fp))
    {
        if ((L->length)<(L->listsize))  //当容量够时
        {
            fscanf(*fp,"%s %d %s %s %s %s",(L->elem[i]).name,&(L->elem[i]).version,(L->elem[i]).author,(L->elem[i]).press,(L->elem[i]).ISBN,(L->elem[i]).publish_time);
            (L->length)++;
            i++;
        }
        else
        {
            ListIncrease(L);
        }

    }
    return OK;
}
Status AddBook(Library *L,FILE **fp)
{
    fseek(*fp,0,SEEK_END);
    int i=L->length;
    if ((L->length)>=(L->listsize))  //当容量不够时
        ListIncrease(L);
    printf("请依次输入图书的书名，版本，作者，出版社，ISBN编号，出版日期，按回车确认\n");    
    scanf("%s %d %s %s %s %s",(L->elem[i]).name,&(L->elem[i]).version,(L->elem[i]).author,(L->elem[i]).press,(L->elem[i]).ISBN,(L->elem[i]).publish_time);
    (L->length)++;
    //写入文件
    fprintf(*fp,"\n%s %d %s %s %s %s",(L->elem[i]).name,(L->elem[i]).version,(L->elem[i]).author,(L->elem[i]).press,(L->elem[i]).ISBN,(L->elem[i]).publish_time);    
    fclose(*fp);
    *fp=fopen(FILE_NAME,"r+");
    printf("添加成功！\n");
    return OK;
}
Status ChangeInformation(Library *L,FILE **fp)
{
    int i,j;
    char inf[50];
    int version;
    enum information book;
    printf("请先查询你所要修改的书籍\n");
    i=SearchBook(L);
    if (i>=0)
    {
        printf("请选择您要修改的信息（1.书名,2.版本,3.作者,4.出版社,5.ISBN,6.出版时间）\n");
        scanf("%d",&book);
        printf("请输入您要修改为的内容\n");
        switch (book)
        {
        case 1:
            scanf("%s",inf);
            StrFullCpy((L->elem)[i].name,inf);
            break;
        case 2:
            scanf("%d",&version);
            (L->elem)[i].version=version;
            break;
        case 3:
            scanf("%s",inf);
            StrFullCpy((L->elem)[i].author,inf);
            break;
        case 4:
            scanf("%s",inf);
            StrFullCpy((L->elem)[i].press,inf);
            break;
        case 5:
            scanf("%s",inf);
            StrFullCpy((L->elem)[i].ISBN,inf);
            break;
        case 6:
            scanf("%s",inf);
            StrFullCpy((L->elem)[i].publish_time,inf);
            break;
        default:
            break;
        }
        BookRefresh(L,fp);
        printf("修改成功！\n");
        return OK;
    }
    return ERROR;
}
int SearchBook(Library *L)
{
    enum information book;
    int booknum=0;
    int i;
    int seq[L->length];
    char inf[50];
    int version;
    printf("请选择您查询所依据的信息（1.书名,2.版本,3.作者,4.出版社,5.ISBN,6.出版时间）\n");
    scanf("%d",&book);
    printf("请输入查询所需要的信息\n");
    switch (book)
    {
    case 1:
        scanf("%s",inf);
        for ( i = 0; i < L->length; i++)
        {
            if (strcmp((L->elem)[i].name,inf)==0)
            {
                seq[booknum]=i;
                booknum++;
            }
        }
        break;
    case 2:
        scanf("%d",&version);
        for ( i = 0; i < L->length; i++)
        {
            if ((L->elem)[i].version==version)
            {
                seq[booknum]=i;
                booknum++;
            }
        }
        break;
    case 3:
        scanf("%s",inf);
        for ( i = 0; i < L->length; i++)
        {
            if (strcmp((L->elem)[i].author,inf)==0)
            {
                seq[booknum]=i;
                booknum++;
            }
        }
        break;
    case 4:
        scanf("%s",inf);
        for ( i = 0; i < L->length; i++)
        {
            if (strcmp((L->elem)[i].press,inf)==0)
            {
                seq[booknum]=i;
                booknum++;
            }
        }
        break;
    case 5:
        scanf("%s",inf);
        for ( i = 0; i < L->length; i++)
        {
            if (strcmp((L->elem)[i].ISBN,inf)==0)
            {
                seq[booknum]=i;
                booknum++;
            }
        }
        break;
    case 6:
        scanf("%s",inf);
        for ( i = 0; i < L->length; i++)
        {
            if (strcmp((L->elem)[i].publish_time,inf)==0)
            {
                seq[booknum]=i;
                booknum++;
            }
        }
        break;
    default:
        break;
    }
    if (booknum==0)
    {
        printf("查无此书\n");
        return -1;
    }
    else if(booknum==1)
        return seq[0];
    else if(booknum>1)    //多本书都具有同样的该信息
    {
        for ( i = 0; i < booknum; i++)
        {
            printf("%d   %s %d %s %s %s %s\n",i+1,(L->elem[seq[i]]).name,(L->elem[seq[i]]).version,(L->elem[seq[i]]).author,(L->elem[seq[i]]).press,(L->elem[seq[i]]).ISBN,(L->elem[seq[i]]).publish_time);
        }
        printf("请选择第几本书，按回车确定\n");
        scanf("%d",&i);
        i--;
        return seq[i];
    }
    else
        return ERROR;
}
Status StrFullCpy(char s1[],char s2[])
{
    char *chp=s2;
    int i=0;
    while ((*chp)!='\0')
    {
        s1[i++]=*chp;
        chp++;
    }
	s1[i]='\0';
    return  OK;
}
Status BookRefresh(Library *L,FILE **fp)
{
    int i;
    fclose(*fp);
    *fp=fopen(FILE_NAME,"w");
    for ( i = 0; i < L->length; i++)
    {
        fprintf(*fp,"%s %d %s %s %s %s",(L->elem[i]).name,(L->elem[i]).version,(L->elem[i]).author,(L->elem[i]).press,(L->elem[i]).ISBN,(L->elem[i]).publish_time);
        if(i!=(L->length-1))fprintf(*fp,"\n");
    }
    fclose(*fp);
    *fp=fopen(FILE_NAME,"r+");
    return OK;
}
Status DeleteBooK(Library *L,FILE **fp)
{
    int i;
    Book *p,*q;
    printf("请先查询你所要删除的书籍\n");
    i=SearchBook(L);
    if (i>=0)
    {
        p=&(L->elem[i]);
        q=L->elem+L->length-1;
        for ( ++p; p <= q; ++p)
            *(p-1)=*p;
        L->length--;
        BookRefresh(L,fp);
        printf("删除成功！\n");
        return OK;
    }
    return ERROR;
}