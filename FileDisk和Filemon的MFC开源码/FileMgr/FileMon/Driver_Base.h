#pragma once

class CDriver_Base
{
public:
	CDriver_Base(void);
	virtual ~CDriver_Base(void);

public: //功能函数集

	char GetDriveLetter(void);

	void GetLastErrorMsg(IN char* pstrPrefix, IN OUT char* pstrErrorMsg);

	void PrintLastError(IN char* Prefix);

};
