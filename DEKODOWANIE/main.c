
#define NULL '\0'
#define MAX_TOKEN_NR 3
#define MAX_KEYWORD_STRING_LTH 10
#define MAX_KEYWORD_NR 3
#define DELIMITER_CHARACTER ' '

enum KeywordCode {LD, ST, RST};

union TokenValue {
    enum KeywordCode eKeyword;
    unsigned int uiValue;
    char* pcString;
};

enum TokenType {KEYWORD, NUMBER, STRING};

struct Token {
    enum TokenType eType;
    union TokenValue uValue;
};

struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr;

struct Keyword {
    enum KeywordCode eCode;
    char cString[MAX_KEYWORD_STRING_LTH + 1];
};

struct Keyword asKeywordList[MAX_KEYWORD_NR] = {
    {RST, "reset"},
    {LD, "load"},
    {ST, "store"}
};

enum CompResult {DIFFERENT, EQUAL};
enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{

    unsigned char ucCharacterCounter;

    for(ucCharacterCounter = 0 ; pcStr1[ucCharacterCounter] != NULL || pcStr2[ucCharacterCounter] != NULL ; ucCharacterCounter++)
    {
        if( pcStr1[ucCharacterCounter] != pcStr2[ucCharacterCounter] )
        {
            return DIFFERENT;
        }
    }
    
    return EQUAL;
}
void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar)
{

    unsigned char ucCharacterCounter;

    for(ucCharacterCounter = 0 ; pcString[ucCharacterCounter] != NULL; ucCharacterCounter++)
    {
        if(pcString[ucCharacterCounter] == cOldChar)
        {
            pcString[ucCharacterCounter] = cNewChar;
        }
    }
}

enum Result {OK, ERROR};

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
    unsigned char ucCharacterCounter;
    unsigned char ucCurrentCharacter;

    *puiValue = 0;
    if((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == NULL))
    {
        return ERROR;
    }
    for(ucCharacterCounter = 2; pcStr[ucCharacterCounter] !=NULL ; ucCharacterCounter++)
    {
        ucCurrentCharacter = pcStr[ucCharacterCounter];
        if(ucCurrentCharacter == NULL)
        {
            return OK;
        }
        else if(ucCharacterCounter == 6)
        {
            return ERROR;
        }
        *puiValue = *puiValue << 4;
        if((ucCurrentCharacter <= '9') && (ucCurrentCharacter >= '0'))
        {
            *puiValue = (*puiValue) | (ucCurrentCharacter - '0');
        }
        else if((ucCurrentCharacter <= 'F') && (ucCurrentCharacter >= 'A'))
        {
            *puiValue = (*puiValue) | (ucCurrentCharacter - 'A' + 10);
        }
        else
        {
            return ERROR;
        }
    }
    return OK;
}







enum State {TOKEN, DELIMITER};

unsigned char ucFindTokensInString(char *pcString) {
    enum State eCurrentState = DELIMITER;
    unsigned char ucCharacterCounter;
    unsigned char ucCurrentCharacter;
    unsigned char ucTokenNumber = 0;

    for (ucCharacterCounter = 0; ; ucCharacterCounter++) {
        ucCurrentCharacter = pcString[ucCharacterCounter];

        switch (eCurrentState) {
            case DELIMITER:
                if (NULL == ucCurrentCharacter) {
                    return ucTokenNumber;
                }
                else if (DELIMITER_CHARACTER == ucCurrentCharacter) {
                    // pozostajemy w DELIMITER
                }
                else {
                    eCurrentState = TOKEN;
                    asToken[ucTokenNumber].uValue.pcString = &pcString[ucCharacterCounter];
                    ucTokenNumber++;
                }
                break;

            case TOKEN:
                if (MAX_TOKEN_NR <= ucTokenNumber) {
                    return ucTokenNumber;
                }
                else if (NULL == ucCurrentCharacter) {
                    return ucTokenNumber;
                }
                else if (DELIMITER_CHARACTER == ucCurrentCharacter) {
                    eCurrentState = DELIMITER;
                }
                else {
                    eCurrentState = TOKEN;
                }
                break;
        }
    }
}


enum Result eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode) {
    unsigned char ucKeywordCounter;

    for (ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++) {
        if (EQUAL == eCompareString(asKeywordList[ucKeywordCounter].cString, pcStr)) {
            *peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
            return OK;
        }
    }
    return ERROR;
}

void DecodeTokens(void) {
    unsigned char ucTokenCounter;
    struct Token* psCurrentToken;

    for (ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++) {
        psCurrentToken = &asToken[ucTokenCounter];

        if (OK == eStringToKeyword(psCurrentToken->uValue.pcString, &psCurrentToken->uValue.eKeyword)) {
            psCurrentToken->eType = KEYWORD;
        }
        else if (OK == eHexStringToUInt(psCurrentToken->uValue.pcString, &psCurrentToken->uValue.uiValue)) {
            psCurrentToken->eType = NUMBER;
        }
        else {
            psCurrentToken->eType = STRING;
        }
    }
}

void DecodeMsg(char *pcString) {
    ucTokenNr = ucFindTokensInString(pcString);
    ReplaceCharactersInString(pcString, DELIMITER_CHARACTER, NULL);
    DecodeTokens();
}



int main() {
    char testString[] = "load 0x20 reset";  // komunikat do przetworzenia

    DecodeMsg(testString);  // uruchom pelny proces dekodowania
}



