#include "LR_parser.h"

int main()
{
	char* program_filename = "lala.la";
	Grammer* target_grammer = load_grammer("TINY_C.cfg");
	Table* action_table = load_table("tiny_c_action.txt");
	Table* goto_table = load_table("tiny_c_goto.txt");
	lr_parser(target_grammer, action_table, goto_table, program_filename);
	getchar();
	return 0;
}