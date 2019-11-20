#include <mem.h>
#include <stdio.h>
#include <graphics.h>
#include <PMathEnt.hpp>
#include <PMath.hpp>
#include "..\source\hilo.hpp"

ostream &operator << (ostream &o,CRPunto p) const {

  char str[81];

  sprintf(str," X%.*Lf Y%.*Lf",Contorno::NDec,p.X,Contorno::NDec,p.Y);
  o << str;

  return o;

  }

real Punto::operator <<= (CRPunto p) const {

  return Sqrt(Cuadrado(p.X-X)+Cuadrado(p.Y-Y));

  }

void Punto::Unitario()  {

  *this /= Hypot(X,Y);

  }

Figura::Figura():Pi() {}

Figura::Figura(PFigura f):Pi(f->Pi) {}

void Figura::printOn(ostream &o) const {

  o << "G92" << (CRPunto) Pi << " Z0" << endl;

  }

void Figura::Argumento(int arg,real valor)  {

  switch (arg)  {
	 case 10:Pi.X=valor; break;
	 case 20:Pi.Y=valor;
	 }

  }

RRectangulo Figura::Cuadrante(CRPunto centro,real radio,real ang,RRectangulo
r) const {

  int c=::Cuadrante(ang);
  Punto v1,v2;

  switch (c)  {
	 case 0:v1.Y=v2.X=1; break;
	 case 1:v1.X=-1; v1.Y=1; break;
	 case 2:v1.X=v2.Y=-1; break;
	 default:v2.X=1; v2.Y=-1;
	 }
  r.Top.X=centro.X+radio*v1.X;
  r.Top.Y=centro.Y+radio*v1.Y;
  r.Bottom.X=centro.X+radio*v2.X;
  r.Bottom.Y=centro.Y+radio*v2.Y;

  return r;

  }

classType PuntoI::isA() const { return PINICIAL; }

PuntoI::PuntoI():Radio(0) {}

char *PuntoI::nameOf() const { return "PuntoI"; }

void PuntoI::Poner() {

  Rectangulo r1,r2;

  Silueta::Pantalla(Cuadrante(Pi,Radio,0,r1),Cuadrante(Pi,Radio,2*M_PI,r2));

  }

void PuntoI::Dibujar() const {

  Silueta::Pantalla.Arco((CRPunto) Pi,Radio,0,2*M_PI);

  }

void PuntoI::Argumento(int arg,real valor) {

  switch (arg)  {
	 case 40:Radio=valor; break;
	 default:Figura::Argumento(arg,valor);
	 }

  }

Linea::Linea():Figura(),Pf() {}

Linea::Linea(PFigura f):Figura(f),Pf(((PLinea) f)->Pf) {}

classType Linea::isA() const { return LINEA; }

char *Linea::nameOf() const { return "Linea"; }

void Linea::printOn(ostream &o) const { o << "G01" << (CRPunto) Pf << endl; }

void Linea::Argumento(int arg,real valor) {

  switch (arg)  {
	 case 11:Pf.X=valor; break;
	 case 21:Pf.Y=valor; break;
	 default:Figura::Argumento(arg,valor);
	 }

  }

void Linea::Poner() { Silueta::Pantalla(Pi,Pf); }

void Linea::Dibujar() const {

  Silueta::Pantalla.Linea((CRPunto) Pi,(CRPunto) Pf);

  }

int Linea::Interseccion(CRArco a,RPunto I) const {

  Punto V=Pi;

  V-(CRPunto) Pf;
  I.Y=(V.X*(V.X*Pi.Y+a.Centro.X*V.Y-Pi.X*V.Y)+a.Centro.Y*Cuadrado(V.Y))/
  (Cuadrado(V.X)+Cuadrado(V.Y));
  I.X=(Igual(V.Y)) ? a.Centro.X : (V.X*I.Y-Pi.Y*V.X+Pi.X*V.Y)/V.Y;

  return Igual(I<<=a.Centro,a.Radio) && operator <<=(I);

  }

