#pragma once
#include <nFramework/nFrameworkMain.h>
#include <nFramework/nIntegrator/nIntegrator.h>

#ifdef GUICONNDLL_EXPORTS
#define GUICONNDLL_API __declspec(dllexport)
#else
#define GUICONNDLL_API __declspec(dllimport)
#endif

class GUICONNDLL_API PlugInManager
{
public:
	PlugInManager(void);
	~PlugInManager(void);

	void setHandleWindow(HWND hndlWnd);
	void sendMsg(tstring name, int length, unsigned char* nomBytes);
	void doPlugIn();
	void doPlugOut();

private:
	void init();
	void release();

private:
	NIntegrator* nIntegrator;
	BaseManager* uiManager;

	HWND handleWindow;
};
