/* CONSTANTES Y VARIABLES */
/*#define TYPE_SMENU1       1
#define TYPE_SMENU2       2
#define TYPE_BOOL         3
#define TYPE_INT          4
#define TYPE_SET          5
*/
enum choix_saisie 
  { 
    TYPE_SMENU1 = 0,
    TYPE_SMENU2,
    TYPE_BOOL ,
    TYPE_INT,
    TYPE_SET
  };

enum menu_mode 
  { 
    MENU_NO = 0,
    MENU_CHOIX,
    MENU_SAISIE
  };
unsigned long tNow      = 0;
unsigned long tPrevious = 0;

int Menu_Position       = 0;
int Menu_Position_Mem   =-1;
int Menu_Rang           = MENU_NO;
int Menu_Rang_Mem       =-1;
int INC = 0;
/* TEXTOS MENUS */
/* 
const byte iMENU = 11;
const char *txMENU[] = {
   "Ver tiempo     ", // Maximo columnsLCD - 1 caracteres
    "Unid. tiempo   ",
    "Eje X tiempo   ",
    "Eje Y tiempo   ",
    "Ver temperatura",
    "Unid. temp.    ",
    "Eje X temp.    ",
    "Eje Y temp.    ",
    "Guardar y salir",
    "Salir          "
*/

const byte iMENU = 15;
const char *txMENU[] = {// Maximo columnsLCD - 1 caracteres
    "Timer1 heure  ", 
    "Timer1 minute ",
    "Timer1 seconde",
    "Aff Mot/Sort  ",
    "Liai Sort *   ",
    "Liai Sort /   ",
    "Mot nb pas/tr ",
    "Mot sens      ",
    "Mot Vit Min   ",
    "Mot Vit Max   ",
    "Mot Vit       ",
    "Mot Accel     ",
    "Mot Test 1 Tr ",
    "Sauve   & Quit",
    "Restore & Quit",
    "Quit          "
    
};

/* TEXTOS SUB MENU 1 */
const byte  iMENU1 = 2;
const char *txSMENU1[] = {
    "   Moteur     ", // Max columnsLCD - 2 caracteres
    "   Sortie     "
};

/* TEXTOS SUB MENU 2 */
/*
const byte  iMENU2 = 2;
const char *txSMENU2[] = {
    "   Degres C   ", // Maximo columnsLCD - 2 caracteres
    "   Degres F   "
};
*/

void T_SubMenu( byte nameID, byte typeMenu, int *value, int minValue, int maxValue );


void set_menu_start()
{
   Menu_Rang = MENU_CHOIX ;// START MENU
   S.enableRepeat(true);
   S.enableLongPress(false);
   Serial.println("START MENU") ;
   aff_TimerState =-1;
} 
void set_menu_exit()
{
   Menu_Rang = MENU_NO; 
   TimerState = 0 ;
   TimerState_old = -1;
   aff_TimerState = -1;
   S.enableRepeat(false);
   S.enableLongPress(true); 
   Serial.println("END MENU") ;  
}

/**
 * Muestra el Menu principal en el LCD.
 */
 MD_UISwitch::keyResult_t k;
 int btn;
 
