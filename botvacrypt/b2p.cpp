//botva2crypt - Inno Setup plugin for protect image's
//work with original botva2.dll
//have a nice day :-)

#include <windows.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#define BOTVA2_GLIFF_ADDR 0x29A6

/*gdiplus.dll func*/
typedef int (WINAPI *GdipLoadImageFromStream_)(IStream*, int);

/*botva2.dll funcs.*/
typedef char *TextBuf[1023];

typedef long (WINAPI *ImgLoad_)(HWND, char*, int, int, int, int, bool, bool);
typedef void (WINAPI *ImgSetVisiblePart_)(long, int, int, int, int);
typedef void (WINAPI *ImgGetVisiblePart_)(long, int, int, int, int);
typedef void (WINAPI *ImgSetPosition_)(long, int, int, int, int);
typedef void (WINAPI *ImgGetPosition_)(long, int, int, int, int);
typedef void (WINAPI *ImgSetVisibility_)(long, bool);
typedef bool (WINAPI *ImgGetVisibility_)(long);
typedef void (WINAPI *ImgSetTransparent_)(long, int);
typedef int  (WINAPI *ImgGetTransparent_)(long);
typedef void (WINAPI *ImgRelease_)(long);
typedef void (WINAPI *ImgApplyChanges_)(HWND);

typedef HWND (WINAPI *BtnCreate_)(HWND, int, int, int, int, char*, int, bool);
typedef void (WINAPI *BtnSetText_)(HWND, char*);
typedef int  (WINAPI *BtnGetText_)(HWND, TextBuf*);
typedef void (WINAPI *BtnSetTextAlignment_)(HWND, int, int, DWORD);
typedef void (WINAPI *BtnSetFont_)(HWND, DWORD);
typedef void (WINAPI *BtnSetFontColor_)(HWND, DWORD, DWORD, DWORD, DWORD);
typedef bool (WINAPI *BtnGetVisibility_)(HWND);
typedef void (WINAPI *BtnSetVisibility_)(HWND, bool);
typedef bool (WINAPI *BtnGetEnabled_)(HWND);
typedef void (WINAPI *BtnSetEnabled_)(HWND, bool);
typedef bool (WINAPI *BtnGetChecked_)(HWND);
typedef void (WINAPI *BtnSetChecked_)(HWND, bool);
typedef void (WINAPI *BtnSetEvent_)(HWND, int, long);
typedef void (WINAPI *BtnGetPosition_)(HWND, int, int, int, int);
typedef void (WINAPI *BtnSetPosition_)(HWND, int, int, int, int);
typedef void (WINAPI *BtnRefresh_)(HWND);
typedef void (WINAPI *BtnSetCursor_)(HWND, DWORD);

typedef int  (WINAPI *GetSysCursorHandle_)(int);
typedef void (WINAPI *gdipShutdown_)();

typedef void (WINAPI *CreateFormFromImage_)(HWND, char*);
typedef int  (WINAPI *CreateBitmapRgn_)(DWORD, HWND, DWORD, int, int);
typedef void (WINAPI *SetMinimizeAnimation_)(bool);
typedef bool (WINAPI *GetMinimizeAnimation_)();

/*global vars.*/
HMODULE	LibModule;
char LibFilePath[MAX_PATH] = { 0 };
IStream *pStream;

/*gdiplus.dll*/
GdipLoadImageFromStream_ GdipLoadImageFromStream;

/*botva2.dll*/
ImgLoad_ MyImgLoad;
ImgSetVisiblePart_ MyImgSetVisiblePart;
ImgGetVisiblePart_ MyImgGetVisiblePart;
ImgSetPosition_ MyImgSetPosition;
ImgGetPosition_ MyImgGetPosition;
ImgSetVisibility_ MyImgSetVisibility;
ImgGetVisibility_ MyImgGetVisibility;
ImgSetTransparent_ MyImgSetTransparent;
ImgGetTransparent_ MyImgGetTransparent;
ImgRelease_ MyImgRelease;
ImgApplyChanges_ MyImgApplyChanges;

