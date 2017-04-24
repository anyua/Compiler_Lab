#include "symbol_table.h"

Identifier * enter(Identifier** table, char * name, int offset, int type)
{
	int index = hashpjw(name);
	Identifier * p = table[index];
	if (p)
	{
		for (; p->next_hash != NULL; p = p->next_hash);
		p->next_hash = (Identifier*)malloc(sizeof(Identifier));
		p->name = (char*)malloc(sizeof(char)*strlen(name));
		strcpy(p->name, name);
		p->offset = offset;
		p->type = type;
		p->next_hash = NULL;
	}
	else
	{
		table[index] = (Identifier*)malloc(sizeof(Identifier));
		p = table[index];
		p->name = (char*)malloc(sizeof(char)*strlen(name));
		strcpy(p->name, name);
		p->offset = offset;
		p->type = type;
		p->next_hash = NULL;
	}
	return p;
}

Identifier * insert(Identifier** table, char * name)
{
	int index = hashpjw(name);
	Identifier * p = table[index];
	if (p)
	{
		for (; p->next_hash != NULL; p = p->next_hash);
		p->next_hash = (Identifier*)malloc(sizeof(Identifier));
		p->name = (char*)malloc(sizeof(char)*strlen(name));
		strcpy(p->name, name);
		p->next_hash = NULL;

	}
	else
	{
		table[index] = (Identifier*)malloc(sizeof(Identifier));
		p = table[index];
		p->name = (char*)malloc(sizeof(char)*strlen(name));
		strcpy(p->name, name);
		p->next_hash = NULL;

	}
	//for (p = table[index]; p != NULL; p = p->next_hash);
	//p = (Identifier*)malloc(sizeof(Identifier));
	//p->name = (char*)malloc(sizeof(char)*strlen(name));
	//strcpy(p->name, name);
	return p;
}

Identifier * lookup(Identifier** table, char * name)
{
	int index = hashpjw(name);
	Identifier * p;
	for (p = table[index]; p != NULL&&strcmp(p->name, name); p = p->next_hash);
	return p;
}

int hashpjw(char * s)
{
	char * p;
	unsigned h = 0, g;
	for (p = s; *p != EOS; p++)
	{
		h = (h << 4) + (*p);
		if (g = h & 0xf0000000)
		{
			h = h ^ (g << 24);
			h = h^g;
		}
	}
	return h%PRIME;
}

Identifier ** mktable()
{
	Identifier ** symbol_table = (Identifier*)malloc(sizeof(Identifier)*BUCKETS);
	for (int i = 0; i < BUCKETS; i++)
	{
		symbol_table[i] = NULL;
	}
	return symbol_table;
}
