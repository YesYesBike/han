#include "han.h"

//A(0x41)~z(0x7A) 아스키에 따른 자모 분류
static const char han_table[] = {
	7,  37, 15, 12, 5,  6,  19, 28, 22, 24, 20, 40, 38, 33, 23,
	27, 9,  2,  3,  11, 26, 18, 14, 17, 32, 16, 70, 70, 70, 70,
	70, 70, 7,  37, 15, 12, 4,  6,  19, 28, 22, 24, 20, 40, 38,
	33, 21, 25, 8,  1,  3,  10, 26, 18, 13, 17, 32, 16
};

static char esc_ch = 0;
static int esc = 0;
static int flag = 0;

inline void han_putc(han_reg *reg, const wchar_t chr)
{
	if (flag & HAN_FLAG_T)
		fputwc(chr, reg->fd);
	putwchar(chr);
}

//p레지스터에 있는 글자 출력 후 비우기
void han_print_p(han_reg *reg)
{
	//TODO: 버그를 내지 않고 이 구문을 없앨 수 있는 방법을 찾기
	if (reg->p == 0)
		return;
	han_putc(reg, reg->p);
	reg->p = 0;
}


//chr에 해당하는 글자를 한글로 처리
void han_trans(han_reg *reg, const char chr)
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
		han_ja(reg, chr);
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
		han_mo(reg, chr);
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


//초성,중성,종성 레지스터에 저장된 수를 조합해 p레지스터에 저장
void han_insert_p(han_reg *reg)
{
	uint32_t sum = reg->cho + reg->jung + reg->jong;
	if (sum == 0)
		return;
	
	if (reg->cho == sum)
		han_cho_p(reg);
	else if (reg->jung == sum)
		reg->p = 0x313B + reg->jung;	//0x314F~0x3163
	else
		han_combine_p(reg);

	reg->cho = 0;
	reg->jung = 0;
	reg->jong = 0;
}


//han_insert_p에서 초성만, 또는 중성만 있는 게 아닌 경우에 호출
void han_combine_p(han_reg *reg)
{
	(reg->cho)--;
	reg->jung -= 20;
	han_jong_p(reg);

	reg->p = 0xAC00 + (588*reg->cho)
				+ (28*reg->jung) + reg->jong;
}


