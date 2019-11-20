unit Taladro;

interface

uses cirflex;

type

  Taladros = object(Layer)
    constructor Init;
    function Run:boolean; virtual;
    end;

  TaladrosPtr = ^Taladros;

implementation

uses dos;

const TalExtStr='.TAL';

constructor Taladros.Init;

  begin

  inherited Init;
  L_Mensaje:='TALADROS';
  Hay_Taladros:=True;

  end;

function Taladros.Run:boolean;

  var

    nombre:PathStr;
    nDir:DirStr;
    nNombre:NameStr;
    nExt:ExtStr;
    nomb4:text;
    diametro:extended;
    tal1,tal2:Tal_Type;
    num:longint;
    n:byte;

  const si:boolean=true;

  begin

  ttss:='';
  nombre:=dir('c:\acad','DXF');
  si:=(nombre<>'');
  if si and Capas(nombre) then
    begin
    FSplit(nombre,nDir,nnombre,nExt);
    nombre:=nnombre+TalExtStr;
    writeln('CREANDO ',nombre,' ...');
    assign(nomb4,'C:\ACAD\TRABAJO\'+nombre);
    rewrite(nomb4);
    seek(tal_temp,0);
    tal2.diametro:=0;
    for num:=nums.num_tal downto 1 do
      begin
      read(tal_temp,tal1);
      with tal1,tal1.xy do
	begin
	if (diametro<>tal2.diametro) then writeln(nomb4,'T',diametro:7:2);
	writeln(nomb4,Decimales(x,y,True));
	tal2:=tal1;
	end;
      end;
    Tp_close;
    close(nomb4);
    end;

  Run:=not(si);

  end;

end.