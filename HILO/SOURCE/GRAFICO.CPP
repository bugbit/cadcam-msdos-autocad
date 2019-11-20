#include <graphics.h>
#include <PMathEnt.hpp>
#include <PMath.hpp>
#include "..\source\hilo.hpp"

#define W_COMENTARIO 31*8

static struct VENTANAS  {
  PBYTE Tinta;
  int TLinea;
  struct viewporttype Ventana;
  } Ventanas[]={
  { WHITE,SOLID_LINE },
  { RED,DOTTED_LINE },
  { CYAN,SOLID_LINE },
  { RED,DOTTED_LINE },
  { CYAN,SOLID_LINE },
  { YELLOW,SOLID_LINE }
  };

void Limite::operator()(RPunto p) {

  Top.X=Min(Top.X,p.X);
  Top.Y=Max(Top.Y,p.Y);
  Bottom.X=Max(Bottom.X,p.X);
  Bottom.Y=Min(Bottom.Y,p.Y);

  }

void Grafico::Comentario(enum H_VENTANA v,PCPSTR com) {

  Ventana(v);
  line(0,4,12,4);
  outtextxy(15,0,(PPSTR) com);

  }

void Grafico::Comentario() {

  Comentario(V_COMENTARIO," Figura ");
  Comentario(V_COMENTARIO2," Recorrido ");

  }

void Grafico::Presentacion() {

  Comentario();
  settextjustify(CENTER_TEXT,TOP_TEXT);
  Ventana(V_LOGOTIPO);
  outtextxy(MaxX/2,0,
	"(C) Hilo 1.8, 1995-97. Programa CNC para maquinas de hilo, Oscar "
	"Hernandez Ba¤o ");
  Ventana(V_DIBUJO);

  }

int Grafico::Inicio() {

  int gdriver=DETECT,gmode;

  registerfarbgidriver(CGA_driver_far);
  registerfarbgidriver(EGAVGA_driver_far);
  registerfarbgidriver(IBM8514_driver_far);
  registerfarbgidriver(Herc_driver_far);
  registerfarbgidriver(ATT_driver_far);
  registerfarbgidriver(PC3270_driver_far);
  initgraph(&gdriver,&gmode,"");
  if ((gmode=graphresult()!=grOk))
	 cerr << "\nError : " << grapherrormsg(gmode) << "\a\n\n";
		else  {
	 MaxX=getmaxx();
	 int maxy=getmaxy();
	 Escala.X=MaxX/Hypot(Abs(Bottom.X-Top.X),Abs(Bottom.Y-Top.Y));
	 Escala.Y=Escala.X*(real) (maxy-20)/MaxX;
	 Ventanas[V_LOGOTIPO].Ventana.right=Ventanas[V_DIBUJO].Ventana.right=
	 Ventanas[V_MENSAJE].Ventana.right=MaxX;
	 Ventanas[V_LOGOTIPO].Ventana.bottom=Ventanas[V_DIBUJO].Ventana.top=12;
	 Ventanas[V_DIBUJO].Ventana.bottom=maxy-20;
	 Ventanas[V_CAMINO].Ventana=Ventanas[V_DIBUJO].Ventana;
	 Ventanas[V_COMENTARIO].Ventana.top=Ventanas[V_MENSAJE].Ventana.top=maxy-13;
	 Ventanas[V_COMENTARIO].Ventana.right=Ventanas[V_MENSAJE].Ventana.left=
	 W_COMENTARIO;
	 Ventanas[V_COMENTARIO].Ventana.bottom=Ventanas[V_MENSAJE].Ventana.bottom=
	 maxy;
	 Ventanas[V_COMENTARIO2].Ventana=Ventanas[V_COMENTARIO].Ventana;
	 Ventanas[V_COMENTARIO2].Ventana.left += 15*8;
	 XMen=(MaxX-W_COMENTARIO)/2;
	 Presentacion();
	 }

  return gmode;

  }

void Grafico::Cierra() const {

  if (MaxX) closegraph();

  }

void Grafico::Ventana(enum H_VENTANA v) {

  struct viewporttype &w=Ventanas[v].Ventana;
  struct VENTANAS &vnt=Ventanas[v];

  setviewport(w.left,w.top,w.right,w.bottom,PTRUE);
  setcolor(vnt.Tinta);
  setlinestyle(vnt.TLinea,SOLID_FILL,NORM_WIDTH);
  VOld=v;

  }

void Grafico::Mensaje(PCPSTR str) {

  enum H_VENTANA v=VOld;

  Ventana(V_MENSAJE);
  clearviewport();
  outtextxy(XMen,0,(PPSTR) str);
  Ventana(v);

  }

real Grafico::ISX(real x) const {

  return Escala.X*Abs(Top.X-x);

  }

real Grafico::ISY(real y) const {

  return Escala.Y*Abs(Top.Y-y);

  }

void Grafico::Arco(CRPunto centro,real radio,real angi,real angf) const {

  const real cx=ISX(centro.X),cy=ISY(centro.Y),rx=EscX(radio),ry=EscY(radio);

  while (angf-angi>2.0*M_PI) angf -= 2.0*M_PI;
  while (angi>angf) angf += 2.0*M_PI;
  const real seg=M_PI/8.0;		//	2/16
  moveto(Ceil(cx+rx*Cos(angi)),Ceil(cy-ry*Sin(angi)));
  while (angi<angf) {
	 angi += seg;
	 if (angi>angf) angi=angf;
    lineto(Ceil(cx+rx*Cos(angi)),Ceil(cy-ry*Sin(angi)));
	 }

  }

void Grafico::Linea(CRPunto pi,CRPunto pf) const {

  line(Ceil(ISX(pi.X)),Ceil(ISY(pi.Y)),Ceil(ISX(pf.X)),Ceil(ISY(pf.Y)));

  }