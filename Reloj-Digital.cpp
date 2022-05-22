#include "SerialClass/SerialClass.h" // Biblioteca para acceder al puerto serie
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#define TAM 30
#define MAX_BUFFER 200
#define PAUSA_MS 200
#define MAX 100
#define LONGCAD 50
#define SI 1
#define NO 0

typedef struct 
{
	int h;
	int m;
	int s;
	char login[LONGCAD];
}alarma;

struct item
{
	alarma alarma;
	struct item* enlace;
};
typedef struct item Nodo;

// Funciones prototipo
int menu_ppal1(void);
int menu_ppal2(void);
int menu(void);
char* Date_Time(void);
char* formato_time(const struct tm*);
void Inicio(Serial* Arduino);
void pausa(Serial* Arduino);
void reset(Serial* Arduino);
void comprobar_mensajes(Serial*);
int Enviar_y_Recibir(Serial*, const char*, char*);
int cadena(char* cadena);
void hora(int tiempo, int v[], int n);
void guardar_en_disco(int v[], int n);
int leer_desde_disco(int v[]);
void consultar_cronos(void);
void alarma leer(void);
int nueva_alarma(Nodo** plista);
void mostrar_datos_usuario(alarma a);
void listado(Nodo* lista);
int eliminar(Nodo** lista);

int main(void)
{
	int opcion;
	int opcion_menu;
	Serial* Arduino;
	char puerto[] = "COM6"; // Puerto serie en el que esté conectado Arduino
	Nodo* lista = NULL;
	char fechayhora[TAM];
	char* fyh;
	
	setlocale(LC_ALL, "es-ES");

	Arduino = new Serial((char*)puerto); // Crea conexión lógica con Arduino

	if (Arduino->IsConnected()) // Si está conectado Arduino
	{
		do
		{
			opcion = menu_ppal1();

		        switch (opcion)
		        {
		        case 1: 
				if (Arduino->IsConnected())
			        {
				    fyh = Date_Time();

				    strcpy(fechayhora, fyh);

				    Arduino->WriteData(fechayhora, strlen(fechayhora));
			        }
				break;
		        case 2: break;
		        case 3:
				do
		                {
			             comprobar_mensajes(Arduino);
			             opcion_menu = menu();
					
			             switch (opcion_menu)
			             {
			              case 0: break;
			              case 1:
				              Inicio(Arduino);
				              break;
			              case 2:
				              pausa(Arduino);
				              break;
			              case 3:
				              reset(Arduino);
				              break;
			              case 4:
				              break;
			              default: printf("\nOpción incorrecta\n");
			              }
		                   } while (opcion_menu != 4);
				   break;
		        case 4: break;
		        }
			
		}while (opcion != 4);
		
	}
	else // Si no está conectado Arduino
	{
		do
		{
			opcion = menu_ppal2();

		        switch (opcion)
		        {
		        case 1: break;
		        case 2: break;
		        case 3: break;
		        case 4:
				consultar_cronos();
				break;
		        case 5: break;
		        }
			
		}while (opcion != 5);
	}
}

// Menú principal si el Arduino está conectado
int menu_ppal1(void)
{
	int opc;

	do
	{
		printf("=================================\n");
		printf("Menú principal\n");
		printf("=================================\n");
		printf("1. Ajustar fecha y hora del reloj\n");
		printf("2. Trasladar alarmas\n");
		printf("3. Obtener medidas cronometradas\n");
		printf("4. Salir\n");
		printf("Elija una opción: ");
		scanf_s("%d", &opc);

		if (opc == 0 || opc > 4)
			printf("Opción incorrecta");

	} while (opc == 0 || opc > 4);

	return opc;
}

// Menú principal si el Arduino no está conectado
int menu_ppal2(void)
{
	int opc;

	do
	{
		printf("=================================\n");
		printf("Menú principal\n");
		printf("================================\n");
		printf("1. Consultar alarmas programadas\n");
		printf("2. Crear nueva alarma\n");
		printf("3. Eliminar alarma programada\n");
		printf("4. Mostrar histórico de las medidas cronometradas\n");
		printf("5. Salir\n");
		printf("Elija una opción: ");
		scanf_s("%d", &opc);

		if (opc == 0 || opc > 5)
			printf("Opción incorrecta");

	} while (opc == 0 || opc > 5);

	return opc;
}

