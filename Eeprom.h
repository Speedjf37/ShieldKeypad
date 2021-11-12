
#include <EEPROM.h>

/*  CONFIGURATION */
struct MYDATA{
    int initialized;//1
    int Timer1_heu;//2
    int Timer1_min;//3
    int Timer1_sec;//4
    int Timer1_vit;//5
    int aff;//6
    int liai_mult;//7
    int liai_div;//8
    int mot_pas;//9
    int mot_sens;///10
    int mot_vitmin;//11
    int mot_vitmax;//12
    int mot_vit;//13
    int mot_accel;//14
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
 
void Write_Val_Defaut()
{
 #ifdef Eeprom_print
  Serial.println("Eeprom write val defaut") ;
 #endif
 // valeurs par defaut 
     // Serial.println("!memory.d.initialized") ;
        memory.d.initialized = true;
        memory.d.Timer1_heu   = 0;
        memory.d.Timer1_min   = 3;
        memory.d.Timer1_sec   = 30;
        memory.d.Timer1_vit   = 300;
        memory.d.aff          = 0;
        memory.d.liai_mult    = 1;
        memory.d.liai_div     = 1;
        memory.d.mot_pas      = 800;
        memory.d.mot_sens     = 0;
        memory.d.mot_vitmin   = 800;
        memory.d.mot_vitmax   = 12000;
        memory.d.mot_accel    = 9000;
        memory.d.mot_vit      = 10000;
        
        writeConfiguration();
}




#define Config_Print

#ifdef Config_Print
  extern const char *txMENU[];
  extern const byte iMENU ;

#endif


void readConfiguration()
{
  int val;
  int index;
 #ifdef Eeprom_print
  Serial.println("Eeprom readConfig") ;
 #endif
 for( int i=0 ; i < sizeof(memory.d) ; i++  )
       {
        memory.b[i] = EEPROM.read(i);
       }


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


  // calibrer les h m s )        
  if (memory.d.Timer1_heu   >23)
    memory.d.Timer1_heu   = 0;
  if (memory.d.Timer1_heu   <0)
    memory.d.Timer1_heu   = 0;
  if (memory.d.Timer1_min   >59)
    memory.d.Timer1_min   = 0;
  if (memory.d.Timer1_min   <0)
    memory.d.Timer1_min   = 0;
  if (memory.d.Timer1_sec   >59)
    memory.d.Timer1_sec   = 0;
  if (memory.d.Timer1_sec   <0)
    memory.d.Timer1_sec   = 0;

     if( !memory.d.initialized )
      {
      Write_Val_Defaut();
      }
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
