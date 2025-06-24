#include "dekodowanie.h"
#include "konwersje.h"
#include "lancuchyznakowe.h"

#define NULL '\0'
#define MAX_KEYWORD_NR 3

struct Keyword asKeywordList[MAX_KEYWORD_NR] = {
    {RST, "reset"}, {LD, "load"}, {ST, "store"}};
    
struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr = 0;


enum State { TOKEN, DELIMITER };

unsigned char ucFindTokensInString(char *pcString) {

  enum State{TOKEN, DELIMITER};
	enum State eCurrentState = DELIMITER;
	unsigned char ucCharacterCounter;
	unsigned char ucCurrentCharacter;
	unsigned char ucTokenNumber = 0;

	for(ucCharacterCounter = 0; ; ucCharacterCounter++) {
		ucCurrentCharacter = pcString[ucCharacterCounter];
		switch(eCurrentState) {
			case DELIMITER:
				if(NULL == ucCurrentCharacter) {
					return ucTokenNumber;
				}
				else if(DELIMITER_CHARACTER == ucCurrentCharacter) {}
				else {
					eCurrentState = TOKEN;
					asToken[ucTokenNumber].uValue.pcString = (pcString + ucCharacterCounter);
					ucTokenNumber++;
				}
				break;
			case TOKEN:
				if(MAX_TOKEN_NR <= ucTokenNumber) {
					return ucTokenNumber;
				}
				else if(NULL == ucCurrentCharacter) {
					return ucTokenNumber;
				}
				else if(DELIMITER_CHARACTER == ucCurrentCharacter) {
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

  for (ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR;
       ucKeywordCounter++) {

    if (eCompareString(pcStr, asKeywordList[ucKeywordCounter].cString) ==
        EQUAL) {
      *peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
      return OK;
    }
  }
  return ERROR;
}

void DecodeTokens(void) {

  unsigned char ucTokenCounter;
  union TokenValue uValue;
  enum TokenType eType;
  struct Token *psCurrentToken;

  for (ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++) {

    psCurrentToken = asToken[ucTokenCounter].uValue.pcString;

    if (OK == eStringToKeyword(psCurrentToken, &uValue.eKeyword)) {
      eType = KEYWORD;
    } else if (OK == eHexStringToUInt(psCurrentToken, &uValue.uiValue)) {
      eType = NUMBER;
    } else {
      eType = STRING;
      uValue.pcString = psCurrentToken;
    }
    asToken[ucTokenCounter].eType = eType;
    asToken[ucTokenCounter].uValue = uValue;
  }
}

void DecodeMsg(char *pcString) {
  ucTokenNr = ucFindTokensInString(pcString);
  ReplaceCharactersInString(pcString, DELIMITER_CHARACTER, NULL);
  DecodeTokens();
}
