#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char opcion = '0';
String total;

int horas = 0;     
int minutos = 0;     
int segundos = 0;    
int decimas = 0;           
int milisegundos = 0;
int cuenta;

int a = 0;
int b = 0;

void setup() 
{
  Serial.begin(9600);
  
  lcd.begin(16, 2);
}

void loop() 
{
  lcd. setCursor(11, 0);
  lcd.print("Crono");
  lcd. setCursor(0, 0);
  lcd.print("h");
  lcd. setCursor(3, 0);
  lcd.print("m");
  lcd. setCursor(6, 0);
  lcd.print("s");
  lcd. setCursor(8, 0);
  lcd.print("ds");

  if( Serial.available()> 0) 
        opcion = Serial.read();

  inicio:
  if (opcion=='1')
  {
    contador:
    a = 1;
    b = 0;
    while (a == 1)
    {
    milisegundos = millis();
    if (milisegundos % 100 == 0) 
    {
    cuenta++;
    decimas++;
    if (decimas == 10) 
    {
    decimas = 0;
          segundos++;
        }
        if (segundos == 60)    {
          segundos = 0;
          minutos++;           }
        if (minutos == 60)     {
          minutos = 0;
          horas++;             }
        lcd.setCursor(0, 1);
        if (horas < 10)        {
          lcd.print("0");      }
        lcd.print(horas);
        lcd.print(":");
        lcd.setCursor(3, 1);
        if (minutos < 10)      {
          lcd.print("0");      }
        lcd.print(minutos);
        lcd.print(":");
        lcd.setCursor(6, 1);
        if (segundos < 10)     {
        lcd.print("0");        }
        lcd.print(segundos);
        lcd.print(":");
        lcd.setCursor(9, 1);
        lcd.print(decimas);

        if( Serial.available()> 0) 
          opcion = Serial.read();
        
        if (opcion=='2')      
        {
        delay(200);
          a = 0;
          b = 1;
          while (b == 1)                       
          {
            if( Serial.available()> 0) 
              opcion = Serial.read();
        
            if (opcion=='1') 
            {
            goto contador;   
            }
            if (opcion=='3')   
            { 
              total = String(cuenta);
              Serial.println(total);
              horas = 0;
              minutos = 0;
              segundos = 0;
              decimas = 0;
              cuenta = 0;
              lcd.clear();
              delay(200);
              goto inicio;    
              }
          }
        }
        
      }
    }
  }
}