//#define Debug_key
void T_Menu(void)
{
//MD_UISwitch::keyResult_t k = S.read();
 k = S.read();
 btn = S.getKey();

#ifdef Debug_key
if ((btn !=0)&&(k !=0))
{
Serial.print("k : ") ;
Serial.print(k) ;
Serial.print(" btn : ") ;
Serial.println(btn) ;
}
#endif

if (k == MD_UISwitch::KEY_LONGPRESS) 
      {
      #ifdef Debug_key
      Serial.println("KEY_LONGPRESS Menu") ;
      #endif
      if (btn == btnSELECT) 
        {
        if (Menu_Rang == MENU_NO)
         set_menu_start();
        else
         if (Menu_Rang == MENU_CHOIX )
          set_menu_exit();
        }
      } // end KEY_LONGPRESS

if (k == MD_UISwitch::KEY_PRESS) 
     {     
     #ifdef Debug_key
      Serial.println("KEY_PRESS Menu") ;
      #endif
      Serial.println("KEY_PRESS Menu") ;
     if   (Menu_Rang == MENU_NO)
      {
      if( btn == btnUP )
        {
        //Serial.println(" btnUP");
        if (  TimerState < 2)
          TimerState ++; //Start Pause
        }
      else if( btn == btnDOWN )
        {
        //Serial.println(" btnDOWN");
          TimerState = 0;
        
        }
       else if( btn == btnRIGHT )
         {
         //Serial.println(" btnRIGHT");
          VitesseMot ++;
         }
       else if( btn == btnLEFT )
         {
         //Serial.println(" btnLEFT");
          if ( VitesseMot >2)
           VitesseMot --;
         }
         /*Serial.print("VitesseMot : ");
         Serial.println(VitesseMot);
         Serial.print("TimerState : ");
         Serial.println(TimerState);
        */
      } //end if   (Menu_Rang == MENU_NO)
     if   (Menu_Rang == MENU_CHOIX)
      {
      if( btn == btnUP )
        {
        Menu_Position--;
        //Serial.println(" Menu_Position --") ;
        if (Menu_Position < 0)
        Menu_Position = iMENU -1 ;
        }
      else if( btn == btnDOWN )
        {
        Menu_Position++;
        //Serial.println(" Menu_Position ++") ;
         if (Menu_Position >= iMENU )
         Menu_Position = 0;
        }
       else if( btn == btnRIGHT )
         {
          Menu_Rang ++; 
          btn=0;
          //Serial.println(" Menu_Rang ++") ;
         }
       else if( btn == btnLEFT )
         {
         set_menu_exit();         
         }
      }//if   (Menu_Rang == MENU_CHOIX)
 } // end KEY_PRESS
 
 if (Menu_Rang == MENU_SAISIE)
          {
            switch( Menu_Position )
            {
                case 0: T_SubMenu(  0, TYPE_INT,    &memory.d.Timer1_heu, 0, 23  ); break;
                case 1: T_SubMenu(  1, TYPE_INT,    &memory.d.Timer1_min, 0, 59  ); break;
                case 2: T_SubMenu(  2, TYPE_INT,    &memory.d.Timer1_sec, 0, 59 ); break;
                case 3: T_SubMenu(  3, TYPE_SMENU1, &memory.d.aff,        0, 2  ); break;
                case 4: T_SubMenu(  4, TYPE_INT,    &memory.d.liai_mult,  0, 100  ); break;
                case 5: T_SubMenu(  5, TYPE_INT,    &memory.d.liai_div,   0, 100  ); break;
                case 6: T_SubMenu(  6, TYPE_INT,    &memory.d.mot_pas,    0, 12000 ); break;
                case 7: T_SubMenu(  7, TYPE_BOOL,   &memory.d.mot_sens,   0, 1  ); break;
                case 8: T_SubMenu(  8, TYPE_INT,    &memory.d.mot_vitmin, 0, 12000 ); break;
                case 9: T_SubMenu(  9, TYPE_INT,    &memory.d.mot_vitmax, 0, 12000 ); break;
                case 10: T_SubMenu(  10, TYPE_INT,   &memory.d.mot_vit  , memory.d.mot_vitmin, memory.d.mot_vitmax  ); break;
                case 11: T_SubMenu( 11, TYPE_INT,   &memory.d.mot_accel,  0, 12000 ); break;
                
                case 12: 
                Mot_1Tr(); 
                Menu_Rang --; 
                btn=0;
                break;  
                
                case 13: 
                writeConfiguration(); 
                set_menu_exit();
                break; 
                
                case 14: 
                readConfiguration();   
                set_menu_exit();         
                break; 
                
                case 15:// Quit
                set_menu_exit();            
                break; 
            }
       
          }
  
       if ( ( Menu_Rang == MENU_CHOIX) && ( (Menu_Position != Menu_Position_Mem)||(millis()%500 == 0 )||(k == MD_UISwitch::KEY_PRESS) ))
        {
            Menu_Position_Mem = Menu_Position ;
            lcd.clear();
          
            if(  Menu_Position % rowsLCD == 0 )
              {
                for( int i=Menu_Position ; i<(Menu_Position+rowsLCD) ; i++ )
                {
                    lcd.setCursor(1, i % rowsLCD);
                    lcd.print( i<iMENU ? txMENU[i] : "                " );
                    //Serial.println( i<iMENU ? txMENU[i] : "                " );
                }
            }

            if( (Menu_Position-(rowsLCD-1)) % rowsLCD == 0 )
            {
                for( int i=(Menu_Position-(rowsLCD-1)) ; i<((Menu_Position-(rowsLCD-1))+rowsLCD) ; i++ )
                {
                    lcd.setCursor(1, i % rowsLCD);
                    lcd.print( i<iMENU ? txMENU[i] : "                " );
                   //  Serial.println( i<iMENU ? txMENU[i] : "                " );
                }
            }


            for( int i=0 ; i<rowsLCD ; i++ )
            {
                lcd.setCursor(0,i);
                lcd.print(" ");
            }
            lcd.setCursor(0, Menu_Position % rowsLCD );
            lcd.write(ARROW);
          }// end change 
          
} //end T_Menu



