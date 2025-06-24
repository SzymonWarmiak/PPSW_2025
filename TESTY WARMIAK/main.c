/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string.h> 

#define NULL '\0' 
#define HEX_DIGITS 4
#define NULL '\0' 
#define MAX_TOKEN_NR 3 
#define MAX_KEYWORD_STRING_LTH 10 
#define MAX_KEYWORD_NR 3
#define DELIMITER_CHARACTER ' ' 
 
void CopyString(char pcSource[], char pcDestination[]) {   
   
    unsigned char ucCharacterCounter;   
       
    for (ucCharacterCounter = 0; pcSource[ucCharacterCounter] != NULL; ucCharacterCounter ++) {   
        pcDestination[ucCharacterCounter] = pcSource[ucCharacterCounter];      
    }   
       
    pcDestination[ucCharacterCounter] = NULL;   
} 


enum CompResult { DIFFERENT, EQUAL };   
   
enum CompResult eCompareString(char pcStr1[], char pcStr2[]) { 
   
    unsigned char ucCharacterCounter;    
    
    for (ucCharacterCounter = 0; pcStr1[ucCharacterCounter] != NULL; ucCharacterCounter ++) {   
        if (pcStr1[ucCharacterCounter] != pcStr2[ucCharacterCounter]) {   
            return DIFFERENT;    
        }   
    }   
    
    if (pcStr2[ucCharacterCounter] != NULL) {   
        return DIFFERENT;   
    }   
    
    return EQUAL;   
} 


void AppendString(char pcSourceStr[], char pcDestinationStr[]) {  
    unsigned char ucCharacterCounter;  
    for (ucCharacterCounter = 0; pcDestinationStr[ucCharacterCounter] != NULL; ucCharacterCounter ++) {}  
    CopyString(pcSourceStr,  pcDestinationStr + ucCharacterCounter);  
} 


void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar) {  
    unsigned char ucCharacterCounter;  
    for (ucCharacterCounter = 0; pcString[ucCharacterCounter] != NULL; ucCharacterCounter++) {  
        if (pcString[ucCharacterCounter] == cOldChar) {  
            pcString[ucCharacterCounter] = cNewChar;  
        }  
    }  
}



void UIntToHexStr(unsigned int uiValue, char pcStr[])
{
	unsigned char ucNibbleCounter;
	unsigned char ucCurrentNibble;
	pcStr[0] = '0';
	pcStr[1] = 'x';
	pcStr[6] = NULL; 
	
	for(ucNibbleCounter = 0; ucNibbleCounter < HEX_DIGITS ; ucNibbleCounter++) 
	{
		ucCurrentNibble = (uiValue >> (ucNibbleCounter * 4)) & 0x0F;
		
		if(ucCurrentNibble < 10)
		{
			pcStr[5 - ucNibbleCounter] = '0' + ucCurrentNibble; 
		}
		else 
		{
			pcStr[5 - ucNibbleCounter] = 'A' + (ucCurrentNibble - 10);
		}
	}
}


