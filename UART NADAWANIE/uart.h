
#define RECIEVER_SIZE 20  
enum eRecieverStatus {EMPTY, READY, OVERFLOW};

struct RecieverBuffer {
    char cData[RECIEVER_SIZE];
    unsigned char ucCharCtr;
    enum eRecieverStatus eStatus;
};


void UART_InitWithInt(unsigned int uiBaudRate);
void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
void Transmiter_SendString(char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);
