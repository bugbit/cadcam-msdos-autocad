#include <dos.h>
#include <ctype.h>
#include <strstream.h>
#include <bios.h>
#include <time.h>
#include <PPath.hpp>
#include "..\source\hilo.hpp"

int Contorno::NDec;
PBOOL Contorno::VIJ;

Grafico Silueta::Pantalla;

Figuras::Figuras() {}

Figuras::~Figuras() {}

void Figuras::fg_dibujo(Object &o,void *) { ((RFigura) o).Dibujar(); }

void Figuras::Dibujo() const { forEach(Figuras::fg_dibujo,NULL); }

Contorno::~Contorno() { Cierra(); }

void Contorno::PreIJ() {

  int c;

  do {
	 cout << "¨ Coordenadas I,J (S/N) ? ";
	 cin >> (char) c;
	 } while (((c=toupper(c))!='S') && c!='N');
  VIJ=c=='S';

  }

void Contorno::PreCM(void) {

  cout << endl;
  do {
	 cout << "¨ Cuantos decimales quiere (1-3) ? ";
	 cin >> NDec;
	 } while (NDec<1 || NDec>3);

  }

void Contorno::Abrir()  {

  PPASTR path,direc,direc2;
  PPath d(_argv[1]);

  ostrstream(direc2,sizeof direc2) << PPaNmbExe.Obt(direc,PPADIRECTORY)
  << "ALMACEN\\.NC" << ends;
  arch=new ofstream(d.Mezcla(path,direc2));
  *arch << "G90" << endl;

  }

void Contorno::Abrir(PFigura f1,PFigura f2) {

  if (!arch) Abrir();
  *arch << *Pini << *f1;
  if (f1!=f2) f1->Dibujar();
  Atras=(PLinea) f2;
  Flag.Proximo=PTRUE;

  }

void Contorno::Cerrando() {

  if (Fg.ln!=Atras && (*(Fg.ln)>>=Atras->Pf))  {
	 Atras=Fg.ln;
	 *arch << *(Fg.fg);
	 Flag.Principio=PTRUE;
	 Flag.Proximo=Fg.ln->Pf==Pini->Pi;
	 }

  }

void Contorno::Interseccion() {

  Punto I;
  PBOOL corta;

  if (Fg.fg!=A1 && (corta=Fg.fg->Interseccion(*A1,I)) && (*A1)(I))  {
	 Arco a=*A1;
	 Linea *L2=(L1[0]->Pf == A1->Pi) ? L1[0] : L1[1];
	 real ang,d1,d2;
	 if (Fg.fg->EsLinea())  {
		ang=A1->Angulo2(I);
		d1=A1->Angulo2(Fg.ln->Pf,ang);
		d2=A1->Angulo2(Fg.fg->Pi,ang);
		corta=d2<d1;
		}
		  else
		corta--;
	 A1->Pi=a.Pf=I;
	 A1->Angi=a.Angf=A1->Angulo(I);
	 Figura *f=(Fg.fg->EsLinea()) ? new Linea(Fg.fg) : new Arco(Fg.fg);
	 if (corta)  {
		f->Corta();
		Fg.fg->Corta();
		}
	 if (Fg.fg->EsArco())  {
		ang=Fg.ac->Angulo(I);
		if (corta)
		  Fg.ac->Angi=((PArco) f)->Angf=ang;
			 else
		  Fg.ac->Angf=((PArco) f)->Angi=ang;
		}
	 Fg.ln->Pf=f->Pi=I;
	 Abrir(L2,f);
	 *arch << (Figura &) a << *f;
	 a.Dibujar();
	 }

  }

void Contorno::Sigue() {

  if (Fg.fg->EsArco() && (Flag.Proximo=*L1[0] == (*(Fg.ln)))) A1=Fg.ac;

  }

void Contorno::Inicio() {

  if (Fg.fg->EsLinea() && Fg.ln->operator >>= (Pini->Pi) && (Flag.Li!=1 ||
  L1[0]!=Fg.ln))  {
	 L1[Flag.Li++]=Fg.ln;
	 if (Flag.Li==2) Flag.Proximo=PTRUE;
	 }
  if (Flag.Fin && Flag.Li==1) {
	 Flag.Proximo=PTRUE;
	 Flag.C+=2;
	 Abrir(L1[0],L1[0]);
	 }

  }

void Contorno::Interprete(const Object &obj) {

  ContainerIterator &cnt=initIterator();

  if (cnt!=0) {
	 clock_t clk=0;
	 Atras=NULL;
	 Pini=&((RFigura) obj);
	 Pini->Dibujar();
	 Flag.Proximo=PTRUE;
	 Flag.Li=Flag.Fin=PFALSE;
	 Flag.C=C_NADA;
	 while (!Flag.Fin)  {
		Fg.ob=&cnt++;
		Flag.Fin=!(int) cnt;
		if (Fg.ob!=&NOOBJECT) {
		  switch (Flag.C)  {
			 case C_INICIO:Inicio(); break;
			 case C_SIGUE:Sigue(); break;
			 case C_INTERSECCION:Interseccion(); break;
			 case C_CERRANDO:Cerrando(); break;
			 default:Flag.Fin=PTRUE;
			 }
		  if (Flag.Proximo) {
			 ++Flag.C;
			 Flag.Proximo=PFALSE;
			 Flag.Principio=PTRUE;
			 }
		  if (Flag.Principio) {
			 if (Atras) {
				if (clk) {
				  long ms=1500-((clock()-clk)/CLK_TCK);
				  if (ms>0) delay((PWORD) ms);
				  }
				Atras->Dibujar();
				clk=clock();
				}
			 cnt.restart();
			 Flag.Fin=Flag.Principio=PFALSE;
			 }
		  }
		}
	 if (arch) *arch << "M02" << endl;
	 }
  delete &cnt;

  }

void Contorno::Cierra() {

  if (arch) {
	 *arch << '%' << endl;
	 arch->close();
	 delete arch;
	 arch=NULL;
	 }

  }

Silueta::Silueta() {}

Silueta::~Silueta() {}

void Silueta::pausa() const { bioskey(0); }

void Silueta::Dibujo() const {

  Pantalla.Ventana(V_DIBUJO);
  Pantalla.Mensaje("Dibujando figura ...");
  Figuras::Dibujo();
  Sup.Dibujo();

  }

void Silueta::Proceso() {

  ContainerIterator &cnt=initIterator();

  if (cnt!=0)  {
	 Sup.PreCM();
	 Sup.PreIJ();
	 if (!Pantalla.Inicio())  {
		Pantalla.Ventana(V_CAMINO);
		Pantalla.Mensaje("Procesando Camino ...");
		while (cnt!=0) Sup.Interprete(cnt++);
		Sup.Cierra();
      Dibujo();
		Pantalla.Mensaje("Proceso concluido. Pulse una tecla para continuar");
		pausa();
		Pantalla.Cierra();
		}
	 }
		else
	 cerr << ", No hay ninguna figura.\n\a";
  delete &cnt;

  }