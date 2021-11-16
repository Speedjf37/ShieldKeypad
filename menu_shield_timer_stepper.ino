

#define version "V 1.00"  //15/11/2021

//menu_shield_timer_stepper.ino
//https://www.usinages.com/threads/reglage-vitesse-moteur-p-a-p-avec-minuterie.148831/post-1876324



// commande d'un moteur pas à pas avec temporisation
// paramétrage par menu déroulant
// Shield Arduino LCD Keypad
// LCD en port // 4 bits
// Clavier par résistances sur port analogique A0
// Les valeurs de test du clavier sont à modifier dans clavier.h en fonction des résistances implantées sur le shield
// La LIB MD_UISwitch est codée avec des temps courts pour la gestion des touches
// Cela peut être modifié dans le code de la LIB
/*
C:\Users\XXXXXX\Documents\Arduino\libraries\MD_UISwitch\src

MD_UISwitch.h

static const uint16_t KEY_PRESS_TIME = 150; ///< Default key press time in milliseconds
static const uint16_t KEY_DPRESS_TIME = 250; ///< Default double press time between presses in milliseconds
static const uint16_t KEY_LONGPRESS_TIME = 600; ///< Default long press detection time in milliseconds
static const uint16_t KEY_REPEAT_TIME = 300; ///< Default time between repeats in in milliseconds
static const uint8_t KEY_ACTIVE_STATE = LOW; ///< Default key is active low - transition high to low detection

Pour que ce soit plus confortable rallonger les tempos comme ceci:
static const uint16_t KEY_PRESS_TIME = 250; ///< Default key press time in milliseconds
static const uint16_t KEY_DPRESS_TIME = 350; ///< Default double press time between presses in milliseconds
static const uint16_t KEY_LONGPRESS_TIME = 700; ///< Default long press detection time in milliseconds
static const uint16_t KEY_REPEAT_TIME = 400; ///< Default time between repeats in in milliseconds
*/

// Saisie améliorée par choix de l'action avec < > ( valeur 0 ,1 ,10 ,100 ,1000 ) 

// Stepper Pin : Step 3 Dir 2

 // Version Speedjf37 à partir du projet:
 //https://giltesa.com/2016/10/24/menu-submenus-shield-lcd-arduino
 
    /**
 * Name:     Arduino - Menu for Shield LCD
 * Autor:    Alberto Gil Tesa
 * Web:      http://giltesa.com
 * License:  CC BY-NC-SA 3.0
 * Date:     2016/12/10
 *
 * Arduino Uno, Pinout:
 *         _______________
 *        |      USB      |
 *        |13           12|
 *        |3V3          11|
 *        |AREF         10|
 *   APAD |A0            9| LCD
 *        |A1            8| LCD
 *        |A2            7| LCD
 *        |A3            6| LCD
 *        |A4            5| LCD
 *        |A5            4| LCD
 *        |          3/SCL|
 *        |          2/SDA|
 *        |5V          GND|
 *        |RST         RST|
 *        |GND   1/INT2/RX|
 *        |VIN   0/INT3/TX|
 *        |MISO         SS|
 *        |SCK        MOSI|
 *        |_______________|
 *
*/




//Variables Globales
int TimerState = 0;
int TimerState_old=-1;
int aff_TimerState =-1;
int aff_VitesseMot = -1;
int VitesseMot = 0;
int VitesseMot_Old = -1;
int Count_Sec=0;
int aff_sec = -1;
int aff_min = -1;
int aff_heu = -1;
int new_sec = 0;
int new_min = 0; 
int new_heu = 0;
int RUN =0;


// Pour alleger le code il est divisé par LIB
void Mot_1Tr(void);


#include "Lcd.h"

char Txt_Run[4]={
'|',
'/',
'-',
C_BackSlash  //customBackslash voir Lcd.h
};





#include "Eeprom.h"
#include "Clavier.h"
#include "Menu.h"


//#define TONE_STEP_PIN 3   

#ifndef TONE_STEP_PIN
  #include "StepperIndexer.h"
#endif





/**
 *
 */
void setup()
{
  
    Serial.begin(57600);
    // Charge la configuration de l' EEPROM sinon config par défaut:
    readConfiguration();


    SetupLcd();
    
    SetupClavier();

    
    
    #ifndef TONE_STEP_PIN
      SetupStepperIndexer();
    #endif   


 
    // affiche le nom du projet en cours pendant 2 secondes:
    lcd.setCursor(0,0); lcd.print("Menu Shield LCD ");
    lcd.setCursor(0,1); lcd.print("Tim + Step ");
    lcd.print(version);
    
    Serial.println();
    Serial.print("Timer + Stepper ");
    Serial.println(version);
    Serial.println();
    delay(2000);
    set_menu_exit(); // Force l'état MENU_NO et affichage principal
}

