#ifndef CARDREADERDATA_H
#define CARDREADERDATA_H
#if MT_CardVersion == MT622V204
#define S1		           					0x30					//Ѱ������ֵ���п�
#define S2		           					0x30					//Ѱ������ֵ��ϵͳ��

enum
{
	NoCardExiting = 0x00,
	CardExiting = 0x01
};

//������ 
typedef struct __MT_CardData__
{
	uint8 byCardNo[8];//����
	uint16 iCardState;//��״̬
	uint8 byBalance[4];//�������
	uint8 ucCardType;//������
	uint32 iDealCounts;	//�������
	uint8 byPseudoRandomNum[4];//α�����
	uint8 byOnlineSerial[2];//�����������
	uint8 byLeftMoney[4];//����Ǯ�����
	uint8 byDealMoney[2];//���ν������
	uint8 ucType;//�������ͱ�־
	uint8 byEterm[6];//׮���
	Common_Time st_CommTime;//ʱ������
	uint8 bytserial[2];//�ѻ��������
} MT_CardData;

//����������
typedef struct __MT_Data__
{
    uint16 iRcvResult;//����������Ľ��
    MT_CardData st_CardData;//������
    uint8 ucReState;//״̬��
    uint8 ucPiceChangeTime[5];//���÷��ʵ�۵����л�ʱ��
    uint8 ucPresentDealPice1[3];//��ǰ���׷���1��������
    uint8 ucPresentDealPice2[3];//��ǰ���׷���2��������
    uint8 ucPresentDealPice3[3];//��ǰ���׷���3��������
    uint8 ucPresentDealPice4[3];//��ǰ���׷���4��������
    uint8 ucPresentDealPice5[3];//��ǰ���׷���5��������
    uint8 ucPresentDealPice6[3];//��ǰ���׷���6��������
    uint8 ucPresentDealPice7[3];//��ǰ���׷���7��������
    uint8 ucPresentDealPice8[3];//��ǰ���׷���8��������
    uint8 ucReserveDealPice1[3];//���ý��׷���1��������
    uint8 ucReserveDealPice2[3];//���ý��׷���2��������
    uint8 ucReserveDealPice3[3];//���ý��׷���3��������
    uint8 ucReserveDealPice4[3];//���ý��׷���4��������
    uint8 ucReserveDealPice5[3];//���ý��׷���5��������
    uint8 ucReserveDealPice6[3];//���ý��׷���6��������
    uint8 ucReserveDealPice7[3];//���ý��׷���7��������
    uint8 ucReserveDealPice8[3];//���ý��׷���8��������
    uint8  ucPraseErr;//����ʧ��
} MT_Data;

//���ö���������
typedef struct __MT_GData__
{
	uint32 iConsume;//���ѽ��
	Common_Time st_CommTime;//ʱ������
	uint32 iElectric ;//�ĵ���
} MT_GData;
bool MT_FindCardGetData(void * pData,uint8 dev);
bool MT_FindCardSetData(void *pData,void * temp);
bool MT_PreStartGetData(void *pData,uint8 dev);
bool MT_PreSetData(void *pData,void * temp);
bool MT_StartGetData(void * pData,uint8 dev);
bool MT_EndGetData(void *pData,uint8 dev);
bool MT_StartEndChargeSetData(void *pData,void * temp);

/**********************************************************************************/
void MT_SetCardExitingFlag(uint8 flag);
uint8 MT_GetCardExitingFlag(void);
bool MT_GetRecResult(void);
bool MT_ClearRecResult(void);
bool MT_GetCardLock(void);
bool MT_ReadCardRemain(uint32 *remain);
bool MT_SetCardConsume(uint32 consume);
#endif

#endif

