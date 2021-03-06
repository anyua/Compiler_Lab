#include "semantics.h"

//变量以及临时变量符号表（哈希表）
Identifier * ID_SymbolTable[BUCKETS] = { 0 };
//变量地址分配偏移量
int addr_offset = 0;
//常量编号计数器
int temp_count = 0;

//常量表（顺序存储）
Identifier * Constant_SymbolTable[BUCKETS] = { 0 };
//常量表偏移量
int constant_offset = 0;

//三地址码表
TAC* three_address_code[1000] = { NULL };
//三地址码写入计数
int nextquad = 0;

int(*semantic_func[])(State* new_state, Stack* parameter_stack) = \
{
	PASS, PASS, PASS, PASS, PASS,	 PASS, F6, pass_param, pass_param, PASS, \
	PASS, PASS, PASS, pass_param, PASS,	 PASS, PASS, pass_param, F18, PASS, \
	pass_param, F21, F22, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	pass_param, PASS, pass_param, F33, pass_param,	 pass_param, pass_param, pass_param, pass_param, pass_param, \
	F40, PASS, pass_param, F43, pass_param,	 F45, pass_param, F50, F50, pass_param, \
	F50, F50, F50, F50, pass_param,	 F55, F55, pass_param, F55, F55, \
	F55, F61, pass_param, PASS, PASS,	 F65, PASS, PASS, F68, pass_param, \
	PASS, pass_param, PASS, pass_param, PASS,		PASS, PASS, PASS, F78, PASS, \
	PASS, PASS, PASS, PASS, pass_param,	 PASS, pass_param, PASS, F88, PASS, \
	F90, PASS, PASS, PASS, PASS,	 PASS, PASS, PASS, PASS, PASS, \
	M, N
};

/*
工具函数
*/

//传递所有属性
Value* pass_param(State * new_state, Stack * parameter_stack)
{
	State* tstate = pop(parameter_stack);
	new_state->value = tstate->value;
	Value* p = NULL;
	for (; tstate; tstate = pop(parameter_stack))
	{
		if (p)
		{
			p->next = tstate->value;
		}
		p = tstate->value;
		for (; p->next ; p = p->next);
	}
	//返回末尾属性
	return p;
}

//不做任何操作
int pass(State * new_state, Stack * parameter_stack)
{
	return 0;
}

//生成一条三地址码
int gencode(int op, Identifier * arg1, Identifier * arg2, Identifier * result)
{
	printf("%d\n", nextquad);
	three_address_code[nextquad] = (TAC*)malloc(sizeof(TAC));
	TAC* tac = three_address_code[nextquad];
	tac->arg1 = arg1;
	tac->arg2 = arg2;
	tac->op = op;
	tac->result = result;
	return nextquad++;
}

//在符号表中添加一个临时变量
Identifier * newtemp(int type)
{
	char temp[10];
	itoa(temp_count++, temp, 8);
	Identifier* result = enter(ID_SymbolTable, temp, 4, type);
	return result;
}

//回填链表管理
BackpatchingList* makelist(int i)
{
	BackpatchingList* newList = (BackpatchingList*)malloc(sizeof(BackpatchingList));
	newList->next = NULL;
	newList->num = i;
	return newList;
}
BackpatchingList* merge(BackpatchingList* p1, BackpatchingList* p2)
{
	BackpatchingList* p;
	if (p1)
	{
		for (p = p1; p->next; p = p->next);
		p->next = p2;
		return p1;
	}
	else if (p2)
	{
		for (p = p2; p->next; p = p->next);
		p->next = p1;
		return p2;
	}
	else
		return NULL;
}
void backpatch(BackpatchingList* p, int i)
{
	BackpatchingList* q;
	if (p)
	{
		q = p;
		three_address_code[q->num]->result = (Identifier*)i;
		for (; q->next; q = q->next)
		{
			three_address_code[q->num]->result = (Identifier*)i;
		}
	}
}

/*
每一个非终结符的操作函数
*/

//Declaration2Specifiers_Declarator
int F68(State * new_state, Stack * parameter_stack)
{
	printf("%d", parameter_stack->size);
	State* specifiers = parameter_stack->data[3];
	State* declarator_list = parameter_stack->data[2];
	Value * declarator = declarator_list->value;
	for (; declarator; declarator = declarator->next)
	{
		if (declarator->tuple->key == ID)
		{
			if (declarator->next&&declarator->next->tuple->key == NUM)
				addr_offset += (declarator->next->tuple->value->const_value->num*specifiers->value->next->width);
			else
				addr_offset += specifiers->value->next->width;
			enter(ID_SymbolTable, declarator->tuple->value->name, \
				addr_offset, specifiers->value->width);
		}
	}
	return 0;
}

