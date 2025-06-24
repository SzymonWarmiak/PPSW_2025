#include <LPC21xx.H>

#define NULL '\0'
#define HEX_bm 0x000F

enum Result { OK, ERROR };

void UIntToHexStr(unsigned int uiValue, char pcStr[]) {
	
   unsigned char ucNibbleCounter;
	 unsigned char ucCurrentNibble;

    pcStr[0] = '0';
    pcStr[1] = 'x';

    for(ucNibbleCounter = 0; ucNibbleCounter < 4; ucNibbleCounter++) {
        ucCurrentNibble = (uiValue >> (ucNibbleCounter * 4)) & HEX_bm;

        if(ucCurrentNibble < 10) {
            pcStr[5 - ucNibbleCounter] = '0' + ucCurrentNibble;
        } else {
            pcStr[5 - ucNibbleCounter] = 'A' + (ucCurrentNibble - 10);
        }
    }

    pcStr[6] = NULL;
}

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue) {
    unsigned char ucCharacterCounter;
    unsigned char ucCurrentCharacter;
		
		*puiValue = 0;
    if ((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == NULL)) {
        return ERROR;
    }

    for (ucCharacterCounter = 2; ucCharacterCounter <= 6; ucCharacterCounter++) {
        ucCurrentCharacter = pcStr[ucCharacterCounter];

        if (ucCurrentCharacter == NULL) {
            return OK;
        } else if (ucCharacterCounter == 6) {
            return ERROR;
        }

        *puiValue = *puiValue << 4;

        if (ucCurrentCharacter >= '0' && ucCurrentCharacter <= '9') {
            ucCurrentCharacter = ucCurrentCharacter - '0';
        } else if (ucCurrentCharacter >= 'A' && ucCurrentCharacter <= 'F') {
            ucCurrentCharacter = ucCurrentCharacter - 'A' + 10;
        } else {
            return ERROR;
        }

        *puiValue |= ucCurrentCharacter;
    }

    return ERROR;
}

void AppendUIntToString(unsigned int uiValue, char pcDestinationStr[]) {
    unsigned char ucCharacterCounter;

    for (ucCharacterCounter = 0; pcDestinationStr[ucCharacterCounter] != NULL; ucCharacterCounter++) {}

    UIntToHexStr(uiValue, &pcDestinationStr[ucCharacterCounter]);
}

    char cBuffer[20] = "Wartosc: ";
    char cHexStr[7] = "abc";
		unsigned int uiValue = 65000;
    unsigned int uiConverted;
    enum Result eStatus;


int main() {

    UIntToHexStr(uiValue, cHexStr);
    AppendUIntToString(uiValue, cBuffer);
    eStatus = eHexStringToUInt(cHexStr, &uiConverted);

    while(1); 
}
