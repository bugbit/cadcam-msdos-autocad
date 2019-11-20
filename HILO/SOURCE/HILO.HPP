#ifndef	__FSTREAM_H
  #include <fstream.h>
  #endif

#ifndef	__DBLLIST_H
  #include <DblList.h>
  #endif

#ifndef	__PDEFS_HPP
  #include <PDefs.hpp>
  #endif

#ifndef	__POIDENTIFY_HPP
  #include <POIdentify.hpp>
  #endif

typedef long double real;

inline int Igual(real n1,real n2=0) const {
  real cmp=n1-n2;
  if (cmp<0) cmp=-cmp;
  return cmp<=1e-6;
  }

_POOCLASSDEF(Punto)

class _POOCLASSTYPE Punto  {

  public:

	 real X,Y;

	 inline Punto() { X=Y=0; }
	 inline Punto(real x,real y):X(x),Y(y) {}
	 inline int operator == (CRPunto p) const {
		return Igual(Y,p.Y) && Igual(X,p.X);
		}
	 void operator >>= (RPunto p);
	 inline RPunto operator - (CRPunto p) { X -= p.X; Y -= p.Y; return *this; }
	 inline RPunto operator + (CRPunto p) { X += p.X; Y += p.Y; return *this; }
	 real operator <<= (CRPunto p) const;
	 inline RPunto operator /= (real q)  { X /= q; Y /= q; return *this; }
	 inline RPunto operator * (real q) { X *= q; Y *= q; return *this; }
	 inline int operator != (CRPunto p) const { int b=*this==p; return !b; }
	 void Unitario();

  };

_POOCLASSDEF(Rectangulo)

class _POOCLASSTYPE Rectangulo  {

  public:

	 Punto Top,Bottom;

	 inline Rectangulo():Top(),Bottom() {}
	 inline Rectangulo(real x1,real y1,real x2,real y2):Top(x1,y1),Bottom(x2,
	 y2) {}
	 inline Rectangulo(CRPunto p1,CRPunto p2):Top(p1),Bottom(p2) {}

  };

inline void Punto::operator >>= (RPunto p)  {

  Punto tmp=*this;

  *this=p;
  p=tmp;

  }

enum H_VENTANA { V_LOGOTIPO,V_DIBUJO,V_CAMINO,V_COMENTARIO,V_COMENTARIO2,
V_MENSAJE };

_POOCLASSDEF(Limite)

class _POOCLASSTYPE Limite : public Rectangulo  {

  public:

	 inline Limite():Rectangulo(3.4e4931,3.4e-4931,3.4e-4931,3.4e4931) {}
	 void operator ()(RPunto p);
	 inline void operator ()(RPunto p1,RPunto p2) {
		operator ()(p1);
		operator ()(p2);
		}
	 inline void operator ()(RRectangulo r) { operator ()(r.Top,r.Bottom); }
	 inline void operator ()(RRectangulo r1,RRectangulo r2) {
		operator ()(r1); operator ()(r2);
		}

  };

_POOCLASSDEF(Arco)
_POOCLASSDEF(Grafico)

class _POOCLASSTYPE Grafico : public Limite  {

	 int MaxX,XMen;
	 Punto Escala;
	 enum H_VENTANA VOld;

	 void Comentario(enum H_VENTANA v,PCPSTR com);
	 void Comentario();
	 void Presentacion();

  public:

	 inline Grafico():Limite() { MaxX=0; }
	 inline ~Grafico() { Cierra(); }
	 int Inicio();
	 void Cierra() const;
	 void Ventana(enum H_VENTANA v);
	 void Mensaje(PCPSTR str);
	 real ISX(real x) const;
	 real ISY(real y) const;
	 inline real EscX(real x) const { return Escala.X*x; }
	 inline real EscY(real y) const { return Escala.Y*y; }
	 void Arco(CRPunto centro,real radio,real angi,real angf) const;
	 void Grafico::Linea(CRPunto pi,CRPunto pf) const;

  };

_POOCLASSDEF(Figura)

class _POOCLASSTYPE Figura : public Object {

	 virtual classType isA() const=0;
	 virtual char *nameOf() const=0;
	 inline int isEqual(const Object &) const { return PTRUE; }
	 inline int isSortable() const { return PFALSE; }
	 inline int isAssociation() const { return PFALSE; }
	 hashValueType hashValue(void) const { return 0; }

  protected:

	 enum FIGURA  { PINICIAL=POIU,LINEA,ARCO };

	 RRectangulo Cuadrante(CRPunto centro,real radio,real ang,RRectangulo r)
	 const;

  public:

	 Punto Pi;

	 Figura();
	 Figura(PFigura f);
	 
