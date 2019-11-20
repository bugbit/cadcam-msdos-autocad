unit Siluetas;

interface

uses Cirflex,Herramienta;

  type

  Silueta = Object(Maquina)
    NumsPtr:NumFigTypePtr;
    constructor Init;
    function Run:boolean; virtual;
    function T_Silueta:boolean;
    end;

  SiluetasPtr = ^Silueta;

  Cnc = Object(Silueta)
    constructor Init;
    function Run:boolean; virtual;
    end;

  CncPtr = ^Cnc;

function Compare(n1,n2:Extended):boolean;

implementation

uses Dos,Crt,Cards;

  type

  SriTypePtr = ^SriType;

  SriType = Record
    tipo:string[3];
    XyStr:String[57];
    end;

  Figuras = Object(CardList)
    Siluetas:SiluetasPtr;
    Sils2:string[3];
    constructor Init(silPtr:SiluetasPtr);
    procedure Add(talp:tal_type);
    procedure AddAnte(talp:tal_type);
    function GetFirst:string;
    procedure F_Sri;
    end;

  P_LiAr = ^F_LiAr;
  F_LiAr = Record
    F_pos,F_num,F_posF:longint;
    F_Fin:boolean;
    end;

  const

  TS_Mensaje:array[E_NoPuntoI..succ(succ(E_Figura_Cerrada))] of string[22]=(
  'BUSCANDO PUNTO INICIAL',
  'BUSCANDO INTERSECCION',
  'CERRANDO FIGURA',
  'FIGURA CERRADA',
  'FIGURA NO CERRADA'
  );
  r_90=1.57079632679490E+0000;
  r_180=3.14159265358979E+0000;
  r_270=4.71238898038469E+0000;
  TipoStr:array[4..6] of string[3]=('G01','G02','G03');

constructor Figuras.Init(silPtr:SiluetasPtr);

  begin

  inherited Init(sizeof(SriType));
  Siluetas:=silPtr;

  end;

procedure Figuras.Add(talp:tal_type);

  var
    Arco:boolean;
    liar:byte;
    cateto,cateto2:extended;

  begin

  Insert;
  with talp,xy2,SriTypePtr(CardData)^ do
    begin
    { if not(Siluetas^.AbsRel) and (count>1) then
      begin
      x:=x-xy.x;
      y:=y-xy.y;
      end; }
    liar:=figura;
    Arco:=liar=5;
    if Arco and not(cambiar) then
      inc(LiAr);
    tipo:=TipoStr[liar];
    if count=1 then
      begin
      Add(talp);
      xy_tex(talp);
      end;
    XyStr:=Decimales(x,y,False);
    if Arco then
      if Siluetas^.IJ then
	begin
	cateto:=centro.x-xy.x;
	cateto2:=sqr(diametro)-sqr(cateto);
	if cateto2<0 then
	  cateto2:=0
	    else
	  begin
	  cateto2:=sqrt(cateto2);
	  if (centro.y<xy.y) then cateto2:=-cateto2;
	  end;
	XyStr:=XyStr+' I'+Decimal(cateto,false,true)+'J'+Decimal(cateto2,
	false,false);
	end
	  else
	XyStr:=XyStr+' R'+Decimal(diametro,false,false);
    end;
  end;

procedure Figuras.AddAnte(talp:tal_type);

  begin

  Add(talp);
  Prev;

  end;

function Figuras.GetFirst:string;

  begin

  First;
  if Siluetas^.Iso then
    sils2:='G01'
      else
    begin
    sils2:='G92';
    SriTypePtr(CardData)^.XyStr:=SriTypePtr(CardData)^.XyStr+' Z0';
    end;
  GetFirst:=sils2+' '+SriTypePtr(CardData)^.XyStr;
  Next;

  end;

procedure Figuras.F_Sri;

  var
    i:integer;

  begin

  { with Siluetas^ do if not(Iso) and not(AbsRel) then WriteCncOtro('G91'); }
  for i:=2 to count do
    begin
    with SriTypePtr(CardData)^ do
      begin
      if (sils2<>tipo) or (tipo<>'G01') then
	begin
	XyStr:=tipo+' '+XyStr;
	sils2:=tipo;
	end;
      Siluetas^.WriteCnc(XyStr);
      end;
    Next;
    end;

  end;

