#include <Servo.h>
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#define motore1 9 (lo posso omettere perche tanto mi serve sempre a low quindi non faccio il collegamento)
#define motorer1 10
#define motore2 4
#define motorer2 5
#include <math.h>

String modalita="manuale";
unsigned long startMillis;  
unsigned long currentMillis;
bool primavolta=true;
bool sensor=false;//PER LA MODALITA ULTRASUONI
bool sensorTracking=false;//PER LA MODALITA TRACKING
bool ricarica=false;

//Ultrasuoni sensore
int trig=12;
int echo=11;
float min_val=4000;
float ultra_distance[181]={0};
int angle[181]={0};
int i2;
int j2;
//__________________

//LedIRTracking
int j=180;
int senLeft=5;      
int senCenter=4;  
int senRight=3;   
int senUp=2;      
int senDown=1;  
int limit=1000;              
int Left = 0; 
int Right = 0;
int Center = 0;
int Up = 0; 
int Down = 0;
int pos=90;
int posUpDown=90;
int minimo;
int min1;
int min2;
//______________

//IR receive
const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
//____________________

//Servomotori
Servo servo1;
Servo servo2;
Servo servo3;
int servoVal;
int n =90;
int m=90;
int d=10;
int buttonPin = 6; 
int buttonState = 0; 
int buttonState2 = 0; 
int i=0;
//____________

//Ricarica
int analogInPin = A0;
int sensorValue = 0;
int ammo=3; //Numero arbitrario, mtto 3 per semplicità
//_____________

LiquidCrystal_I2C schermo(0x27,16,2);
void setup() 
{
  pinMode( analogInPin, INPUT);
  LiquidCrystal_I2C schermo(0x27,16,2);
  pinMode(buttonPin, INPUT);
  //SCHERMO LCD
  schermo.begin();
  schermo.backlight();
  schermo.clear();
  //____________
  servo1.attach(3);
  servo2.attach(8);
  servo3.attach(7);
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  servo1.write(m);
  servo2.write(n);
  //pinMode(motore1, OUTPUT);
  pinMode(motorer1, OUTPUT);
  pinMode(motore2, OUTPUT);
  pinMode(motorer2, OUTPUT);
  servo3.write(180);
  //Ultrasuoni sensore
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  digitalWrite(trig,LOW);
  digitalWrite(echo,LOW);
  //_____
  schermo.clear();
  schermo.setCursor(0,0);
  schermo.print("Mod: "+String(modalita));
  schermo.setCursor(0,1);
  schermo.print("Munizioni: "+String(ammo));
  pinMode(9,OUTPUT);
}

float measure_distance_cm()
{
  float distance =0;
  long time_value=0;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  time_value=pulseIn(echo,HIGH);
  distance= 0.033*time_value/2;
  return distance;
}

void shoot(boolean sensorTracking)//se sto facendo tracking e ultrasensore non voglio vedere il numero di munizioni
{
  if(ammo>0)
  {
  //digitalWrite(motore1, LOW);
  digitalWrite(motorer1, HIGH);
  digitalWrite(motore2, LOW);
  digitalWrite(motorer2, HIGH);
  delay(3000);//numero secondi che dura lo sparo
  servo3.write(180);
  delay(10);
  servo3.write(120);
  delay(100);
  servo3.write(180);//tutto indietro
  delay(10);
  // digitalWrite(motore1, LOW);
  digitalWrite(motorer1, LOW);
  digitalWrite(motore2, LOW);
  digitalWrite(motorer2, LOW);
  ammo-=1;
  delay(500);
  }
  else
  {
    if(sensorTracking==false)//se sto facendo tracking NON visualizza se le munizioni sono finite, NON spara ma continua a seguirlo
    { //se sto facendo ultrasensore invece comunicherà se le munizioni sono finite
      schermo.clear();
      schermo.setCursor(0,0);
      schermo.print("Munizioni finite");
      delay(1000);
    }
  }
}

boolean acquisisci2()//usata per tracking
{
  if (irrecv.decode(&results))
  {
     if (results.value == 0xFF30CF)
     {  
          sensorTracking=false;
          modalita="manuale";
          schermo.clear();
          schermo.setCursor(0,0);
          schermo.print("Mod: "+String(modalita));
          schermo.setCursor(0,1);
          schermo.print("Munizioni: "+String(ammo));
          delay(100);
          servo2.write(90);
          servo1.write(90);
          j=181;
          delay(1000);
          return true;
     }
      irrecv.resume(); 
   }
   return false;
}

