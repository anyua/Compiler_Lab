#include "lexical_analyzer.h"

DoubleBuffer source_buffer = { { '\0' },source_buffer.buffer,source_buffer.buffer,0 };
Identifier * Lexical_SymbolTable[BUCKETS] = { 0 };
char* file_name;

Tuple_2* token_scan(FILE* fp)
{
	Tuple_2* tokenization_tuple = (Tuple_2*)malloc(sizeof(Tuple_2));
	char ch;
	char* token;
	ch = get_char(fp);
	while (ch == ' ' || ch == '\t' || ch == '\n')
	{
		forward();
		ch = get_char(fp);
	}
	if (isaplha(ch))
	{
		ch = get_char(fp);
		while (isalnum(ch))
			ch = get_char(fp);
		retract();
		token = copy_token();
		tokenization_tuple->key = get_token(token);
		tokenization_tuple->value = install_id(token);
	}
	else if (isdigit(ch))
	{
		ch = get_char(fp);
		while (isdigit(ch))
			ch = get_char(fp);
		retract();
		token = copy_token();
		tokenization_tuple->key = NUM;
		tokenization_tuple->num = install_num(token);
	}
	else
	{
		switch (ch)
		{
		case '+':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = PLUS_A;
			else
			{
				retract();
				tokenization_tuple->key = PLUS;
			}
			break;
		case '-':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = MINUS_A;
			else
			{
				retract();
				tokenization_tuple->key = MINUS;
			}
			break;
		case '*':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = MULTI_A;
			else
			{
				retract();
				tokenization_tuple->key = MULTI;
			}
			break;
		case '/':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = RDIV_A;
			else
			{
				retract();
				tokenization_tuple->key = RDIV;
			}
			break;
		case '%':
			tokenization_tuple->key = MOD;
			break;
		case ';':
			tokenization_tuple->key = SEMIC;
			break;
		case ':':
			tokenization_tuple->key = CONLON;
			break;
		case ',':
			tokenization_tuple->key = COMMA;
		case '=':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = EQ;
			else
			{
				retract();
				tokenization_tuple->key = ASSIGN;
			}
			break;
		case '<':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = LE;
			else
			{
				retract();
				tokenization_tuple->key = LT;
			}
			break;
		case '>':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = GE;
			else
			{
				retract();
				tokenization_tuple->key = GT;
			}
			break;
		case '!':
			ch = get_char(fp);
			if (ch == '=')
				tokenization_tuple->key = NOT;
			else
			{
				retract();
				tokenization_tuple->key = NE;
			}
			break;
		case '&':
			ch = get_char(fp);
			if (ch == '&')
				tokenization_tuple->key = AND;
			else
			{
				error_handle(ch);
			}
			break;
		case '|':
			ch = get_char(fp);
			if (ch == '|')
				tokenization_tuple->key = OR;
			else
			{
				error_handle(ch);
			}
			break;
		case '{':
			tokenization_tuple->key = LR_CURL;
			break;
		case '}':
			tokenization_tuple->key = RR_CURL;
			break;
		case '(':
			tokenization_tuple->key = LR_BRAC;
			break;
		case ')':
			tokenization_tuple->key = RR_BRAC;
			break;
		case '[':
			tokenization_tuple->key = LR_SQAR;
			break;
		case ']':
			tokenization_tuple->key = RR_SQAR;
			break;
		case EOF:
			free(tokenization_tuple);
			tokenization_tuple = NULL;
			break;
		default:
			error_handle(ch);
			tokenization_tuple->key = -1;
			break;
		}
	}
	forward();
	if (tokenization_tuple)
		tokenization_tuple->next = NULL;
	return tokenization_tuple;
}

int isaplha(char ch)
{
	return (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z');
}

int isdigit(char ch)
{
	return ch >= '0'&&ch <= '9';
}

int isalnum(char ch)
{
	return isaplha(ch) || isdigit(ch);
}

char get_char(FILE* fp)
{
	//errno_t err;
	//FILE * fp;
	char ch;
	int count = source_buffer.count;
	if (count == 0 || count == SIZE_OF_BUFFER / 2)
	{
		//err = fopen_s(&fp, file_name, "r");
		for (int i = 0; i < SIZE_OF_BUFFER / 2; i++)
		{
			ch = fgetc(fp);
			source_buffer.buffer[count++] = ch;
			if (ch == EOF)
				break;
		}
	}
	if (++source_buffer.count == 1024)
	{
		source_buffer.count = 0;
		source_buffer.forward = source_buffer.buffer;
	}
	return *(source_buffer.forward++);
}

void retract()
{
	source_buffer.forward--;
	source_buffer.count--;
}

void forward()
{
	source_buffer.lexeme_beginning = source_buffer.forward;
}

char * copy_token()
{
	int len = source_buffer.forward - source_buffer.lexeme_beginning;
	char* token = (char*)malloc(sizeof(char)*len);
	memcpy(token, source_buffer.lexeme_beginning, len);
	token[len] = '\0';
	return token;
}

int get_token(char * token)
{
	int flag;
	if (!strcmp(token, "void"))
		flag = VOID;
	else if (!strcmp(token, "char"))
		flag = CHAR;
	else if (!strcmp(token, "int"))
		flag = INT;
	else if (!strcmp(token, "short"))
		flag = SHORT;
	else if (!strcmp(token, "long"))
		flag = LONG;
	else if (!strcmp(token, "float"))
		flag = FLOAT;
	else if (!strcmp(token, "double"))
		flag = DOUBLE;
	else if (!strcmp(token, "signed"))
		flag = SIGNED;
	else if (!strcmp(token, "unsigned"))
		flag = UNSIGNED;
	else if (!strcmp(token, "if"))
		flag = IF;
	else if (!strcmp(token, "else"))
		flag = ELSE;
	else if (!strcmp(token, "break"))
		flag = BREAK;
	else if (!strcmp(token, "continue"))
		flag = CONTINUE;
	else if (!strcmp(token, "do"))
		flag = DO;
	else if (!strcmp(token, "while"))
		flag = WHILE;
	else if (!strcmp(token, "for"))
		flag = FOR;
	else if (!strcmp(token, "return"))
		flag = RETURN;
	else
		flag = ID;

	return flag;
}

Identifier * install_id(char * token)
{
	Identifier* p = lookup(Lexical_SymbolTable, token);
	if (p == NULL)
		p = insert(Lexical_SymbolTable, token);
	return p;
}

int install_num(char * token)
{
	int result = atoi(token);
	return result;
}

void error_handle(char ch)
{
	printf("´íÎó×Ö·û£º%c\n", ch);
	return;
}
