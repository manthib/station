#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <math.h>
LiquidCrystal lcd(8,7,6,5,4,3);
const int potar = 0;
const int potar2 = 1;
const int capteur =2;
const int capteur_amp = 3;
float tension = 0;
float Tare = 0;
float res = 0;
float ntc = 0;
double Rref = 10000.0;
double A = 3.9083*pow(10,-3);
double B = -5.775*pow(10,-7);
double A_0;double A_E;double A_1;
double B_0 = 0;double B_E;double B_1;
double C_0;double C_E;double C_1;
double D_0;double D_E;double D_1;
double R0 = 100; 
double T0 = 0;
double V_IN = 5;
volatile int compteur_fm = 0;
float valeur_entree_borne_inf = 0;
float valeur_entree_borne_sup = 1023;
float valeur_sortie_borne_inf = 0;
float valeur_sortie_borne_sup = 0;
float borne_inf_entree = 0;float borne_sup_entree = 0;
float borne_inf_sortie = 0;float borne_sup_sortie = 0;
float coef_mult;
float coef_add = 0;
const int BP_TARE = 15;
const int BP_SELECT = 10;
const int BP_LEFT = 14;
const int BP_RIGHT = 9;
const int BP_UP = 12;
const int BP_DOWN = 13;
const int BP_EXIT = 11;
const int BP_COEF_UP = 16;
const int BP_COEF_DOWN = 17;
int posMenu_entree = 0;
int posMenu_sortie = 0;
int posMenu_Volt = 0;
int posMenu_Ampere = 0;
int posMenu_Ohm = 0;
int precision = 0;
byte micro[8] = {B00000,B00000,B10001,B10001,B11001,B10110,B10000,B10000};
String Menu_entree[4]= {"Volt","Ampere","Frequence (Hz)","Ohm"};
String Menu_Volt[4] = {"Volt [0-1]","Volt [0-2]","Volt [0-5]","Volt [0-10]"}; 
String Menu_Ampere[2] = {"mA [0-20]","mA [4-20]"};
String Menu_Ohm[6] = {"[0-2]kohm","[0-10]kohm","Pt100","Pt500","Pt1000","Thermistance NTC"};
String Menu_sortie[26]= {"bar","mbar","degre","degre celcius","g","kg","g/m^3","Hz","HR","Lux","kLux","mm","micro-metre","m/s","km/h","mm/min","N","cN","daN","N.m","cN.m","Pa","HPa","tr/min","W/m^2","%"};
byte exitMenu = false;
byte exitBouton = false;
byte etat_select = false;
byte etat_tare = false;

void setup() {
  pinMode(BP_SELECT, INPUT);
  pinMode(BP_LEFT, INPUT);
  pinMode(BP_RIGHT, INPUT);
  pinMode(BP_EXIT, INPUT);
  pinMode(BP_UP, INPUT);
  pinMode(BP_DOWN, INPUT);
  pinMode(BP_TARE, INPUT);
  pinMode(BP_COEF_DOWN, INPUT);
  pinMode(BP_COEF_UP, INPUT);
  pinMode(capteur, INPUT);
  
  lcd.createChar(0, micro);
  lcd.begin(20,4);
  lcd.clear();
}

void loop() {

  exitBouton = false;
  exitMenu = false;
  etat_select = false ;
  lcd.clear();
  while(!etat_select){accueil();}
  etat_select = false;
  while(!exitMenu){
    navigation_menu_entree();
    affichageMenu_entree();
  }
  delay(400);
  exitMenu = false;
  etat_select = false;
  
  /* ############################## BOUCLE MENU VOLT ########################################### */
  
  if(posMenu_entree == 0){
    while(!exitMenu){
      
      navigation_menu_Volt();
      affichageMenu_Volt();
    } 
    
      if(posMenu_Volt == 0){
       analogReference(INTERNAL1V1); 
       delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure();affichage_res_unitee();}  
          Tare = res;
          while(!exitBouton){ tare(); affichage_res_unitee();}
          delay(400);
      }

      if(posMenu_Volt == 1){
       analogReference(INTERNAL2V56);
       delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure_2v56();affichage_res_unitee();}  
          Tare = res;
          while(!exitBouton){ tare_2v56(); affichage_res_unitee();}
          delay(400);
      }
      
      if(posMenu_Volt == 2){
        analogReference(DEFAULT);
        delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure();affichage_res_unitee();}  
          Tare = res;
          while(!exitBouton){ tare(); affichage_res_unitee();}
          delay(400);
      }

      if(posMenu_Volt == 3){
        analogReference(DEFAULT);
       delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure_10v();affichage_res_unitee();}  
          Tare = res;
          while(!exitBouton){ tare_10v(); affichage_res_unitee();}
          delay(400);
      }
  }

  
