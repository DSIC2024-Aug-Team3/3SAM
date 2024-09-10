#include "PlugInManager.h"
#include <nFramework/util/IniHandler.h>
#include <nFramework/util/util.h>
#include <nFramework/nom/NOMParser.h>
#include <filesystem>

using namespace std::filesystem;

/************************************************************************
	constructor / destructor
************************************************************************/
PlugInManager::PlugInManager()
{
	init();
}

PlugInManager::~PlugInManager()
{
	release();
}

void
PlugInManager::init()
{
	nIntegrator = &(NIntegrator::getInstance());
}

void
PlugInManager::release()
{
	nIntegrator = nullptr;
}

/************************************************************************
	process plug in services
************************************************************************/
void PlugInManager::doPlugIn()
{
	IniHandler iniHandler;
	iniHandler.readIni(_T("PluginList.ini"));

	unsigned int numOfComponents = iniHandler.readInteger(_T("Components"), _T("Count"));

	for (unsigned int cnt = 1; cnt <= numOfComponents; cnt++)
	{
		tstringstream dllPrefix;
		dllPrefix << _T("Component_") << cnt;

		tstring dllIndex;
		dllPrefix >> dllIndex;

		tstring dllName = iniHandler.readString(dllIndex, _T("DLL"));
		tstring NOMName = iniHandler.readString(dllIndex, _T("NOM"));
		tstring dllPath = iniHandler.readString(dllIndex, _T("PATH"));

		path p(util::tstring2string(dllPath).c_str());
		current_path(p);

		nIntegrator->plugInComponent(dllName, NOMName);

		path up("..");
		current_path(up);
	}

	for (unsigned int cnt = 1; cnt <= numOfComponents; cnt++)
	{
		tstringstream dllPrefix;
		dllPrefix << _T("Component_") << cnt;

		tstring dllIndex;
		dllPrefix >> dllIndex;

		tstring dllPath = iniHandler.readString(dllIndex, _T("PATH"));

		(nIntegrator->getUserManager(dllPath))->start();
	}

	uiManager = nIntegrator->getUserManager(_T("UIManager"));
}

void PlugInManager::doPlugOut()
{
	IniHandler iniHandler;
	iniHandler.readIni(_T("PluginList.ini"));

	unsigned int numOfComponents = iniHandler.readInteger(_T("Components"), _T("Count"));

	for (unsigned int cnt = numOfComponents; cnt >= 1; cnt--)
	{
		tstringstream dllPrefix;
		dllPrefix << _T("Component_") << cnt;

		tstring dllIndex;
		dllPrefix >> dllIndex;

		tstring dllPath = iniHandler.readString(dllIndex, _T("PATH"));

		(nIntegrator->getUserManager(dllPath))->stop();
	}

	for (unsigned int cnt = 1; cnt <= numOfComponents; cnt++)
	{
		tstringstream dllPrefix;
		dllPrefix << _T("Component_") << cnt;

		tstring dllIndex;
		dllPrefix >> dllIndex;

		tstring dllName = iniHandler.readString(dllIndex, _T("DLL"));
		tstring NOMName = iniHandler.readString(dllIndex, _T("NOM"));
		tstring dllPath = iniHandler.readString(dllIndex, _T("PATH"));

		path p(util::tstring2string(dllPath).c_str());
		current_path(p);
		path p1 = current_path();

		nIntegrator->plugOutComponent(dllPath);

		path up("..");
		current_path(up);
	}
}

void
PlugInManager::setHandleWindow(HWND hndlWnd)
{
	if (uiManager)
	{
		this->handleWindow = hndlWnd;
		uiManager->setData(this->handleWindow);
	}
}

void
PlugInManager::sendMsg(tstring name, int length, unsigned char* nomBytes)
{
	if (uiManager)
	{
		shared_ptr<NOM> nomMsg = nIntegrator->getMEBComponent()->getNOMInstance(uiManager->getUserName(), name);
		nomMsg->deserialize(nomBytes, length);
		nomMsg->setOwner(uiManager->getUserName());

		if (nomMsg->getType() == ENOMType::NOM_TYPE_OBJECT)
		{
			uiManager->updateMsg(nomMsg);
		}
		else
		{
			uiManager->sendMsg(nomMsg);
		}
	}
}