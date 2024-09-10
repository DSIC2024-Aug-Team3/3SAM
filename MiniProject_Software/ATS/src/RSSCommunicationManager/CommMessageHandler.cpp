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
	// if need be, write your code
	setIDNameTable(100, _T("MessageName"));
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