/* ############################## BOUCLE MENU AMPERE ########################################### */


  if(posMenu_entree == 1){
    while(!exitMenu){
      navigation_menu_Ampere();
      affichageMenu_Ampere();
    }

    if(posMenu_Ampere == 0){
       delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure_amp();affichage_res_unitee();}  
          Tare = res;
        //  while(!exitBouton){ tare_amp(); affichage_res_unitee();}
          delay(400);
    }

    if(posMenu_Ampere == 1){
      valeur_entree_borne_inf = 197;
       delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure_amp();affichage_res_unitee();}  
          Tare = res;
         // while(!exitBouton){ tare_amp(); affichage_res_unitee();}
          delay(400);
    
    }
  }
  
/* ############################## BOUCLE MENU FREQUENCE ########################################### */


  if(posMenu_entree == 2){
    delay(400);
    etat_select = false;
    while(!etat_select){select_coef_mult();}
    delay(400);
    etat_select = false;
    while(!etat_select){select_coef_add();}
    delay(400);

    etat_select = false;
    while(!exitMenu){
      navigation_menu_sortie();
      affichageMenu_sortie();
    }
    delay(400);
    etat_select = false;
    while(!etat_select){select_precision();}
    delay(400);
    etat_select = false;
    exitBouton = false;
    while(!etat_select){brancher_capteur();}
    etat_select=false;
    while(!exitBouton && !etat_tare){calcul_fm();affichage_res_unitee();}  
         
  }

/* ############################## BOUCLE MENU OHM ########################################### */


  if(posMenu_entree == 3){
    while(!exitMenu){
      navigation_menu_Ohm();
      affichageMenu_Ohm();
    }
 
    if(posMenu_Ohm == 0 || posMenu_Ohm == 1){
      delay(400);
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_inf();
             valeur_sortie_borne_inf = 1000 * borne_inf_sortie;
          }
        etat_select = false;
          while(!etat_select){
             select_plage_sortie_sup();
             valeur_sortie_borne_sup =1000 * borne_sup_sortie;
          }
        delay(500);
        etat_select = false;
        exitMenu = false;
          while(!exitMenu){
             navigation_menu_sortie();
             affichageMenu_sortie();
          }
          delay(400);
          etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
          while(!etat_select){brancher_capteur();}
          etat_select=false;
          while(!exitBouton && !etat_tare){mesure();affichage_res_unitee();}  
          Tare = res;
          while(!exitBouton){ tare(); affichage_res_unitee();}
          delay(400);
    }
    
    if(posMenu_Ohm == 2 ){
      R0=100;    
      delay(400);
      etat_select=false;
      while(!etat_select){select_precision();}
      delay(400);
      etat_select = false;  
      while(!etat_select){brancher_capteur();}      
      etat_select=false;
      exitBouton=false;
      while(!exitBouton){conversion_pt();}
    }

    if(posMenu_Ohm == 3 ){
      R0=500;
      delay(400);
      etat_select = false;
      while(!etat_select){select_precision();}
      delay(400);
      etat_select = false;  
      while(!etat_select){brancher_capteur();}      
      etat_select=false;
      exitBouton=false;
      while(!exitBouton){conversion_pt();}
    }
    
    if(posMenu_Ohm == 4 ){
      R0 = 1000;
      delay(400);
      etat_select = false;
      while(!etat_select){select_precision();}
      delay(400);
      etat_select = false;;  
      while(!etat_select){brancher_capteur();}      
      etat_select=false;
      exitBouton=false;
      while(!exitBouton){conversion_pt();}
    }

    if(posMenu_Ohm == 5){
      delay(400);
       etat_select = false;
       EEPROM.get(0,A_0);
       while(!etat_select){select_coef_a();}
       EEPROM.put(0,A_0);
         delay(400);
       etat_select = false;       
       EEPROM.get(4,A_E);
       while(!etat_select){select_exp_a();}
       EEPROM.put(4,A_E);
       delay(400);
       A_1 = A_0*pow(10,A_E);
         
       etat_select = false;
       EEPROM.get(8,B_0);
       while(!etat_select){select_coef_b();}
       EEPROM.put(8,B_0);
         delay(400);
       etat_select = false;
       EEPROM.get(12,B_E);
       while(!etat_select){select_exp_b();}
       EEPROM.put(12,B_E);
         delay(400);
         B_1 = B_0*pow(10,B_E);
       etat_select = false;
       EEPROM.get(16,C_0);
       while(!etat_select){select_coef_c();}
       EEPROM.put(16,C_0);
         delay(400);
       etat_select = false;
       EEPROM.get(20,C_E);
       while(!etat_select){select_exp_c();}
       EEPROM.put(20,C_E);
         delay(400);
         C_1 = C_0*pow(10,C_E);
       etat_select = false;
       EEPROM.get(24,D_0);
       while(!etat_select){select_coef_d();}
       EEPROM.put(24,D_0);
         delay(400);
       etat_select = false;
       EEPROM.get(28,D_E);
       while(!etat_select){select_exp_d();}
       EEPROM.put(28,D_E);
         delay(400);
         D_1 = D_0*pow(10,D_E);
       etat_select = false;
       EEPROM.get(32,Rref);
       while(!etat_select){select_Rref();}
       EEPROM.put(32,Rref);
         delay(400);
         etat_select = false;
          while(!etat_select){select_precision();}
          delay(400);
          etat_select = false;
       while(!etat_select){brancher_capteur();}      
         etat_select=false;
         exitBouton=false;
       while(!exitBouton){conversion_ntc();}   
    }
 }
  delay(400);
}


