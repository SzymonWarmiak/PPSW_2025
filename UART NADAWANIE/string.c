#include "string.h"

void CopyString(char pcSource[], char pcDestination[]) {
    unsigned char i = 0;
    while (pcSource[i] != '\0') {
        pcDestination[i] = pcSource[i];
        i++;
    }
    pcDestination[i] = '\0';
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar) {
    unsigned char i = 0;
    while (pcString[i] != '\0') {
        if (pcString[i] == cOldChar) {
            pcString[i] = cNewChar;
        }
        i++;
    }
}

enum CompResult CompareString(char pcStr1[], char pcStr2[]) {
    unsigned char i = 0;
    while (pcStr1[i] != '\0' || pcStr2[i] != '\0') {
        if (pcStr1[i] != pcStr2[i]) return DIFFERENT;
        i++;
    }
    return EQUAL;
}