BtnCreate_ MyBtnCreate;
BtnSetText_ MyBtnSetText;
BtnGetText_ MyBtnGetText;
BtnSetTextAlignment_ MyBtnSetTextAlignment;
BtnSetFont_ MyBtnSetFont;
BtnSetFontColor_ MyBtnSetFontColor;
BtnGetVisibility_ MyBtnGetVisibility;
BtnSetVisibility_ MyBtnSetVisibility;
BtnGetEnabled_ MyBtnGetEnabled;
BtnSetEnabled_ MyBtnSetEnabled;
BtnGetChecked_ MyBtnGetChecked;
BtnSetChecked_ MyBtnSetChecked;
BtnSetEvent_ MyBtnSetEvent;
BtnGetPosition_ MyBtnGetPosition;
BtnSetPosition_ MyBtnSetPosition;
BtnRefresh_ MyBtnRefresh;
BtnSetCursor_ MyBtnSetCursor;

GetSysCursorHandle_ MyGetSysCursorHandle;
gdipShutdown_ MygdipShutdown;

CreateFormFromImage_ MyCreateFormFromImage;
CreateBitmapRgn_ MyCreateBitmapRgn;
SetMinimizeAnimation_ MySetMinimizeAnimation;
GetMinimizeAnimation_ MyGetMinimizeAnimation;

/*hook original function*/
int WINAPI MyGdipLoadImageFromFile(const WCHAR *szFileName, int GpImage)
{
	HMODULE hModule = LoadLibrary("gdiplus.dll");

	if (hModule != NULL)
		GdipLoadImageFromStream = (GdipLoadImageFromStream_)GetProcAddress(hModule, "GdipLoadImageFromStream");
	else
		return 0;

	FreeLibrary(hModule);

	if (GdipLoadImageFromStream == NULL)
		return 0;
	else
		return GdipLoadImageFromStream(pStream, GpImage);
}

/*dll entry point*/
BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD Reason, LPVOID Reserved)
{
	char *b2Path;
	DWORD oldProtect;

	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		GetModuleFileName(hInstance, LibFilePath, MAX_PATH);
		PathRemoveFileSpec(LibFilePath);

		b2Path = strcat(LibFilePath, "\\botva2.dll");

		LibModule = LoadLibrary(b2Path);

		if (!LibModule)		
			return FALSE;

		MyImgLoad = (ImgLoad_)GetProcAddress(LibModule, "ImgLoad");
		MyImgSetVisiblePart = (ImgSetVisiblePart_)GetProcAddress(LibModule, "ImgSetVisiblePart");
		MyImgGetVisiblePart = (ImgGetVisiblePart_)GetProcAddress(LibModule, "ImgGetVisiblePart");
		MyImgSetPosition = (ImgSetPosition_)GetProcAddress(LibModule, "ImgSetPosition");
		MyImgGetPosition = (ImgGetPosition_)GetProcAddress(LibModule, "ImgGetPosition");
		MyImgSetVisibility = (ImgSetVisibility_)GetProcAddress(LibModule, "ImgSetVisibility");
		MyImgGetVisibility = (ImgGetVisibility_)GetProcAddress(LibModule, "ImgGetVisibility");
		MyImgSetTransparent = (ImgSetTransparent_)GetProcAddress(LibModule, "ImgSetTransparent");
		MyImgGetTransparent = (ImgGetTransparent_)GetProcAddress(LibModule, "ImgGetTransparent");
		MyImgRelease = (ImgRelease_)GetProcAddress(LibModule, "ImgRelease");
		MyImgApplyChanges = (ImgApplyChanges_)GetProcAddress(LibModule, "ImgApplyChanges");

		MyBtnCreate = (BtnCreate_)GetProcAddress(LibModule, "BtnCreate");
		MyBtnSetText = (BtnSetText_)GetProcAddress(LibModule, "BtnSetText");
		MyBtnGetText = (BtnGetText_)GetProcAddress(LibModule, "BtnGetText");
		MyBtnSetTextAlignment = (BtnSetTextAlignment_)GetProcAddress(LibModule, "BtnSetTextAlignment");
		MyBtnSetFont = (BtnSetFont_)GetProcAddress(LibModule, "BtnSetFont");
		MyBtnSetFontColor = (BtnSetFontColor_)GetProcAddress(LibModule, "BtnSetFontColor");
		MyBtnGetVisibility = (BtnGetVisibility_)GetProcAddress(LibModule, "BtnGetVisibility");
		MyBtnSetVisibility = (BtnSetVisibility_)GetProcAddress(LibModule, "BtnSetVisibility");
		MyBtnGetEnabled = (BtnGetEnabled_)GetProcAddress(LibModule, "BtnGetEnabled");
		MyBtnSetEnabled = (BtnSetEnabled_)GetProcAddress(LibModule, "BtnSetEnabled");
		MyBtnGetChecked = (BtnGetChecked_)GetProcAddress(LibModule, "BtnGetChecked");
		MyBtnSetChecked = (BtnSetChecked_)GetProcAddress(LibModule, "BtnSetChecked");
		MyBtnSetEvent = (BtnSetEvent_)GetProcAddress(LibModule, "BtnSetEvent");
		MyBtnGetPosition = (BtnGetPosition_)GetProcAddress(LibModule, "BtnGetPosition");
		MyBtnSetPosition = (BtnSetPosition_)GetProcAddress(LibModule, "BtnSetPosition");
		MyBtnRefresh = (BtnRefresh_)GetProcAddress(LibModule, "BtnRefresh");
		MyBtnSetCursor = (BtnSetCursor_)GetProcAddress(LibModule, "BtnSetCursor");

		MyGetSysCursorHandle = (GetSysCursorHandle_)GetProcAddress(LibModule, "GetSysCursorHandle");
		MygdipShutdown = (gdipShutdown_)GetProcAddress(LibModule, "gdipShutdown");

		MyCreateFormFromImage = (CreateFormFromImage_)GetProcAddress(LibModule, "CreateFormFromImage");
		MyCreateBitmapRgn = (CreateBitmapRgn_)GetProcAddress(LibModule, "CreateBitmapRgn");
		MySetMinimizeAnimation = (SetMinimizeAnimation_)GetProcAddress(LibModule, "SetMinimizeAnimation");
		MyGetMinimizeAnimation = (GetMinimizeAnimation_)GetProcAddress(LibModule, "GetMinimizeAnimation");

		if (VirtualProtect(LibModule + BOTVA2_GLIFF_ADDR, 2, PAGE_EXECUTE_READWRITE, &oldProtect))
			*((DWORD*)LibModule + BOTVA2_GLIFF_ADDR) = (DWORD)MyGdipLoadImageFromFile;
		VirtualProtect(LibModule + BOTVA2_GLIFF_ADDR, 2, oldProtect, &oldProtect);

		break;

	case DLL_PROCESS_DETACH:
		FreeLibrary(LibModule);
		break;
	}
	return TRUE;
}

