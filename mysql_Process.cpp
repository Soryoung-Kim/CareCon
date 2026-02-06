#include "pch.h"
#include "CareConn.h"
#include "mysql_Process.h"

extern CCareConnApp* m_App;

mysql_Process::mysql_Process()
{
}


mysql_Process::~mysql_Process()
{
}

// mySQL DB Connect
BOOL mysql_Process::ConnectDB()
{
	mysql_init(&conn);

	mysqlConn = mysql_real_connect(&conn, HOST, DBUser, DBPass, DBName, DBPORT, (char*)NULL, 0);
	if (mysqlConn == NULL)
	{
		AfxMessageBox(L"Database Server에 연결되지 않았습니다");
		LogWrite(L"MySQL", L"Database Server에 연결되지 않았습니다");
		return false;
	}

	mysql_query(&conn, "set Names euckr");

	LogWrite(L"MySQL", L"Database Server 접속 성공!");

	return true;
}


void mysql_Process::DisconnectDB(void)
{
	mysql_close(mysqlConn);
	mysqlConn = NULL;
	LogWrite(L"MySQL", L"Database Server disconnected");

}


void mysql_Process::LoadCompanyInfo(char cls, CString companyName)
{
	CString strQuery;
	char* strquery;
	int resultMsg;

	if (!ConnectDB())
	{
		AfxMessageBox(L"Database가 연결되지 않았습니다.");
		LogWrite(L"MySQL", L"Database에서 제품 정보를 가져오지 못했습니다");
		return;
	}

	companyName = _T("%") + companyName + _T("%");
	// Loading 시도교육청 이름(sc_name), 기업/학교명(comp_name),  ----------------------------------------------------------------------------------------------------------
	strQuery.Format(L"SELECT sc_name, comp_name, tel_num FROM company_info where comp_class = '%c' and comp_name like '%s'", cls, companyName);

	USES_CONVERSION;

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		return;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : Company_info mysql_store_result");
		return;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		m_App->lCoffee[nCntCoffee].mCode.Format(L"%s", A2W(sql_row[0]));
		m_App->lCoffee[nCntCoffee].mProductName.Format(L"%s", A2W(sql_row[1]));
		m_App->lCoffee[nCntCoffee].mClassify.Format(L"%s", A2W(sql_row[2]));
		m_App->lCoffee[nCntCoffee].nPrice = atoi(sql_row[3]);
		m_App->lCoffee[nCntCoffee].nStatus = atoi(sql_row[4]);
		nCntCoffee++;
	}
	m_App->coffeeProductCount = nCntCoffee;
	// ----------------------------------------------------------------------------------------------------------------------- 

	// Loading 라떼 ----------------------------------------------------------------------------------------------------------
	strQuery.Format(L"SELECT pdt_code, pdt_name, classify, price, status FROM product where shop_name = '%s' and Menu='라떼';", m_App->m_ShopName);

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 라떼 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 라떼 정보를 가져오지 못했습니다");
		return;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 라떼 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : 라떼 mysql_store_result");
		return;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		m_App->lLatte[nCntLatte].mCode.Format(L"%s", A2W(sql_row[0]));
		m_App->lLatte[nCntLatte].mProductName.Format(L"%s", A2W(sql_row[1]));
		m_App->lLatte[nCntLatte].mClassify.Format(L"%s", A2W(sql_row[2]));
		m_App->lLatte[nCntLatte].nPrice = atoi(sql_row[3]);
		m_App->lLatte[nCntLatte].nStatus = atoi(sql_row[4]);
		nCntLatte++;
	}
	m_App->latteProductCount = nCntLatte;
	// -----------------------------------------------------------------------------------------------------------------------

	// Loading 디저트 ----------------------------------------------------------------------------------------------------------
	strQuery.Format(L"SELECT pdt_code, pdt_name, classify, price, status FROM product where shop_name = '%s' and Menu='디저트';", m_App->m_ShopName);

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 디저트 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 디저트 정보를 가져오지 못했습니다");
		return;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 디저트 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : 디저트 mysql_store_result");
		return;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		m_App->lSmoothieDesert[nCntSmoothieDesert].mCode.Format(L"%s", A2W(sql_row[0]));
		m_App->lSmoothieDesert[nCntSmoothieDesert].mProductName.Format(L"%s", A2W(sql_row[1]));
		m_App->lSmoothieDesert[nCntSmoothieDesert].mClassify.Format(L"%s", A2W(sql_row[2]));
		m_App->lSmoothieDesert[nCntSmoothieDesert].nPrice = atoi(sql_row[3]);
		m_App->lSmoothieDesert[nCntSmoothieDesert].nStatus = atoi(sql_row[4]);
		nCntSmoothieDesert++;
	}
	m_App->smoothieDesertProductCount = nCntSmoothieDesert;
	// -----------------------------------------------------------------------------------------------------------------------

	// Loading 에이드 ----------------------------------------------------------------------------------------------------------
	strQuery.Format(L"SELECT pdt_code, pdt_name, classify, price, status FROM product where shop_name = '%s' and Menu='에이드';", m_App->m_ShopName);

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 에이드 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 에이드 정보를 가져오지 못했습니다");
		return;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 에이드 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : 에이드 mysql_store_result");
		return;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		m_App->lAdeJuice[nCntAdeJuice].mCode.Format(L"%s", A2W(sql_row[0]));
		m_App->lAdeJuice[nCntAdeJuice].mProductName.Format(L"%s", A2W(sql_row[1]));
		m_App->lAdeJuice[nCntAdeJuice].mClassify.Format(L"%s", A2W(sql_row[2]));
		m_App->lAdeJuice[nCntAdeJuice].nPrice = atoi(sql_row[3]);
		m_App->lAdeJuice[nCntAdeJuice].nStatus = atoi(sql_row[4]);
		nCntAdeJuice++;
	}
	m_App->adeJuiceProductCount = nCntAdeJuice;
	// -----------------------------------------------------------------------------------------------------------------------

	// Loading 음료차 ----------------------------------------------------------------------------------------------------------
	strQuery.Format(L"SELECT pdt_code, pdt_name, classify, price, status FROM product where shop_name = '%s' and Menu='음료차';", m_App->m_ShopName);

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 음료차 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 음료차 정보를 가져오지 못했습니다");
		return;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 음료차 정보를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : 음료차 mysql_store_result");
		return;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		m_App->lTea[nCntTea].mCode.Format(L"%s", A2W(sql_row[0]));
		m_App->lTea[nCntTea].mProductName.Format(L"%s", A2W(sql_row[1]));
		m_App->lTea[nCntTea].mClassify.Format(L"%s", A2W(sql_row[2]));
		m_App->lTea[nCntTea].nPrice = atoi(sql_row[3]);
		m_App->lTea[nCntTea].nStatus = atoi(sql_row[4]);
		nCntTea++;
	}
	m_App->teaProductCount = nCntTea;
	// -----------------------------------------------------------------------------------------------------------------------
}

