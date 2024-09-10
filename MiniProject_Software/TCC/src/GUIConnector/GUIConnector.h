#pragma once
#include "PlugInManager.h"

#ifdef GUICONNDLL_EXPORTS
#define GUICONNDLL_API __declspec(dllexport)
#else
#define GUICONNDLL_API __declspec(dllimport)
#endif

using namespace std;

struct NOMInfo
{
	TCHAR MsgName[1024];
	unsigned int MsgID;
	int MsgLen;
};

class GUICONNDLL_API GUIConnector
{
public:
	GUIConnector(void);
	~GUIConnector(void);

	void setHandleWindow(HWND hndlWnd);
	void SendMsg(NOMInfo nomInfo, unsigned char* nomBytes);
	void doPlugIn();
	void doPlugOut();

private:
	void init();
	void release();

private:
	PlugInManager* plugInManager;
};

