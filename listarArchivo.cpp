#include <iostream>
#include <conio.h>
#include <stdio.h>

using namespace std;

//Definición de registro de archivo
typedef struct TDatoArchivo
{
	int codObra;
	char nomCapataz[20];
	int importe;
	int cantCompras;
}TDatoArchivo;



main()
{
TDatoArchivo obra;
FILE * arch = fopen("Obras.dat","rb");
if(arch != NULL)
{
	fread(&obra,sizeof(obra),1,arch);
	while(!feof(arch))
	{
        if(obra.codObra!=0)
        {
           cout<<"Codigo de obra: "<<obra.codObra;
		   cout<<"  Nombre del capataz: "<<obra.nomCapataz;
		   cout<<"  Importe Total: "<<obra.importe;
		   cout<<"  Cantidad total de compras: "<<obra.cantCompras<<endl;
		   cout<<endl;
         }
		fread(&obra,sizeof(obra),1,arch);
	}
	fclose(arch);
}
getch();
}
