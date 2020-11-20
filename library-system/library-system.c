#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;
#define FILE_NAME "books.txt"  //�洢ͼ����ļ���
enum information {name=1,version,author,press,ISBN,publish_time};  //ö���鱾��Ϣ�������޸���Ϣ
#define LIST_INIT_SIZE 100  //���Ա�洢�ռ�ĳ�ʼ������
#define LISTINCREMENT 10
typedef struct 
{
    char name[50];  //����
    int version;    //�汾
    char author[10];    //����
    char press[20];     //������
    char ISBN[30];      //ISBN
    char publish_time[20];  //����ʱ��
}Book;
typedef struct 
{
    Book *elem; //�洢�ռ��ַ
    int length; //��ǰ����
    int listsize;   //��ǰ����Ĵ洢����
}Library;
Status InitLibrary(Library *L); //���Ա��ʼ��
Status ReadFile(Library *L,FILE **fp);  //��ȡ�ļ����ݲ����浽library��
Status ListIncrease(Library *L); //�������Ա�
Status AddBook(Library *L,FILE **fp); //����һ��ͼ�鲢�洢���ļ���
Status ChangeInformation(Library *L,FILE **fp); //�޸�һ��ͼ�����Ϣ
int SearchBook(Library *L); //�������ĳ����Ϣ�������鼮�������������Ա��е�λ��
Status StrFullCpy(char s1[],char s2[]);//�ַ������Ǹ���
Status BookRefresh(Library *L,FILE **fp);  //���������ļ�
Status DeleteBooK(Library *L,FILE **fp);  //ɾ��һ����
int main(void)
{
    Library library;
    FILE *fp;
    int type=0;
    int i;
    InitLibrary(&library);   //��ʼ�����Ա�
    //��ȡ�ļ����ݲ����浽library��
    if ((fp=fopen(FILE_NAME,"r+"))==NULL)
    {
        printf("file open error\n");
        exit(0);
    }
    else
    {
        ReadFile(&library,&fp);
    }
    printf("��ѡ����Ҫʹ�õĹ��ܣ����س�ȷ��\n1.¼��  2.�޸�  3.ɾ��  4.ͳ������  5.��ѯͼ��  0.�˳�\n");
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
            printf("�ܼ�%d��ͼ��\n",library.length);
            break;
        case 5:
            i=SearchBook(&library);
            if(i>=0)
                printf("%s %d %s %s %s %s\n",library.elem[i].name,library.elem[i].version,library.elem[i].author,library.elem[i].press,library.elem[i].ISBN,library.elem[i].publish_time);
            break;
        default:
            break;
        }
        printf("��ѡ����Ҫʹ�õĹ��ܣ����س�ȷ��\n1.¼��  2.�޸�  3.ɾ��  4.ͳ������  5.��ѯͼ��  0.�˳�\n");
        scanf("%d",&type);
    }
    fclose(fp); //�ر��ļ�
    free(library.elem);//�ͷ�library
    system("pause");
    return 0;
}
Status InitLibrary(Library *L)
{
    L->elem=(Book*)malloc(LIST_INIT_SIZE*sizeof(Book));
    if (!L->elem)exit(OVERFLOW);    //�洢����ʧ��
    L->length=0;                    //�ձ���Ϊ0
    L->listsize=LIST_INIT_SIZE;     //��ʼ�洢����
    return OK;
}
Status ListIncrease(Library *L)
{
    Book *newbase=(Book*)realloc(L->elem,(L->listsize+LISTINCREMENT)*sizeof(Book));
    if (!newbase)exit(OVERFLOW);  //�洢����ʧ��
    L->elem=newbase;
    L->listsize+=LISTINCREMENT;
    return OK;
}
Status ReadFile(Library *L,FILE **fp)
{
    int i=0;
    while (!feof(*fp))
    {
        if ((L->length)<(L->listsize))  //��������ʱ
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
    if ((L->length)>=(L->listsize))  //����������ʱ
        ListIncrease(L);
    printf("����������ͼ����������汾�����ߣ������磬ISBN��ţ��������ڣ����س�ȷ��\n");    
    scanf("%s %d %s %s %s %s",(L->elem[i]).name,&(L->elem[i]).version,(L->elem[i]).author,(L->elem[i]).press,(L->elem[i]).ISBN,(L->elem[i]).publish_time);
    (L->length)++;
    //д���ļ�
    fprintf(*fp,"\n%s %d %s %s %s %s",(L->elem[i]).name,(L->elem[i]).version,(L->elem[i]).author,(L->elem[i]).press,(L->elem[i]).ISBN,(L->elem[i]).publish_time);    
    fclose(*fp);
    *fp=fopen(FILE_NAME,"r+");
    printf("��ӳɹ���\n");
    return OK;
}
Status ChangeInformation(Library *L,FILE **fp)
{
    int i,j;
    char inf[50];
    int version;
    enum information book;
    printf("���Ȳ�ѯ����Ҫ�޸ĵ��鼮\n");
    i=SearchBook(L);
    if (i>=0)
    {
        printf("��ѡ����Ҫ�޸ĵ���Ϣ��1.����,2.�汾,3.����,4.������,5.ISBN,6.����ʱ�䣩\n");
        scanf("%d",&book);
        printf("��������Ҫ�޸�Ϊ������\n");
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
        printf("�޸ĳɹ���\n");
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
    printf("��ѡ������ѯ�����ݵ���Ϣ��1.����,2.�汾,3.����,4.������,5.ISBN,6.����ʱ�䣩\n");
    scanf("%d",&book);
    printf("�������ѯ����Ҫ����Ϣ\n");
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
        printf("���޴���\n");
        return -1;
    }
    else if(booknum==1)
        return seq[0];
    else if(booknum>1)    //�౾�鶼����ͬ���ĸ���Ϣ
    {
        for ( i = 0; i < booknum; i++)
        {
            printf("%d   %s %d %s %s %s %s\n",i+1,(L->elem[seq[i]]).name,(L->elem[seq[i]]).version,(L->elem[seq[i]]).author,(L->elem[seq[i]]).press,(L->elem[seq[i]]).ISBN,(L->elem[seq[i]]).publish_time);
        }
        printf("��ѡ��ڼ����飬���س�ȷ��\n");
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
    printf("���Ȳ�ѯ����Ҫɾ�����鼮\n");
    i=SearchBook(L);
    if (i>=0)
    {
        p=&(L->elem[i]);
        q=L->elem+L->length-1;
        for ( ++p; p <= q; ++p)
            *(p-1)=*p;
        L->length--;
        BookRefresh(L,fp);
        printf("ɾ���ɹ���\n");
        return OK;
    }
    return ERROR;
}