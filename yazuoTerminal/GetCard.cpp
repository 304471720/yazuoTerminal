// GetCard.cpp : 实现文件
//

#include "stdafx.h"
#include "yazuoTerminal.h"
#include "GetCard.h"
#include "afxdialogex.h"
#include <openssl/md5.h> 
#include "CHttpClient.h"
#include "json/json.h"
#include  <vector> 
#include <iostream>

#include <tchar.h>
#include <windows.h>
#include <wchar.h>
#include <string>

#define SIGN "SD6nOWzK"

#define MAX_PATHLENGTH 10000

using namespace std;

CWinApp myapp;

// CGetCard 对话框

IMPLEMENT_DYNAMIC(CGetCard, CDialogEx)

CGetCard::CGetCard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetCard::IDD, pParent)
	, m_No(_T(""))
{

}

CGetCard::~CGetCard()
{
}

void CGetCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_No);
}


BEGIN_MESSAGE_MAP(CGetCard, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGetCard::OnBnClickedOk)
END_MESSAGE_MAP()


// CGetCard 消息处理程序

string Md5(string s)  
{  
    MD5_CTX ctx;
	string ret;
    char *data=(char *)s.c_str();
    unsigned char md[16];
    char buf[33]={'\0'};
    char tmp[3]={'\0'};
    int i;
    MD5_Init(&ctx);
    MD5_Update(&ctx,data,strlen(data));
    MD5_Final(md,&ctx);
    for( i=0; i<16; i++ ){
        sprintf(tmp,"%02X",md[i]);
        strcat(buf,tmp);
    }
    return buf;
}  

string MD5_Digest(const string cleartext)  
{  
    string strDigest;  
    unsigned char tmp[16] = {0};  
  
#if 0  
    MD5((const unsigned char*)cleartext.c_str(), cleartext.length(), tmp);  
#else  
    MD5_CTX c;  
    MD5_Init(&c);  
    MD5_Update(&c, cleartext.c_str(), cleartext.length());  
    MD5_Final(tmp, &c);  
#endif  
  
    char* tmp1 = new char[32 + 1];  
    memset(tmp1, 0, 32 + 1);  
  
    for(int i = 0; i < 16; i++)   
        sprintf(&(tmp1[i*2]), "%02X", tmp[i]);  
        //cout<<hex<<setw(2)<<setfill('0')<<(int)tmp[i];   
  
    strDigest = (char*)tmp1;  
  
    delete [] tmp1;  
  
    return strDigest;  
}  


string UTF8ToGBK(const std::string& strUTF8)  
{  
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);  
    WCHAR* wszGBK = new WCHAR[len+1];
    memset(wszGBK, 0, len * 2 + 2);  
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, wszGBK, len);  
  
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
    char *szGBK = new char[len + 1];  
    memset(szGBK, 0, len + 1);  
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);   
    std::string strTemp(szGBK);  
    delete[]szGBK;  
    delete[]wszGBK;  
    return strTemp;  
}


string GetParamStr(vector<pair<string,string>> my_Map)
{
	 string ret ;
	 vector<pair<string,string>> ::iterator my_Itr = my_Map.begin(); 
	 int mapsize = my_Map.size();
	 int index=0;
	 for(my_Itr=my_Map.begin();my_Itr!=my_Map.end();++my_Itr)   
	 {
		 index++;
		 if(index == mapsize)
		 {
			ret += my_Itr->first+"="+my_Itr->second;
		 }else
		 {
			ret += my_Itr->first+"="+my_Itr->second+"&";
		 }
	 }
	 return ret;
}




string Sign(string sign,vector<pair<string,string>> my_Map,vector<pair<string,string>> signMap)
{
	 string allParam = GetParamStr(my_Map);
	 string signStr = GetParamStr(signMap);
	 
	 string preMd5 =  signStr +"&sign="+sign;
	 string md5 = MD5_Digest(preMd5);
	 string ret = allParam +"&sign="+md5;
	 return ret;
}

