#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Separador.h>

#define pinBuzzer 9
#define TAM 20

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

struct fyhalarma
{
  
  String login;
  int anio, mes, dia, h, m, s;
};

enum {principal, hora, alarm, crono} estado;
enum {inicio, contador} cronom;

// Funciones prototipo
void imprimir(String cadena);
void obtener_datos_visual(void);
void Date_Time(void);
void alarma(void);
void tono_alarma(void);
void comprobar_alarmas(void);
void cronometro(void);

// Declaración de variables
char opcion = '0';
char opc = '0';
String total;

fyhalarma alarmas[TAM];
int total_alarmas=0;

int horas = 0;     
int minutos = 0;     
int segundos = 0;    
int decimas = 0;           
int milisegundos = 0;
int cuenta;

int a = 0;
int b = 0;

// Notas para el tono de la alarma
int Si2 =1975;
int LaS2=1864;
int La2= 1760;
int SolS2=1661;
int Sol2=1567;
int FaS2=1479;
int Fa2= 1396;
int Mi2= 1318;
int ReS2=1244;
int Re2= 1174;
int DoS2=1108;
int Do2= 1046;

int Si = 987;
int LaS= 932;
int La = 880;
int SolS=830;
int Sol= 783;
int FaS= 739;
int Fa=  698;
int Mi=  659;
int ReS= 622;
int Re = 587;
int DoS =554;
int Do = 523;

int rounda=0;
int roundp=0;
int white= 0;
int whitep=0;
int black=0;  
int blackp=0;
int quaver=0;
int quaverp =0;
int semiquaver=0;
int semiquaverp=0;
int bpm= 120;

void setup(){
  Serial.begin(9600);

  pinMode(pinBuzzer , OUTPUT);  //definir buzzer como salida
  
  // Inicializamos la pantalla LCD
  lcd.init();
  lcd.backlight();
  
  rtc.begin();

  #ifndef ESP8266
     while (!Serial); // Esperando conexión
  #endif
  

  // Comprobamos que el RTC funciona
  if (! rtc.begin()) {
    //lcd.setCursor(1,0);
    imprimir("No se encontro el RTC");
    lcd.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    imprimir("RTC no esta en funcionamiento!");
    
    // Ajustamos el RTC a la fecha y hora de la compilación
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // Definir notas alarma
  black= 35000/bpm; 
  blackp=black*1.5;
  white= black*2;
  whitep=white*1.5;
  rounda= black*4;
  roundp= rounda*1.5;
  quaver= black/2;
  quaverp=quaver*1.5;
  semiquaver= black/4;
  semiquaverp=semiquaver*1.5;
}

void loop(){

    switch(estado)
   {
    case principal:
    {
      Date_Time();
      comprobar_alarmas();
      if(Serial.available()>0)
      {
        opc = Serial.read();
        if(opc == '1')
           estado = hora;
        else
        {
          if(opc == '2')
             estado = alarm;
          else
          {
            if(opc == '3')
               estado = crono;
          }
        }
      }
      break;
    }
    case hora:
    {
      obtener_datos_visual();
      estado = principal;
      break;
    }
    case alarm:
    {
      alarma();
      estado = principal;
      break;
    }
    case crono:
    {
      cronometro();
      break;  
    }
  }
}

void imprimir(String cadena)
{
  int i, n;
  
  n=cadena.length();

  for (i=0;i<n;i++)
  {
    if (i>15)
    {
      delay(900);
      lcd.scrollDisplayLeft();
    }
    lcd.write(cadena.charAt(i));
  }
}

void obtener_datos_visual(void)
{
  String data=Serial.readStringUntil('\n');

  Separador s;

  // Separamos la fecha y la hora en dos strings diferentes utilizando la librería <Separador.h>
  String f = s.separa(data, ' ', 0);
  String h = s.separa(data, ' ', 1);

  // Imprimimos la fecha y la hora por separado
  lcd.setCursor(3,0);
  imprimir(f);
  lcd.setCursor(4,1);
  imprimir(h);

  delay(10000);

}

void Date_Time(void)
{
    DateTime now = rtc.now();
     char fecha[20], hora[20];
  
    // Convertimos la fecha y hora obtenida del rtc a formato String para poder mostrarla en el LCD
    sprintf(fecha, "%02d/%02d/%02d",now.day(), now.month(), now.year());
    sprintf(hora, "%02d:%02d:%02d",now.hour(), now.minute(), now.second());

    
    // Imprimimos la fecha y hora
    lcd.setCursor(3,0); 
    imprimir(fecha);
    lcd.setCursor(4,1);
    imprimir(hora);
}

void cronometro(void)
{
  lcd. setCursor(3, 0);
  imprimir("Cronometro");
  lcd. setCursor(2, 1);
  imprimir(" h:");
  lcd. setCursor(5, 1);
  imprimir(" m:");
  lcd. setCursor(8, 1);
  imprimir(" s:");
  lcd. setCursor(11, 1);
  imprimir("ds");

  if( Serial.available()> 0) 
        opcion = Serial.read();
  
  switch(cronom)
 {
  case inicio:
  {
    if (opcion=='1')
       cronom = contador;

    break;
  }
  case contador:
  {
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
        lcd.setCursor(3, 1);
        if (horas < 10)        {
          imprimir("0");      }
        String hour = (String)horas;
        imprimir(hour);
        imprimir(":");
        lcd.setCursor(6, 1);
        if (minutos < 10)      {
          imprimir("0");      }
        String min = (String)minutos;
        imprimir(min);
        imprimir(":");
        lcd.setCursor(9, 1);
        if (segundos < 10)     {
          imprimir("0");        }
        String sec = (String)segundos;
        imprimir(sec);
        imprimir(":");
        lcd.setCursor(12, 1);
        String dec = (String)decimas;
        imprimir(dec);
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
               a = 1;
               b = 0;
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
              delay(200);

              break;
             }
             else
                cronom = inicio;
          }
        }
     }
    }
    break;
  }
}  

