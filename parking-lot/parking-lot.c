#include<stdio.h>
#include<stdlib.h>
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define FULL -1 //ͣ�����Ƿ������ı�־
typedef int Status;
#define N 2 //����ͣ��������
#define PRICE 2 //���嵥λʱ��ɷѵ���
typedef struct car //���峵�ṹ
{
    int CarNum;
    int InTime;
    int OutTime;
    struct car *next;
}Car;
typedef struct 
{
    Car *base;
    Car *top;
    int parkingsize;
}Parking;
typedef struct 
{
    Car *front;
    Car *rear;
    int length;
}LinkQueue;
//ջ����
Status InitParking(Parking *P);//ջ��ʼ��
Status DestoryParking(Parking *P);//ջ����
Status SPush(Parking *P,Car e);//����ջ
Status SPop(Parking *P,Car *e);//����ջ
//���в���
Status InitQueue(LinkQueue *Q);//���г�ʼ��
Status DestoryQueue(LinkQueue *Q);//��������
Status EnQueue(LinkQueue *Q,Car e);//����
Status DeQueue(LinkQueue *Q,Car *e);//����
//��Ҫ����
Status Arrival(Parking *P,LinkQueue *Q,Car e);//�������ﴦ����
Status Departure(Parking *P,LinkQueue *Q,Parking *T,Car e);//�����뿪������
//��������
int PosInStack(Parking *P,Car e);//��ѯ�����ؼ����������ͣ�����е�λ��
int main(void)
{
    char type;
    int carnum,timeclock;
    Car car;
    Parking parking;
    Parking Temp;       //��ʱ����
    LinkQueue queue;
    InitParking(&parking);
    InitParking(&Temp);
    InitQueue(&queue);
    scanf("%c %d %d",&type,&carnum,&timeclock);
    while (type!='E')
    {
        car.CarNum=carnum;
        switch (type)
        {
        case 'A':
            car.InTime=timeclock;
            Arrival(&parking,&queue,car);
            break;
        case 'D':
            car.OutTime=timeclock;
            Departure(&parking,&queue,&Temp,car);
            break;
        default:
            break;
        }
        scanf("%c %d %d",&type,&carnum,&timeclock);
    }
    DestoryParking(&parking);
    DestoryParking(&Temp);
    DestoryQueue(&queue);
    system("pause");
    return 0;
}
Status InitParking(Parking *P)
{
    P->base=(Car*)malloc(N*sizeof(Car));
    if (!P->base)exit(OVERFLOW);
    P->top=P->base;
    P->parkingsize=N;
    return OK;
}
Status InitQueue(LinkQueue *Q)
{
    Q->front=Q->rear=(Car*)malloc(sizeof(Car));
    if(!Q->front)exit(OVERFLOW);
    Q->front->next=NULL;
    Q->length=0;
    return OK;
}
Status DestoryParking(Parking *P)
{
    free(P->base);
    return OK;
}
Status DestoryQueue(LinkQueue *Q)
{
    while (Q->front)
    {
        Q->rear=Q->front->next;
        free(Q->front);
        Q->front=Q->rear;
    }
    return OK;
}
Status SPush(Parking *P,Car e)
{
    if (P->top-P->base>=P->parkingsize)return FULL;
    *(P->top)=e;
    P->top++;
    return OK;
}
Status SPop(Parking *P,Car *e)
{
    if (P->base==P->top)return ERROR;
    P->top--;
    *e=*(P->top);
    return OK;
}
Status EnQueue(LinkQueue *Q,Car e)
{
    Car *p=(Car*)malloc(sizeof(Car));
    if(!p)exit(OVERFLOW);
    (*p)=e;
    p->next=NULL;
    Q->rear->next=p;
    Q->rear=p;
    Q->length++;
    return OK;
}
Status DeQueue(LinkQueue *Q,Car *e)
{
    Car *p;
    if(Q->front==Q->rear)return ERROR;
    p=Q->front->next;
    *e=(*p);
    Q->front->next=p->next;
    if(Q->rear==p)Q->rear=Q->front;
    free(p);
    Q->length--;
    return OK;
}
Status Arrival(Parking *P,LinkQueue *Q,Car e)
{
    if (SPush(P,e)!=FULL)
    {
        printf("%d�ų��ɹ�ͣ�복���%d��λ��\n",e.CarNum,P->top-P->base);
    }
    else
    {
        EnQueue(Q,e);
        printf("%d�ų��ɹ�ͣ������%d��λ��\n",e.CarNum,Q->length);
    }
    return OK;    
}
Status Departure(Parking *P,LinkQueue *Q,Parking *T,Car e)
{
    int i,len;
    int time=0;
    Car car;
    int idx=PosInStack(P,e);
    (P->base)[idx-1].OutTime=e.OutTime;
    len=P->top-P->base;
    if (idx==len)
    {
        SPop(P,&car);
        time=car.OutTime;
        printf("%d�ų�ͣ��ʱ��%dh,Ӧ�ɷ�%dԪ\n",(P->base)[idx-1].CarNum,time-car.InTime,PRICE*(time-car.InTime));
        if(Q->front!=Q->rear)
        {
            DeQueue(Q,&car);
            car.InTime=time;
            Arrival(P,Q,car);
        }
    }
    else
    {
        for ( i = len-1; i > idx-1; i--)
        {
            SPop(P,&car);
            SPush(T,car);
        }
        SPop(P,&car);
        time=car.OutTime;
        printf("%d�ų�ͣ��ʱ��%dh,Ӧ�ɷ�%dԪ\n",(P->base)[idx-1].CarNum,time-car.InTime,PRICE*(time-car.InTime));
        for ( i = len-1; i > idx-1; i--)
        {
            SPop(T,&car);
            SPush(P,car);
        }
        if(Q->front!=Q->rear)
        {
            DeQueue(Q,&car);
            car.InTime=time;
            Arrival(P,Q,car);
        }
    }
    return OK;
}
int PosInStack(Parking *P,Car e)
{
    int i;
    for ( i = 0; i < P->top-P->base; i++)
    {
        if ((P->base)[i].CarNum==e.CarNum)break;
    }
    return i+1;
}