constructor Silueta.Init;

  begin

  inherited Init;
  Hay_Siluetas:=True;
  ttss:='';
  L_Mensaje:='SILUETAS';

  end;

function Silueta.Run:boolean;

  var
    nombre:PathStr;
    si:boolean;

  begin

  nombre:=Dir('C:\ACAD','DXF');
  si:=(nombre<>'');
  if si and Capas(nombre) then
    begin
    if Preguntas and T_Silueta then
      begin
      Fm_Cerrar;
      Imprimir;
      end;
    Tp_Close;
    end;

  Run:=not(si);

  end;

function Silueta.T_Silueta:boolean;

  var NoFin_Tmp:boolean;

procedure Get_Tmp(pos:longint;var tal0:tal_type);

  begin

  seek(tal_Temp,pos);
  NoFin_Tmp:=not(eof(tal_temp));
  if NoFin_Tmp then read(Tal_Temp,tal0);

  end;

procedure Put_Tmp(pos:longint;tal0:tal_type);

  begin

  seek(tal_Temp,pos);
  write(Tal_Temp,tal0);

  end;

  var
    i:byte;
    sale,es_s,primero,figura,viejo,corte:boolean;
    tal1:tal_type;
    Db:DbType;
    posS:longint;
    L:Figuras;

procedure TS_Buscar;

  var

  s_error,s_error2:ErroresType;
  F_Linea,F_Arco:F_LiAr;
  pos2:longint;
  x,y:byte;
  bien,passa:boolean;
  tal2,tali:tal_type;
  FirstXYStr:string[60];

function Coger(var ptrla3:F_LiAr):boolean;

  var si:boolean;

  begin

  with ptrla3 do
    begin
    si:=not(F_Fin);
    if si then
      begin
      si:=F_num+F_posF>F_pos;
      if si then
	begin
	Get_Tmp(F_pos,tal2);
	inc(F_pos);
	end
	  else
	F_Fin:=True;
      end;
    end;
  Coger:=si;

  end;

procedure InterCambiar(var ptrla5:F_LiAr);

procedure F_Principio(var ptrla6:F_LiAr);

  begin

  with ptrla6 do
    begin
    F_pos:=F_posF;
    F_Fin:=False;
    end;

  end;

  var
    ptrt,ptrt2:tal_type;
    tmp:P_LiAr;

  begin

  with ptrla5 do
    begin
    dec(F_pos);
    if F_pos<>F_posF then
      begin
      Get_Tmp(F_pos,ptrt);
      Get_Tmp(F_posF,ptrt2);
      Put_Tmp(F_posF,ptrt);
      Put_Tmp(F_pos,ptrt2);
      end;
    inc(F_PosF);
    dec (F_Num);
    F_pos:=F_PosF;
    F_Principio(ptrla5);
    if @ptrla5=@F_Linea then tmp:=@F_Arco else tmp:=@F_Linea;
    F_Principio(tmp^);
    end;

  end;

procedure Intercambio;

  var ptrla:P_LiAr;

  begin

  if tal2.figura=4 then
    ptrla:=@F_Linea
      else
    ptrla:=@F_Arco;
  Intercambiar(ptrla^);

  end;

function xy_igual(xy1,xy2:xy_type):boolean;

  begin

  with xy2 do xy_igual:=Compare(xy1.y,y) and Compare(xy1.x,x);

  end;

function Cmps(var ptrla2:F_LiAr):boolean;

  var si:boolean;

  begin

  si:=Coger(ptrla2);
  if si then
    begin
    si:=Xy_Igual(tal1.xy2,tal2.xy);
    if not(si) then
      begin
      si:=Xy_Igual(tal1.xy2,tal2.xy2);
      if si then xy_tex(tal2);
      end;
    if si then
      begin
      L.Add(tal2);
      InterCambiar(ptrla2);
      end;
    end;
  Cmps:=si;

  end;

function Cmp_Linea:boolean;

  begin

  Cmp_Linea:=Cmps(F_Linea);

  end;

function Cmp_Arco:boolean;

  begin

  Cmp_Arco:=Cmps(F_Arco);

  end;

function Cmp_LiAr:boolean;

  begin

  Cmp_LiAr:=Cmp_Linea or Cmp_Arco;

  end;

