#define _CRIT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <locale.h>
#include "SerialClass/SerialClass.h"
#define MAX_BUFFER 200
#define PAUSA_MS 200
#define LONGCAD 50
#define SI 1
#define NO 0

typedef struct
{

	char login[LONGCAD], anio[LONGCAD], mes[LONGCAD], dia[LONGCAD], h[LONGCAD], m[LONGCAD], s[LONGCAD];
}alarma;

struct item
{
	alarma alarma;
	struct item* enlace;
};
typedef struct item Nodo;

alarma leer(void);
int nueva_alarma(Nodo** plista);
void mostrar_datos_usuario(alarma a);
void listado(Nodo* lista);
int eliminar(Nodo** lista);
int menu_ppal2(void);
void verifica_sensores(Serial* Arduino, Nodo* lista);
void leer_sensor_distancia(Serial* Arduino, alarma a);
void leer_cont(Serial* Arduino, char c[LONGCAD]);
int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir);

alarma leer(void)
{
	getchar();
	alarma alarma;
	char intro;
	printf("Introduzca nueva alarma\n");
	printf("========================\n");
	printf("nombre:");
	gets_s(alarma.login, LONGCAD);
	printf("año:");
	gets_s(alarma.anio, LONGCAD);
	printf("mes:");
	gets_s(alarma.mes, LONGCAD);
	printf("dia:");
	gets_s(alarma.dia, LONGCAD);
	printf("hora:");
	gets_s(alarma.h, LONGCAD);
	printf("minutos:");
	gets_s(alarma.m, LONGCAD);
	printf("segundos:");
	gets_s(alarma.s, LONGCAD);
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
		cod_error = -1;
	}
	else
	{
		p->alarma = leer();
		p->enlace = cab;
		cab = p;
	}
	*plista = cab;
	return cod_error;
}
void mostrar_datos_usuario(alarma a)
{
	printf("%s\t", a.login);
	printf("%s/%s/%s\t", a.dia, a.mes, a.anio);
	printf("%s:%s:%s\n", a.h, a.m, a.s);

}

void listado(Nodo* lista)
{
	Nodo* p;
	if (lista == NULL)
		printf("En este momento no hay alarmas en la aplicaci¨®n\n");
	else
	{
		printf("nombre\tfecha\t\thora\n");
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
		for (p = q;p != NULL && encontrado == NO;p = p->enlace)
		{
			if (strcmp(login, p->alarma.login) == 0)
			{
				printf("Datos del alarma encontrado:\n");
				mostrar_datos_usuario(p->alarma);
				printf("Desea borra alarma usuario (S/N):");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					if (p == q) // Si se trata del primero
						*lista = p->enlace;
					else
						q->enlace = p->enlace;
					free(p);
					break; // Una de las pocas excepciones del uso razonable.
				}
			}
			else
				if (p != q) // No se avanza el puntero q en la primera iteraci¨®n
					q = q->enlace;
		}
		if (encontrado == NO)
			printf("No se ha encontrado el usuario\n");
	}
	getchar();
	return encontrado;
}




int menu_ppal2(void)
{
	int opc;

	do
	{
		printf("Men¨² principal\n");
		printf("================================\n");
		printf("1. Consultar alarmas programadas\n");
		printf("2. Crear nueva alarma\n");
		printf("3. Eliminar alarma programada\n");
		printf("4. trasladar\n");
		printf("5. Salir\n");
		printf("Elija una opci¨®n: ");
		scanf_s("%d", &opc);

		if (opc == 0 || opc > 5)
			printf("Opci¨®n incorrecta");

	} while (opc == 0 || opc > 5);

	return opc;
}










int main(void)
{

	int opcion;
	int c = 0;
	char cad[LONGCAD];
	Nodo* lista = NULL;
	Serial* Arduino;
	char puerto[] = "COM5";
	setlocale(LC_ALL, "es-ES");
	Arduino = new Serial((char*)puerto);
	do {
		opcion = menu_ppal2();
		switch (opcion)
		{
		case 1:
			listado(lista);
			break;
		case 2:
			nueva_alarma(&lista);
			c++;
			break;
		case 3:
			eliminar(&lista);
			c--;
			break;

		case 4:
			sprintf_s(cad, "%d", c);
			leer_cont(Arduino, cad);
			verifica_sensores(Arduino, lista);
			break;

		}
	} while (opcion != 5);
}

void verifica_sensores(Serial* Arduino,Nodo*lista)
{
	Nodo* p;
	if (Arduino->IsConnected())
	{
		
		if (lista == NULL)
			printf("En este momento no hay alarmas en la aplicaci¨®n\n");
		else
		{
			
			for (p = lista;p != NULL;p = (Nodo*)p->enlace)
			{
				leer_sensor_distancia(Arduino, p->alarma);


			}
			printf("\n");
		}
	}
	else
	{
		printf("Arduino no conectado");
	}

}

void leer_cont(Serial* Arduino, char c[LONGCAD])
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];
	bytesRecibidos = Enviar_y_Recibir(Arduino, c, mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		printf("\nNo se ha recibido respuesta a la petición\n");
		
	}
	else
	{
		printf("\nLa respuesta recibida tiene %d bytes. Recibido=%s\n", bytesRecibidos,
			mensaje_recibido);

	}

}

void leer_sensor_distancia(Serial* Arduino,alarma a)
{
	
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, a.login, mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		printf("\nNo se ha recibido respuesta a la petición\n");
		
	}
	else
	{
		printf("\nLa respuesta recibida tiene %d bytes. Recibido=%s\n", bytesRecibidos,
			mensaje_recibido);

	}

	bytesRecibidos = Enviar_y_Recibir(Arduino, a.anio, mensaje_recibido);
	bytesRecibidos = Enviar_y_Recibir(Arduino, a.mes, mensaje_recibido);
	bytesRecibidos = Enviar_y_Recibir(Arduino, a.dia, mensaje_recibido);
	bytesRecibidos = Enviar_y_Recibir(Arduino, a.h, mensaje_recibido);
	bytesRecibidos = Enviar_y_Recibir(Arduino, a.m, mensaje_recibido);
	bytesRecibidos = Enviar_y_Recibir(Arduino, a.s, mensaje_recibido);
	if (bytesRecibidos <= 0)
	{
		printf("\nNo se ha recibido respuesta a la petición\n");
		
	}
	else
	{
		printf("\nLa respuesta recibida tiene %d bytes. Recibido=%s\n", bytesRecibidos,
			mensaje_recibido);
		
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