
#include <EEPROM.h>

/*  CONFIGURATION */
struct MYDATA{
    int initialized;
    int Timer1_heu;
    int Timer1_min;
    int Timer1_sec;
    int Timer1_vit;
    int aff;
    int liai_mult;
    int liai_div;
    int mot_pas;
    int mot_sens;
    int mot_vitmin;
    int mot_vitmax;
    int mot_vit;
    int mot_accel;
};
union MEMORY{
   MYDATA d;
   byte b[sizeof(MYDATA)];
}
memory;

void writeConfiguration();
void readConfiguration();

void readConfiguration()
{
 // Serial.println("readConfig") ;
    for( int i=0 ; i < sizeof(memory.d) ; i++  )
        memory.b[i] = EEPROM.read(i);
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
        memory.d.mot_accel    = 10000;
        
        writeConfiguration();
    }

 
 
 
 
 
}



/**
 *
 */
void writeConfiguration()
{
 // Serial.println("writeConfig") ;
    for( int i=0 ; i<sizeof(memory.d) ; i++  )
        EEPROM.write( i, memory.b[i] );
}
