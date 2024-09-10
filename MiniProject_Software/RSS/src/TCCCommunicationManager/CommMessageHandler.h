#pragma once
#include <map>
#include <nFramework/util/util.h>

class CommMessageHandler
{
public:
	CommMessageHandler();
	~CommMessageHandler();

	tstring getMsgName(unsigned short msgID);

private:
	void initialize();
	void release();
	void setIDNameTable(unsigned short msgID, tstring msgName);

private:
	map<unsigned short, tstring> IDNameTable;
};