/* ############################## FONCTIONS INDEPENDANTES ########################################### */

/* ############################## ACCUEIL ########################################### */


void accueil(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);

  if(etat_BP_SELECT){etat_select = true;}

  lcd.setCursor(20,0);
  lcd.print("Appuyez sur select!");
  lcd.scrollDisplayLeft();
  delay(300);
}

/* ############################## COEFFICIENTS ########################################### */


void select_coef_mult(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  boolean etat_BP_COEF_DOWN = digitalRead(BP_COEF_DOWN);
  boolean etat_BP_COEF_UP = digitalRead(BP_COEF_UP);
  
  if(etat_BP_COEF_UP){
    coef_mult = coef_mult + 0.1;
    delay(50);
  }

    if(etat_BP_COEF_DOWN){
    coef_mult = coef_mult - 0.1;
     delay(50);
    }
    
  if(etat_BP_UP){
    coef_mult = coef_mult + 0.001;
    delay(50);
  }

    if(etat_BP_DOWN){
    coef_mult = coef_mult - 0.001;
     delay(50);
    }

    if(etat_BP_RIGHT){
    coef_mult = coef_mult + 0.00001;
    delay(50);
  }

  if(etat_BP_LEFT){
    coef_mult = coef_mult - 0.00001;
    delay(50);
  }
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("coef multiplicateur?");
  lcd.setCursor(6,2);
  lcd.print(coef_mult,5);
  delay(50);

  if(etat_BP_SELECT){
    etat_select = true;
    lcd.clear();
    }
  }

void select_coef_add(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  boolean etat_BP_COEF_DOWN = digitalRead(BP_COEF_DOWN);
  boolean etat_BP_COEF_UP = digitalRead(BP_COEF_UP);
 
  if(etat_BP_COEF_UP){
    coef_add = coef_add + 0.1;
    delay(50);
  }

    if(etat_BP_COEF_DOWN){
    coef_add = coef_add - 0.1;
     delay(50);
    }
    
  if(etat_BP_UP){
    coef_add = coef_add + 0.001;
    delay(50);
  }

    if(etat_BP_DOWN){
    coef_add = coef_add - 0.001;
     delay(50);
    }

    if(etat_BP_RIGHT){
    coef_add = coef_add + 0.00001;
    delay(50);
  }

  if(etat_BP_LEFT){
    coef_add = coef_add - 0.00001;
    delay(50);
  }
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("coef additionneur?");
  lcd.setCursor(6,2);
  lcd.print(coef_add,5);
  delay(50);

  if(etat_BP_SELECT){
    etat_select = true;
    lcd.clear();
    }
  } 

  /* ############################## FONCTION DES PLAGES ########################################### */
  
