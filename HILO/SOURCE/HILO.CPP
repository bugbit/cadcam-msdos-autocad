#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <StrStream.h>
#include <PPath.hpp>
#ifndef __MEXICO__
  #include <PPCnc.hpp>
  #endif
#include "..\source\hilo.hpp"

static PCPSTR Objs[]={ "CIRCLE\n","LINE\n","ARC\n",NULL },
args[]={ "  0\n"," 10\n"," 11\n"," 12\n"," 20\n"," 21\n"," 40\n"," 50\n",
" 51\n",NULL };

class _POOCLASSTYPE Hilo : private Silueta  {

  enum PROCESO { P_PRIMERO,P_COMANDO,P_ARGUMENTO,P_ERROR };

  enum PROCESO Pro;
  char Orden[91];
  PFigura Fig;
  int Arg;

  int Procesa();
  int operator == (char **l);
  int Numero();
  inline void BorraFig() { delete Fig; Fig=NULL; }
  void Primero();
  void Comando();
  void Argumento();
  void Poner();

  public:

	 Hilo();
	 int Run();

  };

Hilo::Hilo():Silueta() {

  #ifndef __MEXICO__
	 PPCnc Pcc;
		#else
	 CncProteccionMexico Pcc;
	 #endif

  Pcc.Run();
  cout <<
	"\n(C) HILO 1.8, 1995-97. Programa CNC para maquinas de hilo.\n"
	"Hecho por Oscar Hern ndez Ba¤¢.\n\n";

  }

int Hilo::Run()  {

  if (_argc!=2)
	 cerr << "Sintaxis : HILO <fichero>\n\n\a";
		else
	 return Procesa();

  return EXIT_SUCCESS;

  }

int Hilo::operator == (char **l)  {

  for (;*l && strcmp(*l,(PCPSTR) &Orden);l++);

  return *l!=NULL;

  }

int Hilo::Numero()  {

  if (*this==(char **) &args) {
	 int n;
	 istrstream(Orden) >> n;
	 return n;
	 }
		else
	 return -1;

  }

inline void Hilo::Poner() {

  if (Fig)  {
	 Fig->Poner();
	 add(*Fig);
	 Fig=NULL;
	 }

  }

void Hilo::Primero()  {

  int opc=Numero();

  switch (opc)  {
	 case -1: break;
	 case 0:Poner(); ++(int) Pro; break;
	 default:
		if (Fig)  {
		  Arg=opc;
		  Pro=P_ARGUMENTO;
		  }
	 }

  }

void Hilo::Comando()  {

  if (*this==(char **) &Objs)  {
	 switch (*Orden)  {
		case 'A':Fig=new Arco; break;
		case 'C':Fig=new PuntoI; break;
		case 'L':Fig=new Linea;
		}
	 if (!Fig) Pro=P_ERROR;
	 }
		else
	 BorraFig();
  Pro=P_PRIMERO;

  }

void Hilo::Argumento()  {

  Pro=P_PRIMERO;
  real num;
  istrstream(Orden) >> num;
  Fig->Argumento(Arg,num);

  }

int Hilo::Procesa()  {

  PPASTR path;
  PPath d(_argv[1]);
  ifstream fic(d.Mezcla((PPSTR) path,".DXF"));

  if (fic) {
	 cout << "Leyendo " << path << "...";
	 Fig=NULL;
	 Pro=P_PRIMERO;
	 while (!fic.eof())  {
		fic.getline((PPSTR) &Orden,90);
		switch (Pro)  {
		  case P_PRIMERO:Primero(); break;
		  case P_COMANDO:Comando(); break;
		  case P_ARGUMENTO:Argumento(); break;
		  case P_ERROR:cerr << "\nNo hay memoria.\a" << endl; return 0;
		  }
		}
	 Proceso();
	 fic.close();
	 #ifdef __BORRADXF__
		unlink((PPSTR) path);
		#endif
	 }
		else {
	 cerr << "No puedo leer " << path << '.' << endl;
	 return EXIT_FAILURE;
	 }

  return EXIT_SUCCESS;

  }

int main()  {

  Hilo H;

  return H.Run();

  }