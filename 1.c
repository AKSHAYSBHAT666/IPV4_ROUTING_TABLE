#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#define create_table(t) table t;\
		initialise(&t)

//01111011100000000101000000001001
//123.128.80.9/10 
//128.128.128.128/32

typedef struct NODE mtrie_node;
typedef struct TABLE table;
typedef enum bit_type {ZERO,ONE,DONT_CARE,BIT_TYPE_MAX} BIT_TYPE;

struct NODE
{
	int prefix[32];
	int wildcard[32];
	int prefix_len;
	
	mtrie_node* parent;
	mtrie_node* child[3];
};

struct TABLE
{
	mtrie_node mroot;
	//function pointer's to interface each of them.
	void	(*fptr_insert)(table*,char*);
};


int create(table*tt,mtrie_node*mn,int index)
{
	if((tt->mroot).child[index]==NULL)
	{
		// if node is not present.
		(tt->mroot).child[index]=mn;
		printf("\nentered first time\n");
		return 1;
	}
	else
	{
		int ans=1;
		int i=0;
		printf("\nnot first time\n");
		mtrie_node*temp=(tt->mroot).child[0];
		temp->child[0]=mn;
		return 1;
	}
	return 0;
}


void insert(table*tt,char*str)
{
	int sum=0;
	int shift=0;
	
	mtrie_node*mnode=malloc(sizeof(mtrie_node));//node holding current ip;
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
	
	
	if(mnode->prefix[0]==0)
	{
		printf("\nfirst bit 0\n");
		create(tt,mnode,0);
	}
	else
	{
		printf("\nfirst bit 1\n");
		create(tt,mnode,1);
	}
	
	printf("\n");
};

void initialise(table*tt)
{
	(tt->mroot).parent=NULL;
	for(int i=0;i<3;i++) (tt->mroot).child[i]=NULL;
	
	tt->fptr_insert=insert;
}

int main()
{
			create_table(r1);
			printf("is inserted?:%d\n",(r1.mroot).child[0]!=NULL);
			char *str=malloc(sizeof(char)*50);
			scanf("%s",str);
			r1.fptr_insert(&r1,str);
			char *str1=malloc(sizeof(char)*50);
			scanf("%s",str1);
			r1.fptr_insert(&r1,str1);
			printf("\n");
			for(int i=0;i<32;i++)
			{
				printf("%d",r1.mroot.child[0]->child[0]->prefix[i]);
			}
			
}