void select_plage_sortie_inf(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if(etat_BP_UP){
    borne_inf_sortie = borne_inf_sortie + 5;
    delay(50);
  }

    if(etat_BP_DOWN){
      borne_inf_sortie = borne_inf_sortie - 5;
     delay(50);
    }

    if(etat_BP_RIGHT){
    borne_inf_sortie = borne_inf_sortie + 1;
    delay(50);
  }

  if(etat_BP_LEFT){
    borne_inf_sortie = borne_inf_sortie - 1;
    delay(50);
  }
  lcd.clear();   
  lcd.setCursor(0,0);
  lcd.write("B inf sortie?");
  lcd.setCursor(0,1);
  lcd.print(borne_inf_sortie);
  delay(200);

  if(etat_BP_SELECT){
    etat_select = true;
    lcd.clear();
    delay(400);
    }
  }  

  void select_plage_sortie_sup(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if(etat_BP_UP){
    borne_sup_sortie = borne_sup_sortie + 5;
    delay(50);
  }

    if(etat_BP_DOWN){
      borne_sup_sortie = borne_sup_sortie - 5;
     delay(50);
    }

    if(etat_BP_RIGHT){
    borne_sup_sortie = borne_sup_sortie + 1;
    delay(50);
  }

  if(etat_BP_LEFT){
    borne_sup_sortie = borne_sup_sortie -1;
    delay(50);
  }
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("B sup sortie?");
  lcd.setCursor(0,1);
  lcd.print(borne_sup_sortie);
  delay(200);

  if(etat_BP_SELECT){
    etat_select = true;
    lcd.clear();
    }
  }  


  /* ############################## NAVIGATION DES MENU ########################################### */

  
  void navigation_menu_entree() {
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if( etat_BP_LEFT && posMenu_entree > 0) {
    lcd.clear();
    posMenu_entree = (posMenu_entree - 1) ;
    delay(400);
  }

  if(etat_BP_RIGHT && posMenu_entree < 3) {
    lcd.clear();
    posMenu_entree = (posMenu_entree + 1) ;
    delay(400);
  }
  if (etat_BP_SELECT){exitMenu = true;}
}

void navigation_menu_Volt() {
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if( etat_BP_LEFT && posMenu_Volt > 0) {
    lcd.clear();
    posMenu_Volt = (posMenu_Volt - 1) ;
    delay(400);
  }

  if(etat_BP_RIGHT && posMenu_Volt < 3) {
    lcd.clear();
    posMenu_Volt = (posMenu_Volt + 1) ;
    delay(400);
  }
  if (etat_BP_SELECT){exitMenu = true;}
}

void navigation_menu_Ampere() {
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if( etat_BP_LEFT && posMenu_Ampere > 0) {
    lcd.clear();
    posMenu_Ampere = (posMenu_Ampere - 1) ;
    delay(400);
  }

  if(etat_BP_RIGHT && posMenu_Ampere < 1) {
    lcd.clear();
    posMenu_Ampere = (posMenu_Ampere + 1) ;
    delay(400);
  }
  if (etat_BP_SELECT){exitMenu = true;}
}

void navigation_menu_Ohm() {
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if( etat_BP_LEFT && posMenu_Ohm > 0) {
    lcd.clear();
    posMenu_Ohm = (posMenu_Ohm - 1) ;
    delay(400);
  }

  if(etat_BP_RIGHT && posMenu_Ohm < 5) {
    lcd.clear();
    posMenu_Ohm = (posMenu_Ohm + 1) ;
    delay(400);
  }
  if (etat_BP_SELECT){exitMenu = true;}
}

  void navigation_menu_sortie() {
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  
  if( etat_BP_LEFT && posMenu_sortie > 0) {
    lcd.clear();
    posMenu_sortie = (posMenu_sortie - 1) ;
    delay(400);
  }

  if(etat_BP_RIGHT && posMenu_sortie < 25) {
    lcd.clear();
    posMenu_sortie = (posMenu_sortie + 1) ;
    delay(400);
  }
  if (etat_BP_SELECT){exitMenu = true;}
}


