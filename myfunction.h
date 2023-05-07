#include "cs402.h"
#include "my402list.h"

typedef struct Transaction_record
{
    int type;	//1='+' 0='-'
	time_t timestamp;
	int amount;
	char descript[25];
} transaction;

extern void BubbleExchange(My402List*, My402ListElem**, My402ListElem**);
extern int BubbleSortList(My402List*, int);

extern void num_to_string(char*, int);
extern int PrintTransaction(transaction*, int);
extern void PrintList(My402List*);

extern int parse_type(char*, transaction*);
extern int parse_time(char*, transaction*);
extern int parse_amount(char*, transaction*);
extern int parse_description(char*, transaction*);
extern transaction* parse_line(char*);

