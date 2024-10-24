#include "han.h"

static const char han_table[] = {
	7,  37, 15, 12, 5,  6,  19, 28, 22, 24, 20, 40, 38, 33, 23,
	27, 9,  2,  3,  11, 26, 18, 14, 17, 32, 16, 70, 70, 70, 70,
	70, 70, 7,  37, 15, 12, 4,  6,  19, 28, 22, 24, 20, 40, 38,
	33, 21, 25, 8,  1,  3,  10, 26, 18, 13, 17, 32, 16
};

inline void han_putc(han_reg *reg, wchar_t chr)
{
	if (reg->flag & HAN_FLAG_T)
		fputwc(chr, reg->fd);
	putwchar(chr);
}

void han_print_p(han_reg *reg)
{
	if (reg->p == 0)
		return;
	han_putc(reg, reg->p);
	reg->p = 0;
}


void han_trans(char chr, han_reg *reg)
{
	switch (chr) {
	case 'r':	/* ㄱ */
	case 'R':	/* ㄲ */
	case 's':	/* ㄴ */
	case 'S':	/* ㄴ */
	case 'e':	/* ㄷ */
	case 'E':	/* ㄸ */
	case 'f':	/* ㄹ */
	case 'F':	/* ㄹ */
	case 'a':	/* ㅁ */
	case 'A':	/* ㅁ */
	case 'q':	/* ㅂ */
	case 'Q':	/* ㅃ */
	case 't':	/* ㅅ */
	case 'T':	/* ㅆ */
	case 'd':	/* ㅇ */
	case 'D':	/* ㅇ */
	case 'w':	/* ㅈ */
	case 'W':	/* ㅉ */
	case 'c':	/* ㅊ */
	case 'C':	/* ㅊ */
	case 'z':	/* ㅋ */
	case 'Z':	/* ㅋ */
	case 'x':	/* ㅌ */
	case 'X':	/* ㅌ */
	case 'v':	/* ㅍ */
	case 'V':	/* ㅍ */
	case 'g':	/* ㅎ */
	case 'G':	/* ㅎ */
		han_ja(chr, reg);
		break;

	case 'k':	/* ㅏ */
	case 'K':	/* ㅏ */
	case 'o':	/* ㅐ */
	case 'i':	/* ㅑ */
	case 'I':	/* ㅑ */
	case 'O':	/* ㅒ */
	case 'j':	/* ㅓ */
	case 'J':	/* ㅓ */
	case 'p':	/* ㅔ */
	case 'u':	/* ㅕ */
	case 'U':	/* ㅕ */
	case 'P':	/* ㅖ */
	case 'h':	/* ㅗ */
	case 'H':	/* ㅗ */
	case 'y':	/* ㅛ */
	case 'Y':	/* ㅛ */
	case 'n':	/* ㅜ */
	case 'N':	/* ㅜ */
	case 'b':	/* ㅠ */
	case 'B':	/* ㅠ */
	case 'm':	/* ㅡ */
	case 'M':	/* ㅡ */
	case 'l':	/* ㅣ */
	case 'L':	/* ㅣ */
		han_mo(chr, reg);
		break;

	case '[':
	case '\\':
	case ']':
	case '^':
	case '_':
	case '`':
		han_insert_p(reg);
		han_print_p(reg);
		han_putc(reg, chr);
	}
}


void han_insert_p(han_reg *reg)
{
	unsigned checksum = reg->cho + reg->jung + reg->jong;
	if (checksum == 0)
		return;
	
	if (reg->cho == checksum)
		han_cho_p(reg);
	else if (reg->jung == checksum)
		reg->p = 0x313B + reg->jung;	//0x314F~0x3163
	else
		han_combine_p(reg);

	reg->cho = 0;
	reg->jung = 0;
	reg->jong = 0;
}


void han_combine_p(han_reg *reg)
{
	if (reg->jong >= 3)
		han_jong_p(reg);

	(reg->cho)--;
	reg->jung -= 20;

	reg->p = 0xAC00 + (588*reg->cho) + (28*reg->jung) + reg->jong;
}


void han_cho_p(han_reg *reg)
{
	switch (reg->cho) {
	case 1:
	case 2:
		reg->cho -= 1;
		break;
	case 41:					/* ㄳ */
		reg->cho = 2;
		break;
	case 3:
		break;
	case 42:					/* ㄵ */
	case 43:					/* ㄶ */
		reg->cho -= 38;
		break;
	case 4:
	case 5:
	case 6:
		reg->cho += 2;
		break;
	case 44:					/* ㄺ */
	case 45:					/* ㄻ */
	case 46:					/* ㄼ */
	case 47:					/* ㄽ */
	case 48:					/* ㄾ */
	case 49:					/* ㄿ */
	case 50:					/* ㅀ */
		reg->cho -= 35;
		break;
	case 7:
	case 8:
	case 9:
		reg->cho += 9;
		break;
	case 51:					/* ㅄ */
		reg->cho = 19;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		reg->cho += 10;
	}

	reg->p = 0x3131 + reg->cho;		//0x3131~0x314E
}


