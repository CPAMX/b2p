[Code]
const
  BTN_MAX_PATH = 1024;

  BtnClickEventID      = 1;
  BtnMouseEnterEventID = 2;
  BtnMouseLeaveEventID = 3;
  BtnMouseMoveEventID  = 4;
  BtnMouseDownEventID  = 5;
  BtnMouseUpEventID    = 6;

  balLeft    = 0;
  balCenter  = 1;
  balRight   = 2;
  balVCenter = 4;

type
  TBtnEventProc = procedure(hWnd: HWND);
  TTextBuf = array [0..BTN_MAX_PATH - 1] of Char;

function LibImgLoad(hWnd: HWND; Buffer: PAnsiChar; BuffSize: Longint; Left, Top, Width, Height: Integer; Stretch, IsBkg: Boolean): Longint; external 'ImgLoad@{tmp}\b2p.dll stdcall delayload';
procedure ImgSetVisiblePart(Img: Longint; NewLeft, NewTop, NewWidth, NewHeight: Integer); external 'ImgSetVisiblePart@{tmp}\b2p.dll stdcall delayload';
procedure ImgGetVisiblePart(Img: Longint; var Left, Top, Width, Height: Integer); external 'ImgGetVisiblePart@{tmp}\b2p.dll stdcall delayload';
procedure ImgSetPosition(Img: Longint; NewLeft, NewTop, NewWidth, NewHeight: Integer); external 'ImgSetPosition@{tmp}\b2p.dll stdcall delayload';
procedure ImgGetPosition(Img: Longint; var Left, Top, Width, Height: Integer); external 'ImgGetPosition@{tmp}\b2p.dll stdcall delayload';
procedure ImgSetVisibility(Img: Longint; Visible: Boolean); external 'ImgSetVisibility@{tmp}\b2p.dll stdcall delayload';
function ImgGetVisibility(Img: Longint): Boolean; external 'ImgGetVisibility@{tmp}\b2p.dll stdcall delayload';
procedure ImgSetTransparent(Img: Longint; Value: Integer); external 'ImgSetTransparent@{tmp}\b2p.dll stdcall delayload';
function ImgGetTransparent(Img: Longint): Integer; external 'ImgGetTransparent@{tmp}\b2p.dll stdcall delayload';
procedure ImgRelease(Img: Longint); external 'ImgRelease@{tmp}\b2p.dll stdcall delayload';
procedure ImgApplyChanges(hWnd: HWND); external 'ImgApplyChanges@{tmp}\b2p.dll stdcall delayload';

function LibBtnCreate(hWnd: HWND; Left, Top, Width, Height: Integer; Buffer: PAnsiChar; Size: Longint; ShadowWidth: Integer; IsCheckBtn: Boolean): HWND; external 'BtnCreate@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetText(hWnd: HWND; Text: PAnsiChar); external 'BtnSetText@{tmp}\b2p.dll stdcall delayload';
function BtnGetText_(hWnd: HWND; var Text: TTextBuf): Integer; external 'BtnGetText@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetTextAlignment(hWnd: HWND; HorIndent, VertIndent: Integer; Alignment: DWORD); external 'BtnSetTextAlignment@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetFont(hWnd: HWND; Font: Cardinal); external 'BtnSetFont@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetFontColor(hWnd: HWND; NormalFontColor, FocusedFontColor, PressedFontColor, DisabledFontColor :Cardinal); external 'BtnSetFontColor@{tmp}\b2p.dll stdcall delayload';
function BtnGetVisibility(hWnd: HWND): Boolean; external 'BtnGetVisibility@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetVisibility(hWnd: HWND; Value: Boolean); external 'BtnSetVisibility@{tmp}\b2p.dll stdcall delayload';
function BtnGetEnabled(hWnd: HWND): Boolean; external 'BtnGetEnabled@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetEnabled(hWnd: HWND; Value: Boolean); external 'BtnSetEnabled@{tmp}\b2p.dll stdcall delayload';
function BtnGetChecked(hWnd: HWND): Boolean; external 'BtnGetChecked@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetChecked(hWnd: HWND; Value: Boolean); external 'BtnSetChecked@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetEvent(hWnd: HWND; EventID: Integer; Event :Longword); external 'BtnSetEvent@{tmp}\b2p.dll stdcall delayload';
procedure BtnGetPosition(hWnd: HWND; var Left, Top, Width, Height: Integer);  external 'BtnGetPosition@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetPosition(hWnd: HWND; NewLeft, NewTop, NewWidth, NewHeight: Integer);  external 'BtnSetPosition@{tmp}\b2p.dll stdcall delayload';
procedure BtnRefresh(hWnd: HWND); external 'BtnRefresh@{tmp}\b2p.dll stdcall delayload';
procedure BtnSetCursor(hWnd: HWND; hCur:Cardinal); external 'BtnSetCursor@{tmp}\b2p.dll stdcall delayload';

