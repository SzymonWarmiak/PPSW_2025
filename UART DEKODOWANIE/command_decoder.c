#include "command_decoder.h"
#include "string.h"

struct Keyword {
    enum KeywordCode eCode;
    char cString[MAX_KEYWORD_STRING_LTH + 1];
};

struct Keyword asKeywordList[MAX_KEYWORD_NR] = {
    {CALLIB, "callib"},
    {GOTO, "goto"}
};

struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr;




enum CompResult eCompareString(char pcStr1[], char pcStr2[]) {
    unsigned char ucCharacterCounter;
    for (ucCharacterCounter = 0; pcStr1[ucCharacterCounter] != '\0' || pcStr2[ucCharacterCounter] != '\0'; ucCharacterCounter++) {
        if (pcStr1[ucCharacterCounter] != pcStr2[ucCharacterCounter]) return DIFFERENT;
    }
    return EQUAL;
}

// -- Tokenizacja i dekodowanie:
unsigned char ucFindTokensInString(char *pcString) {
    enum {DELIMITER, TOKEN} eState = DELIMITER;
    unsigned char i = 0, tokenNr = 0;
    while (1) {
        char c = pcString[i];
        switch(eState) {
            case DELIMITER:
                if (c == '\0') return tokenNr;
                else if (c == DELIMITER_CHARACTER) {}
                else {
                    eState = TOKEN;
                    asToken[tokenNr].uValue.pcString = &pcString[i];
                    tokenNr++;
                }
                break;
            case TOKEN:
                if (tokenNr >= MAX_TOKEN_NR) return tokenNr;
                else if (c == '\0') return tokenNr;
                else if (c == DELIMITER_CHARACTER) eState = DELIMITER;
                break;
        }
        i++;
    }
}

// -- Zamiana stringa na kod slowa kluczowego
enum Result {OK, ERROR};
enum Result eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode) {
    unsigned char i;
    for (i = 0; i < MAX_KEYWORD_NR; i++) {
        if (EQUAL == eCompareString(asKeywordList[i].cString, pcStr)) {
            *peKeywordCode = asKeywordList[i].eCode;
            return OK;
        }
    }
    return ERROR;
}


enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue) {
    unsigned char uiCharacterCounter;
    unsigned char uiCurrentCharacter;
    *puiValue = 0;

    if((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == '\0')) {
        return ERROR;
    }

    for(uiCharacterCounter = 2; uiCharacterCounter <= 6; uiCharacterCounter++) {
        uiCurrentCharacter = pcStr[uiCharacterCounter];

        if(uiCurrentCharacter == '\0') {
            return OK;
        } else if (uiCharacterCounter == 6) {
            return ERROR;
        }

        *puiValue = *puiValue << 4;

        if(uiCurrentCharacter >= '0' && uiCurrentCharacter <= '9') {
            uiCurrentCharacter = uiCurrentCharacter - '0';
        } else if(uiCurrentCharacter >= 'A' && uiCurrentCharacter <= 'F') {
            uiCurrentCharacter = uiCurrentCharacter - 'A' + 10;
        } else if(uiCurrentCharacter >= 'a' && uiCurrentCharacter <= 'f') {
            uiCurrentCharacter = uiCurrentCharacter - 'a' + 10;
        } else {
            return ERROR;
        }
        *puiValue |= uiCurrentCharacter;
    }
    return ERROR;
}


void DecodeTokens(void) {
    unsigned char i;
    for (i = 0; i < ucTokenNr; i++) {
        if (OK == eStringToKeyword(asToken[i].uValue.pcString, &asToken[i].uValue.eKeyword)) {
            asToken[i].eType = KEYWORD;
        }
        else if (OK == eHexStringToUInt(asToken[i].uValue.pcString, &asToken[i].uValue.uiValue)) {
            asToken[i].eType = NUMBER;
        }
        else {
            asToken[i].eType = STRING;
        }
    }
}


void DecodeMsg(char *pcString) {
    ucTokenNr = ucFindTokensInString(pcString);
    ReplaceCharactersInString(pcString, DELIMITER_CHARACTER, '\0');
    DecodeTokens();
}
