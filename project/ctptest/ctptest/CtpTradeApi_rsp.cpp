
#include "CtpTradeApi.h"

#include "ThostFtdcUserApiStruct.h"


///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CCtpTradeApi::OnFrontConnected()
{
	printf("%s",__FUNCTION__);
	
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CCtpTradeApi::OnFrontDisconnected(int nReason)
{
	printf("%s nReason[%d]",__FUNCTION__ ,nReason);
}
///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CCtpTradeApi::OnHeartBeatWarning(int nTimeLapse)
{
	printf("%s nTimeLapse[%d]",__FUNCTION__,nTimeLapse);
}

//��¼������Ӧ
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


//�ǳ�������Ӧ
void CCtpTradeApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//��ѯ��󱨵�������Ӧ
void CCtpTradeApi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//Ͷ���߽�����ȷ����Ӧ

//�����ѯ������Ӧ
void CCtpTradeApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

//�����ѯ�ɽ���Ӧ
void CCtpTradeApi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯ�ʽ��˻���Ӧ
void CCtpTradeApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pAccount, 
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

//�����ѯ��Լ��֤������Ӧ
void CCtpTradeApi:: OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯ��Լ����������Ӧ
void CCtpTradeApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯ��������Ӧ
void CCtpTradeApi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯ��Ʒ��Ӧ
void CCtpTradeApi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯ��Լ��Ӧ
void CCtpTradeApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯͶ���ֲ߳���ϸ��Ӧ
void CCtpTradeApi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//�����ѯ������Ӧ
void CCtpTradeApi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){}

//��Լ����״̬֪ͨ
void CCtpTradeApi::OnRtnInstrumentStatus ( CThostFtdcInstrumentStatusField *pInstrumentStatus )
{
	
}

//����֪ͨ
void CCtpTradeApi::OnRtnTradingNotice ( CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo )
{
}

//����¼�����ر�
void CCtpTradeApi::OnErrRtnOrderInsert ( CThostFtdcInputOrderField *pInputOrder , CThostFtdcRspInfoField *pRspInfo )
{
	
}

//������������ر�
void CCtpTradeApi::OnErrRtnOrderAction ( CThostFtdcOrderActionField *pOrderAction , CThostFtdcRspInfoField *pRspInfo )
{
	
}

//��������������Ӧ
void CCtpTradeApi::OnRspOrderAction ( CThostFtdcInputOrderActionField *pInputOrderAction , CThostFtdcRspInfoField *pRspInfo , int nRequestID , bool bIsLast )
{
	
}

//����¼��������Ӧ
void CCtpTradeApi::OnRspOrderInsert ( CThostFtdcInputOrderField *pInputOrder , CThostFtdcRspInfoField *pRspInfo , int nRequestID , bool bIsLast )
{
	
}



//����֪ͨ
void CCtpTradeApi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

	

}

//�ɽ�֪ͨ
void CCtpTradeApi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	
}

void CCtpTradeApi::OnRspQryInvestorPosition (CThostFtdcInvestorPositionField *p ,
	CThostFtdcRspInfoField *pRspInfo , int nRequestID , bool bIsLast )
{
	
}


