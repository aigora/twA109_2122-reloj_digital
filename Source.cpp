#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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

alarma leer(void)
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
		printf("En este momento no hay alarmas en la aplicaci車n\n");
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
		printf("La lista est芍 vac赤a\n");
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
				if (p != q) // No se avanza el puntero q en la primera iteraci車n
					q = q-> enlace;
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
		printf("Men迆 principal\n");
		printf("================================\n");
		printf("1. Consultar alarmas programadas\n");
		printf("2. Crear nueva alarma\n");
		printf("3. Eliminar alarma programada\n");
		printf("4. Mostrar hist車rico de las medidas cronometradas\n");
		printf("5. Salir\n");
		printf("Elija una opci車n: ");
		scanf_s("%d", &opc);

		if (opc == 0 || opc > 5)
			printf("Opci車n incorrecta");

	} while (opc == 0 || opc > 5);

	return opc;
}


int main(void)
{
	int opcion;
	Nodo* lista = NULL;

	do {
		opcion = menu_ppal2();
		switch (opcion)
		{
		case 1:
			listado(lista);
			break;
		case 2: 
			nueva_alarma(&lista);
			break;
		case 3: 
			eliminar(&lista);
			break;

		case 4: break;
		
		}
	} while (opcion != 5);
}
			