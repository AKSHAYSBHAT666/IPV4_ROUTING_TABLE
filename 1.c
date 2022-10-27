#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>

//01111011100000000101000000001001
//123.128.80.9/10 

typedef struct NODE mtrie_node;
typedef struct TABLE table;
typedef enum bit_type {ZERO,ONE,DONT_CARE,BIT_TYPE_MAX} BIT_TYPE;

struct NODE
{
	uint8_t prefix[32];
	uint8_t wildcard[32];
	uint8_t prefix_len;
	
	mtrie_node* parent;
	mtrie_node* child[3];
};

struct TABLE
{
	mtrie_node mroot;
	//function pointer's to interface each of them.
	void	(*fptr_insert)(table*,char*);
};

void insert(table*tt,char*str)
{
	int sum=0;
	int shift=0;
	
	mtrie_node*mnode=(mtrie_node*)malloc(sizeof(tt->mroot));
	mnode->parent=NULL;
	for(int i=0;i<3;i++) mnode->child[i]=NULL;
	
	//converting ip into prefix.
	for(int i=0;i<100 && str[i]!='\0';i++)
	{
		sum*=10;
		sum+=str[i]-'0';
		if(str[i]=='.')
		{
			// to get the 32-bit representation.
			sum-=str[i]-'0';
			sum=sum/10;
			printf("sum:=%d\n",sum);
			for(int k=0;k<8;k++)
			{
				if(sum & (1<<(k)))	mnode->prefix[7-k+shift]=1;
				else 				mnode->prefix[7-k+shift]=0;
			}
			shift+=8;
			sum=0;
		}
		else if(str[i]=='/')
		{
			// to calculate wildcard and subnet-mask.
			sum-=str[i]-'0';
			sum=sum/10;
			printf("sum:=%d\n",sum);
			for(int k=0;k<8;k++)
			{
				if(sum & (1<<(k)))	mnode->prefix[7-k+shift]=1;
				else 				mnode->prefix[7-k+shift]=0;
			}
			sum=0;
			
			for(int j=i+1;j<100 && str[j]!='\0';j++)
			{
				sum*=10;
				sum+=str[j]-'0';
			}
			mnode->prefix_len=sum;
			for(int k=0;k<sum;k++) mnode->wildcard[k]=2;
			break;
		}
	}
	// processing of ip completed, proceed to insertion.
};

int main()
{
		table r1;
		char *str=malloc(sizeof(char)*50);
		scanf("%[^\n]s",str);
		insert(&r1,str);
}


