#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;
typedef struct //Huffman��
{
    int weight;
    int parent,lchild,rchild;
}HTNode,*HuffmanTree;
typedef struct //��������
{
    char ch;
    char *code;
}HCNode,*Huffmancode;
//����
Status Initialization(HuffmanTree *HT,Huffmancode *HC,int *n); //�������ݲ�����Huffman��
Status Selet(HuffmanTree HT,int n,int *s1,int *s2);//HT[1..,i-1]ѡ��parentΪ0��weight��С�������ڵ�
void Tree_printing(HuffmanTree HT,int idx,int level,FILE *fp);//ֱ����ʽ��ӡHuffman����д��treePrint
Status WriteInHmfTree(HuffmanTree HT,Huffmancode HC,int n,int m);//��Huffman�����浽hfmTree
Status Encoding(HuffmanTree HT,Huffmancode HC,int n);//���ɱ������鲢��ToBeTran����Ҫ������ַ������벢���浽CodeFile
Status ReadHfmTree(HuffmanTree *HT,Huffmancode *HC,int *n);//��ȡHuffman��������
int FileLineNum(char name[]);//���ļ�����
char *findc(Huffmancode HC,char ch,int n);//�ҵ��ַ�ch��Ӧ�ı����ָ��
Status Print(void);//��������Ĵ����ӡ��������CodePrin
Status Decoding(Huffmancode HC,int n);//��CodeFile�д������벢������TextFile��
Status FreeTree(HuffmanTree HT,int m);//�ͷ�Huffman��
Status FreeCode(Huffmancode HC,int n);//�ͷű�������
int main(void)
{
    //������ʼ��
    int type;
    int n;
    FILE *TreePrint_fp;
    HuffmanTree HT=NULL;
    Huffmancode HC=NULL;
    printf("�˵�\n1.��ʼ��   2.����   3.����   4.��ӡ�����ļ�   5.��ӡ��������   6.�˳�\n");
    scanf("%d",&type);
    //ѭ�������˵�
    while (type!=6)
    {
        //5������
        switch (type)
        {
        case 1:
            Initialization(&HT,&HC,&n);
            printf("��ʼ���ɹ�\n");
            break;
        case 2:
            if(HC==NULL)ReadHfmTree(&HT,&HC,&n);
            Encoding(HT,HC,n);
            printf("����ɹ�\n");
            break;
        case 3:
            Decoding(HC,n);
            printf("����ɹ�\n");
            break;
        case 4:
            Print();
            break;
        case 5:
            TreePrint_fp=fopen("TreePrint.txt","r+");
            Tree_printing(HT,2*n-1,0,TreePrint_fp);
            fclose(TreePrint_fp);
            break;        
        default:
            break;
        }
        //�ظ�����
        printf("�˵�\n1.��ʼ��   2.����   3.����   4.��ӡ�����ļ�   5.��ӡ��������   6.�˳�\n");
        scanf("%d",&type);
    }
    //�ͷŶ�ջ
    FreeTree(HT,2*n-1);
    FreeCode(HC,n);
    system("pause");
    return 0;
}
Status Initialization(HuffmanTree *HT,Huffmancode *HC,int *n)
{
    int m;
    int i;
    int s1;
    int s2;
    scanf("%d",n);
    if (*n<=1) return ERROR;
    m=2*(*n)-1;
    (*HT)=(HuffmanTree)malloc((m+1)*sizeof(HTNode));
    (*HC)=(Huffmancode)malloc((*n+1)*sizeof(HCNode));
    for ( i = 1; i <= *n; i++)
    {
        getchar();
        scanf("%c %d",&(*HC)[i].ch,&(*HT)[i].weight);
        (*HT)[i].parent=0;(*HT)[i].lchild=0;(*HT)[i].rchild=0;
        (*HC)[i].code=NULL;
    }
    for(i=*n+1;i<=m;i++)
    {
        (*HT)[i].weight=0;(*HT)[i].parent=0;(*HT)[i].lchild=0;(*HT)[i].rchild=0;
    }
    for ( i = *n+1; i <= m; i++)
    {
        Selet(*HT,i-1,&s1,&s2);
        (*HT)[i].lchild=s1;(*HT)[i].rchild=s2;
        (*HT)[i].weight=(*HT)[s1].weight+(*HT)[s2].weight;
    }
    WriteInHmfTree(*HT,*HC,*n,m);
    return OK;
}
Status Selet(HuffmanTree HT,int n,int *s1,int *s2)//HT[1..,i-1]ѡ��parentΪ0��weight��С�������ڵ�
{
    int i=1;
    int min;
    while (HT[i].parent!=0&&i<=n)i++;
    min =HT[i].weight;
    *s1=i;
    for ( ; i < n; i++)
    {
        if (HT[i+1].parent==0&&HT[i+1].weight<min)
        {
            min=HT[i+1].weight;
            *s1=i+1;
        }   
    }
    HT[*s1].parent=n+1;
    i=1;
    while (HT[i].parent!=0&&i<=n)i++;
    min =HT[i].weight;
    *s2=i;
    for ( ; i < n; i++)
    {
        if (HT[i+1].parent==0&&HT[i+1].weight<min)
        {
            min=HT[i+1].weight;
            *s2=i+1;
        }   
    }
    HT[*s2].parent=n+1;
    return OK;
}
void Tree_printing(HuffmanTree HT,int idx,int level,FILE *fp)//��level=0��ʼ
{
    int i;
    if(HT[idx].rchild!=0)
    Tree_printing(HT,HT[idx].rchild,level+1,fp);
    for ( i = 0; i < 2*level; i++)
    {  
        printf(" ");
        fprintf(fp," ");
    }
    printf("%d\n",HT[idx].weight);
    fprintf(fp,"%d\n",HT[idx].weight);
    if(HT[idx].lchild!=0)
    Tree_printing(HT,HT[idx].lchild,level+1,fp);
}
Status WriteInHmfTree(HuffmanTree HT,Huffmancode HC,int n,int m)
{
    int i;
    FILE *fp=fopen("hfmTree.txt","r+");
    for ( i = 1; i <=n ; i++)
    {
        fprintf(fp,"%d %d %d %d %d %c\n",i,HT[i].weight,HT[i].parent,HT[i].lchild,HT[i].rchild,HC[i].ch);
    }
    for ( i = n+1; i < m; i++)
    {
        fprintf(fp,"%d %d %d %d %d\n",i,HT[i].weight,HT[i].parent,HT[i].lchild,HT[i].rchild);
    }
    fprintf(fp,"%d %d %d %d %d",i,HT[i].weight,HT[i].parent,HT[i].lchild,HT[i].rchild);
    fclose(fp);
    return OK;
}
Status Encoding(HuffmanTree HT,Huffmancode HC,int n)
{
    FILE *fp_r=fopen("ToBeTran.txt","r");
    FILE *fp_w=fopen("CodeFile.txt","r+");
    //��ÿ���ַ����б���
    int i,c,f;
    int start=n-1;
    char *cd=(char *)malloc((n)*sizeof(char));
    cd[n-1]='\0';
    for ( i = 1; i <= n; i++)
    {
        start=n-1;
        for ( c=i,f=HT[i].parent ; f!=0 ; c=f,f=HT[f].parent )
        {
            if (HT[f].lchild==c)cd[--start]='0';
            else cd[--start]='1';
        }
        HC[i].code=(char*)malloc((n-start)*sizeof(char));
        strcpy(HC[i].code,&cd[start]);
    }
    free(cd);
    //���ļ��ַ������б��벢����
    char ch=' ';
    char *str;
    while (ch==' '||(ch>='A'&&ch<='Z'))
    {
        ch=fgetc(fp_r);
        if (ch==' ')
        fprintf(fp_w," \n");
        else if(ch>='A'&&ch<='Z')
        {
            str=findc(HC,ch,n);
            fprintf(fp_w,"%s\n",str);
        }
    }
    fclose(fp_r);
    fclose(fp_w);
    return OK;
}
Status ReadHfmTree(HuffmanTree *HT,Huffmancode *HC,int *n)
{
    *n=(FileLineNum("hfmTree.txt")+1)/2;
    FILE *fp=fopen("hfmTree.txt","r");
    int i;
    int t;
    int m=2*(*n)-1;
    (*HT)=(HuffmanTree)malloc((m+1)*sizeof(HTNode));
    (*HC)=(Huffmancode)malloc((*n+1)*sizeof(HCNode));
    for ( i = 1; i <= *n; i++)
    {
        fscanf(fp,"%d %d %d %d %d %c",&t,&(*HT)[i].weight,&(*HT)[i].parent,&(*HT)[i].lchild,&(*HT)[i].rchild,&(*HC)[i].ch);
    }
    for ( i = *n+1; i <= 2*(*n)-1; i++)
    {
        fscanf(fp,"%d %d %d %d %d",&t,&(*HT)[i].weight,&(*HT)[i].parent,&(*HT)[i].lchild,&(*HT)[i].rchild);
    }
    fclose(fp);
    return OK;
}
int FileLineNum(char name[])
{
    FILE *fp=fopen(name,"r");
    char s[100];
    int lines=0;
    while ((fgets(s,100,fp))!=NULL)
        if(s[strlen(s)-1]=='\n')lines++;
    fclose(fp);
    return lines+1;
}
char *findc(Huffmancode HC,char ch,int n)//�ҵ��ַ�ch��Ӧ�ı����ָ��
{
    int i=1;
    while (ch!=HC[i].ch&&i<=n)
    {
        i++;
    }
    return HC[i].code;
}
Status Print(void)
{
    FILE *fp_r=fopen("CodeFile.txt","r");
    FILE *fp_w=fopen("CodePrin.txt","r+");
    char str[100];
    int linemax=0;
    int i;
    while ((fgets(str,100,fp_r))!=NULL)
    {
        if(str[0]!=' ')
        {
            str[strlen(str)-1]='\0';
            if (linemax+strlen(str)>50)
            {
                for ( i = 0; i <50-linemax ; i++)
                {
                    printf("%c",str[i]);
                    fprintf(fp_w,"%c",str[i]);
                }
                printf("\n");
                fprintf(fp_w,"\n");
                linemax=linemax+strlen(str)-50;
                for ( ; i <strlen(str) ; i++)
                {
                    printf("%c",str[i]);
                    fprintf(fp_w,"%c",str[i]);
                }   
            }
            else
            {
                printf("%s",str);
                fprintf(fp_w,"%s",str);
                linemax+=strlen(str);
            }
        }
    }
    printf("\n");
    fclose(fp_r);
    fclose(fp_w);
    return OK;
}
Status Decoding(Huffmancode HC,int n)
{
    FILE *fp_r=fopen("CodeFile.txt","r");
    FILE *fp_w=fopen("TextFile.txt","r+");
    char str[100];
    int i;
    while ((fgets(str,100,fp_r))!=NULL)
    {
        str[strlen(str)-1]='\0';
        if(strcmp(str," ")==0)fprintf(fp_w," ");
        else
        {
            for ( i = 1; i <= n; i++)
            {
                if (strcmp(str,HC[i].code)==0)
                break;
            }
            fprintf(fp_w,"%c",HC[i].ch);
        }
    }
    fclose(fp_r);
    fclose(fp_w);
    return OK;
}
Status FreeTree(HuffmanTree HT,int m)
{
    int i;
    for ( i = 0; i <= m; i++)
    {
        free(&HT[i]);
    }
    return OK;
}
Status FreeCode(Huffmancode HC,int n)
{
    int i;
    for ( i = 0; i <= n; i++)
    {
        free(&HC[i]);
    }
    return OK;
}