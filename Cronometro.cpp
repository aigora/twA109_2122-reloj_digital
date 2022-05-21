#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include "SerialClass/SerialClass.h"

#define MAX_BUFFER 200
#define PAUSA_MS 200
#define MAX 100

// Funciones prototipo
int menu(void);
int menu_ppal2(void);
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

int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM4"; // Puerto serie al que está conectado Arduino
	int opcion_menu;

	setlocale(LC_ALL, "es-ES");
	Arduino = new Serial((char*)puerto);

	if (Arduino->IsConnected()) // Si está conectado Arduino
	{
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
	}
	else
	{
		do
		{
			opcion_menu = menu_ppal2();

			switch (opcion_menu)
			{
			case 1: 
				consultar_cronos();
				break;
			case 2: 
				break;
			}

		} while (opcion_menu != 2);
	}

	return 0;
}

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

int menu_ppal2(void)
{
	int opc;

	do
	{
		printf("=================================\n");
		printf("Menú Crono\n");
		printf("================================\n");
		printf("1. Mostrar histórico de las medidas cronometradas\n");
		printf("2. Salir\n");
		printf("Elija una opción: ");
		scanf_s("%d", &opc);

		if (opc == 0 || opc > 5)
			printf("Opción incorrecta");

	} while (opc == 0 || opc > 5);

	return opc;
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