string getUrl(string pathname)
{
	char host[MAX_PATH] ={0};
	char des[MAX_PATH] ={0};
	char path[MAX_PATH] ={0};
	//USES_CONVERSION;
	GetCurrentDirectory(MAX_PATHLENGTH,des);
	strcat(des,"\\config.ini");
	GetPrivateProfileString(_T("baseurl"),_T("host"),_T(""),host,50,des);
	GetPrivateProfileString(_T("action"),pathname.c_str(),_T(""),path,50,des);
	char url[MAX_PATH] ={0};
	strcat(url,host);
	strcat(url,path);
	strcat(url,"?");
	return url;
}


extern "C" __declspec(dllexport) int GetCard(
		char *Phone,                
		char *pszTrack2,           
		char *pszRetcode,            
		char *pszRestInfo             
		)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CGetCard dlg;
	 CString m_str1;
	 if (IDOK == dlg.DoModal() )
	 {
		m_str1 = dlg.m_No;
	 }
	 //string ret = Md5("vid=2317724798961427&sign=SD6nOWzK");
	 //AfxMessageBox(ret.c_str());
	 StrCpyA(Phone,m_str1);
	 StrCpyA(pszTrack2,m_str1);
   return 0;
}

extern "C" __declspec(dllexport) int VoidSale(
    char *pszRetcode,          
    char *pszRestInfo,         
    char *pszRefnum,           
    char *pszTrace,            
    char *pszPan,              
    char *pszSerial,           
    char *pszTrack2,    
    char *pszCreditsPoint,     
    char *psStoredCardsBalance,
    char *pszTicketList,       
    char *psIntegralOverall,   
    char *pszStore,            
    char *com)
{
	string host = getUrl(_T("VoidSale"));
	vector<pair<string,string>> mymap;
	if(pszSerial!=NULL  &&  strlen(pszSerial) >0)
	{
		mymap.push_back(make_pair("pszSerial",pszSerial));
	}

	if(pszTrack2!=NULL   &&  strlen(pszTrack2)>0)
	{
		mymap.push_back(make_pair("pszTrack2",pszTrack2));
	}

	if(pszTrace!=NULL   &&  strlen(pszTrace)>0)
	{
		mymap.push_back(make_pair("pszTrace",pszTrace));
	}

	string param = Sign(SIGN,mymap,mymap);
	string url =host+param ;
	string retStr;
	CHttpClient tt ;
	tt.Get(url,retStr);
	retStr = UTF8ToGBK(retStr);
	Json::Reader reader;
    Json::Value value;
	if (!reader.parse(retStr, value))  
    {
		strncpy(pszRestInfo,param.c_str(),50); 
		strcpy(pszStore,"222");
		return 2;
    }
	string json_success;
	string json_pszTrace;
	string json_pszPan;
	string json_psStoredCardsBalance;
	string json_psIntegralOverall;
	string json_pszCreditsPoint;
	string json_pszTicketList;
	string json_pszStore;
	if( !value["success"].isNull())
	{
		json_success = value["success"].asCString();  
	}
	if( !value["pszTrace"].isNull())
	{
		json_pszTrace = value["pszTrace"].asCString();  
	}
	if( !value["pszPan"].isNull())
	{
		json_pszPan = value["pszPan"].asCString();  
	}
	if( !value["psStoredCardsBalance"].isNull())
	{
		json_psStoredCardsBalance = value["psStoredCardsBalance"].asCString();  
	}
	if( !value["psIntegralOverall"].isNull())
	{
		json_psIntegralOverall = value["psIntegralOverall"].asCString();  
	}
	if( !value["pszCreditsPoint"].isNull())
	{
		json_pszCreditsPoint = value["pszCreditsPoint"].asCString();  
	}
	if( !value["pszTicketList"].isNull())
	{
		json_pszTicketList = value["pszTicketList"].asCString();  
	}
	if( !value["pszStore"].isNull())
	{
		json_pszStore = value["pszStore"].asCString();  
	}
	if(json_success == "0")
	{
		strncpy(pszTrace,json_pszTrace.c_str(),50);
		strncpy(pszPan,json_pszPan.c_str(),19);
		strncpy(psStoredCardsBalance,json_psStoredCardsBalance.c_str(),12);
		strncpy(psIntegralOverall,json_psIntegralOverall.c_str(),12);
		strncpy(pszCreditsPoint,json_pszCreditsPoint.c_str(),12);
		strncpy(pszTicketList,json_pszTicketList.c_str(),strlen(json_pszTicketList.c_str()));	
		strncpy(pszStore,json_pszStore.c_str(),12);
		return 0;
	}else
	{
		return 2;
	}
   return 0;
}

