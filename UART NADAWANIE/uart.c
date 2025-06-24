#include <LPC210X.H>
#include "string.h"

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

#define NULL '\0'
////////////// Zmienne globalne ////////////
char cOdebranyZnak;

#define RECIEVER_SIZE 20 

enum eRecieverStatus {EMPTY, READY, OVERFLOW};

struct RecieverBuffer {
    char cData[RECIEVER_SIZE];
    unsigned char ucCharCtr;
    enum eRecieverStatus eStatus;
};

#define TERMINATOR '\r' 

struct RecieverBuffer sBuffer;

void Reciever_PutCharacterToBuffer(char cCharacter) {
   if (sBuffer.ucCharCtr == RECIEVER_SIZE) {
        sBuffer.eStatus = OVERFLOW;
    }
		else if (cCharacter == TERMINATOR) {
        sBuffer.cData[sBuffer.ucCharCtr] = NULL;
        sBuffer.eStatus = READY;
        sBuffer.ucCharCtr = 0;
    }
		else {
        sBuffer.cData[sBuffer.ucCharCtr] = cCharacter;
        sBuffer.ucCharCtr++;
        sBuffer.eStatus = EMPTY;
		}
}

enum eRecieverStatus eReciever_GetStatus(void) {
    return sBuffer.eStatus;
}

void Reciever_GetStringCopy(char pcDestination[]) {
    CopyString(sBuffer.cData, pcDestination);
    sBuffer.eStatus = EMPTY;
}


enum eTransmiterStatus {FREE, BUSY};

#define TRANSMITER_SIZE 20 

typedef struct TransmiterBuffer {
    char cData[TRANSMITER_SIZE];
    enum eTransmiterStatus eStatus;
    unsigned char fLastCharacter;  
    unsigned char cCharCtr;        
} TransmiterBuffer;

TransmiterBuffer sTransmiterBuffer;

char Transmiter_GetCharacterFromBuffer(void) {
	
    char cChar = sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr];
	
		if (sTransmiterBuffer.fLastCharacter == 1) {
        sTransmiterBuffer.eStatus = FREE;
        sTransmiterBuffer.fLastCharacter = 0;
        return NULL; 
    }
		
    if (cChar == NULL) {
        sTransmiterBuffer.fLastCharacter = 1; 
        return '\r'; 
    }
    sTransmiterBuffer.cCharCtr++;
    return cChar;
}

void Transmiter_SendString(char cString[]) {
	
    unsigned char i = 0;
    while (cString[i] != NULL && i < TRANSMITER_SIZE-1) {
        sTransmiterBuffer.cData[i] = cString[i];
        i++;
    }
    sTransmiterBuffer.cData[i] = NULL; 

    sTransmiterBuffer.cCharCtr = 0;
    sTransmiterBuffer.eStatus = BUSY;
    sTransmiterBuffer.fLastCharacter = 0;

    U0THR = Transmiter_GetCharacterFromBuffer();
		U0IER |= mTHRE_INTERRUPT_ENABLE;
}






///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
      Reciever_PutCharacterToBuffer(U0RBR);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
			char cChar = Transmiter_GetCharacterFromBuffer();
			if (cChar != NULL) {
					U0THR = cChar;
			} else {
					U0IER &= ~mTHRE_INTERRUPT_ENABLE; // wylacz przerwanie nadajnika, bo juz nie ma co wysylac!
			}
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | (1 << 2);                                     // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000)/16)/uiBaudRate;                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE;               // // Wlaczamy przerwanie od odebrania znaku (gdy cos przyjdzie na UART0)

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
		
}
