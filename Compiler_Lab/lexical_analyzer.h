#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#include "type_define.h"

Tuple_2* token_scan(FILE* fp); //ɨ���ַ���

int isaplha(char ch);
int isdigit(char ch);
int isalnum(char ch);

char get_char(FILE* fp); //�ӻ�������ȡ��һ�������ַ�
void retract(); //����ǰָ������һ���ַ���λ��
void forward(); //lexeme_beginningָ��ǰ��
char* copy_token(); //���ؿ�ʼָ�뵽��ǰָ��֮�����ַ���
int get_token(char* token); //��ȡ��������
Identifier * install_id(char* token); //��ȡ��������ֵ
Identifier * install_num(char* token); //�����ַ�����ʾ����ֵ
void error_handle(char ch); //��������


#endif // !LEXICAL_ANALYZER