/* ############################## FONCTION POUR MESURER LES VOLT ET AMPERE ########################################### */

float map1(float tension,float valeur_entree_borne_inf, float valeur_entree_borne_sup,
                     float valeur_sortie_borne_inf, float valeur_sortie_borne_sup)
{
  return (tension - valeur_entree_borne_inf) * (valeur_sortie_borne_sup - valeur_sortie_borne_inf) / (valeur_entree_borne_sup - valeur_entree_borne_inf) + valeur_sortie_borne_inf;
}

void mesure(){
    lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    boolean etat_BP_TARE = digitalRead(BP_TARE);
    tension = analogRead(potar);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
     res = (valeur/1000);
    
    if(etat_BP_TARE){etat_tare = true;}

    if(etat_BP_EXIT){exitBouton = true;}
}

void mesure_10v(){
    lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    boolean etat_BP_TARE = digitalRead(BP_TARE);
    tension = analogRead(potar2);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
     res = (valeur/1000);
    
    if(etat_BP_TARE){etat_tare = true;}

    if(etat_BP_EXIT){exitBouton = true;}
}

void mesure_2v56(){
    lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    boolean etat_BP_TARE = digitalRead(BP_TARE);
    tension = analogRead(potar);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
     res = (valeur/1000)/0.82;
    
    if(etat_BP_TARE){etat_tare = true;}

    if(etat_BP_EXIT){exitBouton = true;}
}

void mesure_amp(){
    lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    boolean etat_BP_TARE = digitalRead(BP_TARE);
    tension = analogRead(capteur_amp);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
     res = (valeur);
    
    if(etat_BP_TARE){etat_tare = true;}
    
    if(etat_BP_EXIT){exitBouton = true;}
}

/* ##############################  AFFICHAGE DES MENU ########################################### */


void affichageMenu_entree(){
  lcd.clear();
  lcd.write("unitee en entree?");
  lcd.setCursor(0,1);
  lcd.print(Menu_entree[posMenu_entree]); 
  delay(50);
}

void affichageMenu_Volt(){
  lcd.clear();
  lcd.write("Plage en entree?");
  lcd.setCursor(0,1);
  lcd.print(Menu_Volt[posMenu_Volt]); 
  delay(50);
}

void affichageMenu_Ampere(){
  lcd.clear();
  lcd.write("Plage en entree?");
  lcd.setCursor(0,1);
  lcd.print(Menu_Ampere[posMenu_Ampere]); 
  delay(50);
}

void affichageMenu_Ohm(){
  lcd.clear();
  lcd.write("Type de sonde?");
  lcd.setCursor(0,1);
  lcd.print(Menu_Ohm[posMenu_Ohm]); 
  delay(50);
}

void affichageMenu_sortie(){
  lcd.clear();
  lcd.write("unitee en sortie?");
  lcd.setCursor(0,1);
  lcd.print(Menu_sortie[posMenu_sortie]); 
  delay(50);
}

void affichage_res_unitee(){
  lcd.clear();  
  boolean etat_BP_EXIT = digitalRead(BP_EXIT);
  switch(posMenu_sortie){
      case 0 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" bar"));delay(100); 
        break;
      case 1 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" mbar"));delay(100); 
        break; 
      case 2 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" degre"));delay(100);  
        break;
      case 3 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" *C"));delay(100);  
        break;
      case 4 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" g"));delay(100);  
        break;
      case 5 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" kg"));delay(100);  
        break;
      case 6 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" g/m^3"));delay(100);  
        break;
      case 7 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" Hz"));delay(100);  
        break;
      case 8 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" HR"));delay(100);  
        break;
      case 9 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" Lux"));delay(100);  
        break;
      case 10 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" kLux"));delay(100);  
        break;
      case 11 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" mm"));delay(100);  
        break;
      case 12 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(res,precision);lcd.setCursor(12,1);lcd.write((byte)0);lcd.setCursor(13,1);lcd.write("m");delay(100);  
        break;
      case 13 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" m/s"));delay(100);  
        break;
      case 14 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" km/h"));delay(100);  
        break;
      case 15 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" mm/min"));delay(100);  
        break;
      case 16 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" N"));delay(100);  
        break;
      case 17 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" cN"));delay(100);  
        break;
      case 18 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" daN"));delay(100);  
        break;
      case 19 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" N.m"));delay(100);  
        break;
      case 20 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" cN.m"));delay(100);  
        break;
      case 21 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" Pa"));delay(100);  
        break;
      case 22 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" HPa"));delay(100);  
        break;
      case 23 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" tr/min"));delay(100);  
        break;
      case 24 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" W/m^2"));delay(100);  
        break;
      case 25 :
        lcd.setCursor(3,0);lcd.write("mesure :");lcd.setCursor(4,1);lcd.print(String (res,precision) + String (" %"));delay(100);  
        break;
    }
}
/* ############################## FONCTION TARE POUR LES VOLTS ########################################### */


