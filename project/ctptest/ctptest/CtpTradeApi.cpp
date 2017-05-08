#include "CtpTradeApi.h"



CCtpTradeApi::CCtpTradeApi(const char* pAddr):
m_pApi(NULL)
{
	m_nReqId = 0;
	m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi("");
	if(m_pApi)
	{
		m_pApi->RegisterSpi(this);
		m_pApi->SubscribePrivateTopic(THOST_TERT_QUICK);
		m_pApi->SubscribePublicTopic(THOST_TERT_QUICK);
		m_pApi->RegisterFront((char*)pAddr);
		m_pApi->Init();
		printf("aaaaaaaaaaa[%s]\n",pAddr);
	}
}

CCtpTradeApi::~CCtpTradeApi ( )
{

}

int CCtpTradeApi::ReqLogin ( const char* pUserId , const char* pwd ,const char* pBrokerId)
{
	int nRet (-1);
	if(m_pApi)
	{
		m_strUserId = pUserId;
		m_strPswd = pwd;
		m_strBrokerId = pBrokerId;
		CThostFtdcReqUserLoginField lo={0};
		strcpy( lo.UserID , pUserId );
		strcpy( lo.Password , pwd );

		if (pBrokerId)
		{	
			strcpy(lo.BrokerID, pBrokerId);
		}
		nRet = m_pApi->ReqUserLogin(&lo,++m_nReqId);
	}
	return nRet;
}


int CCtpTradeApi::ReqQryAccount()
{ 
	int nRet(-1);
	if (m_pApi)
	{
		CThostFtdcQryTradingAccountField qry = { 0 };

		nRet = m_pApi->ReqQryTradingAccount(&qry, ++m_nReqId);
	}
	return nRet;
}


void CCtpTradeApi::Release ( )
{
	if(m_pApi)
	{
		m_pApi->Release();
		m_pApi->Join();
		m_pApi = NULL;
	}
}