void han_jong_p(han_reg *reg)
{
	switch (reg->jong) {
	//case 1:
	//case 2:
	//	break;
	case 41:					/* ㄳ */
		reg->jong = 3;
		break;
	case 3:
		reg->jong = 4;
		break;
	case 42:					/* ㄵ */
	case 43:					/* ㄶ */
		reg->jong -= 37;
		break;
	case 4:
		reg->jong = 7;
		break;
	//5: ㄸ
	case 6:
		reg->jong = 8;
		break;
	case 44:					/* ㄺ */
	case 45:					/* ㄻ */
	case 46:					/* ㄼ */
	case 47:					/* ㄽ */
	case 48:					/* ㄾ */
	case 49:					/* ㄿ */
	case 50:					/* ㅀ */
		reg->jong -= 35;
		break;
	case 7:
	case 8:
		reg->jong += 9;
		break;
	//9: ㅃ
	case 51:					/* ㅄ */
		reg->jong = 18;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		reg->jong += 9;
		break;
	//14: ㅉ
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		reg->jong += 8;
	}
}


void han_dja(char chr, unsigned *cho_jong, han_reg *reg)
{
	unsigned buf = *cho_jong;
	switch (buf) {
	case 1:						/* ㄱ */
		if (chr == 't') {
			*cho_jong = 41;			/* ㄳ */
			return;
		}
		break;
	case 3:						/* ㄴ */
		switch (chr) {
		case 'w':				/* ㄵ */
			*cho_jong = 42;
			return;
		case 'g':				/* ㄶ */
			*cho_jong = 43;
			return;
		}
		break;
	case 6:						/* ㄹ */
		switch (chr) {
		case 'r':				/* ㄺ */
			*cho_jong = 44;
			return;
		case 'a':				/* ㄻ */
			*cho_jong = 45;
			return;
		case 'q':				/* ㄼ */
			*cho_jong = 46;
			return;
		case 't':				/* ㄽ */
			*cho_jong = 47;
			return;
		case 'x':				/* ㄾ */
			*cho_jong = 48;
			return;
		case 'v':				/* ㄿ */
			*cho_jong = 49;
			return;
		case 'g':				/* ㅀ */
			*cho_jong = 50;
			return;
		}
		break;
	case 8:						/* ㅂ */
		if (chr == 't') {
			*cho_jong = 51;			/* ㅄ */
			return;
		}
		break;
	}
	han_insert_p(reg);
	reg->cho = HAN_T(chr);
	return;
}


void han_split_jong(char chr, han_reg *reg)
{
	unsigned buffer;
	if (reg->jong >= 41) {
		buffer = han_break(&(reg->jong));
	} else {
		buffer = reg->jong;
		reg->jong = 0;
	}

	han_insert_p(reg);
	reg->cho = buffer;
	reg->jung = HAN_T(chr);
}


unsigned han_break(unsigned *cho_jong)
{
	unsigned ret;

	switch (*cho_jong) {
	case 41:				/* ㄳ */
		*cho_jong = 1;
		ret = 10;
		break;
	case 42:				/* ㄵ */
		*cho_jong = 3;
		ret = 13;
		break;
	case 43:				/* ㄶ */
		*cho_jong = 3;
		ret = 19;
		break;
	case 44:				/* ㄺ */
		*cho_jong = 6;
		ret = 1;
		break;
	case 45:				/* ㄻ */
		*cho_jong = 6;
		ret = 7;
		break;
	case 46:				/* ㄼ */
		*cho_jong = 6;
		ret = 8;
		break;
	case 47:				/* ㄽ */
		*cho_jong = 6;
		ret = 10;
		break;
	case 48:				/* ㄾ */
		*cho_jong = 6;
		ret = 17;
		break;
	case 49:				/* ㄿ */
		*cho_jong = 6;
		ret = 18;
		break;
	case 50:				/* ㅀ */
		*cho_jong = 6;
		ret = 19;
		break;
	case 51:				/* ㅄ */
		*cho_jong = 8;
		ret = 10;
		break;
	}

	return ret;
}


void han_dmo(char chr, han_reg *reg)
{
	switch (reg->jung) {
	case 28:					/* ㅗ */
		switch (chr) {
		case 'k':				/* ㅘ */
			reg->jung = 29;
			break;
		case 'o':				/* ㅙ */
			reg->jung = 30;
			break;
		case 'l':				/* ㅚ */
			reg->jung = 31;
			break;
		}
		break;
	case 33:					/* ㅜ */
		switch (chr) {
		case 'j':				/* ㅝ */
			reg->jung = 34;
			break;
		case 'p':				/* ㅞ */
			reg->jung = 35;
			break;
		case 'l':				/* ㅟ */
			reg->jung = 36;
			break;
		}
		break;
	case 38:					/* ㅡ */
		if (chr == 'l')
			reg->jung = 39;			/* ㅢ */
		break;
	default:
		han_insert_p(reg);
		reg->jung = HAN_T(chr);
	}

}