// Menú para el cronómetro
int menu(void)
{
	static int opcion = -1;

	if (opcion != 0)
	{
		printf("\n");
		printf("Menú Crono\n");
		printf("==============\n");
		printf("1 - Iniciar.\n");
		printf("2 - Pausar\n");
		printf("3 - Reset/Guardar marca\n");
		printf("4 - Salir de la aplicación\n");
		printf("Opción:");
	}
	if (_kbhit())
	{
		opcion = (int)_getch() - '0';
		printf("%d\n", opcion);
	}
	else
		opcion = 0;
	return opcion;
}

// Función para obtener la fecha y hora del sistema
char* Date_Time()
{
	
	time_t t;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);

	return formato_time(tm);
}

// Función para hacer el formato con el que se va a imprimir la fecha y hora
char* formato_time(const struct tm* timeptr)
{
	//static char result[TAM];
	char* result = NULL;
	result = (char*)malloc(sizeof(char) * TAM);
	if (result == NULL)
		printf("AVISO: Memoria insuficiente\n");
	else
	{
		sprintf(result, "%.2d/%.2d/%d  %.2d:%.2d:%.2d\n", timeptr->tm_mday, 1 + timeptr->tm_mon, 1900 + timeptr->tm_year, timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec);
	}
	  return result;
}

void comprobar_mensajes(Serial* Arduino)
{
	int bytesRecibidos, total = 0;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Arduino->ReadData(mensaje_recibido, sizeof(char) * MAX_BUFFER - 1);
	while (bytesRecibidos > 0)
	{
		Sleep(PAUSA_MS);
		total += bytesRecibidos;
		bytesRecibidos = Arduino->ReadData(mensaje_recibido + total, sizeof(char) * MAX_BUFFER - 1);
	}
	if (total > 0)
	{
		mensaje_recibido[total - 1] = '\0';
		printf("\nMensaje recibido: %s\n", mensaje_recibido);
	}
}

void Inicio(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "1", mensaje_recibido);
}

void pausa(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "2", mensaje_recibido);
}

void reset(Serial* Arduino)
{
	int tiempo = 0;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];
	int numeros[MAX];
	int total = 0;

	bytesRecibidos = Enviar_y_Recibir(Arduino, "3", mensaje_recibido);
	tiempo = cadena(mensaje_recibido);
	if (tiempo != 0)
	{
		total = leer_desde_disco(numeros);
		hora(tiempo,numeros,total);	
	}
}

int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
	int bytes_recibidos = 0, total = 0;
	int intentos = 0, fin_linea = 0;


	Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
	Sleep(PAUSA_MS);

	bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);

	while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)
	{
		if (bytes_recibidos > 0)
		{
			total += bytes_recibidos;
			if (mensaje_recibir[total - 1] == 13 || mensaje_recibir[total - 1] == 10)
				fin_linea = 1;
		}
		else
			intentos++;
		Sleep(PAUSA_MS);
		bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
	}
	if (total > 0)
		mensaje_recibir[total - 1] = '\0';

	return total;
}

int leer_desde_disco(int v[])
{
	FILE* fichero;
	errno_t e;
	int total = 0, numero;

	e = fopen_s(&fichero, "Tiempos.txt", "rt");
	if (fichero == NULL)
		printf("Error: No se ha podido abrir el fichero\n");
	else
	{
		fscanf_s(fichero, "%d", &numero);
		while (!feof(fichero))
		{
			v[total] = numero;
			total++;
			fscanf_s(fichero, "%d", &numero);
		}
		fclose(fichero);
	}
	return total;
}

void hora(int num, int v[], int n)
{
	int h = 0, m = 0, s = 0, ds = 0;

	ds = num % 10;	
	num = num / 10;
	s = num % 60;
	num = num / 60;
	m = num % 60;
	num = num / 60;
	h = num % 60;
	num = num / 60;

	v[n] = h;
	n++;
	v[n] = m;
	n++;
	v[n] = s;
	n++;
	v[n] = ds;
	n++;

	guardar_en_disco(v, n);

}