extern "C" __declspec(dllexport) int Sale(
	char *pszRetcode,                   
    char *pszRestInfo,              
    char *pszTrace,                 
    char *pszPan,                   
    char *pszCreditsPoint,          
    char *pszGiftsTicket,           
    char *pszSerial,                
    char *pszTrack2,                
	char *pszStore,
    char *pszCash,                  
    char *pszPoint,                 
    char *pszTicketList,            
	char *psDishesList,
    char *psStoredCardsBalance,     
    char *psIntegralOverall,        
    char *realConsumeAmount,        
    char *com)
{
	AfxMessageBox(" Sale ");
	/*string host = getUrl(_T("Sale"));
	vector<pair<string,string>> mymap;
	vector<pair<string,string>> mySign;

	if(pszSerial != NULL &&  strlen(pszSerial) >0)
	{
		mymap.push_back(make_pair("pszSerial",pszSerial));
		mySign.push_back(make_pair("pszSerial",pszSerial));
	}
	if(pszTrack2 != NULL &&  strlen(pszTrack2) >0)
	{
		mymap.push_back(make_pair("pszTrack2",pszTrack2));
		mySign.push_back(make_pair("pszTrack2",pszTrack2));
	}
	if(pszStore != NULL &&  strlen(pszStore) >0)
	{
		mymap.push_back(make_pair("pszStore",pszStore));
		mySign.push_back(make_pair("pszStore",pszStore));
	}	
	if(pszCash != NULL &&  strlen(pszCash) >0)
	{
		mymap.push_back(make_pair("pszCash",pszCash));
	}
	if(pszPoint != NULL &&  strlen(pszPoint) >0)
	{
		mymap.push_back(make_pair("pszPoint",pszPoint));
	}
	if(pszTicketList != NULL &&  strlen(pszTicketList) >0)
	{
		mymap.push_back(make_pair("pszTicketList",pszTicketList));
	}
	if(psDishesList != NULL &&  strlen(psDishesList) >0)
	{
		mymap.push_back(make_pair("psDishesList",psDishesList));
	}
	if(realConsumeAmount != NULL &&  strlen(realConsumeAmount) >0)
	{
		mymap.push_back(make_pair("realConsumeAmount",realConsumeAmount));
		mySign.push_back(make_pair("realConsumeAmount",realConsumeAmount));
	}
	string param = Sign(SIGN,mymap,mySign);
	string url = host + param;
	AfxMessageBox(url.c_str());
	//url="http://news.xinhuanet.com/local/2017-04/20/c_129555308.htm";
	string retStr;	
	CHttpClient tt ;
	tt.Get(url,retStr);
	retStr = UTF8ToGBK(retStr);
	AfxMessageBox(retStr.c_str());
	//printf("%s",newstr);
	//getchar();
	Json::Reader reader;
    Json::Value value;
	//string test = "{\"success\":\"2\",\"msg\":\"xxx\"}";
	//bool a = reader.parse(test, value);
	//test = "{'success':'2','msg':'test'}";
	//a = reader.parse(test, value);
	if (!reader.parse(retStr, value))  
    {
		strncpy(pszRestInfo,param.c_str(),50); 
		strcpy(pszGiftsTicket,"100.1"); 
		strcpy(psStoredCardsBalance,"1000");
		strcpy(psIntegralOverall,"2000.10");
		strcpy(pszPan,"11111122223");
		strcpy(pszTrace,"987654321100");
        return 2;
    }
	string json_success;
	string json_pszRestInfo;
	string json_pszTrace;
	string json_pszPan;
	string json_pszCreditsPoint;
	string json_pszGiftsTicket;
	string json_psStoredCardsBalance;
	string json_psIntegralOverall;

	if( !value["success"].isNull())
	{
		json_success = value["success"].asCString();  
	}
	if( !value["pszTrace"].isNull())
	{
		json_pszTrace = value["pszTrace"].asCString();  
	}
	if( !value["pszPan"].isNull())
	{
		json_pszPan = value["pszPan"].asCString();  
	}
	if( !value["pszCreditsPoint"].isNull())
	{
		json_pszCreditsPoint = value["pszCreditsPoint"].asCString();  
	}
	if( !value["pszGiftsTicket"].isNull())
	{
		json_pszGiftsTicket = value["pszGiftsTicket"].asCString();  
	}
	if( !value["psStoredCardsBalance"].isNull())
	{
		json_psStoredCardsBalance = value["psStoredCardsBalance"].asCString();  
	}
	if( !value["psIntegralOverall"].isNull())
	{
		json_psIntegralOverall = value["psIntegralOverall"].asCString();  
	}
	if( !value["msg"].isNull())
	{
		json_pszRestInfo = value["msg"].asCString();  
	}
	if(json_success == "0")
	{
		strcpy(pszTrace,json_pszTrace.c_str());
		strcpy(pszPan,json_pszPan.c_str());
		strcpy(pszCreditsPoint,json_pszCreditsPoint.c_str());
		strcpy(pszGiftsTicket,json_pszGiftsTicket.c_str());
		strcpy(psStoredCardsBalance,json_psStoredCardsBalance.c_str());
		strcpy(psIntegralOverall,json_psIntegralOverall.c_str());
		strncpy(pszRestInfo,json_pszRestInfo.c_str(),50); 
		return 0;
	}else
	{
		return 2;
	}*/
	return 0;	
}

