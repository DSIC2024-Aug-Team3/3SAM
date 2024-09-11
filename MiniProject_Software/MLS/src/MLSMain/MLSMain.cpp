#include <nFramework/nIntegrator/nIntegrator.h>
#include <nFramework/util/IniHandler.h>
#include <nFramework/util/util.h>
#include <filesystem>

using namespace std::filesystem;

int main()
{
	NIntegrator* nIntegrator = &(NIntegrator::getInstance());

	IniHandler iniHandler;
	iniHandler.readIni(_T("PlugInList.ini"));

	unsigned int numOfComponents = iniHandler.readInteger(_T("Components"), _T("Count"));

	/************************************************************************
		plug in components
	************************************************************************/
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
	tstring dllPath;
	for (unsigned int cnt = 1; cnt <= numOfComponents; cnt++)
	{
		tstringstream dllPrefix;
		dllPrefix << _T("Component_") << cnt;

		tstring dllIndex;
		dllPrefix >> dllIndex;

		dllPath = iniHandler.readString(dllIndex, _T("PATH"));

		(nIntegrator->getUserManager(dllPath))->start();
	}
	tcin.get();

	/************************************************************************
		plug out components
	************************************************************************/
	for (unsigned int cnt = 1; cnt <= numOfComponents; cnt++)
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

		tstring dllPath = iniHandler.readString(dllIndex, _T("PATH"));

		nIntegrator->plugOutComponent(dllPath);
	}

	tcout << _T("The End") << endl;

	tcin.get();

	return 0;
}
