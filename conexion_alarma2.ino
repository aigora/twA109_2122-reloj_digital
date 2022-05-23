struct fyhalarma
{

  String login;
  int anio, mes, dia, h, m, s;
};
const int pinBuzzer = 9;
int f = 0;

void setup ()
{
  Serial.begin(9600); // Configura velocidad puerto serie 9600 bits/sg.
  while (!Serial) {
    ;  // Mientras no tenga conexión se queda en bucle.
  }

}
void loop ()
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
