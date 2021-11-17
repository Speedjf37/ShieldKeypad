
#define SIMU
//#define SIMU_print

#ifdef SIMU
#define Simu_Cpt_Max 3  // nb de tour avant de simuler la butée
int Simu_Trou ;
int Simu_Butee ;
int Simu_Cpt ;
#endif

#define version "V 1.00"  //17/11/2021

//menu_shield_vis_mere_andoche.ino
//https://www.usinages.com/threads/programmation-compteur-vis-mere.147050/post-1852122


// affichage des informations pour le filetage au repère
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

int menu_Rang ;
int menu_Rang_old ;
int TimerState = 0;
int RUN =0;




//Programme pour retomber dans le pas après débrayage vis mère. AVEC BUTEE ARRIERE EN FONCTION
#define Pin_Codeur A1
#define Pin_Butee A3

int Trous = 0;                 //lecture de l'état de la pin analogique "A1" dans trous
int Etat_ButeeAR;              //lecture de l'état de la pin analogique "A3" dans "ButeeAR" Si ButeeAR=1
int VM;                        //Compteur Nombre de tours de la vis mère Evolu pendant la marche du tour
int CT ;                   //compte le nombre de trous sur le disque de la vis mère  10trous = 1 tour complet de vis mère
int VRCVM;                     //valeur référence compteur vis mère varie en fonction du pas à faire rentrée avec les boutons
//(résultat rapport pas à faire sur pas vis mère (pour Ramo T37 pas de 6mm))
int LastTrous = 0;              //inverse de Trous pour faire un FRONT MONTANT
int Last_lcd_key;


// Pour alleger le code il est divisé par LIB


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

    // affiche le nom du projet en cours pendant 2 secondes:
    lcd.setCursor(0,0); lcd.print(" Menu Shield LCD");
    lcd.setCursor(0,1); lcd.print("Vis Mere Andoche");
    lcd.print(version);
    
    Serial.println();
    Serial.print("Vis Mere Andoche");
    Serial.println(version);
    Serial.println();
    delay(2000);
    
    set_menu_exit(); // Force l'état MENU_NO ,lcd.clear et affichage principal
        
    CT = memory.d.CptTr +1 ; //11;  //Au départ mettre 1 trou en face de la fourche   

    Serial.print("CT : ");
    Serial.println(CT);
}