void tracking()  //posso fermarlo solo se non ha captato qualcosa (mentre è agganciato al bersaglio no)
{ 
  delay(150);
  if(acquisisci2()==true)
  {
    delay(100);
    j=181;
  }
  Left=analogRead(senLeft);         
  Center=analogRead(senCenter);   
  Right=analogRead(senRight); 
  Up=analogRead(senUp);  
  Down=analogRead(senDown);
  while((Right < limit || Left < limit || Center < limit || Up<limit || Down<limit))
  /*Entro solo se un valore di uno dei sensori scende ossia se ha captato il led bianco, di default i valori sono circa 1020*/
  {
      Left=analogRead(senLeft);         
      Center=analogRead(senCenter);   
      Right=analogRead(senRight); 
      Up=analogRead(senUp);  
      Down=analogRead(senDown); 
      minimo=min(Right,Left);
      min1=min(minimo,min(Up,Down));
      min2=min(min1,Center);
      if(min2==Right)
      {
        if (pos > 20)
        {
          pos--;
          servo2.write(pos);
          primavolta=true;
        }
      }
      if(min2==Left)
      {
        if (pos < 160)
        {
          pos += 1;
          servo2.write(pos);
          primavolta=true;
        }
      }
      if(min2==Up)
      {
        if (posUpDown < 160)
        {
          posUpDown += 1;
          servo1.write(posUpDown);
          primavolta=true;
        }
      }
      if(min2==Down)
      {
        if (posUpDown>20)
        {
          posUpDown--;
          servo1.write(posUpDown);
          primavolta=true;
        }
      }
      if(min2==Center)
      {
        if(primavolta==true)
        {
          startMillis = millis(); 
          primavolta=false;
        }
        else
        {
          //spara al bersaglio solo se rimane fermo per almeno 2 secondi (se continua a muoversi lo segue senza sparare)
          currentMillis = millis(); 
          if (currentMillis - startMillis >= 2000)
          {
            shoot(sensorTracking);
            primavolta=true;
          }
        } 
      }
      delay(30);//Aumentare il delay lo farà andare piu lento
  }
}  

boolean acquisisci()//usata per ultrasensore
{
   delay(100);
   if (irrecv.decode(&results))
      {
        //il delay(100) si può spostare anche qui rispetto a sopra
         if(results.value == 0xFF6897)
         {
            sensor=false;
            modalita="manuale";
            schermo.clear();
            schermo.setCursor(0,0);
            schermo.print("Mod: "+String(modalita));
            schermo.setCursor(0,1);
            schermo.print("Munizioni: "+String(ammo));
            delay(100);
            servo2.write(90);
            delay(1000);  
            i=181;  
            return true;
         }  
         irrecv.resume();
      }
      return false;
}