function GetSysCursorHandle(Id: Integer): Cardinal; external 'GetSysCursorHandle@{tmp}\b2p.dll stdcall delayload';
procedure gdipShutdown; external 'gdipShutdown@{tmp}\b2p.dll stdcall delayload';

procedure LibCreateFormFromImage(hWnd: HWND; Buffer: PAnsiChar; Size: Longint); external 'CreateFormFromImage@{tmp}\b2p.dll stdcall delayload';
function CreateBitmapRgn(DC: LongWord; Bitmap: HBITMAP; TransClr: DWORD; dX: Integer; dY: Integer): LongWord; external 'CreateBitmapRgn@{tmp}\b2p.dll stdcall delayload';
procedure SetMinimizeAnimation(Value: Boolean); external 'SetMinimizeAnimation@{tmp}\b2p.dll stdcall delayload';
function GetMinimizeAnimation: Boolean; external 'GetMinimizeAnimation@{tmp}\b2p.dll stdcall delayload';

function BtnGetText(hBtn:HWND):AnsiString;
var
  Buffer: TTextBuf;
  tmp: AnsiString;
  i: Integer;
begin
  BtnGetText_(hBtn, Buffer);
  i:= 0;
  tmp:= '';
  while Buffer[i] <> #0 do begin
    tmp:= tmp + Buffer[i];
    i:= i + 1;
  end;
  Result:= tmp;
end;

function ImgLoad(hWnd: HWND; FileName: AnsiString; Left, Top, Width, Height: Integer; Stretch, IsBkg: Boolean): Longint;
var
  FileSize: Longint;
  Buffer: AnsiString;
begin
  Delete(FileName, Pos('{tmp}\', FileName), Length('{tmp}\'));
  Delete(FileName, Pos(ExpandConstant('{tmp}\'), FileName), Length(ExpandConstant('{tmp}\')));

  FileSize:= ExtractTemporaryFileSize(FileName);
  SetLength(Buffer, FileSize);
  #ifdef UNICODE
    ExtractTemporaryFileToBuffer(FileName, CastAnsiStringToInteger(Buffer));
  #else
    ExtractTemporaryFileToBuffer(FileName, CastStringToInteger(Buffer));
  #endif
  Result:= LibImgLoad(hWnd, Buffer, FileSize, Left, Top, Width, Height, Stretch, IsBkg);
end;

function BtnCreate(hWnd: HWND; Left, Top, Width, Height: Integer; FileName: AnsiString; ShadowWidth: Integer; IsCheckBtn: Boolean): HWND;
var
  FileSize: Longint;
  Buffer: AnsiString;
begin
  Delete(FileName, Pos('{tmp}\', FileName), Length('{tmp}\'));
  Delete(FileName, Pos(ExpandConstant('{tmp}\'), FileName), Length(ExpandConstant('{tmp}\')));

  FileSize:= ExtractTemporaryFileSize(FileName);
  SetLength(Buffer, FileSize);
  #ifdef UNICODE
    ExtractTemporaryFileToBuffer(FileName, CastAnsiStringToInteger(Buffer));
  #else
    ExtractTemporaryFileToBuffer(FileName, CastStringToInteger(Buffer));
  #endif
  Result:= LibBtnCreate(hWnd, Left, Top, Width, Height, Buffer, FileSize, ShadowWidth, IsCheckBtn);
end;

procedure CreateFormFromImage(hWnd: HWND; Filename: AnsiString);
var
  FileSize: Longint;
  Buffer: AnsiString;
begin
  Delete(FileName, Pos('{tmp}\', FileName), Length('{tmp}\'));
  Delete(FileName, Pos(ExpandConstant('{tmp}\'), FileName), Length(ExpandConstant('{tmp}\')));

  FileSize:= ExtractTemporaryFileSize(FileName);
  SetLength(Buffer, FileSize);
  #ifdef UNICODE
    ExtractTemporaryFileToBuffer(FileName, CastAnsiStringToInteger(Buffer));
  #else
    ExtractTemporaryFileToBuffer(FileName, CastStringToInteger(Buffer));
  #endif
  LibCreateFormFromImage(hWnd, Buffer, FileSize);
end;