//#define MENU_NO_print
void loop()
{

  char buffercar[17];
    tNow = millis();

    T_Menu();

    if (Menu_Rang == MENU_NO )  // Si pas de menu en cours
      { // affichage principal
      if (aff_TimerState == -1)
        { 
        //AFFICHAGE GLOBAL 
        #ifdef MENU_NO_print
          Serial.println(" AFFICHAGE GLOBAL ");  
        #endif
        lcd.clear();
        lcd.setCursor(2,0);
        if (TimerState == 0)
          lcd.print("S:");
        if (TimerState == 1)
          lcd.print("R:");
        if (TimerState == 2)
          lcd.print("P:");
            
        // AFF TIMER
        sprintf(buffercar,"T1:%02dh%02dm%02ds",Count_Sec/3600 %24 ,Count_Sec/60 %60,Count_Sec %60);
        lcd.print(buffercar);
        #ifdef MENU_NO_print
        Serial.print(" AFF TIMER :");
        Serial.println(buffercar);
        #endif
        }
        
      if(TimerState_old != TimerState)
      {
      #ifdef MENU_NO_print
        Serial.print("TimerState : ");
        Serial.println(TimerState);
      #endif
      
      if ( (TimerState == 1)&&(TimerState_old  == 0))//start
       {
       #ifdef MENU_NO_print
       Serial.println("Start Timer :");
       #endif
       Count_Sec =  (memory.d.Timer1_heu * 3600) + (memory.d.Timer1_min * 60) + memory.d.Timer1_sec ;
       
       #ifdef TONE_STEP_PIN
         tone(TONE_STEP_PIN, VitesseMot);
       #else
        {
        MyStepper.SetAccel(memory.d.mot_accel);
        VitesseMot=memory.d.mot_vit;
        MyStepper.SetSpeed(VitesseMot);
        lcd.print((VitesseMot/memory.d.mot_pas)*60);//tr/mn
        }
       #endif
       }
       
      if ( (TimerState == 0)&&(TimerState_old  != 0))//stop
        {
        //ARRET du moteur
        #ifdef MENU_NO_print
        Serial.println("Stop Timer :");
        #endif
      
        #ifdef TONE_STEP_PIN
          noTone(TONE_STEP_PIN);
        #else
         {
         VitesseMot =0;
         MyStepper.SetSpeed(VitesseMot);
         }
        #endif
        }
        
    TimerState_old = TimerState;
    }//end if (TimerState_old!=TimerState)

  
     // Traitement si temps ecoulé 1 seconde:
      if ( tNow - tPrevious >= 1000 )
        {
        tPrevious = tNow;

        RUN++;
        if (RUN>3)
          RUN=0;
        lcd.setCursor(0,0);
        lcd.print(Txt_Run[RUN]);

        if (TimerState == 1)
         if (Count_Sec>0)
          {
          Count_Sec --;
          if (Count_Sec == 0)// ARRET du moteur
           {
            // end timer Stop Stepper
            #ifdef TONE_STEP_PIN
              noTone(TONE_STEP_PIN);
            #else
            {
              VitesseMot =0;
              MyStepper.SetSpeed(VitesseMot);
            }
            #endif
           }
           new_sec=Count_Sec %60;
           new_min=Count_Sec/60 %60;
           new_heu=Count_Sec/3600 %24;
          }
          
       
       
        lcd.setCursor(0,2);
        lcd.print("Vit: ");
        lcd.setCursor(5,1);
        lcd.print("      ");
          
        lcd.setCursor(5,1);
        lcd.print((VitesseMot/memory.d.mot_pas)*60);//tr/mn
        lcd.setCursor(11,1);
        lcd.print("Tr/mn");
        
        
        aff_TimerState = TimerState ;
        
      }// end t 1s
      
      // AFFICHAGE sur changement de valeur
      if (TimerState != aff_TimerState)
        { 
        lcd.setCursor(2,0);
        if (TimerState == 0)
          lcd.print("S:");
        if (TimerState == 1)
          lcd.print("R:");
        if (TimerState == 2)
          lcd.print("P:");
        aff_TimerState = TimerState ;
        }
        
       if ( new_heu != aff_heu)
            {
            aff_heu = new_heu;
            lcd.setCursor(7,0);
            sprintf(buffercar,"%02d", new_heu);
            lcd.print(buffercar);
            Serial.print("Aff heu :");
            Serial.println(buffercar);
            }
       if ( new_min != aff_min)
            {
            aff_min = new_min;
            lcd.setCursor(10,0);
            sprintf(buffercar,"%02d", new_min);
            lcd.print(buffercar);
            Serial.print("Aff min :");
            Serial.println(buffercar);
            }
       if ( new_sec != aff_sec)
            {
            aff_sec = new_sec;
            lcd.setCursor(13,0);
            sprintf(buffercar,"%02d", new_sec);
            lcd.print(buffercar);
            Serial.print("Aff sec :");
            Serial.println(buffercar);
            }
          
            
        if (VitesseMot != aff_VitesseMot)
          {
          lcd.setCursor(5,1);
          lcd.print("      ");
          lcd.setCursor(5,1);
          lcd.print((VitesseMot/memory.d.mot_vit)*60);//tr/mn memory.d.mot_vit step/s
        
          aff_VitesseMot = VitesseMot;
          }
  

      
    }// end aff no_menu
} // end loop