void tare(){

  lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    tension = analogRead(potar);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
    res = (valeur)/1000 - Tare;
    
    if(etat_BP_EXIT){exitBouton = true;}
}      

void tare_10v(){

  lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    tension = analogRead(potar2);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
    res = (valeur)/1000 - Tare;

    if(etat_BP_EXIT){exitBouton = true;}
}      

  void tare_2v56(){

  lcd.clear();
    boolean etat_BP_EXIT = digitalRead(BP_EXIT);
    tension = analogRead(potar);
    float valeur = map1(tension,valeur_entree_borne_inf,valeur_entree_borne_sup,
                      valeur_sortie_borne_inf,valeur_sortie_borne_sup);
    res = (valeur)/1000 - Tare;
  
    if(etat_BP_EXIT){exitBouton = true;}
}  

/* ############################## FONCTION DE COMPTAGE DE FRONTS MONTANT POUR LES HERTZ  ########################################### */

 
void calcul_fm(){
  boolean etat_BP_EXIT = digitalRead(BP_EXIT);
  compteur_fm = 0;
  res = coef_mult*compteur_fm + coef_add;
  delay(1000);
  attachInterrupt(0, anemometre, RISING); 
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.write("Vistesse du vent :");
  lcd.setCursor(6,1); 

   if(etat_BP_EXIT){exitBouton = true;}
}

void anemometre(){
  compteur_fm++;
}

/* ############################## FONCTION DE CONVERSION POUR LES Pt ########################################### */


void conversion_pt(){
  boolean etat_BP_EXIT = digitalRead(BP_EXIT);
   lcd.clear();
  double entree = analogRead(potar);
  double V = entree/1024 * V_IN;
  double Rpt = (R0 * V) / (V_IN - V);
   double temperature = calcul_pt(Rpt);
  lcd.print(temperature,precision);
   delay(400);

   if(etat_BP_EXIT){exitBouton = true;}
  }

double calcul_pt (double R){
  double equation1 = (R/R0)-1;
  double equation2 = pow(A,2) + 4*B*equation1;
  double equation3 = pow(equation2,0.5)-A;
  double equation = T0 + (equation3/(2*B));
  return equation;
  }

/* ############################## FONCTIONS DE SELECTION POUR LES COEF DE LA NTC ########################################### */


void select_coef_a(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  boolean etat_BP_COEF_DOWN = digitalRead(BP_COEF_DOWN);
  boolean etat_BP_COEF_UP = digitalRead(BP_COEF_UP);

  if(etat_BP_COEF_UP && etat_BP_UP){A_0 = A_0 + 1;delay(350);}
  if(etat_BP_COEF_DOWN && etat_BP_DOWN){A_0 = A_0 - 1;delay(350);}  
  if(etat_BP_COEF_UP && !etat_BP_UP){A_0 = A_0 + 0.01;delay(50);}
  if(etat_BP_COEF_DOWN && !etat_BP_DOWN){A_0 = A_0 - 0.01;delay(50);}  
  if(etat_BP_UP && !etat_BP_COEF_UP){A_0 = A_0 + 0.0001;delay(50);}
  if(etat_BP_DOWN && !etat_BP_COEF_DOWN){A_0 = A_0 - 0.0001;delay(50);}
  if(etat_BP_RIGHT){A_0 = A_0 + 0.000001;delay(50);}
  if(etat_BP_LEFT){A_0 = A_0 - 0.000001;delay(50);}
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("coef A-0?");
  lcd.setCursor(6,2);
  lcd.print(A_0,6);
  delay(50);

  if(etat_BP_SELECT){etat_select = true;lcd.clear();}
  }