void ultrasensore()
{
    schermo.clear();
    schermo.setCursor(0,0);
    schermo.print("Mod: "+String(modalita));
    schermo.setCursor(0,1);
    schermo.print("Cerco bersaglio");
    delay(100);
    if(primavolta==true)
    {
     servo2.write(60);
    }
    else
    {
      servo2.write(i);//riparte dalla posizione dell'ultimo bersaglio che ha trovato
      //non riparte dall'angolo di 60°(dall'inizio) ma prosegue con l'angolo corrente del bersaglio appena trovato
    }
    min_val=100;//piu basso e piu è l'oggetto devo metterlo vicino
    delay(500);
    j=60;
    int index=0;
    for (i=60; i<=180;i+=2,j++)
    {
        if(acquisisci()==true)
        {
          break;
        }
        if(primavolta==false)//Se il bersaglio è ancora li e non si muove non parte da 60° ma riparte dalla posizione dell'ultimo bersagli
        {
          primavolta=true;
          i=i2;
          j=j2;
        }
        servo2.write(i);
        delay(160);
        angle[j]=i;
        ultra_distance[j]=measure_distance_cm();
        if(ultra_distance[j]<min_val)
        {
          schermo.clear();
          schermo.setCursor(0,0);
          schermo.print("Mod: "+String(modalita));
          schermo.setCursor(0,1);
          schermo.print("TROVATO");
          delay(10);
          shoot(sensorTracking);
          primavolta=false;
          i2=i;
          j2=j;
          break;
        }  
    }
   
}
void loop()
{
   if (irrecv.decode(&results))
   {
        if (results.value == 0XFFFFFFFF)
        {
          results.value = key_value;
        }
        switch(results.value)
        {
          case 0xFF22DD: // "|<<"
          if(d!=1)
          {
          d-=1;
          }
          break;
          
          case 0xFF02FD: // ">|"
          d=10;
          break;
            
          case 0xFFC23D: // ">>|"
          if(d!=90)
          {
          d+=1;
          }
          break;  
                       
          case 0xFF6897: // "0"
          //MODALITA ULTRASUONI
          do
          {
             sensor=true;
             irrecv.resume();//pronto a ricevere il prossimo valore
             modalita="ultrasuoni";
             ultrasensore();
          }while(sensor==true);
          break;  
          
          case 0xFF30CF: // "1"
          //MODALITA TRACKING
          modalita="tracking";
          schermo.clear();
          schermo.setCursor(0,0);
          schermo.print("Mod: "+String(modalita));
          delay(100);
          do
          {
             sensorTracking=true;
             irrecv.resume();//pronto a ricevere il prossimo valore
             j=180;
             tracking();
          }while(sensorTracking==true);
          break;
          
          case 0xFF18E7: // "2"
          m-=d;
          servo1.write(m);
          break;
          
          case 0xFF7A85: // "3"
          //MODALITA RICARICA (carica i proiettili dopodichè ripremere 3)
          delay(200);
          irrecv.resume();
          ricarica=true;
          if(ammo==3)//Se ho già il caricatore pieno non puo ricaricare
          {
              schermo.clear();
              schermo.setCursor(0,0);
              schermo.print("Impossibile");
              schermo.setCursor(0,1);
              schermo.print("Ricaricare"); 
              delay(2000);
              modalita="manuale";
              schermo.clear();
              schermo.setCursor(0,0);
              schermo.print("Mod: "+String(modalita));
              schermo.setCursor(0,1);
              schermo.print("Munizioni: "+String(ammo));
              delay(100);
              ricarica=false;
          }
          if(ricarica==true)
          {
            digitalWrite(9,HIGH);
             delay(100);
          }
          while(ricarica==true)
          {
              sensorValue = analogRead(analogInPin);
              if(sensorValue<300)
              {
                ammo+=1;
                if(ammo==4)
                {
                  ammo=3;
                }
              }
              schermo.clear();
              schermo.setCursor(0,0);
              schermo.print("Ricarica...");
              schermo.setCursor(0,1);
              schermo.print("Munizioni: "+String(ammo)); //Se è in modalita ricarica non puo fare niente altro (è in standby)
              delay(20);//tempo che impiega a venir giu il proittile
              if (irrecv.decode(&results))
              {
                 if(results.value == 0xFF7A85)
                 {
                    schermo.clear();
                    schermo.setCursor(0,0);
                    schermo.print("Ricarica");
                    schermo.setCursor(0,1);
                    schermo.print("Completata");
                    delay(2000);
                    modalita="manuale";
                    schermo.clear();
                    schermo.setCursor(0,0);
                    schermo.print("Mod: "+String(modalita));
                    schermo.setCursor(0,1);
                    schermo.print("Munizioni: "+String(ammo));
                    delay(100);
                    ricarica=false;
                }  
                irrecv.resume();
              }
          }
          digitalWrite(9,LOW);
          break;
          
          case 0xFF10EF: // "4"
          n+=d;
          servo2.write(n);
          break;
          
          case 0xFF38C7: // "5"
          shoot(sensorTracking);
          schermo.clear();
          schermo.setCursor(0,0);
          schermo.print("Mod: "+String(modalita));
          schermo.setCursor(0,1);
          schermo.print("Munizioni: "+String(ammo));
          delay(100);
          break;
          
          case 0xFF5AA5: // "6"
          n-=d;
          servo2.write(n);
          break;
          
          case 0xFF4AB5: // "8"
          m+=d;
          servo1.write(m);
          break;      
        }
        delay(100);
        key_value = results.value;
        irrecv.resume(); 
  }  
}