//초성만 출력할 경우 p레지스터에 입력하기 전 최종 처리
void han_cho_p(han_reg *reg)
{
	switch (reg->cho) {
	case 1:
	case 2:
		(reg->cho)--;
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


//종성 레지스터에 있는 값을 p레지스터에 입력하기 전 최종적으로 처리
void han_jong_p(han_reg *reg)
{
	switch (reg->jong) {
	case 1:
	case 2:
		break;
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


//겹자음 합성. 초성과 종성 두 경우에 맞춰서 유동적으로 적용 가능
void han_dja(han_reg *reg, uint32_t *cho_jong, const char chr)
{
	switch (*cho_jong) {
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
}


//초중종성 레지스터에 값이 있는 상태에서 중성 입력 시 종성 분리
void han_split_jong(han_reg *reg, const char chr)
{
	uint32_t buf;
	if (reg->jong >= 41) {
		buf = han_break(&(reg->jong));
	} else {
		buf = reg->jong;
		reg->jong = 0;
	}

	han_insert_p(reg);
	reg->cho = buf;
	reg->jung = HAN_T(chr);
}


//겹자음 분리. han_dja와 마찬가지
uint32_t han_break(uint32_t *cho_jong)
{
	switch (*cho_jong) {
	case 41:				/* ㄳ */
		*cho_jong = 1;
		return 10;
	case 42:				/* ㄵ */
		*cho_jong = 3;
		return 13;
	case 43:				/* ㄶ */
		*cho_jong = 3;
		return 19;
	case 44:				/* ㄺ */
		*cho_jong = 6;
		return 1;
	case 45:				/* ㄻ */
		*cho_jong = 6;
		return 7;
	case 46:				/* ㄼ */
		*cho_jong = 6;
		return 8;
	case 47:				/* ㄽ */
		*cho_jong = 6;
		return 10;
	case 48:				/* ㄾ */
		*cho_jong = 6;
		return 17;
	case 49:				/* ㄿ */
		*cho_jong = 6;
		return 18;
	case 50:				/* ㅀ */
		*cho_jong = 6;
		return 19;
	case 51:				/* ㅄ */
		*cho_jong = 8;
		return 10;
	}
}


//겹모음
void han_dmo(han_reg *reg, const char chr)
{
	switch (reg->jung) {
	case 28:					/* ㅗ */
		switch (chr) {
		case 'k':				/* ㅘ */
			reg->jung = 29;
			return;
		case 'o':				/* ㅙ */
			reg->jung = 30;
			return;
		case 'l':				/* ㅚ */
			reg->jung = 31;
			return;
		}
		break;
	case 33:					/* ㅜ */
		switch (chr) {
		case 'j':				/* ㅝ */
			reg->jung = 34;
			return;
		case 'p':				/* ㅞ */
			reg->jung = 35;
			return;
		case 'l':				/* ㅟ */
			reg->jung = 36;
			return;
		}
		break;
	case 38:					/* ㅡ */
		if (chr == 'l') {
			reg->jung = 39;			/* ㅢ */
			return;
		}
	}
	han_insert_p(reg);
	reg->jung = HAN_T(chr);
}


//초성,중성,종성 레지스터 상태 확인.
//CHK_CJJ: 초성,중성,종성 다 있음
//CHK_CJ:  초성,중성만 있음
//CHK_C:   초성만 있음
//CHK_J:   중성만 있음
//CHK_N:   아무 글자도 없음
inline uint32_t han_check_reg(const han_reg *reg)
{
	return ((reg->cho != 0)<<2 | (reg->jung != 0)<<1 | (reg->jong != 0));
}


void han_ja(han_reg *reg, const char chr)
{
	switch (han_check_reg(reg)) {
	case CHK_CJJ:
		han_dja(reg, &(reg->jong), chr);
		break;
	case CHK_CJ:
		reg->jong = HAN_T(chr);
		break;
	case CHK_C:
		han_dja(reg, &(reg->cho), chr);
		break;
	case CHK_J:
		han_insert_p(reg);
	case CHK_N:
		reg->cho = HAN_T(chr);
		break;
	}
}


void han_mo(han_reg *reg, const char chr)
{
	switch (han_check_reg(reg)) {
	case CHK_CJJ:
		han_split_jong(reg, chr);
		break;
	case CHK_CJ:
	case CHK_J:
		han_dmo(reg, chr);
		break;
	case CHK_C:
		if (reg->cho >= 41) {
			uint32_t buf = han_break(&(reg->cho));
			han_insert_p(reg);
			reg->cho = buf;
		}
	case CHK_N:
		reg->jung = HAN_T(chr);
		break;
	}
}


//최초 글자 처리. A-z의 글자만 선별해 한글로 변환
//이스케이프 여부에 따른 처리도 여기에 포함
//XXX if문을 줄일 수 있는 방법이 있을까?
void han(const wchar_t *str, han_reg *reg)
{
	static int bef_esc = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		if (reg->p)
			han_print_p(reg);

		if (str[i] == '\n' && flag & HAN_FLAG_C)
			continue;
		if (str[i] == esc_ch) {
			if (esc == 0)
				han_insert_p(reg);

			esc = esc ? 0 : 1;

			//-E일 때 bef_esc는 무시
			if (flag & HAN_FLAG_E) {
				han_print_p(reg);
				han_putc(reg, esc_ch);
				continue;
			}
			if (bef_esc) {
				bef_esc = 0;
				han_putc(reg, esc_ch);
				continue;
			}
			bef_esc = 1;
			continue;
		}

		bef_esc = 0;
		if (esc) {
			han_putc(reg, str[i]);
			continue;
		}

		if (str[i]>='A' && str[i]<='z') {
			han_trans(reg, str[i]);
		} else {
			han_insert_p(reg);
			han_print_p(reg);
			han_putc(reg, str[i]);
		}
	}
}


void help(void)
{
	printf("[한영타 변환기 도움말]\n"
		"사용법: 표준 입출력에서 알파벳을 한글로 변환해야 할 상황에서 활용할 수 있음.\n"
		"-h: 지금 보고 있는 것.\n"
		"-c: 개행문자를 제외하고 출력.\n"
		"-e: 이스케이프 문자를 지정하여 다음 이스케이프 문자까지 변환 무시.\n"
		"    해당 문자를 출력하려면 둘을 붙여쓰면 됨.\n"
		"-E: -e와 달리 이스케이프 문자를 그대로 출력.\n"
		"-o: -e와 반대로 지정한 부분만 한글로 변환.\n"
		"-O: -E와 반대.\n"
		"-t: stdin과 파일에 동시 출력. 옵션을 주지 않고 tee를 후처리 필터로 사용할 시\n"
		"    입력할 때마다 결과를 볼 수 없었던 점을 감안하여 추가함.\n"
		"-T: -t와 기존 파일에 덧붙이는 것 빼고 동일\n");

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
	wchar_t buf[BUFSIZE];
	han_reg reg = {};
	int opt;
	extern int opterr;

	char t_optstr[] = "w";
	opterr = 0;

	while ((opt = getopt(argc, argv, "ce:E:ho:O:T:t:")) != -1) {
		switch (opt) {
		case 'c':
			flag |= HAN_FLAG_C;
			break;
		case 'E':
			flag |= HAN_FLAG_E;
		case 'e':
MAIN_ESCAPE:
			esc_ch = optarg[0];
			break;
		case 'h':
			help();
		case 'O':
			flag |= HAN_FLAG_E;
		case 'o':
			esc = 1;
			goto MAIN_ESCAPE;
		case 'T':
			*t_optstr = 'a';
		case 't':
			flag |= HAN_FLAG_T;
			if ((reg.fd = fopen(optarg, t_optstr)) == NULL)
				print_error("%s: open error\n", optarg);
			break;
		case '?':
			print_error(
				"사용법: %s [-c] [-e|E|o|O 문자] [-h] [-t|T 파일]\n", argv[0]);
		}
	}

	while (fgetws(buf, BUFSIZE, stdin) != NULL)
		han(buf, &reg);

	han_insert_p(&reg);
	han_print_p(&reg);

	if (flag & HAN_FLAG_T) {
		//TODO 여러 파일
		if (fclose(reg.fd) != 0)
			print_error("close error\n");
	}

	return 0;
}
