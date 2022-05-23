#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Separador.h>

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// GND pin al pin tierra en el arduino
// VCC pin al pin 5V en el arduino
// SDA pin al pin A4 en el arduino
// SCL pin al pin A5 en el arduino

struct fyhalarma
{
  
  String login;
  int anio, mes, dia, h, m, s;
};

enum {principal, hora, alarm, crono} estado;

// Funciones prototipo
void imprimir(String cadena);
void obtener_datos_visual(void);
void Date_Time(void);
void alarma(void);
void cronometro(void);

// Declaración de variables
char opcion = '0';
char opc = '0';
String total;

int horas = 0;     
int minutos = 0;     
int segundos = 0;    
int decimas = 0;           
int milisegundos = 0;
int cuenta;

int a = 0;
int b = 0;

const int pinBuzzer = 9;
int f=0;

void setup(){
  Serial.begin(9600);

  // Inicializamos la pantalla LCD
  lcd.init();
  lcd.backlight();

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
}

void loop(){

    switch(estado)
   {
    case principal:
    {
      Date_Time();
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
               estado = principal;
        }
      }
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

void alarma(void)
{
  String c, str;
  int n = 1;
  int anio = 1;
  int mes = 2;
  int dia = 3;
  int h = 4;
  int m = 5;
  int s = 6;
  int i, j;


  fyhalarma*alarma;


  if ( Serial.available() > 0) // Si hay datos disponibles en el puerto serie.
  {


    alarma = (fyhalarma*)malloc(sizeof(fyhalarma) * n);
    for (i = 0; i < n; i++)
    {
      str = Serial.readStringUntil('\n');
      alarma[i].login = str;
      Serial.println(str);// Envía la respuesta a través del puerto serie.
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarma[i].anio = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarma[i].mes = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarma[i].dia = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarma[i].h = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarma[i].m = str.toInt();
      Serial.println(str);
      str = Serial.readStringUntil('\n'); // Lee una cadena hasta \n
      alarma[i].s = str.toInt();
      Serial.println(str);
      n++;
      alarma = (fyhalarma*)realloc(alarma, sizeof(fyhalarma) * n);
      
    }
  }
  
  for(i=0;i<n;i++)
  {
  if (alarma[i].anio == anio && alarma[i].mes == mes && alarma[i].dia == dia && alarma[i].h == h && alarma[i].m == m && alarma[i].s == s)
  {
    //generar tono de 440Hz durante 1000 ms
    tone(pinBuzzer, 440);
    delay(1000);
    //detener tono durante 500ms
    noTone(pinBuzzer);
    delay(500);
    //generar tono de 523Hz durante 500ms, y detenerlo durante 500ms.
    tone(pinBuzzer, 523, 300);
    delay(500);

  }

 }
}
