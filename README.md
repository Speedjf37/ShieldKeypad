# ShieldKeypad

Cretion d'un menu pour Arduino shield LCD Keypad
Testé avec les cartes Arduino UNO  et MEGA2560

Gestion du LCD mode // 4 bits BackLight pin 10
* LIB LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


Gestion des boutons pin  Analog(0)
* LIB MD_UISwitch.h
Gestion des appuis courts/longs repeat doubles

Gestion de l'Eeprom pour sauvegarde des paramètres
* LIB EEPROM

Gestion de menu construit à partir du projet :

 * Name:     Arduino - Menu for Shield LCD
 * Autor:    Alberto Gil Tesa
 * Web:      http://giltesa.com
 * License:  CC BY-NC-SA 3.0
 * Date:     2016/12/10
 *
 
 Gestion du menu ( LCD : 16 car 2 lignes) (5 boutons: Select ,Haut, Bas, Gauche, Droit)
 Sous menu avec saisie de paramètres ,sous programme , sauve et restore Eeprom .
 
 le shield laisse libre les pins:
 * Digital 2,3 et 11,12,13 
 * Analog  A1, A2, A3, A4, A5
 * VIn Gnd(2) 5V 3V3 Reset
 
 
