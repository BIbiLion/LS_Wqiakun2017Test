
#include "CtpTradeApi.h"

#include "ThostFtdcUserApiStruct.h"


///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CCtpTradeApi::OnFrontConnected()
{
	printf("%s",__FUNCTION__);
	
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CCtpTradeApi::OnFrontDisconnected(int nReason)
{
	printf("%s nReason[%d]",__FUNCTION__ ,nReason);
}
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CCtpTradeApi::OnHeartBeatWarning(int nTimeLapse)
{
	printf("%s nTimeLapse[%d]",__FUNCTION__,nTimeLapse);
}

//登录请求响应
void CCtpTradeApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( NULL != pRspInfo )
	{
		if ( 0 == pRspInfo->ErrorID && pRspUserLogin )
		{
			printf( "%s success.  errid[%d] msg[%s]\n" ,__FUNCTION__,pRspInfo->ErrorID,pRspInfo->ErrorMsg);
           
			

			if (m_pApi)
			{
				CThostFtdcSettlementInfoConfirmField confirmSettle = { 0 };
				strcpy(confirmSettle.InvestorID, m_strUserId.c_str());
				strcpy(confirmSettle.BrokerID, m_strBrokerId.c_str());

               
				m_pApi->ReqSettlementInfoConfirm(&confirmSettle,++m_nReqId);
			}
		}
	}
}

void CCtpTradeApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pSettlementInfoConfirm)
	{
		
		printf("%s\n",__FUNCTION__);
	}
}


//登出请求响应
void CCtpTradeApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//查询最大报单数量响应
void CCtpTradeApi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//投资者结算结果确认响应

//请求查询报单响应
void CCtpTradeApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

//请求查询成交响应
void CCtpTradeApi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询资金账户响应
void CCtpTradeApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pAccount, 
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

//请求查询合约保证金率响应
void CCtpTradeApi:: OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询合约手续费率响应
void CCtpTradeApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询交易所响应
void CCtpTradeApi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询产品响应
void CCtpTradeApi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询合约响应
void CCtpTradeApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询投资者持仓明细响应
void CCtpTradeApi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//请求查询汇率响应
void CCtpTradeApi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//合约交易状态通知
void CCtpTradeApi::OnRtnInstrumentStatus ( CThostFtdcInstrumentStatusField *pInstrumentStatus )
{
	
}

//交易通知
void CCtpTradeApi::OnRtnTradingNotice ( CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo )
{
}

//报单录入错误回报
void CCtpTradeApi::OnErrRtnOrderInsert ( CThostFtdcInputOrderField *pInputOrder , CThostFtdcRspInfoField *pRspInfo )
{
	
}

//报单操作错误回报
void CCtpTradeApi::OnErrRtnOrderAction ( CThostFtdcOrderActionField *pOrderAction , CThostFtdcRspInfoField *pRspInfo )
{
	
}

//报单操作请求响应
void CCtpTradeApi::OnRspOrderAction ( CThostFtdcInputOrderActionField *pInputOrderAction , CThostFtdcRspInfoField *pRspInfo , int nRequestID , bool bIsLast )
{
	
}

//报单录入请求响应
void CCtpTradeApi::OnRspOrderInsert ( CThostFtdcInputOrderField *pInputOrder , CThostFtdcRspInfoField *pRspInfo , int nRequestID , bool bIsLast )
{
	
}



//报单通知
void CCtpTradeApi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

	

}

//成交通知
void CCtpTradeApi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	
}

void CCtpTradeApi::OnRspQryInvestorPosition (CThostFtdcInvestorPositionField *p ,
	CThostFtdcRspInfoField *pRspInfo , int nRequestID , bool bIsLast )
{
	
}


