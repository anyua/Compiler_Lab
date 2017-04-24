#include "LR_parser.h"

Stack state_stack = { .size = 0 };

Table * load_table(char * filename)
{
	Table* p = (Table*)malloc(sizeof(Table));
	FILE *fp;
	fp = fopen(filename, "r");
	char buffer[BUFFER_SIZE];
	int width = 0, length = 0;
	char* delim = "\t\n";
	char* temp = NULL;

	while (fgets(buffer, BUFFER_SIZE, fp))
	{
		char* buff = buffer;
		width = 0;
		while ((temp = strtok(buff, delim)))
		{
			p->t[length][width++] = atoi(temp);
			buff = NULL;
		}
		length++;
	}
	p->width = width;
	p->length = length;
	return p;
}

Grammer * load_grammer(char * filename)
{
	Grammer* p = (Grammer*)malloc(sizeof(Grammer));
	FILE *fp;
	fp = fopen(filename, "r");
	char buffer[BUFFER_SIZE];
	char* buff = NULL;
	int n_v = 0, n_t = 0, n_p = 0;
	char* delim = " \t\n";
	char* temp = NULL;

	fgets(buffer, BUFFER_SIZE, fp);
	buff = buffer;
	while ((temp = strtok(buff, delim)))
	{
		int len = 0;
		if ((len = strlen(temp)) > 0)
		{
			p->V[n_v] = (char*)malloc(sizeof(char)*len);
			strcpy(p->V[n_v++], temp);
		}
		buff = NULL;
	}
	p->n_v = n_v;

	fgets(buffer, BUFFER_SIZE, fp);
	buff = buffer;
	while ((temp = strtok(buff, delim)))
	{
		int len = 0;
		if ((len = strlen(temp)) > 0)
		{
			p->T[n_t] = (char*)malloc(sizeof(char)*len);
			strcpy(p->T[n_t++], temp);
		}
		buff = NULL;
	}
	p->n_t = n_t;
	while (fgets(buffer, BUFFER_SIZE, fp))
	{
		char* rule = strtok(buffer, "\n");
		int len = 0;
		if ((len = strlen(rule)) > 0)
		{
			p->P[n_p] = (char*)malloc(sizeof(char)*len);
			strcpy(p->P[n_p++], rule);
		}
	}
	p->n_p = n_p;
	return p;
}

State* pop(Stack* stack)
{
	State* p = stack->data[stack->size];
	stack->data[stack->size--] = NULL;
	return p;
}

State * top(Stack* stack)
{
	State* p = stack->data[stack->size];
	return p;
}

void push(Stack* stack, State* state)
{
	stack->data[++(stack->size)] = state;
}

char * get_a_symbol(FILE* fp)
{
	char* symbol = symbol = (char*)malloc(sizeof(char) * 10);
	if (fscanf(fp, "%s", symbol) != EOF)
		return symbol;
	else
	{
		free(symbol);
		return NULL;
	}
}

int index_of_V(char ** list, int n, char * target, Table* action_table)
{
	if (target == NULL)
		return action_table->width - 1;
	int i = -1;
	for (i = 0; i < n; i++)
	{
		if (!strcmp(list[i], target))
			break;
	}
	return i;
}

int index_of_T(Tuple_2* target, Table* action_table)
{
	if (target == NULL)
		return action_table->width - 1;
	else
		return target->key;
}

int beta_len(char* rule)
{
	int len = -2;
	char* buff = (char*)malloc(sizeof(char)*strlen(rule));
	strcpy(buff, rule);
	char* delim = " \t\n";
	char* temp = NULL;
	while (temp = strtok(buff, delim))
	{
		len++;
		buff = NULL;
	}
	return len;
}

char * get_A(char * rule)
{
	char* result = NULL;
	int len = 0;
	for (len = 0; rule[len] != ' '; len++);
	result = (char*)malloc(sizeof(char)*len);
	for (int i = 0; i < len; i++)
	{
		result[i] = rule[i];
	}
	result[len] = '\0';
	return result;
}

void lr_parser(Grammer * grammer, Table * action_table, Table * goto_table, char* program_filename)
{
	State* init_state = (State*)malloc(sizeof(State));
	init_state->value = NULL;
	init_state->state = 0;
	push(&state_stack, init_state);
	// 输入
	FILE* fp;
	fp = fopen(program_filename, "r");
	// 来自词法分析器
	//char* next_symbol = get_a_symbol(fp);
	Tuple_2* next_symbol = token_scan(fp);
	while (1)
	{
		State* temp_state = top(&state_stack);
		//int action_state = action_table->t[temp_state->state]\
		//	[index_of(grammer->T, grammer->n_t, next_symbol, action_table)];
		int action_state = action_table->t[temp_state->state]\
			[index_of_T(next_symbol, action_table)];
		if (action_state >= 0 && action_state<STATE_OFFSET)
		{
			State* new_state = (State*)malloc(sizeof(State));
			new_state->state = action_state;
			new_state->value = (Tuple_2*)malloc(sizeof(Tuple_2));
			//strcpy(new_state->character, next_symbol);
			memcpy(new_state->value, next_symbol, sizeof(Tuple_2));
			push(&state_stack, new_state);
			next_symbol = token_scan(fp);
		}
		else if (action_state >= STATE_OFFSET && action_state <= STATE_OFFSET * 2)
		{
			int rule_index = action_state - STATE_OFFSET;
			char* rule = grammer->P[rule_index];
			int beta = beta_len(rule);
			char* A = get_A(rule);
			Stack parameter_stack = { .size = 0 };
			for (int i = 0; i < beta; i++)
				push(&parameter_stack, pop(&state_stack));
			State* now_state = top(&state_stack);
			int goto_state = goto_table->t[now_state->state]\
				[index_of_V(grammer->V, grammer->n_v, A, action_table) - 1];
			State* new_state = (State*)malloc(sizeof(State));
			new_state->state = goto_state;
			//hook
			//new_state->character = (char*)malloc(sizeof(char)*strlen(rule));
			//strcpy(new_state->character, A);
			//if (next_symbol) {
			//	new_state->value = (Tuple_2*)malloc(sizeof(Tuple_2));
			//	memcpy(new_state->value, next_symbol, sizeof(next_symbol));
			//}
			//else
			//	new_state->value = NULL;
			printf("%d\t%s\n", rule_index, rule);

			call_semantic_define(rule_index,new_state,&parameter_stack);
			push(&state_stack, new_state);
		}
		else if (action_state == ACCEPT_STATE)
		{
			char* rule = grammer->P[0];
			//hook
			printf("%s\n", rule);
			return;
		}
		else
		{
			//hook
			printf("ERROR!\n");
			return;
		}
	}
}

