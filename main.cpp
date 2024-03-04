#include <iostream>
#include <string.h>
#include <conio.h>
#include <stdio.h>

using namespace std;

//Definición de registro de archivo
typedef struct TDatoArchivo
{
	int codObra;
	char nomCapataz[20];
	int importe;
	int cantVentas;
}TDatoArchivo;

//Definición lista de compras
typedef struct TDatoLista
{
	int nroFactura;
   	int importe;
}TDatoLista;

typedef struct NodoLista * PuntLista;

typedef struct NodoLista
{
	TDatoLista info;
	PuntLista sig;
}NodoLista;

//Definición multilista de obras
typedef struct TDatoML
{
	int codObra;
	char nomCapataz[20];
   	PuntLista priCompras;
}TDatoML;

typedef struct NodoML * PuntML;

typedef struct NodoML
{
	TDatoML info;
	PuntML sigNom, sigCod;
}NodoML;



//Prototipos de Lista
void agregar(PuntLista &, TDatoLista);
void mostrarCompras(PuntLista);
int sum (PuntLista);
int cont (PuntLista);

//Portotipo de multilistas
void buscarAgregarCodigo(PuntML &, PuntML &, int, char[], PuntML &);
void agregarNom(PuntML &, PuntML);

//Prototipo funcion hashing
int hashing(int);

//prototipo opciones de menu
void opcion1(PuntML &, PuntML &);
void opcion2(PuntML); 
void opcion3(PuntML); 
void opcion4(PuntML); 

//prototipo función uso de archivo
void actualizarArchivo(PuntML);


main()
{
  PuntML entCod=NULL, entNom=NULL;
  int opcion;
  do
  {
	cout<<"1 - Agregar una compra"<<endl;
	cout<<"2 - Mostrar las compras realizadas hasta el momento para cada obra, ordenadas por nombre del capataz"<<endl;
	cout<<"3 - Mostrar las compras realizadas hasta el momento para cada obra, ordenadas por codigo de obra"<<endl;
	cout<<"4 - Salir (Obligatorio para actualizacion de Archivo)"<<endl;
    cout<<"Ingrese opcion: ";
    cin>>opcion;
    switch(opcion)
    {
     case 1:
          opcion1(entCod,entNom);
          break;
     case 2:
            if(entNom!=NULL)
                opcion2(entNom);
                else
                {
                    cout<< "No hubo compras ingresadas en el dia" << endl;
                    cout<< endl;
                }
          break;
     case 3:
            if (entCod!=NULL)
                opcion3(entCod);
                else
                {
                    cout<< "No hubo compras ingresadas en el dia" << endl;
                    cout << endl;
                }
          break;
     case 4:
            opcion4(entCod);
            
          break;
     }
 }
  while(opcion != 4);
  
  getch();
  return(0);
}

//Funciones de lista simple
void agregar(PuntLista & pri, TDatoLista dato)
{
   PuntLista r, ant, n;
   r = pri;
   n = new NodoLista;
   n->info = dato;
   while (r!= NULL && r->info.nroFactura < dato.nroFactura)
   {
    ant = r;
    r = r->sig;
   }
   n->sig = r;
   if(r == pri)
   	pri = n;
   else
   	ant->sig = n;
}

void mostrarCompras(PuntLista pri)
{
    PuntLista r=pri;
    while(r!=NULL)
    {
        cout<<"Nro. factura "<<r->info.nroFactura<<" Importe: "<<r->info.importe<<endl;
        r=r->sig;
    }
}

//funcion hashing
int hashing(int clave)
{
   return clave % 10;
}

void opcion1(PuntML & entCod, PuntML & entNom)
{
    TDatoLista datoLista;
    PuntML p=NULL;
    
    int cod;
    char nom[20];
    
    cout<<"Ingrese numero de factura"<<endl;
	cin>>datoLista.nroFactura;
	cout<<"Ingrese codigo de obra"<<endl;
	cin>>cod;
    cout<<"Ingrese nombre del Capataz"<<endl;
	cin>>nom;
    cout<<"Ingrese importe de la Compra"<<endl;
	cin>>datoLista.importe;
	buscarAgregarCodigo(entCod,entNom,cod,nom,p);
	agregar(p->info.priCompras, datoLista);
}

