#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <stdarg.h>

#define BUFSIZE		1024
#define HAN_T(a)	han_table[(a)-'A']

#define HAN_FLAG_C		1
#define HAN_FLAG_T		1 << 1
#define HAN_FLAG_E		1 << 2

typedef struct han_reg {
	unsigned	flag;
	unsigned 	cho;
	unsigned 	jung;
	unsigned 	jong;
	FILE		*fd;
	wchar_t		p;
} han_reg;


void help(void);
void print_error(char *fmt, ...);
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