void alarma(void)
{
  String c, str;
  int i;

  if ( Serial.available() > 0) // Si hay datos disponibles en el puerto serie.
  {
      i = total_alarmas;

      str = Serial.readStringUntil('\n');
      alarmas[i].login = str;
      Serial.println(str);// Envía la respuesta a través del puerto serie.
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarmas[i].anio = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarmas[i].mes = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarmas[i].dia = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarmas[i].h = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarmas[i].m = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarmas[i].s = str.toInt();
      Serial.println(str);
      
      total_alarmas++;
      
  }
}

void comprobar_alarmas(void)
{
  int j;

  DateTime now = rtc.now();
  for(j=0;j<total_alarmas;j++)
  {
   if (alarmas[j].anio == now.year() && alarmas[j].mes == now.month() && alarmas[j].dia == now.day() && alarmas[j].h == now.hour() && alarmas[j].m == now.minute() && alarmas[j].s == now.second())
      tono_alarma();
  }
}

void tono_alarma(void)
{
  tone(pinBuzzer,Mi,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  delay(black+50);
  tone(pinBuzzer,Si,black);
  delay(black+50);
  tone(pinBuzzer,Do2,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  
  delay(2*white+50);
  
  tone(pinBuzzer,Mi,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  delay(black+50);
  tone(pinBuzzer,Si,black);
  delay(black+50);
  tone(pinBuzzer,Do2,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  
  delay(2*white+50);
  
  tone(pinBuzzer,Mi,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  delay(black+50);
  tone(pinBuzzer,Si,black);
  delay(black+50);
  tone(pinBuzzer,Do2,white*1.3);
  delay(2*black+50);
  
  tone(pinBuzzer,Si,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  delay(black+50);
  tone(pinBuzzer,Do2,white*1.3);
  delay(2*black+50);
  
  tone(pinBuzzer,Si,black);
  delay(black+50);
  tone(pinBuzzer,La,black);
  delay(black+50);
  tone(pinBuzzer,Mi2,black);
  delay(white+50);
  tone(pinBuzzer,Mi2,black);
  delay(white+100);
  
  tone(pinBuzzer,Mi2,black);
  delay(white+50);
  tone(pinBuzzer,Re2,black);
  delay(black+50);
  tone(pinBuzzer,Mi2,black);
  delay(black+50);
  tone(pinBuzzer,Fa2,black);
  delay(black+50);
  tone(pinBuzzer,Fa2,white*1.3);
  delay(rounda+100);
  
  tone(pinBuzzer,Fa2,black);
  delay(black+50);
  tone(pinBuzzer,Mi2,black);
  delay(black+50);
  tone(pinBuzzer,Re2,black);
  delay(black+50);
  tone(pinBuzzer,Fa2,black);
  delay(black+50);
  tone(pinBuzzer,Mi2,white*1.3);
  delay(rounda+100);
  
  tone(pinBuzzer,Mi2,black);
  delay(black+50);
  tone(pinBuzzer,Re2,black);
  delay(black+50);
   tone(pinBuzzer,Do2,black);
  delay(black+50);
  tone(pinBuzzer,Si,white*1.3);
  delay(white+50);
  tone(pinBuzzer,Mi2,white*1.3);
  delay(white+50);
  tone(pinBuzzer,Si,white*1.3);
  delay(white+50);
  tone(pinBuzzer,Do2,white*1.3);
  delay(white+50);
  tone(pinBuzzer,La,rounda*1.3);
  delay(rounda+50);
}