/**
 * Muestra el SubMenu en el LCD.
 *
 * @param nameID    Indice del array que contiene el titulo del submenu.
 * @param typeMenu  Segun el tipo, se representara el submenu de una forma u otra.
 * @param value     Puntero a la variable que almacena el dato, y que se modificara.
 * @param minValue  Valor minimo que puede tener la variable.
 * @param maxValue  Valor maximo que puede tener la variable.
 */
 /**
 * Affiche le sous-menu sur l'écran LCD.
 *
 * @param nameID Index du tableau qui contient le titre du sous-menu.
 * @param typeMenu Selon le type, le sous-menu sera représenté d'une manière ou d'une autre.
 * @param value Pointeur vers la variable qui stocke les données, et qui sera modifiée.
 * @param minValue Valeur minimale que peut avoir la variable.
 * @param maxValue Valeur maximale que peut avoir la variable.
 */


 
void T_SubMenu( byte nameID, byte typeMenu, int *value, int minValue, int maxValue )
{
/*  
  Serial.println("T_SubMenu");
  Serial.print(" btn : ") ;
  Serial.println(btn) ;
  */
  if (k == MD_UISwitch::KEY_PRESS) 
    {
      #ifdef Debug_key
      Serial.println("KEY_PRESS SubMenu") ;
      #endif
      
    if( btn == btnLEFT  )//Exit SubMenu
        {
        Menu_Rang = MENU_CHOIX;
        //Serial.println("Exit SubMenu");
        }
        else if( INC == 0 && btn == btnDOWN  && (*value)-1 >= minValue )
        {
            (*value)--;
        }
        else if( INC == 1 && btn == btnDOWN  && (*value)-10 >= minValue )
        {
            (*value)-=10;
        }
        else if( INC ==2 && btn == btnDOWN  && (*value)-100 >= minValue )
        {
            (*value)-=100;
        }
        else if( INC ==3 && btn == btnDOWN  && (*value)-1000 >= minValue )
        {
            (*value)-=1000;
        }
        else if( INC ==4 &&  btn == btnDOWN ) 
        {
        (*value)=minValue;
        }
        else if( INC ==0 &&  btn == btnUP && (*value)+1 <= maxValue )
        {
            (*value)++;
        }
        else if( INC ==1 &&  btn == btnUP && (*value)+10 <= maxValue )
        {
            (*value)+=10;
        }
        else if( INC ==2 &&  btn == btnUP && (*value)+100 <= maxValue )
        {
            (*value)+=100;
        }
        else if( INC ==3 &&  btn == btnUP && (*value)+1000 <= maxValue )
        {
            (*value)+=1000;
        }
        else if( INC ==4 &&  btn == btnUP ) 
        {
        (*value)=minValue;
        }
        else if( btn == btnRIGHT )
        {
        INC++;
        if (INC >4) 
        INC=0;
        //Serial.print("INC= ") ;
        //Serial.println(INC) ;
        }
  }// end KEY_PRESS
  
      if  ( ( Menu_Rang == MENU_SAISIE) && (( Menu_Rang != Menu_Rang_Mem) || (millis()%250 == 0 ) ||(k == MD_UISwitch::KEY_PRESS)) )
          {
            Menu_Rang_Mem =  Menu_Rang ;
            lcd.clear();
            //Serial.println("AFF T_SubMenu");
            lcd.setCursor(0,0);
            lcd.print(txMENU[nameID]);

            lcd.setCursor(0,1);
            lcd.print("<");
            
            if( typeMenu == TYPE_INT )
             lcd.setCursor(columnsLCD-5,1);
            else
             lcd.setCursor(columnsLCD-1,1);
             
            lcd.print(">");
            if( typeMenu == TYPE_INT )
              {
              if (INC==0) 
               lcd.print("   1"); 
              if (INC==1) 
               lcd.print("  10"); 
              if (INC==2) 
               lcd.print(" 100"); 
              if (INC==3) 
               lcd.print("1000"); 
              if (INC==4) 
               lcd.print("   0"); 
              }
            
            
            if( typeMenu == TYPE_SMENU1 )
            {
                lcd.setCursor(1,1);
                lcd.print(txSMENU1[*value]);
            }
/*            else if( typeMenu == TYPE_SMENU2 )
            {
                lcd.setCursor(1,1);
                lcd.print(txSMENU2[*value]);
            }
*/            else if( typeMenu == TYPE_BOOL )
            {
                lcd.setCursor(columnsLCD/2-1, 1);
                lcd.print(*value == 0 ? "0" : "1");
            }
              else if( typeMenu == TYPE_SET )
            {
                lcd.setCursor(columnsLCD/2-1, 1);
                lcd.print(*value == 0 ? "NON" : "OUI");
            }
            else if( typeMenu == TYPE_INT )
            {
                lcd.setCursor( (columnsLCD-4)/2-1, 1);
                lcd.print(*value);
                lcd.print(" ");
            }
        }// end menu_rang change
    
} //end T_SubMenu