//#define MENU_NO_print
void loop()
{

  char buffercar[17];
    tNow = millis();

    T_Menu();

    if (Menu_Rang == MENU_NO )  // Si pas de menu en cours
      { // affichage principal
      if (menu_Rang != menu_Rang_old)
        {
        #ifdef MENU_NO_print
        Serial.println(" AFFICHAGE GLOBAL ");  
        #endif
        //lcd.clear(); déplacé dans set_menu_exit
        lcd.setCursor(1,0); 
        lcd.print("Tr:    ");
        lcd.setCursor(4, 0);
        lcd.print(memory.d.CptTr);
        
        lcd.setCursor(9,0); 
        lcd.print("VM:    ") ;           //affichage valeur compteur vis mère VM sur ardunio
        lcd.setCursor(12, 0);
        lcd.print(memory.d.CptVM);

        lcd.setCursor(9, 1);  
        lcd.print("       ") ;
        
        menu_Rang_old = menu_Rang ;
        }


 
/****** Traitement des entrées ***************/
     Etat_ButeeAR = analogRead(Pin_Butee);    // ON RECUPERE LES INFOS DE LA BUTEE ARRIERE
     if (Etat_ButeeAR > 500)
      {
        Etat_ButeeAR = 0;
      }
     else
      {
        Etat_ButeeAR = 1;
      }
 #ifdef SIMU
  Etat_ButeeAR = Simu_Butee;
 #endif 
         
 Trous = analogRead(Pin_Codeur);//lit la pin Analogique A1 pour récupérer les impulsions des trous (capteur fourche)
  // Serial.println (Trous);
  if (Trous >= 300)     //valeur référence élevée car les retour de la fourche varient
    {
    Trous = 1;
    }
 #ifdef SIMU
  Trous = Simu_Trou;
 #endif 
  if (Trous != LastTrous)     //l'action se fera sur le front montant de Trous (capteur fourche)
    {
     if (Trous == 1)
      {
      CT --;            // à chaque trou on fait moins 1 à CT (compteur trous)

  if ((VM == 1) && ( Etat_ButeeAR == 1))      //SI compteur vis mère=1 et compteur trous=10 (donc on attaque le dernier tour de vis mère avant embrayage) on met en route le chenillard
    { //_______________________________OUVERTURE BOUCLE CHENILLARD__________
    lcd.setCursor (1, 0);
      // ____________SI VM=1 ET CT= 0_____ON LANCE LE DEBUT DU CHENILLARD_______________________________
      if (CT == 8)          //Dans le dernier tour de vis mère suivant la valeur du compteur trous on affiche le chenillard avec CT 9 et on efface  1 à 1 avec  9 8 7 6....
        lcd.print("       88888888") ;   //les espaces vides permettent d'effacer les digits de l'afficheur
      else
      {
      lcd.setCursor (15-CT, 0);
      
      lcd.print(" ");
      if (CT == 0) 
        {
        menu_Rang_old--; // force le raffichage ligne 0
        #ifdef SIMU
        Simu_Cpt = 0;
        Simu_Butee =0;// simu l'enclenchement VM
        #endif
        
        }
      }   
    }

      
      if (CT == 0)          //SI compteur trous arrive à zéro
        {
        CT = memory.d.CptTr;     
        VM --;        // donc si CT=0 (la vis a fait un tour complet)  on fait -1 au compteur VM de la vis mère
        }
      if (VM == 0)              
        {
        VM =  memory.d.CptVM;    //(résultat rapport pas à faire sur pas vis mère (pour Ramo T37 pas de 6mm)
        }
      }// end  if (Trous == 1)


   lcd.setCursor(1, 1);        // Curseur sur ligne N°1 (bas) et position 9 (0, 1, 2, 3, >>>>>> 15) de l'afficheur LiquidCrystal
    lcd.print("CT:    ") ;            // affichage  valeur  compteur trous sur l'écran ARDUINO
    lcd.setCursor(4, 1);
    lcd.print(CT) ;            // affichage  valeur  compteur trous sur l'écran ARDUINO

    lcd.setCursor(9, 1);     // Curseur sur ligne N°1 (bas) et position 12    (0, 1, 2, 3, >>>>>> 15)
    if (Etat_ButeeAR == 1)           // si le detecteur arrière de Ø8mm (ou contact d'un inter) est enclanché (idem butée aux repères)
      {
      lcd.print("VM:    ") ;           //affichage valeur compteur vis mère VM sur ardunio
      lcd.setCursor(12, 1);
      lcd.print(VM) ;           //affichage valeur compteur vis mère VM sur ardunio
      }
     else
      lcd.print("       ") ;           //affichage valeur compteur vis mère VM sur ardunio
      

/*
    if (VM < 100)               //efface le dernier zéro du 100 quand le décrément arrive à 99 sur ardunio
    {
      lcd.setCursor(14, 1);
      lcd.print(" ") ;
    }
    if (VM < 10)                 //efface le  zéro du 10 quand le décrément arrive à 9 sur arduino
    {
      lcd.setCursor(13, 1);
      lcd.print(" ") ;
    }
*/
     LastTrous = Trous;    
    }// end !=Last trous
 
         
  
     // Traitement si temps ecoulé 1 seconde:
      if ( tNow - tPrevious >= 1000 )
        {
        tPrevious = tNow;

        RUN++;
        if (RUN>3)
          RUN=0;
        lcd.setCursor(0,0);
        lcd.print(Txt_Run[RUN]);

#ifdef SIMU
          Simu_Trou ^= 1;
          if (CT == memory.d.CptTr)
            {
            Simu_Cpt++;
            if (Simu_Cpt== Simu_Cpt_Max)
              Simu_Butee = 1;
            }
          #ifdef SIMU_print
          Serial.print("SIMU : ");
          Serial.print(Simu_Trou);
          Serial.print(" : ");
          Serial.println(Simu_Butee);
          Serial.print("CT : ");
          Serial.println(CT);
          #endif
#endif
        
        }// end t 1s
      
      // AFFICHAGE sur changement de valeur
            
  

      
    }// end aff no_menu
} // end loop
