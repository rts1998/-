#include<stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct
{
	int c_custkey;    	   //�˿ͱ��
	char c_mkgsegment[20]; //��Ӧ��ĳ���г�����
}customer;				   //�˿ͽṹ�� 

typedef struct
{
	int o_orderkey;    	 //������ 
	int o_custkey;    	 //�˿ͱ��
	char o_orderdate[10];//�������� 
}orders;				 //����

typedef struct
{
	int l_orderkey;//������
	double l_extendedprice;//����۸�
	char l_shipdate[10];//�������� 
}lineitem; //��Ʒ��Ϣ 

typedef struct
{
	int l_orderkey;//������
	char o_orderdate[10];//�������� 
	double l_extendedprice;//����۸�
}select_result;


customer * read_customer_txt() //��ȡcustomer��txt���� 
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
orders * read_orders_txt()//��ȡorders.txt���� 
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

lineitem * read_lineitem_txt()//��ȡlineitem.txt����
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

select_result * Select(customer * cus,orders * ord,lineitem * item,char * order_date,char * ship_date,char * mktsegment)//����ѡ�� 
{	
	int i,j,k;
	int temp;
	int index=0;
	select_result * a=NULL;
	a = (select_result *)malloc(4001*sizeof(select_result));
	//ɸѡ 
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
	//��� 
	index--;
	for(i=0;i<index;i++)
		for(j=0;j<index+1;j++)
			if(a[i].l_orderkey==a[j].l_orderkey)
			{
				a[i].l_extendedprice=a[i].l_extendedprice+a[j].l_extendedprice;
				a[j].l_extendedprice=0;
			}
	//����
	for(i=0;i<index-1;i++)
		for(j=i;j<index;j++)
			if(a[j].l_extendedprice>a[i].l_extendedprice)
			{
				temp=a[i].l_extendedprice;
				a[i].l_extendedprice=a[j].l_extendedprice;
				a[j].l_extendedprice=temp;
			}
	//ɾ��������
	 /*for(i=0;i<index;i++)
		if(a[i].l_extendedprice==0) 
			free(a[i]);*/
	
	return a;
}
						
	
	

//�������ƣ�����������������

	

int change_argv_to_number(char s[])//�������������������ַ���ת��Ϊ�������� 
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

int main(int argc,char * argv[])//argc��ʾ�������ݵ��ܸ�����argv[]�ڱ�������������� 
{
	int i,j;
	int num;
	
	int limit=3;
	char order_date[15];
	char ship_date[10];
	char mktsegment[20];
	printf("helle");
	select_result *result=NULL;
	customer * cus = NULL;//ָ��ͻ����ָ�� 
	orders * ord = NULL;//ָ�򶩵����ָ�� 
	lineitem * item = NULL;//ָ�� ��Ʒ���ָ�� 
	cus = read_customer_txt();//��ȡcustomer.txt������ ������ͻ��� 
	ord = read_orders_txt();//��ȡorders.txt������ �����붩���� 
	item = read_lineitem_txt();//��ȡlineitem.txt������ �������Ʒ�� 
	num = change_argv_to_number(argv[4]);//�ܹ�����Ĵ���
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
