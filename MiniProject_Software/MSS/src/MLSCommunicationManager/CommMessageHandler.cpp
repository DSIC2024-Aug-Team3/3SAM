#include "CommMessageHandler.h"

/************************************************************************
	initialize / release
************************************************************************/
CommMessageHandler::CommMessageHandler()
{
	initialize();
}

CommMessageHandler::~CommMessageHandler()
{
	release();
}

void
CommMessageHandler::initialize()
{
	setIDNameTable(1, _T("deployMsg"));
	setIDNameTable(2, _T("deployChkMsg"));
	setIDNameTable(3, _T("startMsg"));
	setIDNameTable(4, _T("startChkMsg"));
	setIDNameTable(5, _T("R_ATSPositionMsg"));
	setIDNameTable(6, _T("O_ATSPositionMsg"));
	setIDNameTable(7, _T("launchingMsg"));
	setIDNameTable(8, _T("fireMsg"));
	setIDNameTable(9, _T("R_missileInfoMsg"));
	setIDNameTable(10, _T("MSSPositionMsg"));
	setIDNameTable(11, _T("interceptResMsg"));
	setIDNameTable(12, _T("stopMsg"));
	setIDNameTable(13, _T("stopChkMsg"));
	setIDNameTable(14, _T("detectMsg"));
	setIDNameTable(15, _T("stockMsg"));
}

void
CommMessageHandler::release()
{
	IDNameTable.clear();
}

/************************************************************************
	ID_Name table management
************************************************************************/
void
CommMessageHandler::setIDNameTable(unsigned short msgID, tstring msgName)
{
	IDNameTable.insert(pair<unsigned short, tstring>(msgID, msgName));
}

tstring
CommMessageHandler::getMsgName(unsigned short msgID)
{
	tstring msgName;
	if (auto itr = IDNameTable.find(msgID); itr != IDNameTable.end())
	{
		msgName = itr->second;
	}
	else
	{
		msgName = _T("");
	}

	return msgName;
}