//Type2int
int F78(State * new_state, Stack * parameter_stack)
{
	Value* p = pass_param(new_state, parameter_stack);
	//添加一个属性int类型占用的空间
	p->next = (Value*)malloc(sizeof(Value));
	p->next->width = 4;
	p->next->next = NULL;
	return 0;
}

//Declarator2Declarator_BRAC
int F88(State * new_state, Stack * parameter_stack)
{
	//说明声明的是一个函数，暂不进行操作把声明类型设为不可用的-1
	new_state->value = (Value*)malloc(sizeof(Value));
	new_state->value->tuple = (Tuple_2*)malloc(sizeof(Tuple_2));
	new_state->value->tuple->key = -1;
	new_state->value->next = NULL;
	return 0;
}

//Declarator2Declarator_SQAR
int F90(State * new_state, Stack * parameter_stack)
{
	//new_state->value = (Tuple_2*)malloc(sizeof(Tuple_2));
	//memcpy(new_state->value, parameter_stack->data[4]->value, sizeof(Tuple_2));
	//new_state->value->next = (Tuple_2*)malloc(sizeof(Tuple_2));
	//memcpy(new_state->value->next, parameter_stack->data[2]->value, sizeof(Tuple_2));
	//暂不考虑多维数组，把数组名字和数组长度加入属性链表
	new_state->value = parameter_stack->data[4]->value;
	new_state->value->next = parameter_stack->data[2]->value;
	return 0;

}

//Assign2Exp_Op_Assign
int F33(State * new_state, Stack * parameter_stack)
{
	//从栈中取出相应位置的属性生成三地址码
	Identifier* arg1 = parameter_stack->data[1]->value->tuple->value;
	Identifier* result = parameter_stack->data[3]->value->tuple->value;
	int op = parameter_stack->data[2]->value->tuple->key;
	//Identifier* result = newtemp(INT);
	gencode(op,arg1,NULL,result);
	// 添加回填链表
	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->backpatchingList = NULL;
	p->next = NULL;
	return 0;
}

//PosExp2ID
int F40(State * new_state, Stack * parameter_stack)
{
	//对于ID先检查符号表这个id有没有定义，用的话向上传递值没有的话报错
	//pass_param(new_state, parameter_stack);
	Value* p = (Value*)malloc(sizeof(Value));
	State* tstate = parameter_stack->data[1];
	Value* tvalue = tstate->value;
	Identifier* tid = lookup(ID_SymbolTable, tvalue->tuple->value->name);
	if (tid)
	{
		p->tuple = (Tuple_2*)malloc(sizeof(Tuple_2));
		p->tuple->key = 0;
		p->tuple->value = tid;
		p->next = NULL;
		new_state->value = p;
	}
	else
	{
		printf("error!\n");
		pass_param(new_state, parameter_stack);
	}
	return 0;
}

//AddExp2AddExp_puls_MulExp
int F55(State * new_state, Stack * parameter_stack)
{
	//创建一个临时变量，生成一个三地址码，把临时变量地址作为属性
	Identifier* arg1 = parameter_stack->data[3]->value->tuple->value;
	Identifier* arg2 = parameter_stack->data[1]->value->tuple->value;
	int op = parameter_stack->data[2]->value->tuple->key;
	Identifier* result = newtemp(INT);
	gencode(op, arg1, arg2, result);

	Value* p = (Value*)malloc(sizeof(Value));
	p->tuple = (Tuple_2*)malloc(sizeof(Tuple_2));
	p->tuple->key = 0;
	p->tuple->value = result;
	p->next = NULL;
	new_state->value = p;

	return 0;
}


//PrimaryExp2ID
int F61(State * new_state, Stack * parameter_stack)
{
	//同前置表达式的操作
	//pass_param(new_state, parameter_stack);
	Value* p = (Value*)malloc(sizeof(Value));
	State* tstate = parameter_stack->data[1];
	Value* tvalue = tstate->value;
	Identifier* tid = lookup(ID_SymbolTable, tvalue->tuple->value->name);
	if (tid)
	{
		p->tuple = (Tuple_2*)malloc(sizeof(Tuple_2));
		p->tuple->key = 0;
		p->tuple->value = tid;
		p->next = NULL;
		new_state->value = p;
	}
	else
	{
		printf("error!\n");
		pass_param(new_state, parameter_stack);
	}
	return 0;
}

//ConstExp2iconst
int F65(State * new_state, Stack * parameter_stack)
{
	Constant_SymbolTable[constant_offset++] = parameter_stack->data[1]->value->tuple->value;
	pass_param(new_state, parameter_stack);
	return 0;
}

