#include "lancuchyznakowe.h"

#define NULL 		'\0'

void CopyString(char pcSource[], char pcDestination[]){
	
	unsigned char iCharacterCounter;
	
	for (iCharacterCounter = 0; pcSource[iCharacterCounter] != NULL; iCharacterCounter++){
		pcDestination[iCharacterCounter] = pcSource[iCharacterCounter];
	}
	pcDestination[iCharacterCounter] = NULL;
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){	
	unsigned char iCharacterCounter;
	
	for (iCharacterCounter = 0; pcStr1[iCharacterCounter] != NULL; iCharacterCounter++){
		if(pcStr1[iCharacterCounter] != pcStr2[iCharacterCounter]){
			return DIFFERENT;
		}
	}
	if(pcStr2[iCharacterCounter] == NULL){
		return EQUAL;
	} 
	return DIFFERENT;
}

void AppendString (char pcSourceStr[], char pcDestinationStr[]){
	
	unsigned char iNullPosition;
	
	for (iNullPosition = 0; pcDestinationStr[iNullPosition] != NULL; iNullPosition++){
	}
	CopyString(pcSourceStr, pcDestinationStr + iNullPosition);
}

void ReplaceCharactersInString (char pcString[], char cOldChar, char cNewChar){
	unsigned char iCharacterCounter;
	
	for(iCharacterCounter = 0; pcString[iCharacterCounter] != NULL; iCharacterCounter++){
		if(pcString[iCharacterCounter] == cOldChar){
			pcString[iCharacterCounter] = cNewChar;
		}
	}
}
