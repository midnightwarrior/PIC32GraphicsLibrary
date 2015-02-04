// Function prototypes

int main(void);
void Enable_Pulse(void);
void Draw_Point(unsigned short, unsigned short, unsigned short);
void GLCD_CLR(void);
void GLCD_Clrln(unsigned short);
void GLCD_Data(BYTE);
void GLCD_FlashRapidly(void);
void GLCD_ON(void);
unsigned short GLCD_Read(unsigned short);
void GLCD_Write(BYTE);
void GOTO_COL(unsigned int);
void GOTO_ROW(unsigned int);
void Set_Start_Line(unsigned short);
void GOTO_XY(unsigned int, unsigned int);
void delay_us(WORD);
void setDataBusAsRead(void);
void initLCD(void);
char** str_split(char* , const char);
char * strtok_single (char *, char const *);