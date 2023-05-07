#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cs402.h"
#include "my402list.h"
#include "myfunction.h"

void BubbleExchange(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}

int BubbleSortList(My402List *list, int num_items)
{
    My402ListElem *elem=NULL;
    for (int i=0; i < num_items; i++) {
        int something_swapped=FALSE;
        My402ListElem *next_elem=NULL;
		int j;
        for (elem=My402ListFirst(list), j=0; j < num_items-i-1; elem=next_elem, j++) {
            time_t cur_val=((transaction*)(elem->obj))->timestamp;
            next_elem=My402ListNext(list, elem);
            time_t next_val = ((transaction*)(next_elem->obj))->timestamp;

			if(cur_val==next_val)
			{
				fprintf(stderr,"Failed to sort as two transactions have identical timestamp!\n");
				return 0;
			}
            if(cur_val > next_val) {
                BubbleExchange(list, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
	return 1;
}

void num_to_string(char* s, int num)
{
	char temp[13];
	if(num<0) num=-num;
	int interger=num/100;
	int cent=num%100;
	if(interger>=10000000)
	{
		sprintf(temp,"?,???,???.??");
	}
	else if(interger>=1000000)
	{
		int million=interger/1000000;
		interger%=1000000;
		int thousand=interger/1000;
		int unit=interger%1000;
		sprintf(temp,"%d,%03d,%03d.%02d",million,thousand,unit,cent);
	}
	else if(interger>=1000)
	{
		int thousand=interger/1000;
		int unit=interger%1000;
		sprintf(temp,"%d,%03d.%02d",thousand,unit,cent);
	}
	else sprintf(temp,"%d.%02d",interger,cent);
	
	while(strlen(s)+strlen(temp)<12) strcat(s, " ");
	strncat(s, temp, sizeof(temp)-1);
	return;
}

int PrintTransaction(transaction *t, int balance)
{
	char ans[81];
	char buf[25];
	memset(buf,0,sizeof(buf));
	strncpy(buf, ctime(&(t->timestamp)), sizeof(buf)-1);
	buf[10]='\0';

	while(strlen(t->descript)<24) strcat(t->descript, " ");

	if(t->type==1) balance+=t->amount;
	else balance-=t->amount;
	
	char buf2[13];
	memset(buf2,0,sizeof(buf2));
	num_to_string(buf2,t->amount);
	char buf3[13];
	memset(buf3,0,sizeof(buf3));
	num_to_string(buf3,balance);

	sprintf(ans,"| %s %s | %s |  %s  |  %s  |",buf,buf+20,t->descript,buf2,buf3);
	if(t->type==0)
	{
		ans[47]='(';
		ans[60]=')';
	}
	if(balance<0)
	{
		ans[64]='(';
		ans[77]=')';
	}

	printf("%s\n",ans);
	return balance;
}

void PrintList(My402List *list)
{
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|       Date      | Description              |         Amount |        Balance |\n");
	printf("+-----------------+--------------------------+----------------+----------------+\n");
    My402ListElem *elem=NULL;
	int cur_balance=0;
    for (elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
		cur_balance=PrintTransaction((transaction*)(elem->obj), cur_balance);
    }
	printf("+-----------------+--------------------------+----------------+----------------+\n");
}

int parse_type(char* s, transaction* t)
{
	if(strlen(s)!=1 || (s[0]!='+' && s[0]!='-'))
	{
		fprintf(stderr,"Invalid transaction type since it should only be '+' or '-'!\n");
		return 0;
	}
	if(s[0]=='+') t->type=1;
	else t->type=0;
	return 1;
}

int parse_time(char* s, transaction* t)
{
	if(strlen(s)==0 || s[0]=='0' || strlen(s)>=11)
	{
		fprintf(stderr,"Invalid input in timestamp!\n");
		return 0;
	}
	for(int i=0;i<strlen(s);i++)
	{
		if(s[i]<'0' || s[i]>'9')
		{
			fprintf(stderr,"Invalid input in timestamp!\n");
			return 0;
		}
	}
	time_t tm=atoi(s);
	time_t cur_tm=time(NULL);
	if(tm<=0 || tm>=cur_tm)
	{
		fprintf(stderr,"Inappropriate timestamp since it should be >0 and <current time!\n");
		return 0;
	}
	t->timestamp=tm;
	return 1;
}

int parse_amount(char* s, transaction* t)
{
	char* i=s;
	while(*i!='.')
	{
		if(*i<'0' || *i>'9')
		{
			fprintf(stderr,"Invalid input in amount!\n");
			return 0;
		}
		i++;
	}
	*i='\0';
	char* cent=i+1;
	if(strlen(s)<=0 || (strlen(s)>1 && s[0]=='0') || strlen(s)>7)
	{
		fprintf(stderr,"Invalid amount due to inappropriate interger!\n");
		return 0;
	}
	if(strlen(cent)!=2 || *cent<'0' || *cent>'9' || *(cent+1)<'0' || *(cent+1)>'9')
	{
		fprintf(stderr,"Invalid amount due to inappropriate decimal!\n");
		return 0;
	}
	t->amount=atoi(s)*100+atoi(cent);
	return 1;
}	

int parse_description(char* s, transaction* t)
{
	char* i=s;
	while(*i==' ') i++;
	char* ds=i;
	if(strlen(ds)<=0)
	{
		fprintf(stderr,"Empty string in description!\n");
		return 0;
	}
	memset(t->descript,0,sizeof(t->descript));
	strncpy(t->descript, ds, sizeof(t->descript)-1);
	return 1;
}

transaction* parse_line(char* s)
{
	int tab=0;
	char *p=s,*i=s;
	char *type, *time, *amount, *descript;
	while(*i!='\0')
	{
		if(*i=='\t')
		{
			tab++;
			switch(tab)
			{
				case 1:
				{
					type=p;
					*i='\0';
					p=i+1;
					break;
				}
				case 2:
				{
					time=p;
					*i='\0';
					p=i+1;
					break;
				}
				case 3:
				{
					amount=p;
					*i='\0';
					p=i+1;
					descript=p;
					break;
				}
				default:
					fprintf(stderr,"Too many fields since there should only be 4!\n");
					return NULL;
			}
		}
		i++;
	}
	if(tab!=3)
	{
		fprintf(stderr,"Inappropriate number of fields since there should be 4!\n");
		return NULL;
	}
	
	transaction* t=(transaction*)malloc(sizeof(transaction));
	if(t==NULL)
	{
		fprintf(stderr,"Failed to create new transaction log!\n");
		return NULL;
	} 

	if(parse_type(type, t)==0) return NULL;
	if(parse_time(time, t)==0) return NULL;
	if(parse_amount(amount, t)==0) return NULL;
	if(parse_description(descript, t)==0) return NULL;
		
	return t;
}

