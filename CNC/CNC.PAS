{$M 16384,0,655360}

uses cirflex,taladro,Herramienta,siluetas;

type

  ProgramaStr = string[14];
  ProgramaPtr = ^ProgramaTipo;
  ObjetoTipo = function(vtm:word;obj:longint):pointer;
  ProgramaTipo = record
    Str:ProgramaStr;
    Obj:pointer;
    Vtm:Word;
    end;

const

  Num_Programas=5;
  programas:array[1..Num_Programas] of ProgramaTipo =(
  (Str:'TALADROS';Obj:@Taladros.Init;Vtm:ofs(typeof(Taladros)^)),
  (Str:'HERRAMIENTAS';Obj:@Herramientas.Init;Vtm:ofs(typeof(Herramientas)^)),
  (Str:'MAQUINA';Obj:@Maquina.Init;Vtm:ofs(typeof(Maquina)^)),
  (Str:'SILUETAS';Obj:@Silueta.Init;Vtm:ofs(typeof(Silueta)^)),
  (Str:'CENTROCNC';Obj:@Cnc.Init;Vtm:ofs(typeof(Cnc)^))
  );

procedure Run; near;

procedure exec(ptr:ProgramaPtr); near;

  var
    obj:P_RunPtr;
    si:boolean;

  begin

  repeat
    obj:=ObjetoTipo(ptr^.Obj)(ptr^.Vtm,0);
    pantalla_pre(ptr^.Str);
    si:=obj^.Run;
    dispose(obj);
    borrar_pre;
    until si;

  end;

var

  num:word;
  str:ProgramaStr;

begin

  if proteccion then
    begin
    Cursor(NoCursor);
    str:=paramstr(1);
    num:=FindString(programas,str,sizeof(ProgramaTipo),Num_Programas);
    if num<>0 then
      exec(@programas[num])
	else
      begin
      Writeln('CNC <Programa>');
      Writeln('   Programa :');
      for num:=1 to Num_Programas do Writeln(Programas[num].str);
      end;
    Cursor(NormalCursor);
    end;
  end;

begin

  Run;

end.