	 virtual void printOn(ostream &o) const;
	 virtual void Argumento(int arg,real valor);
	 inline virtual void Corta() {}
	 virtual void Poner()=0;
	 virtual void Dibujar() const=0;
	 inline virtual int Interseccion(CRArco,RPunto) const { return PFALSE; }
	 inline int EsPuntoI() const { return isA()==PINICIAL; }
	 inline int EsLinea() const { return isA()==LINEA; }
	 inline int EsArco() const { return isA()==ARCO; }

  };

_POOCLASSDEF(PuntoI)

class _POOCLASSTYPE PuntoI : public Figura {

	 real Radio;

	 classType isA() const;
	 char *nameOf() const;

  public:

	 PuntoI();
	 void Poner();
	 void Dibujar() const;
	 void Argumento(int arg,real valor);

  };

_POOCLASSDEF(Linea)

class _POOCLASSTYPE Linea : public Figura  {

    virtual classType isA() const;
	 virtual char *nameOf() const;
	 virtual void printOn(ostream &o) const;

	 static inline int Esta(real n1,real n2) const {
		return (n2>=0) ? (n1>=0 && n1<=n2) : (n1<=0 && n1>=n2);
		}

  public:

	 Punto Pf;

	 Linea();
	 Linea(PFigura f);
	 virtual void Argumento(int arg,real valor);
	 inline virtual void Corta() { Pf >>= Pi; }
	 virtual void Poner();
	 virtual void Dibujar() const;
	 virtual int Interseccion(CRArco a,RPunto p) const;
	 inline int operator == (CRPunto p) const { return Pi==p || Pf==p; }
	 inline int operator == (CRLinea l) const {
		return *this==l.Pi || *this==l.Pf;
		}
	 int operator >>= (CRPunto p);
	 int operator <<= (CRPunto p) const;

  };

_POOCLASSDEF(Arco)

class _POOCLASSTYPE Arco : public Linea  {

    classType isA() const;
	 char *nameOf() const;
	 void printOn(ostream &o) const;

	 void PuntoA(RPunto P,real ang);

  public:

	 Punto Centro;
	 real Radio,Angi,Angf;
	 PBOOL Swp;

	 Arco();
	 Arco(PFigura f);
	 void Argumento(int arg,real valor);
	 inline void Arco::Corta() { Linea::Corta(); Swp ^= 1; }
	 void Poner();
	 void Dibujar() const;
	 int Interseccion(CRArco a,RPunto p) const;
	 PPSTR RadStr() const;
	 int operator ()(CRPunto p) const;
	 real Angulo(CRPunto p) const;
	 real Angulo2(CRPunto p) const;
	 real Angulo2(CRPunto p,real ang) const;

  };

union FIGURAS {
  Object *ob;
  PFigura fg;
  PPuntoI pi;
  PLinea ln;
  PArco ac;
  };

_POOCLASSDEF(Figuras)

class _POOCLASSTYPE Figuras : public DoubleList {

	 static void fg_dibujo(Object &o,void *);

	 friend class Silueta;

  protected:

	 virtual void Dibujo() const;

  public:

	 Figuras();
	 ~Figuras();

  };

_POOCLASSDEF(Contorno)

class _POOCLASSTYPE Contorno : public Figuras {

	 enum CONTORNO { C_NADA=-1,C_INICIO,C_SIGUE,C_INTERSECCION,C_CERRANDO,
	 C_FINAL };

	 struct CONTROL  {
		unsigned C:3;
		unsigned Li:2;
		unsigned Principio:1;
		unsigned Proximo:1;
		unsigned Fin:1;
		} Flag;
	 union FIGURAS Fg;
	 PFigura Pini;
	 PLinea L1[2];
	 PArco A1;
	 PLinea Atras;

	 void Abrir();
	 void Abrir(PFigura f1,PFigura f2);
	 void Inicio();
	 void Sigue();
	 void Interseccion();
	 void Cerrando();

  protected:

	 ofstream *arch;

  public:

    static int NDec;
	 static PBOOL VIJ;

	 inline Contorno():arch(NULL) {}
	 ~Contorno();
	 void PreIJ();
    void PreCM(void);
	 void Interprete(const Object &obj);
	 void Cierra();

  };

_POOCLASSDEF(Silueta)

class _POOCLASSTYPE Silueta : public Figuras  {

	 static void pausa() const;

  protected:

	 Contorno Sup;

	 inline virtual void add(Object &f) {
		if (((RFigura) f).EsPuntoI()) Figuras::add(f); else Sup.add(f);
		}
	 void Dibujo() const;
	 void Proceso();

  public:

	 static Grafico Pantalla;

	 Silueta();
	 ~Silueta();

  };