long WINAPI ImgLoad(HWND hWnd, void *Buffer, long Size, int Left, int Top, int Width, int Height, bool Stretch, bool IsBkg)
{
	long result = 0;
	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, Size);

	if (hBuffer)
	{
		void* pBuffer = GlobalLock(hBuffer);
		CopyMemory(pBuffer, Buffer, Size);
		if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) == S_OK)
			result = MyImgLoad(hWnd, LibFilePath, Left, Top, Width, Height, Stretch, IsBkg);
		GlobalUnlock(hBuffer);
	}
	GlobalUnlock(hBuffer);
	return result;
}

void WINAPI ImgSetVisiblePart(long Img, int NewLeft, int NewTop, int NewWidth, int NewHeight)
{
	MyImgSetVisiblePart(Img, NewLeft, NewTop, NewWidth, NewHeight);
}

void WINAPI ImgGetVisiblePart(long Img, int NewLeft, int NewTop, int NewWidth, int NewHeight)
{
	MyImgGetVisiblePart(Img, NewLeft, NewTop, NewWidth, NewHeight);
}

void WINAPI ImgSetPosition(long Img, int NewLeft, int NewTop, int NewWidth, int NewHeight)
{
	MyImgSetPosition(Img, NewLeft, NewTop, NewWidth, NewHeight);
}

void WINAPI ImgGetPosition(long Img, int NewLeft, int NewTop, int NewWidth, int NewHeight)
{
	MyImgGetPosition(Img, NewLeft, NewTop, NewWidth, NewHeight);
}

void WINAPI ImgSetVisibility(long Img, bool Visible)
{
	MyImgSetVisibility(Img, Visible);
}

bool WINAPI ImgGetVisibility(long Img)
{
	return MyImgGetVisibility(Img);
}

void WINAPI ImgSetTransparent(long Img, int Value)
{
	MyImgSetTransparent(Img, Value);
}

int WINAPI ImgGetTransparent(long Img)
{
	return MyImgGetTransparent(Img);
}

void WINAPI ImgRelease(long Img)
{
	MyImgRelease(Img);
}

