#pragma once

class mysql_Process
{
	mysql_Process();
	virtual ~mysql_Process();

public:

	BOOL ConnectDB();
	void DisconnectDB(void);

	void LoadCompanyInfo(char cls, CString companyName);

	CString GetpdtNamebyOrdercode(CString code);

	BOOL Insert_OrderProduct(CString dateTime, CString orderCode, CString pdtName, CString classify, int amount, int shotAmount, int price);
	BOOL Update_Sales_making(CString mCode, CString making);
	BOOL WriteDBQuery(CString strquery);
};

