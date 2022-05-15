#include "SerialPort.h" // Biblioteca para acceder al puerto serie
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define TAM 30

// Funciones prototipo
int menu_ppal1(void);
int menu_ppal2(void);
char* Date_Time(void);
char* formato_time(const struct tm*);

int main(void)
{
	int opcion;
	Serial* Arduino;
	char puerto[] = "COM6"; // Puerto serie en el que esté conectado Arduino
	char BufferEntrada[200];
	int bytesRecibidos;
	char fechayhora[TAM];
	char* fyh;
	int i, n;
	
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
		        case 3: break;
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
		        case 4: break;
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