//<
int F50(State * new_state, Stack * parameter_stack)
{
	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->backpatchingList = makelist(nextquad);
	p->next = (Value*)malloc(sizeof(Value));
	p->next->backpatchingList = makelist(nextquad + 1);
	p->next->next = NULL;
	Identifier* arg1 = parameter_stack->data[3]->value->tuple->value;
	Identifier* arg2 = parameter_stack->data[1]->value->tuple->value;
	int op = parameter_stack->data[2]->value->tuple->key;
	gencode(op, arg1, arg2, NULL);
	gencode(GOTO, NULL, NULL, NULL);
	
	return 0;
}

//Selection - statement -> if (Expression) M Statement
int F21(State * new_state, Stack * parapeter_stack)
{
	Value* mValue = parapeter_stack->data[2]->value;
	Value* s1Value = parapeter_stack->data[1]->value;
	Value* expressionValue = parapeter_stack->data[4]->value;
	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = NULL;
	backpatch(expressionValue->backpatchingList, mValue->quad);
	p->backpatchingList = merge(expressionValue->next->backpatchingList, s1Value->backpatchingList);
	return 0;
}


//Selection-statement -> if ( Expression ) M Statement N else M Statement
int F22(State * new_state, Stack * parapeter_stack)
{
	Value* expressionValue = parapeter_stack->data[8]->value;
	Value* m1Value = parapeter_stack->data[6]->value;
	Value* s1Value = parapeter_stack->data[5]->value;
	Value* nValue = parapeter_stack->data[4]->value;
	Value* m2Value = parapeter_stack->data[2]->value;
	Value* s2Value = parapeter_stack->data[1]->value;
	backpatch(expressionValue->backpatchingList, m1Value->quad);
	backpatch(expressionValue->next->backpatchingList, m2Value->quad);

	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = NULL;
	p->backpatchingList = merge(s1Value->backpatchingList, \
		merge(nValue->backpatchingList,s2Value->backpatchingList));
	return 0;
}

//Iteration-statement -> while ( M Expression ) M Statement
int F23(State * new_state, Stack * parapeter_stack)
{
	Value* expressionValue = parapeter_stack->data[4]->value;
	Value* m1Value = parapeter_stack->data[5]->value;
	Value* s1Value = parapeter_stack->data[1]->value;
	Value* m2Value = parapeter_stack->data[2]->value;
	backpatch(s1Value->backpatchingList, m1Value->quad);
	backpatch(expressionValue->backpatchingList, m2Value->quad);

	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = NULL;
	p->backpatchingList = expressionValue->next->backpatchingList;
	gencode(GOTO, NULL, NULL, (Identifier*)m1Value->quad);
	return 0;
}

int F6(State * new_state, Stack * paraeter_stack)
{
	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = NULL;
	p->backpatchingList = paraeter_stack->data[1]->value->next->backpatchingList;
	return 0;
}

// Statement_list->Statement_list M Statement
int F18(State * new_state, Stack * paraeter_stack)
{
	Value* mValue = paraeter_stack->data[2]->value;
	Value* l1Value = paraeter_stack->data[3]->value;
	Value* sValue = paraeter_stack->data[1]->value;

	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = NULL;
	p->backpatchingList = sValue->backpatchingList;

	backpatch(l1Value->backpatchingList, mValue->quad);
}

//Logical - OR - expression->Logical - OR - expression || M Logical - AND - expression
int F43(State * new_state, Stack * paraeter_stack)
{
	Value* b1Value = paraeter_stack->data[4]->value;
	Value* mValue = paraeter_stack->data[2]->value;
	Value* b2Value = paraeter_stack->data[1]->value;
	backpatch(b1Value->next->backpatchingList, mValue->quad);

	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = (Value*)malloc(sizeof(Value));
	p->backpatchingList = merge(b1Value->backpatchingList, b2Value->backpatchingList);
	p->next->backpatchingList = b2Value->next->backpatchingList;
	p->next->next = NULL;
	return 0;
}

//Logical-AND-expression -> Logical-AND-expression && M Equality-expression
int F45(State * new_state, Stack * paraeter_stack)
{
	Value* b1Value = paraeter_stack->data[4]->value;
	Value* mValue = paraeter_stack->data[2]->value;
	Value* b2Value = paraeter_stack->data[1]->value;
	backpatch(b1Value->backpatchingList, mValue->quad);

	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = (Value*)malloc(sizeof(Value));
	p->backpatchingList = b2Value->backpatchingList;
	p->next->backpatchingList = merge(b1Value->next->backpatchingList, b2Value->next->backpatchingList);
	p->next->next = NULL;
	return 0;
}

int M(State * new_state, Stack * paraeter_stack)
{
	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->next = NULL;

	p->quad = nextquad;
	return 0;
}

int N(State * new_state, Stack * paraeter_stack)
{
	Value* p = (Value*)malloc(sizeof(Value));
	new_state->value = p;
	p->backpatchingList = makelist(nextquad);
	p->next = NULL;
	gencode(GOTO, NULL, NULL, NULL);
	return 0;
}