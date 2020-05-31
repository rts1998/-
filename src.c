#include<stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct
{
	int c_custkey;    	   //顾客编号
	char c_mkgsegment[20]; //对应的某个市场部门
}customer;				   //顾客结构体 

typedef struct
{
	int o_orderkey;    	 //订单号 
	int o_custkey;    	 //顾客编号
	char o_orderdate[10];//订货日期 
}orders;				 //订单

typedef struct
{
	int l_orderkey;//订单号
	double l_extendedprice;//额外价格
	char l_shipdate[10];//发货日期 
}lineitem; //商品信息 

typedef struct
{
	int l_orderkey;//订单号
	char o_orderdate[10];//订货日期 
	double l_extendedprice;//额外价格
}select_result;


customer * read_customer_txt() //读取customer。txt内容 
{
	int i=0;
	char b;
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(101*sizeof(customer));
	fp = fopen("customer.txt","r");
	if(NULL==fp)
	{
		printf("cannot open customer.txt!");
		return NULL;
	}
	while(!feof(fp))
	{	
		
		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);
		
		i++;
	}
	fclose(fp);
	return a;
}
orders * read_orders_txt()//读取orders.txt内容 
{
	char b,c;
	long long d;
	int i =0; 
	orders * a=NULL;
	FILE *fp;
	a = (orders * )malloc(4001*sizeof(orders));
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("cannot open orders.txt!");
		return NULL;
	}
	while(!feof(fp))
	{	
		
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;
		
		i++;
	}
	fclose(fp);
	return a;
}

lineitem * read_lineitem_txt()//读取lineitem.txt内容
{
	int i=0;
	char b,c;
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1001*sizeof(lineitem));
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("cannot open lineitem.txt!");
		return NULL;
	}
	while(!feof(fp))
	{
		
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);
		
		i++;
	}
	fclose(fp);
	return l; 
}

select_result * Select(customer * cus,orders * ord,lineitem * item,char * order_date,char * ship_date,char * mktsegment)//进行选择 
{	
	int i,j,k;
	int temp;
	int index=0;
	select_result * a=NULL;
	a = (select_result *)malloc(4001*sizeof(select_result));
	//筛选 
	for(i=0;i<100;i++) 
		if(strcmp(mktsegment,cus[i].c_mkgsegment)==0)
			for(j=0;j<4000;j++)
				if(cus[i].c_custkey==ord[j].o_custkey && strcmp(ord[j].o_orderdate,order_date)<0)
					for(k=0;k<1000;k++)
						if(item[k].l_orderkey==ord[j].o_orderkey && strcmp(item[k].l_shipdate,ship_date)>0)
						{
							a[index].l_orderkey=item[k].l_orderkey;
							a[index].l_extendedprice=item[k].l_extendedprice;
							strcpy(a[index].o_orderdate, ord[j].o_orderdate);
							index++;
						}
	//求和 
	index--;
	for(i=0;i<index;i++)
		for(j=0;j<index+1;j++)
			if(a[i].l_orderkey==a[j].l_orderkey)
			{
				a[i].l_extendedprice=a[i].l_extendedprice+a[j].l_extendedprice;
				a[j].l_extendedprice=0;
			}
	//排序
	for(i=0;i<index-1;i++)
		for(j=i;j<index;j++)
			if(a[j].l_extendedprice>a[i].l_extendedprice)
			{
				temp=a[i].l_extendedprice;
				a[i].l_extendedprice=a[j].l_extendedprice;
				a[j].l_extendedprice=temp;
			}
	//删除多余项
	 /*for(i=0;i<index;i++)
		if(a[i].l_extendedprice==0) 
			free(a[i]);*/
	
	return a;
}
						
	
	

//补充完善！！！！！！！！！

	

int change_argv_to_number(char s[])//将命令行里读入的数字字符串转化为整形数字 
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);
		
		i++;
	}
	return number;
}

int main(int argc,char * argv[])//argc表示输入内容的总个数，argv[]内保存着输入的内容 
{
	int i,j;
	int num;
	
	int limit=3;
	char order_date[15];
	char ship_date[10];
	char mktsegment[20];
	printf("helle");
	select_result *result=NULL;
	customer * cus = NULL;//指向客户表的指针 
	orders * ord = NULL;//指向订单表的指针 
	lineitem * item = NULL;//指向 产品表的指针 
	cus = read_customer_txt();//读取customer.txt的内容 ，导入客户表 
	ord = read_orders_txt();//读取orders.txt的内容 ，导入订单表 
	item = read_lineitem_txt();//读取lineitem.txt的内容 ，导入产品表 
	num = change_argv_to_number(argv[4]);//总共计算的次数
	printf("helle");
	for(i=0;num>0;num--,i=i+4)
	{
		strcpy(mktsegment,argv[5+i]);
		strcpy(ship_date,argv[7+i]);
		limit = change_argv_to_number(argv[8+i]);
		//printf("%d	mktsegment:%s	order_date:%s	ship_date:%s	limit:%d\n",num,mktsegment,order_date,ship_date,limit);
		strcpy(order_date,argv[6+i]);
		result=Select(cus,ord,item,order_date,ship_date,mktsegment);
		printf("l_orderkey|o_orderdate|revenue\n");
		for(j=0;j<limit;j++)
		{
			if(result[j].l_extendedprice==0)
				printf("null      |null       |null   \n");
			else
				printf("%-10d|%-11s|%-20.2lf\n",result[j].l_orderkey,result[j].o_orderdate,result[j].l_extendedprice);
		}
		free(result);
		result = NULL;
	} 
	
	return 0;
}