Sales_Order mysql_Process::GetSaleDataByProductCode(CString code)
{
	Sales_Order saleDta;
	CString strQuery, strAmount, strPrice;
	int resultMsg;

	strQuery.Format(L"SELECT * from sales where product_code = '%s';", code);

	USES_CONVERSION;
	resultMsg = mysql_query(&conn, W2A(strQuery));
	if (resultMsg == 0)
	{
		if ((sql_result = mysql_store_result(&conn)) != NULL)
		{
			sql_row = mysql_fetch_row(sql_result);

			saleDta.mDate = sql_row[1];			// date
			saleDta.mTime = sql_row[2];			// time
			saleDta.mClassify = sql_row[3];		// classify
			saleDta.mTakeOut = sql_row[16];		// take out
			strAmount = sql_row[6];
			saleDta.nTotalAmount = Str2Int(strAmount);	// amount
			strPrice = sql_row[8];
			saleDta.nTotalPrice = Str2Int(strPrice);	// amount

			saleDta.mTakeOut = sql_row[16];		// take out
			saleDta.mEtc = sql_row[14];			// reserve time
			//saleDta.mPhone = sql_row[];		/ mobile phone
			saleDta.mPay_classify = sql_row[9];	// pay classify
			saleDta.mCardBank = sql_row[10];	// card company
			saleDta.mCardNum = sql_row[11];			// card number
			saleDta.mCardApprovalNum = sql_row[12];	// card approval number
		}
	}

	return saleDta;
}


