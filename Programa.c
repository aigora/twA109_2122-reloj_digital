#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Funciones prototipo
int menu_ppal1(void);
int menu_ppal2(void);

int main(void)
{
	int opcion;

	if () // Si está conectado Arduino
	{
		opcion = menu_ppal1();

		switch (opcion)
		{
		case 1: break;
		case 2: break;
		case 3: break;
		case 4: break;
		}
	}
	else // Si no está conectado Arduino
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
	}
}

// Menú principal si el Arduino está conectado
int menu_ppal1(void)
{
	int opc;

	do
	{
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

