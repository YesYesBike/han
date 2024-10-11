#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>

#define BUFSIZE		1024
//#define HAN_FIRST	0xAC00
//#define HAN_LAST	0xD7A3
#define HAN_T(a)	_han_table[(a)-'A']

typedef struct __han_reg {
	unsigned flag;
	unsigned cho;
	unsigned jung;
	unsigned jong;
	wchar_t p;
} han_reg;


void help(void);
void han(wchar_t *str, han_reg *reg);
void han_trans(char chr, han_reg *reg);
void han_ja(char chr, han_reg *reg);
void han_mo(char chr, han_reg *reg);
unsigned han_check_reg(han_reg *reg);
void han_insert_p(han_reg *reg);
void han_combine_p(han_reg *reg);
void han_print_p(han_reg *reg);
void han_cho(char chr, han_reg *reg);
void han_jung(char chr, han_reg *reg);
void han_jong(char chr, han_reg *reg);
void han_dmo(char chr, han_reg *reg);
void han_dja(char chr, unsigned *cho_jong, han_reg *reg);
void han_split_jong(char chr, han_reg *reg);
unsigned han_break(unsigned *cho_jong);
void han_cho_p(han_reg *reg);
void han_jong_p(han_reg *reg);