procedure TS_PuntoI;

  begin

  passa:=Cmp_Linea;
  if passa then
    begin
    viejo:=true;
    if HayFresar then L.AddAnte(tal2);
    F_Arco.F_pos:=F_Arco.F_posF;
    end
      else
    bien:=F_Linea.F_Fin;

  end;

function F_Final:boolean;

  begin

  bien:=F_Linea.F_Fin and F_Arco.F_Fin;

  F_Final:=bien;

  end;

procedure TS_Inter;

  var
    beta,vx,vy,alfa:extended;
    talp1,talp2:tal_type;

function Esta_ang(angulop,angulof:Extended):boolean;

  begin

  Esta_ang:=(alfa>=angulop) and (alfa<=angulof);

  end;

  begin

  passa:=Cmp_Linea;
  corte:=not(passa);
  if not(passa) and not(F_Linea.F_Fin) then
    with tal1.xy2,tal2 do
      begin
      vx:=xy2.x-xy.x;
      if Compare(vx,0) then
	passa:=Compare(x,xy.x) and (y>=xy.y) and (y<=xy2.y)
	  else
	begin
	vy:=(xy2.y-xy.y);
	if Compare(vy,0)  then
	  passa:=Compare(y,xy.y) and (x>=xy.x) and (x<=xy2.x)
	    else
	  begin
	  alfa:=(x-xy.x)/vx;
	  if Compare(alfa,(y-xy.y)/vy) then
	    begin
	    { beta:=(tal1.xy2.x-xy.x)/vx;
	    gamma:=(tal1.xy.x-xy.x)/vx; }
	    passa:=(y>=xy.y) and (y<=xy2.y);
	    end;
	  end;
	end;
      end;
  if not(passa) then
    begin
    passa:=Cmp_Arco;
    corte:=not(passa);
    end;
  if not(passa) and not(F_Arco.F_Fin) then
    with tal2.centro,tal1 do
      if Compare(sqrt(sqr(xy2.x-x)+sqr(xy2.y-y)),tal2.diametro) then
	begin
	beta:=xy2.x-x;
	if beta<>0 then
	  alfa:=arctan((xy2.y-y)/beta)
	    else
	  alfa:=r_90;
	if (xy2.x<=x) then
	  alfa:=alfa+r_180
	    else
	  if (xy2.y<=y) then alfa:=alfa+r_360;
	if (tal2.angp>tal2.angf) then
	  passa:=Esta_ang(tal2.angp,r_360) or Esta_ang(0,tal2.angf)
	    else
	  passa:=Esta_ang(tal2.angp,tal2.angf);
	corte:=passa;
	end;
  if passa then
    begin
    viejo:=passa;
    if corte then
      begin
      if tal2.figura=4 then xy_tex(tal2);
      talp1:=tal2;
      talp2:=tal2;
      talp1.xy:=tal1.xy2;
      talp2.xy2:=tal1.xy2;
      L.Add(talp1);
      L.AddAnte(talp2);
      Intercambio;
      end;
    tali:=tal2;
    end
      else
    F_Final;

  end;

procedure TS_Figura_Cerrada;

  begin

  if Cmp_LiAr then
    begin
    passa:=Xy_Igual(tal2.xy2,tali.xy);
    viejo:=True;
    end
      else
    if F_Final then passa:=not(corte);

  end;

  var pos3:longint;