CString mysql_Process::GetpdtNamebyOrdercode(CString code)
{
	CString strQuery, resDta, strChoice;
	int resultMsg, count = 0;

	USES_CONVERSION;

	strQuery.Format(L"SELECT pdt_name, choice, amount, pay from sale_product where order_code = '%s';", code);
	
	resultMsg = mysql_query(&conn, W2A(strQuery));
	if (resultMsg != 0)
	{
		AfxMessageBox(L"Failed MySQL query.\n");
		return NULL;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Failed mysql_store_result.\n");
		return NULL;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		if (count == 0)
		{
			strChoice.Format(L"%s", A2W(sql_row[1]));
			if (strChoice == L"H")
				resDta.Format(L"(Hot)%s", A2W(sql_row[0]));
			else if (strChoice == L"I")
				resDta.Format(L"(Ice)%s", A2W(sql_row[0]));
		}
		count++;
	}

	if (count > 1)
		resDta.Format(L"%s 외 %d건", resDta, count-1);

	return resDta;
}


BOOL mysql_Process::Insert_OrderProduct(CString dateTime, CString orderCode, CString pdtName, CString classify, int amount, int shotAmount, int price)
{
	CString	strQuery, strQuery1, strQuery2;

	strQuery1 = L"INSERT INTO sale_product(datetime, order_code, pdt_name, choice, amount, shot, pay)";
	strQuery2.Format(L" VALUES('%s','%s','%s','%s',%d,%d,%d);", dateTime, orderCode, pdtName, classify, amount, shotAmount, price);
	strQuery = strQuery1 + strQuery2;

	if (!WriteDBQuery(strQuery)) {
		AfxMessageBox(L"Failed to DB write : order_product");
		return false;
	}

	return true;
}


BOOL mysql_Process::Insert_Sales(Sales_Order saleCash)
{
	CString	strQuery, strQuery1, strQuery2;

	strQuery1 = L"INSERT INTO sales(date, time, classify, shop_name, amount, product_code, total_pay, pay_classify, making, takeout)";
	strQuery2.Format(L" VALUES('%s','%s','%s','%s',%d,'%s',%d,'%s','%s','%s');", 
		saleCash.mDate, saleCash.mTime, saleCash.mClassify, saleCash.mShopName, saleCash.nTotalAmount, saleCash.mOrderCode, 
		saleCash.nTotalPrice, saleCash.mPay_classify, saleCash.mMaking, saleCash.mTakeOut);
	strQuery = strQuery1 + strQuery2;

	if (!WriteDBQuery(strQuery)) {
		AfxMessageBox(L"Failed to DB write : sales");
		return false;
	}

	return true;
}


BOOL mysql_Process::Update_Sales_making(CString mCode, CString making)
{
	CString	strQuery;

	strQuery.Format(L"UPDATE sales SET making = '%s' WHERE product_code = '%s';", making, mCode);

	if (!WriteDBQuery(strQuery)) {
		AfxMessageBox(L"Failed to DB Update : sales");
		return false;
	}

	return true;
}


BOOL mysql_Process::WriteDBQuery(CString strQuery)
{
	char* strquery;
	int resultMsg;

	USES_CONVERSION;
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Failed MySQL query.\n");
		return false;
	}

	sql_result = mysql_store_result(&conn);
	mysql_free_result(sql_result);

	return true;
}