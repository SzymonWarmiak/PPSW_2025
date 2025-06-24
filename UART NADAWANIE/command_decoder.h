#define MAX_TOKEN_NR 3
#define MAX_KEYWORD_STRING_LTH 10
#define MAX_KEYWORD_NR 2
#define DELIMITER_CHARACTER ' '

enum KeywordCode {CALLIB, GOTO, TEST};
enum TokenType {KEYWORD, NUMBER, STRING};

union TokenValue {
    enum KeywordCode eKeyword;
    unsigned int uiValue;
    char* pcString;
};

struct Token {
    enum TokenType eType;
    union TokenValue uValue;
};

extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;

void DecodeMsg(char *pcString);
