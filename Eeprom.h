
#include <EEPROM.h>

/*  CONFIGURATION */
struct MYDATA{
    int initialized;//1
    int CptVM;//2
    int CptTr;//3
 };
union MEMORY{
   MYDATA d;
   byte b[sizeof(MYDATA)];
}
memory;

void writeConfiguration();
void readConfiguration();
void Write_Val_Defaut();

#define Eeprom_print

#define Config_Print

#ifdef Config_Print
  extern const char *txMENU[];
  extern const byte iMENU ;

#endif

void PrintConfiguration()
{
  int val;
  int index;

 #ifdef Config_Print
        Serial.print(" sizeof(memory.d) :");
        Serial.println(sizeof(memory.d) );
    #endif 

    index = -1;// 1 case memoire hors menu
       
    for( int i=0 ; i < sizeof(memory.d) ; i+=2  )
       {
       #ifdef Config_Print 
        Serial.print(index+1);
        Serial.print(" :");
        if (index == -1)//case memoire hors menu
          Serial.print("Initialised ");
        else
          Serial.print(txMENU[index]);
        
        Serial.print(" :");
        val = memory.b[i+1 ]*256  + memory.b[i];// convertion 8 bits > 16 bits
        Serial.println(val);
       #endif
       
       index++;
       }

}


 
void Write_Val_Defaut()
{
 #ifdef Eeprom_print
  Serial.println("Eeprom write val defaut") ;
 #endif
 // valeurs par defaut 
     // Serial.println("!memory.d.initialized") ;
        memory.d.initialized = true;
        memory.d.CptVM   = 8;
        memory.d.CptTr   = 10;
        
 writeConfiguration();

 PrintConfiguration();
}






void readConfiguration()
{
 #ifdef Eeprom_print
  Serial.println("Eeprom readConfig") ;
 #endif
 for( int i=0 ; i < sizeof(memory.d) ; i++  )
       {
        memory.b[i] = EEPROM.read(i);
       }


     if( !memory.d.initialized )
      {
      Write_Val_Defaut();
      }
PrintConfiguration();      
}


/**
 *
 */
void writeConfiguration()
{
 #ifdef Eeprom_print
  Serial.println("Eeprom writeConfig") ;
 #endif
      for( int i=0 ; i<sizeof(memory.d) ; i++  )
        EEPROM.write( i, memory.b[i] );
}