enum Result { OK, ERROR };

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue) 
{ 
	unsigned char ucCharacterCounter; 
	unsigned int uiCurrentCharacter = 0; 
	
	if ((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == NULL))
	{
		return ERROR; 
	}
	
	for (ucCharacterCounter = 2; pcStr[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		if(ucCharacterCounter >= 6) {
			return ERROR; 
		} 
		uiCurrentCharacter <<= 4; 
		
		if ((pcStr[ucCharacterCounter] >= '0') && (pcStr[ucCharacterCounter] <= '9'))
		{
			uiCurrentCharacter += (pcStr[ucCharacterCounter] - '0');
		}
		else if ((pcStr[ucCharacterCounter] >= 'A') && (pcStr[ucCharacterCounter] <= 'F'))
		{
			uiCurrentCharacter += (pcStr[ucCharacterCounter] - 'A' + 10);
		}
		else { 
			return ERROR; 
		}
	}
	*puiValue = uiCurrentCharacter;
	return OK; 
}


void AppendUIntToString(unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucLength;
	
	for (ucLength = 0; pcDestinationStr[ucLength] != NULL; ucLength++) {}
	UIntToHexStr(uiValue, &pcDestinationStr[ucLength]);
}

enum KeywordCode {LD, ST, RST};

union TokenValue {
	enum KeywordCode eKeyword; 
	unsigned int uiValue;
	char* pcString ; 
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

enum State {TOKEN, DELIMITER};

unsigned char ucFindTokensInString(char *pcString) {

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
    
    for(ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++) {
        if(EQUAL == eCompareString(asKeywordList[ucKeywordCounter].cString, pcStr)) {
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



void TestOf_CopyString(void){
    char cSource[] = "qwe123";
    char cDestination[20] = "";
    char cSource1[] = "";
    char cDestination1[20] = "";
    char cSource3[] = "Z";
    char cDestination3[20] = "";
    char cSource4[] = "klmnopqr";
    char cDestination4[5] = "";

    printf("Test funkcji CopyString:\n");

    CopyString(cSource, cDestination);
    printf("Test 1 - kopiowanie łańcucha: %s\n", (eCompareString(cSource, cDestination) == EQUAL) ? "OK" : "ERROR");

    CopyString(cSource1, cDestination1);
    printf("Test 2 - kopiowanie pustego łańcucha: %s\n", (eCompareString(cSource1, cDestination1) == EQUAL) ? "OK" : "ERROR");

    CopyString(cSource3, cDestination3);
    printf("Test 3 - kopiowanie pojedynczego znaku: %s\n", (eCompareString(cSource3, cDestination3) == EQUAL) ? "OK" : "ERROR");

    CopyString(cSource4, cDestination4);
    printf("Test 4 - kopiowanie za długiego łańcucha: %s\n", (eCompareString(cSource4, cDestination4) == EQUAL) ? "OK\n\n" : "ERROR\n\n");
}

void TestOf_eComapreString(void) { 
    char cString[] = "foo1";
    char cIdenticalString[] = "foo1"; 
    char cShorterString[] = "foo";
    char cLongerString[] = "foo12";
    char cDifferentString[] = "bar1";
    
    printf("Test funkcji eCompareString:\n");
    
    eCompareString(cString, cIdenticalString);
    printf("Test 1 - porównanie dwóch jednakowych łańcuchów: %s\n", (eCompareString(cString, cIdenticalString) == EQUAL) ? "OK" : "ERROR");

    eCompareString(cString, cShorterString);
    printf("Test 2 - porównanie, gdy jeden łańcuch jest krótszy: %s\n", (eCompareString(cString, cShorterString) == EQUAL) ? "OK" : "ERROR");
    
    eCompareString(cString, cLongerString);
    printf("Test 3 - porówananie, gdy jeden łańcuch jest dłuższy: %s\n", (eCompareString(cString, cLongerString) == EQUAL) ? "OK" : "ERROR"); 
    
    eCompareString(cString, cDifferentString);
    printf("Test 4 - porównanie dwóch różnych łańcuchów: %s\n", (eCompareString(cString, cDifferentString) == EQUAL) ? "OK\n\n" : "ERROR\n\n"); 
}


void TestOf_AppendString(void) { 
    char cSourceString[] = "bartek"; 
    char cDestinationString[20] = "imię: ";
    char cEmptySourceString[] = "";
    char cDestinationString2[20] = "student";
    char cEmptyDestinationString[] = "";
    
    printf("Test funkcji AppendString:\n");
    
    AppendString(cSourceString, cDestinationString); 
    printf("Test 1 - dodanie niepustego łańcucha do niepustego łańcucha: %s\n", (eCompareString(cDestinationString, "imię: bartek") == EQUAL) ? "OK" : "ERROR");

    AppendString(cEmptySourceString, cDestinationString2);
    printf("Test 2 - dodanie pustego łańcucha do niepustego łańcucha: %s\n", (eCompareString(cDestinationString2, "student") == EQUAL) ? "OK" : "ERROR");

    AppendString(cSourceString, cEmptyDestinationString);
    printf("Test 3 - dodanie niepustego łańcucha do pustego łańcucha: %s\n", (eCompareString(cEmptyDestinationString, "bartek") == EQUAL) ? "OK\n\n" : "ERROR\n\n");
}


void TestOf_ReplaceCharactersInString(void) { 
    char cBasicString[] = "matrix max mix"; 
    char cOldChar = 'x';
    char cNewChar = 'z';
    
    printf("Test funkcji ReplaceCharactersInString:\n");
    
    ReplaceCharactersInString(cBasicString, cOldChar, cNewChar);
    printf("Test 1 - zamiana poszczególnych znaków w łańcuchu: %s\n", (eCompareString(cBasicString, "matriz maz miz") == EQUAL) ? "OK\n\n" : "ERROR");
}


void TestOf_UIntToHexStr(void) { 
    char cStr[7]; 
    unsigned int uiValue1 = 123;

    printf("Test funkcji UIntToHexStr:\n");

    UIntToHexStr(uiValue1, cStr);
    printf("Test 1 - skonwertowanie liczby dziesiętnej na heksadecymalną: %s\n", (eCompareString(cStr, "0x007B") == EQUAL) ? "OK\n\n" : "ERROR\n\n");
}


void TestOf_eHexStringToUInt(void) {
    char cExpectedStr[7];
    char cGoodFormatStr[] = "0x00AF";
    char cWrongFormatStr1[] = "0xABCDE";
    char cWrongFormatStr2[] = "0x";
    char cWrongFormatStr3[] = "0xZZ99";
    unsigned int uiResult; 
    
    printf("Test funckji eHexStringToUInt:\n");
    
    eHexStringToUInt(cGoodFormatStr, &uiResult); 
    UIntToHexStr(uiResult, cExpectedStr);
    printf("Test 1 - konwertowanie poprawnego łańcucha heksadecymalnego na wartość: %s\n", (eCompareString(cGoodFormatStr, cExpectedStr) == EQUAL) ? "OK" : "ERROR");
    
    eHexStringToUInt(cWrongFormatStr1, &uiResult); 
    UIntToHexStr(uiResult, cExpectedStr);
    printf("Test 2 - konwertowanie za długiego łańcucha heksadecymalnego na wartość: %s\n", (eCompareString(cWrongFormatStr1, cExpectedStr) == EQUAL) ? "OK" : "ERROR");
    
    eHexStringToUInt(cWrongFormatStr2, &uiResult); 
    UIntToHexStr(uiResult, cExpectedStr);
    printf("Test 3 - konwertowanie zbyt krótkiego łańcucha na wartość: %s\n", (eCompareString(cWrongFormatStr2, cExpectedStr) == EQUAL) ? "OK" : "ERROR");
    
    eHexStringToUInt(cWrongFormatStr3, &uiResult); 
    UIntToHexStr(uiResult, cExpectedStr);
    printf("Test 4 - konwertowanie łańcucha z nieporawnymi znakami na wartość: %s\n", (eCompareString(cWrongFormatStr3, cExpectedStr) == EQUAL) ? "OK\n\n" : "ERROR\n\n");
}


void TestOf_AppendUIntToString(void) { 
    char cDestinationStr1[15] = "XYZ";
    char cDestinationStr2[15] = "";
    unsigned int uiValue = 255;
    
    printf("Test funkcji AppendUIntToString: \n");
    
    AppendUIntToString(uiValue, cDestinationStr1);
    printf("Test 1 - dodanie wartości przekonwertowanej na łańcuch heksadecymalny do niepustego łańcucha: %s\n", (eCompareString(cDestinationStr1, "XYZ0x00FF") == EQUAL) ? "OK" : "ERROR");

    AppendUIntToString(uiValue, cDestinationStr2);
    printf("Test 2 - dodanie wartości przekonwertowanej na łańcuch heksadecymalny do pustego łańcucha: %s\n", (eCompareString(cDestinationStr2, "0x00FF") == EQUAL) ? "OK\n\n" : "ERROR\n\n");
}


    
void TestOf_ucFindTokensInString(void) {
    char cString1[] = "load reset store";
    char cString2[] = "  reset   store load ";
    char cString3[] = "";
    char cString4[] = "     ";

    printf("Test funkcji ucFindTokensInString: \n");

    printf("Test 1 - sprawdzenie czy funkcja poprawnie zlicza znalezione tokeny: %s\n", (ucFindTokensInString(cString1) == 3) ? "OK" : "ERROR");
    printf("Test 2 - sprawdzenie czy funkcja poprawnie zlicza znalezione tokeny przy większej ilości delimiterów: %s\n", (ucFindTokensInString(cString2) == 3) ? "OK" : "ERROR");
    printf("Test 3 - sprawdzenie czy funkcja poprawnie zlicza znalezione tokeny w pustym łańcuchu: %s\n", (ucFindTokensInString(cString3) == 0) ? "OK" : "ERROR");
    printf("Test 4 - sprawdzenie czy funkcja poprawnie zlicza znalezione tokeny w łańcuchu z samymi delimiterami: %s\n", (ucFindTokensInString(cString4) == 0) ? "OK\n\n" : "ERROR\n\n");
}    
    
void TestOf_eStringToKeyword(void) { 
    enum KeywordCode eTest;
    char cString1[] = "load";
    char cString2[] = "loading";
    char cString3[] = "";
    
    printf("Test funkcji eStringToKeyword: \n");
    
    printf("Test 1 - sprawdzenie czy funkcja poprawnie zamienia słowo kluczowe na wartość enum: %s\n", ((eStringToKeyword(cString1, &eTest) == OK) && eTest == LD) ? "OK" : "ERROR");
    printf("Test 2 - sprawdzenie czy funkcja działa poprawnie przy innym słowie niż słowo kluczowe: %s\n", (eStringToKeyword(cString2, &eTest) == ERROR) ? "OK" : "ERORR");
    printf("Test 3 - sprawdzenie czy funkcja działa poprawnie przy pustym łańcuchu: %s\n", (eStringToKeyword(cString3, &eTest) == ERROR) ? "OK\n\n" : "ERROR\n\n");
}


void TestOf_DecodeTokens(void) {
    char cTest[] = "store 0x10 test";
    ucTokenNr = ucFindTokensInString(cTest);
    ReplaceCharactersInString(cTest, ' ', '\0');

    DecodeTokens();

    printf("Test funkcji DecodeTokens:\n");

    printf("Test 1 - token 0 to słowo kluczowe: %s\n", (asToken[0].eType == KEYWORD && asToken[0].uValue.eKeyword == ST) ? "OK" : "ERROR");
    printf("Test 2 - token 1 to liczba: %s\n", (asToken[1].eType == NUMBER && asToken[1].uValue.uiValue == 16) ? "OK" : "ERROR");
    printf("Test 3 - token 2 to tekst: %s\n", (asToken[2].eType == STRING && (eCompareString(asToken[2].uValue.pcString, "test")) == EQUAL) ? "OK\n\n" : "ERORR\n\n");
}

void TestOf_DecodeMsg(void) { 
    char cTest[] = "reset 0x20 wynik";
    
    DecodeMsg(cTest); 
    
    printf("Test funkcji DecodeMsg:\n");
    printf("Test 1 - dekodowanie pełnej wiadomości: %s\n", (
        (asToken[0].eType == KEYWORD && asToken[0].uValue.eKeyword == RST) &&
        (asToken[1].eType == NUMBER && asToken[1].uValue.uiValue == 32) &&
        (asToken[2].eType == STRING && asToken[2].uValue.pcString == &cTest[11])
    ) ? "OK" : "ERORR");
}


int main() {
    //testy do operacji prostych
    TestOf_CopyString();
    TestOf_eComapreString();
    TestOf_AppendString(); 
    TestOf_ReplaceCharactersInString();
    
    //testy do konwersji 
    TestOf_UIntToHexStr();
    TestOf_eHexStringToUInt();
    TestOf_AppendUIntToString();
    
    //testy do dekodowania
    TestOf_ucFindTokensInString();
    TestOf_eStringToKeyword();
    TestOf_DecodeTokens();
    TestOf_DecodeMsg();
    
    return 0;
}
