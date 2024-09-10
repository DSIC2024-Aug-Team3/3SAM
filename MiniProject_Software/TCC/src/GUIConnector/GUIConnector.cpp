#include "GUIConnector.h"

/************************************************************************
	constructor / destructor
************************************************************************/
GUIConnector::GUIConnector()
{
	init();
}

GUIConnector::~GUIConnector()
{
	release();
}

void
GUIConnector::init()
{
	plugInManager = new PlugInManager;
}

void
GUIConnector::release()
{
	delete plugInManager;
}

/************************************************************************
	interacting with PlugInManager
************************************************************************/
void
GUIConnector::setHandleWindow(HWND hndlWnd)
{
	plugInManager->setHandleWindow(hndlWnd);
}

void
GUIConnector::SendMsg(NOMInfo nomInfo, unsigned char* nomBytes)
{
	plugInManager->sendMsg(nomInfo.MsgName, nomInfo.MsgLen, nomBytes);
}

void GUIConnector::doPlugIn()
{
	plugInManager->doPlugIn();
}

void GUIConnector::doPlugOut()
{
	plugInManager->doPlugOut();
}

/************************************************************************
	external function (interacting with WPF_GUI components)
************************************************************************/
extern "C" GUICONNDLL_API
GUIConnector * CreateGUIConn()
{
	return new GUIConnector;
}

extern "C" GUICONNDLL_API
void DeleteGUIConn(GUIConnector * guiConn)
{
	if (guiConn) delete guiConn;
}

extern "C" GUICONNDLL_API
void SetHandle(GUIConnector * guiConn, HWND hndlWnd)
{
	guiConn->setHandleWindow(hndlWnd);
}

extern "C" GUICONNDLL_API
void SendMsg(GUIConnector * guiConn, NOMInfo nomInfo, unsigned char* nomBytes)
{
	guiConn->SendMsg(nomInfo, nomBytes);
}

extern "C" GUICONNDLL_API
void DoPlugIn(GUIConnector * guiConn)
{
	guiConn->doPlugIn();
}

extern "C" GUICONNDLL_API
void DoPlugOut(GUIConnector * guiConn)
{
	guiConn->doPlugOut();
}