procedure Init_F_LiAr(var LiAr:F_LiAr;num:longint);

  begin

  with LiAr do
    begin
    F_pos:=pos3;
    F_PosF:=pos3;
    F_num:=num;
    F_Fin:=False;
    end;
  inc(pos3,num);

  end;

  begin

  with NumsPtr^ do
    begin
    pos2:=PosS;
    pos3:=pos2+num_tal;
    Init_F_LiAr(F_Linea,num_line);
    Init_F_LiAr(F_Arco,num_arc);
    end;
  Get_Tmp(PosS+pred(i),tal1);
  xy_tex(tal1);
  Write('Punto Inicial N. ',i:3,', ');
  y:=WhereY;
  x:=WhereX;
  s_error:=E_NoPuntoI;
  s_error2:=E_NoInter;
  passa:=false;
  bien:=false;
  repeat
    if passa then
      begin
      inc(s_error);
      if not(viejo) then inc (s_error);
      end;
    if s_error<>s_error2 then
      begin
      InversoXY(x,y,TS_Mensaje[s_error]);
      s_error2:=s_error;
      end;
    passa:=False;
    viejo:=False;
    case s_error of
      E_NoPuntoI:TS_PuntoI;
      E_NoInter:TS_Inter;
      E_Figura_Cerrada:TS_Figura_Cerrada;
	else
      passa:=True;
      end;
    if viejo then tal1:=tal2;
    bien:=bien and not(passa);
  until (s_error>E_Figura_Cerrada) or bien;
  if bien then
    begin
    gotoxy(x,y);
    W_Error(s_error,false);
    end
      else
    begin
    if primero or not(HayFresar) then
      begin
      if primero then
	begin
	New_IF(f_cnc,'C:\ACAD\TRABAJO\ALMACEN\'+placa+'.NC',false);
	poner_cab(True,SilStr,@Placa);
	end;
      figura:=F_IniFin(True);
      primero:=false;
      end;
    if figura then
      begin
      FirstXYStr:=L.GetFirst;
      if HayFresar then
	begin
	with Db,V_TalTmp do nums2:=1;
	cambio[8]:=FirstXYStr;
	F_Cambio(not(tipot));
	poner('T1',Db);
	end
	  else
	begin
	espacio:=1;
	Tab(8,'H');
	Tab(20,'  0.20');
	writeln(matr);
	WriteCnc(FirstXYStr);
	if not(ISO) then
	  begin
	  WriteCncOtro('G29');
	  WriteCncOtro('C***             (PARAMETRO DE CORTE)');
	  WriteCncOtro('G4* H***         (CORRECION DE RADIO)');
	  end;
	end;
      L.F_Sri;
      if not(HayFresar) then figura:=F_IniFin(False);
      end;
    end;
  L.Done;
  WriteLN;

  end;

  begin

  sale:=true;
  if SilStr<>#0 then
    begin
    L.Init(@Self);
    posS:=0;
    figura:=false;
    n_placa:=0;
    repeat
      Get_Tmp(posS,tal1);
      es_s:=NoFin_Tmp and (n_placa<=nlayer) and (not(tipot) or
      (tal1.capa[1]='S'));
      if es_s then
	begin
	if tipot then
	  begin
	  placa:=tal1.capa;
	  poner_capa;
	  sale:=not(HayFresar and Pre_AltPro(true));
	  if sale then NumsPtr:=@layers[FindString(layers,tal1.capa,
	  sizeof(layerType),nlayer)].nums;
	  end
	    else
	  begin
	  placa:=codigo;
	  NumsPtr:=@nums;
	  end;
	sale:=es_s and (not(HayFresar) or Dbs('F',D_Fresa,Db,True));
	es_s:=sale;
	end;
      if es_s then
	with NumsPtr^ do
	  begin
	  if (num_tal<>0) or (num_arc<>0) or (num_line<>0) then
	    begin
	    primero:=true;
	    figura:=false;
	    for i:=1 to num_tal do TS_Buscar;
	    if figura then
	      begin
	      ralla('-');
	      if HayFresar then es_s:=F_IniFin(False);
	      close(f_cnc);
	      end;
	    end
	      else
	    W_Error(E_NoLiAr,True);
	  inc(PosS,num_Tal+num_line+num_arc);
	  end;
    until not(es_s);
    dec(lon_m);
    end;
  T_Silueta:=sale;

  end;

constructor Cnc.Init;

  begin

  inherited Init;
  Hay_Taladros:=True;
  Hay_Siluetas:=True;
  ttss:='TS';
  L_Mensaje:='TALADROS, SILUETAS';

  end;

function Cnc.Run:boolean;

  begin

  if Capas(TalDxfStr) then
    begin
    if Preguntas and T_Silueta and trabajos then
      begin
      Fm_Cerrar;
      Imprimir;
      end;
    Tp_Close;
    end;

  Run:=True;

  end;

function Compare(n1,n2:Extended):boolean;

  var diff:Extended;

  begin

  diff:=abs(n1-n2);
  Compare:=diff<=1e-1;  { 1e-5 }

  end;

  end.

function Compare(n1,n2:Extended):boolean;

  var num1,num2:string;

  begin

  str(n1:10:2,num1);  { 10::11 }
  str(n2:10:2,num2);
  Compare:=num1=num2;

  end;