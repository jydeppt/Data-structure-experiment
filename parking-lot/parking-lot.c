#include<stdio.h>
#include<stdlib.h>
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define FULL -1 //停车库是否已满的标志
typedef int Status;
#define N 2 //定义停车库容量
#define PRICE 2 //定义单位时间缴费单价
typedef struct car //定义车结构
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
//栈操作
Status InitParking(Parking *P);//栈初始化
Status DestoryParking(Parking *P);//栈销毁
Status SPush(Parking *P,Car e);//车入栈
Status SPop(Parking *P,Car *e);//车出栈
//队列操作
Status InitQueue(LinkQueue *Q);//队列初始化
Status DestoryQueue(LinkQueue *Q);//队列销毁
Status EnQueue(LinkQueue *Q,Car e);//入列
Status DeQueue(LinkQueue *Q,Car *e);//出列
//主要函数
Status Arrival(Parking *P,LinkQueue *Q,Car e);//车辆到达处理函数
Status Departure(Parking *P,LinkQueue *Q,Parking *T,Car e);//车辆离开处理函数
//其他函数
int PosInStack(Parking *P,Car e);//查询并返回即将出库的在停车场中的位置
int main(void)
{
    char type;
    int carnum,timeclock;
    Car car;
    Parking parking;
    Parking Temp;       //临时车道
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
        printf("%d号车成功停入车库第%d号位置\n",e.CarNum,P->top-P->base);
    }
    else
    {
        EnQueue(Q,e);
        printf("%d号车成功停入便道第%d号位置\n",e.CarNum,Q->length);
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
        printf("%d号车停留时间%dh,应缴费%d元\n",(P->base)[idx-1].CarNum,time-car.InTime,PRICE*(time-car.InTime));
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
        printf("%d号车停留时间%dh,应缴费%d元\n",(P->base)[idx-1].CarNum,time-car.InTime,PRICE*(time-car.InTime));
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