void select_exp_a(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);

  if(etat_BP_RIGHT){A_E = A_E + 1;delay(250);}
  if(etat_BP_LEFT){A_E = A_E - 1;delay(250);}
  if(etat_BP_SELECT){etat_select = true;lcd.clear();}

  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("Exposant A-E?");
  lcd.setCursor(10,2);
  lcd.print(A_E,0);
  delay(50);
}

void select_coef_b(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  boolean etat_BP_COEF_DOWN = digitalRead(BP_COEF_DOWN);
  boolean etat_BP_COEF_UP = digitalRead(BP_COEF_UP);
  
  if(etat_BP_COEF_UP && etat_BP_UP){B_0 = B_0 + 1;delay(350);}
  if(etat_BP_COEF_DOWN && etat_BP_DOWN){B_0 = B_0 - 1;delay(350);}
  if(etat_BP_COEF_UP && !etat_BP_UP){B_0 = B_0 + 0.01;delay(50);}
  if(etat_BP_COEF_DOWN && !etat_BP_DOWN){B_0 = B_0 - 0.01;delay(50);} 
  if(etat_BP_UP && !etat_BP_COEF_UP){B_0 = B_0 + 0.0001;delay(50);}
  if(etat_BP_DOWN && !etat_BP_COEF_DOWN){B_0 = B_0 - 0.0001;delay(50);}
  if(etat_BP_RIGHT){B_0 = B_0 + 0.000001;delay(50);}
  if(etat_BP_LEFT){B_0 = B_0 - 0.000001;delay(50);}
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("coef B-0?");
  lcd.setCursor(6,2);
  lcd.print(B_0,6);
  delay(50);

  if(etat_BP_SELECT){etat_select = true;lcd.clear();}
}

void select_exp_b(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);

  if(etat_BP_RIGHT){B_E = B_E + 1;delay(250);}
  if(etat_BP_LEFT){B_E = B_E - 1;delay(250);}
  if(etat_BP_SELECT){etat_select = true;lcd.clear();}

  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("Exposant B-E?");
  lcd.setCursor(10,2);
  lcd.print(B_E,0);
  delay(50);
}

void select_coef_c(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  boolean etat_BP_COEF_DOWN = digitalRead(BP_COEF_DOWN);
  boolean etat_BP_COEF_UP = digitalRead(BP_COEF_UP);
  
  if(etat_BP_COEF_UP && etat_BP_UP){C_0 = C_0 + 1;delay(350);}
  if(etat_BP_COEF_DOWN && etat_BP_DOWN){C_0 = C_0 - 1;delay(350);} 
  if(etat_BP_COEF_UP && !etat_BP_UP){C_0 = C_0 + 0.01;delay(50);}
  if(etat_BP_COEF_DOWN && !etat_BP_DOWN){C_0 = C_0 - 0.01;delay(50);}
  if(etat_BP_UP && !etat_BP_COEF_UP){C_0 = C_0 + 0.0001;delay(50);}
  if(etat_BP_DOWN && !etat_BP_COEF_DOWN){C_0 = C_0 - 0.0001;delay(50);}
  if(etat_BP_RIGHT){C_0 = C_0 + 0.000001;delay(50);}
  if(etat_BP_LEFT){C_0 = C_0 - 0.000001;delay(50);}
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("coef C-0?");
  lcd.setCursor(6,2);
  lcd.print(C_0,6);
  delay(50);

  if(etat_BP_SELECT){etat_select = true;lcd.clear();}
}

void select_exp_c(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);

  if(etat_BP_RIGHT){C_E = C_E + 1;delay(250);}
  if(etat_BP_LEFT){C_E = C_E - 1;delay(250);}
  if(etat_BP_SELECT){etat_select = true;lcd.clear();}

  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("Exposant C-E?");
  lcd.setCursor(10,2);
  lcd.print(C_E,0);
  delay(50);
}

