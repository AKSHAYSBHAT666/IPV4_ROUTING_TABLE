#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#define create_table(t) table t;\
		initialise(&t)

//01111011100000000101000000001001
//123.128.80.9/10 
//128.128.128.128/32
//128.2.0.0/3
//128.2.4.0/4


typedef struct NODE mtrie_node;
typedef struct TABLE table;
typedef enum bit_type {ZERO,ONE,DONT_CARE,BIT_TYPE_MAX} BIT_TYPE;

struct NODE
{
	int prefix[32];
	int wildcard[32];
	int prefix_len;
	int starting;
	int ending;
	
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
		// if node is not present, insert into the specified index.
		(tt->mroot).child[index]=mn;
		return 1;
	}
	else
	{
		mtrie_node*temp=(tt->mroot).child[index];
		int start=temp->starting;
		while(temp!=NULL)
		{
			for(start=(temp->starting);start<(temp->ending);start++)
			{
				if((temp->prefix[start])!=mn->prefix[start])//point of ip mismatch.
				{
					mtrie_node*daughter=malloc(sizeof(mtrie_node));//create daughter node.
					mtrie_node*	  neice=malloc(sizeof(mtrie_node));//create neice node.
					
					daughter->parent=temp;//assigning the parent back.
					neice->parent	=temp;
					for(int i=0;i<3;i++) 
					{
						daughter->child[i]=temp->child[i];
						neice->child[i]=NULL;
					}
					for(int i=0;i<start;i++) 
					{
						//copy item's untill mismatch.
						daughter->prefix[i]=temp->prefix[i];
						neice->prefix[i]=temp->prefix[i];
					}
					for(int i=start;i<32;i++)
					{      
						//continue from mismatch.    
						daughter->prefix[i]=temp->prefix[i];
						neice->prefix[i]=mn->prefix[i];
					}
					
					daughter->starting=fmin(start,31);
					daughter->ending=temp->ending;
					neice->starting=fmin(31,start);
					neice->ending=temp->ending;
					temp->ending=start;
					
					if(start<32) 
					{
						//assigning the daughter and neice node's to respective mismatch parent node.
						temp->child[mn->prefix[start]]=neice;
						temp->child[!(mn->prefix[start])]=daughter;
					}
					else free(mn);//if already exists, then free the node.
					return 1;
				}
			}
			if(start<32) temp=temp->child[mn->prefix[start]];//continue to next node.
			else {free(mn);return 0;}
		}
		return 0;
	}
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
			//to calculate wildcard and subnet-mask.
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
			mnode->prefix_len=32;
			mnode->starting=0;
			mnode->ending=32;
			for(int k=0;k<sum;k++) mnode->wildcard[k]=2;
			break;
		}
	}
	// processing of ip completed, proceed to insertion.
	if(mnode->prefix[0]==0)
	{
		printf("%d",create(tt,mnode,0));
	}
	else
	{
		printf("%d",create(tt,mnode,1));
	}
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
			
			char *str=malloc(sizeof(char)*50);
			scanf("%s",str);
			r1.fptr_insert(&r1,str);
			printf("\n");
			char *str1=malloc(sizeof(char)*50);
			scanf("%s",str1);
			r1.fptr_insert(&r1,str1);
			printf("\n");
			char *str2=malloc(sizeof(char)*50);
			scanf("%s",str2);
			r1.fptr_insert(&r1,str2);
			printf("\n");
			/*int cnt=0;
			for(int i=0;i<32;i++)
			{
				if(cnt==8) printf(" "),cnt=0;
				cnt++;
				printf("%d",(r1.mroot.child[1])->child[0]->child[1]->prefix[i]);
			}
			printf("\n");*/
			return 0;
}


