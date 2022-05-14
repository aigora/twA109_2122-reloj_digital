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

void imprimir(String cadena);

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
    lcd.setCursor(1,0);
    imprimir("RTC NOT FOUND!");
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
  
  if(Serial.available()> 0)
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
  else
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
}

void imprimir(String cadena)
{
  int i, n;
  
  n=cadena.length();

  for(i=0;i<n;i++)
  {
    lcd.write(cadena.charAt(i));
  }
}
