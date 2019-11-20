
{ Turbo Cards }
{ Copyright (c) 1989 by Borland International, Inc. }

unit Cards;
{ Turbo Pascal 5.5 object-oriented example.
  This unit defines a Rolodex-like database of cards.
  Refer to OOPDEMOS.DOC for an overview of this unit.
}

{$S-}

interface

type

  BasePtr = ^Base;
  Base = object
    destructor Done; virtual;
  end;

  CardNodePtr = ^CardNode;
  CardNode = record
    Next: CardNodePtr;
    Prev: CardNodePtr;
    Data: record end;
  end;

  CardListPtr = ^CardList;
  CardList = object(Base)
    Current,FirstPtr: CardNodePtr;
    Count, DataSize: Integer;
    constructor Init(PDataSize: Integer);
    destructor Done; virtual;
    function CardData: Pointer;
    procedure Delete;
    procedure Insert;
    procedure Next;
    procedure Prev;
    procedure First;
  end;

implementation

destructor Base.Done;
begin
end;

constructor CardList.Init(PDataSize: Integer);
begin
  Current := nil;
  Count := 0;
  DataSize := PDataSize;
end;

destructor CardList.Done;
var
  I: Integer;
begin
  for I := 1 to Count do Delete;
end;

function CardList.CardData: Pointer;
begin
  CardData := @Current^.Data;
end;

procedure CardList.Delete;
var
  N: CardNodePtr;
begin
  Dec(Count);
  N := Current;
  if Count = 0 then
    begin
    Current := nil;
    FirstPtr:=nil;
    end
      else
  begin
    Current:=N^.Prev;
    Current^.Next:=N^.Next;
    N^.Next^.Prev := Current;
  end;
  FreeMem(N, DataSize + SizeOf(CardNode));
end;

procedure CardList.Insert;
var
  N: CardNodePtr;
begin
  GetMem(N, DataSize + SizeOf(CardNode));
  if Count = 0 then
  begin
    N^.Next := N;
    N^.Prev := N;
    FirstPtr:= N;
  end else
  begin
    N^.Next := Current^.Next;
    Current^.Next^.Prev := N;
    N^.Prev := Current;
    Current^.Next := N;
  end;
Current := N;
Inc(Count);

end;

procedure CardList.Next;
begin
  if Current <> nil then Current := Current^.Next;
end;

procedure CardList.Prev;
begin
  if Current <> nil then Current := Current^.Prev;
end;

procedure CardList.First;

begin

if Count<>0 then Current:=FirstPtr;

end;

end.