void guardar_en_disco(int v[], int n)
{
	FILE* fichero;
	errno_t e;
	int i;

	e = fopen_s(&fichero, "Tiempos.txt", "wt");
	if (fichero == NULL)
		printf("Error: No se ha podido guardar el fichero\n");
	else
	{
		for (i = 0; i < n; i++)
			fprintf(fichero, "%d ", v[i]);
		fclose(fichero);
	}
}

int cadena(char* cadena)
{
	int numero = 0;
	int i, divisor = 10, estado = 0;


	for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
		switch (estado)
		{
		case 0:// Antes del número
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = cadena[i] - '0';
				estado = 1;
			}
			break;
		case 1:// Durante el número
			if (cadena[i] >= '0' && cadena[i] <= '9')
				numero = numero * 10 + cadena[i] - '0';
			else
				estado = 3;
			break;
		}
	return numero;
}

void consultar_cronos(void)
{
	int numeros[MAX];
	int total = 0,i,a = 0;

	total = leer_desde_disco(numeros);

	printf("================================\n");
	printf("h:m:s:ds\n");

	for (i = 0; i < total; i++)
	{
		if (a >= 0 && a <= 2)
		{
			printf("%d:", numeros[i]);
		}
		if (a == 3)
		{
			printf("%d\n", numeros[i]);
			a = -1;
		}
		a++;
	}
}

void alarma leer(void)
{
	getchar();
	alarma alarma;
	char intro;
	printf("Introduzca nueva alarma\n");
	printf("========================\n");
	printf("nombre:");
	gets_s(alarma.login, LONGCAD);
	printf("hora:");
	scanf_s("%d", &alarma.h);
	printf("minutos:");
	scanf_s("%d", &alarma.m);
	printf("segundos:");
	scanf_s("%d", &alarma.s);
	printf("\n");
	intro = getchar();
	return alarma;
}

int nueva_alarma(Nodo** plista)
{
	Nodo* cab = *plista;
	Nodo* p;
	int cod_error = 0;
	p = (Nodo*)malloc(sizeof(Nodo));
	if (p == NULL)
	{
		printf("No se ha podido reservar memoria para una nueva alarma\n");
		cod_error =-1;
	}
	else
	{
		p -> alarma = leer();
		p -> enlace = cab;
		cab = p;
	}
	*plista = cab;
	return cod_error;
}

void mostrar_datos_usuario(alarma a)
{
	printf("%s\t", a.login);
	printf("%d\t", a.h);
	printf("%d\t", a.m);
	printf("%d\n", a.s);
	
}

void listado(Nodo* lista)
{
	Nodo* p;
	if (lista == NULL)
		printf("En este momento no hay alarmas en la aplicaci¨®n\n");
	else
	{
		printf("login\thora\tmin\tseg\n");
		printf("======\t==============\t=====\n");
		for (p = lista;p != NULL;p = (Nodo*)p->enlace)
		{
			mostrar_datos_usuario(p->alarma);
		}
		printf("\n");
	}
}


int eliminar(Nodo** lista)
{
	getchar();
	char login[LONGCAD], respuesta[2];
	int encontrado = NO;
	Nodo* p, * q;
	if (*lista == NULL)
		printf("La lista est¨¢ vac¨ªa\n");
	else
	{
		printf("Introduzca nombre de alarma a eliminar:");
		gets_s(login, LONGCAD);
		
		q = *lista;
		for (p = q;p != NULL && encontrado == NO;p = p-> enlace)
		{
			if (strcmp(login, p-> alarma.login) == 0)
			{
				printf("Datos del usuario encontrado:\n");
				mostrar_datos_usuario(p-> alarma);
				printf("Desea borra este usuario (S/N):");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					if (p == q) // Si se trata del primero
						*lista = p-> enlace;
					else
						q-> enlace = p-> enlace;
					free(p);
					break; // Una de las pocas excepciones del uso razonable.
				}
			}
			else
				if (p != q) // No se avanza el puntero q en la primera iteraci¨®n
					q = q-> enlace;
		}
		if (encontrado == NO)
			printf("No se ha encontrado el usuario\n");
	}
	getchar();
	return encontrado;
}