int Linea::operator >>= (CRPunto p)  {

  int in=(Pf==p);

  if (in) Corta(); else in= Pi==p;

  return in;

  }

int Linea::operator <<= (CRPunto p) const {

  Punto Vf,Vi;

  Vf=Vi=Pi;
  Vf-(CRPunto) Pf;
  Vi-p;

  return Esta(Vi.X,Vf.X) && Esta(Vi.Y,Vf.Y);

  }

Arco::Arco():Linea(),Centro() { Radio=Angi=Angf=Swp=0; }

Arco::Arco(PFigura f):Linea(f),Swp(((PArco) f)->Swp),Centro(((PArco) f)->
Centro),Radio(((PArco) f)->Radio),Angi(((PArco) f)->Angi),Angf(((PArco) f)->
Angf) {}

classType Arco::isA() const { return ARCO; }

char *Arco::nameOf() const { return "Arco"; }

void Arco::printOn(ostream &o) const {

  char str[81];
  real cateto,cateto2;

  if (!Contorno::VIJ)
	 sprintf(str," R%.*Lf",Contorno::NDec,Radio);
		else  {
	 cateto=Centro.X-Pi.X;
	 cateto2=Cuadrado(Radio)-Cuadrado(cateto);
	 if (cateto2<0)
		cateto2=0;
		  else {
		cateto2=Sqrt(cateto2);
		if (Centro.Y<Pi.Y) cateto2 *= -1;
		}
	 sprintf(str," I%.*Lf J%.*Lf",Contorno::NDec,cateto,Contorno::NDec,cateto2);
	 }
  o << (char *) ((Swp) ? "G02" : "G03") << (CRPunto) Pf << (char *)
  str << endl;

  }

void Arco::PuntoA(RPunto P,real ang) {

  P.X=Centro.X+Radio*Cos(ang);
  P.Y=Centro.Y+Radio*Sin(ang);

  }

void Arco::Argumento(int arg,real valor) {

  switch (arg)  {
	 case 10:Centro.X=valor; break;
	 case 20:Centro.Y=valor; break;
	 case 40:Radio=valor; break;
	 case 50:Angi=AngaRad(valor); PuntoA(Pi,Angi); break;
	 case 51:
		Angf=AngaRad(valor);
		if (Angf<Angi) Angf += 2*M_PI;
		PuntoA(Pf,Angf);
	 }

  }

void Arco::Poner() {

  Rectangulo r1,r2;

  Silueta::Pantalla(Cuadrante(Centro,Radio,Angi,r1),Cuadrante(Centro,Radio,
  Angf,r2));

  }

void Arco::Dibujar() const {

  Silueta::Pantalla.Arco((CRPunto) Centro,Radio,Angi,Angf);

  }

real Arco::Angulo(CRPunto p) const {

  real ang=ArcTg2(p.Y-Centro.Y,p.X-Centro.X);
  if (ang<0) ang += 2*M_PI;

  return ang;

  }

real Arco::Angulo2(CRPunto p) const { return RadaAng(Angulo(p)); }

real Arco::Angulo2(CRPunto p,real ang) const {

  real ang2=Angulo2(p);

  if (ang2<ang) ang2 += 360;

  return ang2-ang;

  }

int Arco::operator ()(CRPunto p) const {

  real ang=Angulo(p);

  return (ang>=Angi && ang<=Angf) || (Angf>2*M_PI && ang<=Angf-2*M_PI);

  }

int Arco::Interseccion(CRArco a,RPunto I) const {

  real d=Centro<<=a.Centro;
  int ext;

  I=a.Centro;
  I-(CRPunto) Centro;
  ext=Igual(d,Radio+a.Radio);
  if (ext || Igual(d,Radio-a.Radio))  {
	 I.Unitario();
	 (I*Radio)+(CRPunto) Centro;
	 return (operator ()(I)) ? ext+1 : PFALSE;
	 }

  return ext;

  }