void select_coef_d(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_UP = digitalRead(BP_UP);
  boolean etat_BP_DOWN = digitalRead(BP_DOWN);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  boolean etat_BP_COEF_DOWN = digitalRead(BP_COEF_DOWN);
  boolean etat_BP_COEF_UP = digitalRead(BP_COEF_UP);
  
  if(etat_BP_COEF_UP && etat_BP_UP){D_0 = D_0 + 1;delay(350);}
  if(etat_BP_COEF_DOWN && etat_BP_DOWN){D_0 = D_0 - 1;delay(350);}
  if(etat_BP_COEF_UP && !etat_BP_UP){D_0 = D_0 + 0.01;delay(50);}
  if(etat_BP_COEF_DOWN && !etat_BP_DOWN){D_0 = D_0 - 0.01;delay(50);}
  if(etat_BP_UP && !etat_BP_COEF_UP){D_0 = D_0 + 0.0001;delay(50);}
  if(etat_BP_DOWN && !etat_BP_COEF_DOWN){D_0 = D_0 - 0.0001;delay(50);}
  if(etat_BP_RIGHT){D_0 = D_0 + 0.000001;delay(50);}
  if(etat_BP_LEFT){D_0 = D_0 - 0.000001;delay(50);}
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("coef D-0?");
  lcd.setCursor(6,2);
  lcd.print(D_0,6);
  delay(50);

  if(etat_BP_SELECT){etat_select = true;lcd.clear();}
}

void select_exp_d(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);

  if(etat_BP_RIGHT){D_E = D_E + 1;delay(250);}
  if(etat_BP_LEFT){D_E = D_E - 1;delay(250);}
  if(etat_BP_SELECT){etat_select = true;lcd.clear();}

  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("Exposant D-E?");
  lcd.setCursor(10,2);
  lcd.print(D_E,0);
  delay(50);
}

/* ############################## FONCTION DE SELECTION DE Rref POUR LA NTC ########################################### */

void select_Rref(){
   boolean etat_BP_SELECT = digitalRead(BP_SELECT);
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);

  if(etat_BP_RIGHT){Rref = Rref + 1000;delay(250);}
  if(etat_BP_LEFT){Rref = Rref - 1000;delay(250);}
  if(etat_BP_SELECT){etat_select = true;lcd.clear();}

  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("Rref ?");
  lcd.setCursor(6,2);
  lcd.print(Rref,0);
  delay(50);
  
}

/* ##############################  FONCTION DE CONVERSION POUR LA NTC  ########################################### */


void conversion_ntc(){
  boolean etat_BP_EXIT = digitalRead(BP_EXIT);
  lcd.clear();
  ntc = analogRead(potar);
  double V = ntc/1024 * V_IN;
  double Rth = (Rref * V) / (V_IN - V);
  double kelvin = calcul(Rth);
  float celsius = kelvin - 273.15;
  lcd.print(celsius,precision);
  delay(400);

  if(etat_BP_EXIT){exitBouton = true;}
}

double calcul (double R){
  double equationB1 = B_1 * log(R/Rref);
  double equationC1 = C_1 * pow(log(R/Rref),2);
  double equationD1 = D_1 * pow(log(R/Rref),3);
  double equation = A_1 + equationB1 + equationC1 + equationD1;
  return pow(equation, -1);
}

/* ############################## PRECISION ########################################################################### */

void select_precision(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);  
  boolean etat_BP_LEFT = digitalRead(BP_LEFT);
  boolean etat_BP_RIGHT = digitalRead(BP_RIGHT);
  

    if(etat_BP_RIGHT){
      if(precision < 6){
    precision = precision + 1;
    delay(200);
      }
  }

  if(etat_BP_LEFT){
    if(precision > 0){
    precision = precision - 1;
    delay(200);
    }
  }
  
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.write("Precision ?");
  lcd.setCursor(6,2);
  lcd.print(precision);
  delay(50);

  if(etat_BP_SELECT){
    etat_select = true;
    lcd.clear();
    }
  }

/* ############################## AFFICHAGE POUR LE BRANCHEMENT DU CAPTEUR  ########################################### */


void brancher_capteur(){
  boolean etat_BP_SELECT = digitalRead(BP_SELECT);

  if(etat_BP_SELECT){etat_select = true;}
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.write("Veuillez brancher");
  lcd.setCursor(2,2);
  lcd.write("votre capteur svp!");
  delay(300);
}
