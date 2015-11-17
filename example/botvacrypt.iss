[Setup]
AppName=exec
AppVerName=0.1.0.0      
CreateAppDir=no
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Files]
Source: botva2.dll; DestDir: {tmp}; Flags: dontcopy;
Source: b2p.dll; DestDir: {tmp}; Flags: dontcopy;
Source: test.bmp; DestDir: {tmp}; Flags: dontcopy;

[Code]
#include "botva2.iss"

procedure InitializeWizard();
var
img: longint;
hNextBtn: HWND;
begin
  with WizardForm do begin
    InnerNotebook.Hide;
    OuterNotebook.Hide;
    Bevel.Hide;
    Center;
  end;

  ExtractTemporaryFile('botva2.dll');
  ExtractTemporaryFile('b2p.dll');

  img:= ImgLoad(WizardForm.Handle, 'test.bmp', 0, 0, 125, 458, true, true);

  ImgApplyChanges(WizardForm.Handle);
end;

procedure DeinitializeSetup;
begin
  gdipShutdown;
end;