void opcion2(PuntML entNom)
{
    PuntML r=entNom;
        
        while (r!=NULL)
        {
            mostrarCompras(r->info.priCompras);
            r=r->sigNom;
        }
}

void opcion3(PuntML entCod)
{
    PuntML r=entCod;
    
    while (r!=NULL)
    {
        mostrarCompras(r->info.priCompras);
        r=r->sigCod;
    }
    
}
void opcion4(PuntML entCod)
{
    if (entCod!=NULL)
        actualizarArchivo(entCod);
    cout<< endl;
    cout<<"Fin de ejecucion" << endl;
}

void actualizarArchivo(PuntML entCod)
{
    PuntML r=entCod;
    FILE*fd;
    
    fd=fopen("Obras.dat","rb+");
    if (fd!=NULL)
    {
        TDatoArchivo obArch;
        int pos;
        
        while (r!=NULL)
        {
            pos=hashing(r->info.codObra);
            
            fseek(fd,pos*sizeof(obArch),SEEK_SET);
            fread(&obArch, sizeof(obArch),1,fd);
            
            if (obArch.codObra==r->info.codObra) //está en el archivo. Actualizo importe y cantidad de ventas.
            {
                obArch.importe = obArch.importe+sum(r->info.priCompras);;
                obArch.cantVentas = obArch.cantVentas+cont(r->info.priCompras);;
                fseek (fd,-sizeof(obArch),SEEK_CUR);
                fwrite (&obArch,sizeof(obArch),1,fd);
            }
            else //busco en el área de rebalse
            {
                fseek(fd, 10*sizeof(obArch),SEEK_SET);
                int nb;
                nb=fread(&obArch,sizeof(obArch),1,fd);
                while ((nb>0) && (obArch.codObra!=r->info.codObra))
                    nb=fread(&obArch,sizeof(obArch),1,fd);

                if (obArch.codObra!=r->info.codObra) 
                    cout<< "ERROR: no existe obra con codigo " << r->info.codObra << endl;
                    else //está. Actualizo
                    {
                        obArch.importe = obArch.importe+sum(r->info.priCompras);
                        obArch.cantVentas = obArch.cantVentas+cont(r->info.priCompras);
                        fseek (fd,-sizeof(obArch),SEEK_CUR);
                        fwrite (&obArch,sizeof(obArch),1,fd);
                    }
            }
             
            r=r->sigCod;
        }
    }   
    
    fclose(fd);
}

void buscarAgregarCodigo(PuntML & entCod, PuntML & entNom, int cod, char nom[], PuntML & p)
{
     //Busca la obra en la lista ordenada por código
     //si la encuentra devuelve el puntero al nodo correspondiente
     //sino lo agrega a la lista ordenada por código y a la ordenada por nombre
     //y devuelve el puntero al nodo agregado
     PuntML ant, r = entCod;
     while(r!=NULL && r->info.codObra < cod)
     {
         ant=r;
         r=r->sigCod;
     }
     if(r!=NULL && r->info.codObra==cod)
         p=r;
     else
     {
         p=new NodoML;
         p->info.codObra = cod;
         strcpy(p->info.nomCapataz,nom);
         p->info.priCompras = NULL;
         p->sigCod = r;
         if(r == entCod)
            entCod = p;
         else
   	        ant->sigCod = p;

         agregarNom(entNom,p);
     }

}

void agregarNom(PuntML & entNom, PuntML p)
{
     PuntML ant,r = entNom;
     while(r!=NULL && strcmp(r->info.nomCapataz,p->info.nomCapataz)<0)
     {
        ant=r;
        r=r->sigNom;
     }
     p->sigNom = r;
     if(r == entNom)
        entNom = p;
     else
        ant->sigNom = p;
}

int sum (PuntLista lista)
{
    PuntLista r=lista;
    int sum=0;
    
    while (r!=NULL)
    {
        sum=sum+r->info.importe;
        r=r->sig;
    }
    
    return(sum);
}

int cont (PuntLista lista)
{
    PuntLista r=lista;
    int cont=0;
    while (r!=NULL)
    {
        cont=cont+1;
        r=r->sig;
    }
    return(cont);
}
