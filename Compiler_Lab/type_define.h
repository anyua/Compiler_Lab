#ifndef TYPE_DEFINE
#define TYPE_DEFINE
#define _CRT_SECURE_NO_DEPRECATE 


#include<stdio.h>
#include <string.h>

/*
���ű���
*/
#define ID		0 // ����
#define NUM		1 // ��������
#define REAL	2 // ʵ������
#define STRING	3 // �ַ�������
#define PLUS	4 // +
#define MINUS	5 // -
#define MULTI	6 // *
#define RDIV	7 // /
#define MOD		8 // %
#define SEMIC	9 // ;
#define CONLON	10 //:
#define COMMA	11 // ,
#define ASSIGN	12 // =
#define PLUS_A	13 // +=
#define MINUS_A	14 // -=
#define RDIV_A	15 // /=
#define MULTI_A	16 // *=
#define AND		17 // &&
#define OR		18 // ||
#define LT		19 // <
#define GT		20 // >
#define NOT		21 // !
#define EQ		22 // ==
#define LE		23 // <=
#define GE		24 // >=
#define NE		25 // !=
#define LR_CURL 26 // {
#define RR_CURL 27 // }
#define LR_BRAC	28 // (
#define RR_BRAC	29 // )
#define LR_SQAR 30 // [
#define RR_SQAR 31 // ]

#define VOID	32 
#define CHAR	33
#define INT		34
#define SHORT	35
#define LONG	36
#define FLOAT	37
#define DOUBLE	38
#define SIGNED	39
#define UNSIGNED 40
#define IF		41
#define ELSE	42
#define BREAK	43
#define CONTINUE 44
#define DO		45
#define WHILE	46
#define FOR		47
#define RETURN	48

/*
���ֿռ��С����
*/
#define BUFFER_SIZE 2048 //�ķ��ļ���ȡ��������С
#define TABLE_SIZE_LENGTH 5000
#define TABLE_SIZE_WIDTH 60
#define STATE_OFFSET 10000 
#define ACCEPT_STATE 23333
#define STACK_SIZE 1000
#define SIZE_OF_BUFFER 1024 //Դ�ļ���ȡ˫��������С

/*
���ű��ϣ����
*/
#define BUCKETS 109
#define PRIME 211
#define EOS '\0' //�ַ�����β��־

/*
���ֽṹ����������
*/


typedef struct DoubleBuffer //˫������
{
	char buffer[SIZE_OF_BUFFER];
	char* lexeme_beginning;
	char* forward;
	int count;
}DoubleBuffer;

typedef struct Identifier //���ű���
{
	char * name;
	int type;
	int offset;
	ConstantValue* const_value;
	struct Identifier* next_hash;
}Identifier;

typedef struct Constant_value
{
	char* str;
	int num;
	double real;
}ConstantValue;

typedef struct Tuple_2 //�ʷ���������Ķ�Ԫ��
{
	int key;
	Identifier* value;
	int num;
	float real;
	struct Tuple_2* next;
}Tuple_2;

typedef struct State //ջ�洢��
{
	int state;
	Tuple_2* value;
}State;

typedef struct Stack //ջ
{
	int size;
	State* data[STACK_SIZE];
}Stack;

typedef struct Grammer //�ķ�
{
	char* V[100];
	int n_v;
	char* T[100];
	int n_t;
	char* P[200];
	int n_p;
	char S;
}Grammer;

typedef struct Table //�﷨������
{
	int t[TABLE_SIZE_LENGTH][TABLE_SIZE_WIDTH];
	int width;
	int length;
}Table;

typedef struct Three_address_code //����ַ��
{
	int op;
	Identifier* arg1;
	Identifier* arg2;
	Identifier* result;
}TAC;

#endif // !TYPE_DEFINE