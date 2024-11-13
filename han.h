#include <locale.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#define BUFSIZE			BUFSIZ
#define HAN_T(a)		han_table[(a) - 'A']

#define HAN_FLAG_C		1
#define HAN_FLAG_T		1 << 1
#define HAN_FLAG_E		1 << 2

typedef struct han_reg {
	uint32_t	flag;
	unsigned 	cho;
	unsigned 	jung;
	unsigned 	jong;
	FILE		*fd;
	wchar_t		p;
} han_reg;


void		help(void);
void		print_error(char *fmt, ...);
void		han(wchar_t *str, han_reg *reg, char esc_ch);
void		han_trans(char chr, han_reg *reg);
void		han_ja(char chr, han_reg *reg);
void		han_mo(char chr, han_reg *reg);
unsigned	han_check_reg(han_reg *reg);
void		han_insert_p(han_reg *reg);
void		han_combine_p(han_reg *reg);
void		han_print_p(han_reg *reg);
void		han_putc(han_reg *reg, wchar_t chr);
void		han_cho(char chr, han_reg *reg);
void		han_jung(char chr, han_reg *reg);
void		han_jong(char chr, han_reg *reg);
void		han_dmo(char chr, han_reg *reg);
void		han_dja(char chr, unsigned *cho_jong, han_reg *reg);
void		han_split_jong(char chr, han_reg *reg);
unsigned	han_break(unsigned *cho_jong);
void		han_cho_p(han_reg *reg);
void		han_jong_p(han_reg *reg);