extern "C" __declspec(dllexport) int Login (char *UserID, char *UserPassword,char * com)
{
	return 0;
}
extern "C" __declspec(dllexport) int Logout (char * com)
{
	return 0;
}
extern "C" __declspec(dllexport) int QueryBalance(
	char *pszRetcode,          
    char *pszRestInfo,         
    char *pszPan,              
    char *psStoredCardsBalance,
    char *psIntegralOverall,   
	char *psIntegralAvail,     
	char *psTicketInfo,
	char *pszTrack2){return 0;}

extern "C" __declspec(dllexport) int PredictStore (
    char *pszTrack2,                
	char *pszStore,
    char *psStoreReal,        
    char *psStoreGive
	){return 0;}

extern "C" __declspec(dllexport) int QueryBalanceEx(
	char *pszRetcode,          
    char *pszRestInfo,         
    char *pszPan,              
    char *psStoredCardsBalance,
    char *psIntegralOverall,   
	char *psIntegralAvail,     
	char *psTicketInfo,
	char *pszCardType,
	char *pszMobile, 
	char *pszName,
	char *pszGender,	
	char *pszBirthday,	
    char *pszTrack2
)
{
	pszTrack2 = "1";
	string host = getUrl(_T("QueryBalanceEx"));
	vector<pair<string,string>> mymap;
	if( pszTrack2!= NULL && strlen(pszTrack2)> 0 )
	{
		mymap.push_back(make_pair("vid",pszTrack2));
	}
	string param = Sign(SIGN,mymap,mymap);
	string url = host+param;
	//url="http://localhost:8081/listUsers";
	string retStr;
	string ret = Md5(url);
	//AfxMessageBox(url.c_str());
	//AfxMessageBox(ret.c_str());
	CHttpClient tt ;
	AfxMessageBox(url.c_str());
	tt.Get(url,retStr);
	AfxMessageBox(retStr.c_str());
	retStr = UTF8ToGBK(retStr);
	Json::Reader reader;
    Json::Value value;
	if (!reader.parse(retStr, value))  
    {
		strncpy(pszRestInfo,param.c_str(),50); 
		strcpy(pszMobile,"15810220593"); 
		strcpy(pszBirthday,"20170101");
		strcpy(pszCardType,"这是例子");
		strcpy(pszName,"某某某");
        return 2;
    }
	string json_success;
	string json_pszPan;
	string json_psStoredCardsBalance;
	string json_psIntegralOverall;
	string json_psIntegralAvail;
	string json_pszRestInfo;

	if( !value["success"].isNull())
	{
		json_success = value["success"].asCString();  
	}
	if( !value["pszPan"].isNull())
	{
		json_pszPan = value["pszPan"].asCString();  
	}
	if( !value["psStoredCardsBalance"].isNull())
	{
		json_psStoredCardsBalance = value["psStoredCardsBalance"].asCString();  
	}
	if( !value["psIntegralOverall"].isNull())
	{
		json_psIntegralOverall = value["psIntegralOverall"].asCString();  
	}
	if( !value["psIntegralAvail"].isNull())
	{
		json_psIntegralAvail = value["psIntegralAvail"].asCString();  
	}
	if( !value["msg"].isNull())
	{
		json_pszRestInfo = value["msg"].asCString();  
	}
	
	if(json_success == "0")
	{
		string json_psIntegralOverall = "0";
		string json_psIntegralAvail="0";
		string json_psStoredCardsBalance="1111111111222";
		string json_pszPan = "1111111111222222222";
		StrCpy(pszRestInfo,"111111111122222222223333333333444444444455555555556");
		StrNCpy(pszPan,json_pszPan.c_str(),19);
		StrCpy(psStoredCardsBalance,json_psStoredCardsBalance.c_str());
		//StrCpy(psIntegralOverall,json_psIntegralOverall.c_str());
		
		StrCpy(psIntegralAvail,json_psIntegralAvail.c_str());

		StrCpy(pszCardType,"20000");
		StrCpy(pszMobile,"18800977672");
		StrCpy(pszName,"dddd");
		StrCpy(pszGender,"01");
		StrNCpy(pszBirthday,"1989-01-01",11);
		//StrNCpy(pszRestInfo,json_pszRestInfo.c_str(),50);
		return 0;
	/*}else
	{
		return 2;
	}*/
	return 0;
}

void CGetCard::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnOK();
}

int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow)   
{   
    //AfxMessageBox("AfxWinMain");
	char *pszRetcode   = new char[10000];       
    char *pszRestInfo   = new char[10000];             
    char *pszPan   = new char[10000];                    
    char *psStoredCardsBalance   = new char[10000];       
    char *psIntegralOverall   = new char[10000];          
	char *psIntegralAvail   = new char[10000];            
	char *psTicketInfo   = new char[10000];       
	char *pszCardType   = new char[10000];       
	char *pszMobile   = new char[10000];        
	char *pszName   = new char[10000];       
	char *pszGender   = new char[10000];       	
	char *pszBirthday   = new char[10000];       	
    char *pszTrack2   = "2317724798961427";
	QueryBalanceEx(pszRetcode,pszRestInfo,pszPan,psStoredCardsBalance,psIntegralOverall,psIntegralAvail,psTicketInfo,pszCardType,pszMobile,pszName,pszGender,pszBirthday,"15810220593");
	return 0;
}