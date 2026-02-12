#pragma once

#define REG_ROOT	1
#define REG_USER	2
#define REG_MACHINE	3

//#define HOST			"183.111.148.116"	// 개발서버 IP
//#define DBUser			"root"
//#define DBPass			"uBpass4862"
//#define DBName			"bizstory"

//#define DBPORT			3306
//#define DBPORT			4306

//#define HOST			"218.145.31.93"		// 운영 서버
//#define HOST			"112.219.127.2"		
#define DBUser			"bizstory"
#define DBPass			"uBpass4862"
#define DBName			"test_bizstory"

#define IDC_txtManagerPlace 9000
#define IDC_txtCompanyName	9001
#define IDC_txtButton1		9002
#define IDC_txtButton2		9003
#define IDC_txtButton3		9004
#define IDC_txtButton4		9005
#define IDC_noticeType		9006
#define IDC_noticeContent	9007
#define IDC_receiptSubject1 9008
#define IDC_receiptSubject2 9009
#define IDC_receiptSubject3 9010
#define IDC_receiptSubject4 9011
#define IDC_btnReception	9012
#define IDC_ASType1			9013
#define IDC_ASType2			9014
#define IDC_ASType3			9015
#define IDC_ASType4			9016
#define IDC_btnExit			9017
#define IDC_receiptWriter1	9018
#define IDC_receiptWriter2	9019
#define IDC_receiptWriter3	9020
#define IDC_receiptWriter4	9021
#define IDC_receiptDate1	9022
#define IDC_receiptDate2	9023
#define IDC_receiptDate3	9024
#define IDC_receiptDate4	9025

#define tBannerSec			9100
#define tNoticeSec			9101


#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#define SAFE_DELETE(x)  { if(x) delete (x); x = NULL;}
#define SAFE_DELETE_ARRAY(x)  {if(x) delete[] (x); x = NULL;}