unsigned han_check_reg(han_reg *reg)
{
	if (reg->cho != 0) {
		if (reg->jung != 0)
			return reg->jong ? 1 : 2;
		else
			return 3;
	} else {
		return (reg->jung != 0) ? 4 : 5;
	}
}


void han_ja(char chr, han_reg *reg)
{
	switch (han_check_reg(reg)) {
	case 1:
		han_dja(chr, &(reg->jong), reg);
		break;
	case 2:
		reg->jong = HAN_T(chr);
		break;
	case 3:
		han_dja(chr, &(reg->cho), reg);
		break;
	case 4:
		han_insert_p(reg);
		han_print_p(reg);
	case 5:
		reg->cho = HAN_T(chr);
		break;
	}
}


void han_mo(char chr, han_reg *reg)
{
	switch (han_check_reg(reg)) {
	case 1:
		han_split_jong(chr, reg);
		break;
	case 2:
	case 4:
		han_dmo(chr, reg);
		break;
	case 3:
		if (reg->cho >= 41) {
			unsigned buffer = han_break(&(reg->cho));
			han_insert_p(reg);
			reg->cho = buffer;
			reg->jung = HAN_T(chr);
		}
	case 5:
		reg->jung = HAN_T(chr);
		break;
	}
}


void han(wchar_t *str, han_reg *reg)
{
	static int escape = 0;
	static int bef_dollar = 0;

	for (int i=0; str[i] != '\0'; i++) {
		if (reg->p)
			han_print_p(reg);

		if (str[i] == '\n' && reg->flag & HAN_FLAG_C)
			continue;
		if (str[i] == '$' && reg->flag & HAN_FLAG_E) {
			escape = escape ? 0 : 1;

			if (reg->flag & HAN_FLAG_EE) {
				han_putc(reg, '$');
				continue;
			}
			if (bef_dollar) {
				bef_dollar = 0;
				han_putc(reg, '$');
				continue;
			}
			if (escape == 0) {
				//TODO 버그 가능성 있음
				han_insert_p(reg);
				han_print_p(reg);
			}
			bef_dollar = 1;
			continue;
		}

		bef_dollar = 0;
		if (escape) {
			han_putc(reg, str[i]);
			continue;
		}

		if (str[i]<'A' || str[i]>'z') {
			if (reg->cho || reg->jung) {
				han_insert_p(reg);
				han_print_p(reg);
			}
			han_putc(reg, str[i]);
		} else {
			han_trans(str[i], reg);
		}
	}
}


void help(void)
{
	printf("[한영타 변환기 도움말]\n\
사용법: 표준 입출력에서 알파벳을 한글로 변환해야 할 상황에서 활용할 수 있음.\n\
옵션\n\
-h: 지금 보고 있는 것.\n\
-c: 개행문자를 제외하고 출력.\n\
-e: $부터 다음 $까지 한글 변환 무시($는 미출력).\n\
	$를 입력하고 싶으면 두 번 입력.\n\
-E: -e와 달리 $를 그대로 출력.\n\
-t: stdin과 파일에 동시 출력(tee)\n\
-T: -t와 기존 파일에 덧붙이는 것 빼고 동일\n");

	exit(EXIT_SUCCESS);
}

void print_error(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	wchar_t buffer[BUFSIZE];
	han_reg reg = {};
	int opt;
	opterr = 0;
	char t_optstr[] = "w";

	while ((opt = getopt(argc, argv, "ceEhT:t:")) != -1) {
		switch (opt) {
		case 'c':
			reg.flag |= HAN_FLAG_C;
			break;
		case 'E':
			reg.flag |= HAN_FLAG_EE;
		case 'e':
			reg.flag |= HAN_FLAG_E;
			break;
		case 'h':
			help();
		case 'T':
			t_optstr[0] = 'a';
		case 't':
			reg.flag |= HAN_FLAG_T;
			if ((reg.fd = fopen(optarg, t_optstr)) == NULL)
				print_error("%s: open error\n", optarg);
			break;
		case '?':
			print_error(
				"사용법: %s [-c] [-e] [-h] [-t|T 파일]\n", argv[0]);
		}
	}

	while (fgetws(buffer, BUFSIZE, stdin) != NULL)
		han(buffer, &reg);

	han_insert_p(&reg);
	han_print_p(&reg);

	if (reg.flag & HAN_FLAG_T) {
		//TODO 여러 파일
		if (fclose(reg.fd) != 0)
			print_error("close error\n");
	}

	return 0;
}
