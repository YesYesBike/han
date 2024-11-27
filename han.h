#include <locale.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#define BUFSIZE			BUFSIZ
#define HAN_T(a)		han_table[(a) - 'A']

#define HAN_FLAG_C		1 << 0
#define HAN_FLAG_T		1 << 1
#define HAN_FLAG_EE		1 << 2
#define HAN_FLAG_E		1 << 3
#define HAN_FLAG_O		1 << 4

#define CHK_CJJ			0b111
#define CHK_CJ			0b110
#define CHK_C			0b100
#define CHK_J			0b010
#define CHK_N			0b000

typedef struct han_reg {
	wchar_t		p;
	uint32_t 	cho;
	uint32_t 	jung;
	uint32_t 	jong;
	FILE		*fd;
} han_reg;


void		help(void);
void		print_error(char *fmt, ...);
void		han(const wchar_t *str, han_reg *reg);
void		han_trans(han_reg *reg, const char chr);
void		han_ja(han_reg *reg, const char chr);
void		han_mo(han_reg *reg, const char chr);
uint32_t	han_check_reg(const han_reg *reg);
void		han_insert_p(han_reg *reg);
void		han_combine_p(han_reg *reg);
void		han_print_p(han_reg *reg);
void		han_putc(han_reg *reg, const wchar_t chr);
void		han_dmo(han_reg *reg, const char chr);
void		han_dja(han_reg *reg, uint32_t *cho_jong, const char chr);
void		han_split_jong(han_reg *reg, const char chr);
uint32_t	han_break(uint32_t *cho_jong);
void		han_cho_p(han_reg *reg);
void		han_jong_p(han_reg *reg);
