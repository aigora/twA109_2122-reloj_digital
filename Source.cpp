
#include "SerialClass.h" // Biblioteca para acceder al puerto serie
#include <stdio.h>
#include <stdlib.h>

int main()
{
	Serial* Arduino;
	char puerto[] = "COM5"; // Puerto serie en el que est¨¢ conectado Arduino
	char BufferEntrada[200];
	int bytesRecibidos;

	Arduino = new Serial((char*)puerto); // Crea conexi¨®n l¨®gica con Arduino
	if (Arduino->IsConnected())
		printf("Arduino conectado\n");
	else
		printf("Error\n");
	while (Arduino->IsConnected())
	{

		bytesRecibidos = Arduino->ReadData(BufferEntrada, sizeof(char) * 199);
		if (bytesRecibidos != -1) // Lectura de mensaje desde el puerto
		{
			BufferEntrada[bytesRecibidos - 1] = '\0';
			printf(" %s\n", BufferEntrada);
		}
		else
			Sleep(1000);
	}
}