void WINAPI ImgApplyChanges(HWND hWnd)
{
	MyImgApplyChanges(hWnd);
}

HWND WINAPI BtnCreate(HWND hWnd, int Left, int Top, int Width, int Height, void *Buffer, long Size, int ShadowWidth, bool IsCheckBtn)
{
	HWND result = 0;
	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, Size);

	if (hBuffer)
	{
		void* pBuffer = GlobalLock(hBuffer);
		CopyMemory(pBuffer, Buffer, Size);
		if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) == S_OK)
			result = MyBtnCreate(hWnd, Left, Top, Width, Height, LibFilePath, ShadowWidth, IsCheckBtn);
		GlobalUnlock(hBuffer);
	}
	GlobalUnlock(hBuffer);
	return result;
}

void WINAPI BtnSetText(HWND hWnd, char *Text)
{
	return MyBtnSetText(hWnd, Text);
}

int WINAPI BtnGetText(HWND hWnd, TextBuf *TextBuff)
{
	return MyBtnGetText(hWnd, TextBuff);
}

void WINAPI BtnSetTextAlignment(HWND hWnd, int HorIndent, int VertIndent, DWORD Alignment)
{
	MyBtnSetTextAlignment(hWnd, HorIndent, VertIndent, Alignment);
}

void WINAPI BtnSetFont(HWND hWnd, DWORD Font)
{
	MyBtnSetFont(hWnd, Font);
}

void WINAPI BtnSetFontColor(HWND hWnd, DWORD NormalFontColor, DWORD FocusedFontColor, DWORD PressedFontColor, DWORD DisabledFontColor)
{
	MyBtnSetFontColor(hWnd, NormalFontColor, FocusedFontColor, PressedFontColor, DisabledFontColor);
}

bool WINAPI BtnGetVisibility(HWND hWnd)
{
	return BtnGetVisibility(hWnd);
}

void WINAPI BtnSetVisibility(HWND hWnd, bool Value)
{
	MyBtnSetVisibility(hWnd, Value);
}

bool WINAPI BtnGetEnabled(HWND hWnd)
{
	return MyBtnGetEnabled(hWnd);
}

void WINAPI BtnSetEnabled(HWND hWnd, bool Value)
{
	MyBtnSetEnabled(hWnd, Value);
}

bool WINAPI BtnGetChecked(HWND hWnd)
{
	return MyBtnGetChecked(hWnd);
}

void WINAPI BtnSetChecked(HWND hWnd, bool Value)
{
	MyBtnSetChecked(hWnd, Value);
}

void WINAPI BtnSetEvent(HWND hWnd, int EventID, long Event)
{
	MyBtnSetEvent(hWnd, EventID, Event);
}

void WINAPI BtnGetPosition(HWND hWnd, int Left, int Top, int Width, int Height)
{
	MyBtnGetPosition(hWnd, Left, Top, Width, Height);
}

void WINAPI BtnSetPosition(HWND hWnd, int Left, int Top, int Width, int Height)
{
	MyBtnSetPosition(hWnd, Left, Top, Width, Height);
}

void WINAPI BtnRefresh(HWND hWnd)
{
	MyBtnRefresh(hWnd);
}

void WINAPI BtnSetCursor(HWND hWnd, DWORD hCur)
{
	MyBtnSetCursor(hWnd, hCur);
}

int WINAPI GetSysCursorHandle(int Id)
{
	return MyGetSysCursorHandle(Id);
}

void WINAPI gdipShutdown()
{	
	MygdipShutdown();
}

void WINAPI CreateFormFromImage(HWND hWnd, char *Buffer, long dwSize)
{
	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, dwSize);

	if (hBuffer)
	{
		void* pBuffer = GlobalLock(hBuffer);
		CopyMemory(pBuffer, Buffer, dwSize);
		if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) == S_OK)
			MyCreateFormFromImage(hWnd, LibFilePath);
		GlobalUnlock(hBuffer);
	}
	GlobalUnlock(hBuffer);
}

int WINAPI CreateBitmapRgn(DWORD hDC, HWND Bitmap, DWORD TransClr, int dX, int dY)
{
	return MyCreateBitmapRgn(hDC, Bitmap, TransClr, dX, dY);
}

void WINAPI SetMinimizeAnimation(bool Value)
{
	MySetMinimizeAnimation(Value);
}

bool WINAPI GetMinimizeAnimation()
{
	return MyGetMinimizeAnimation();
}
