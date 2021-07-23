
#ifndef __MT318_H__
#define __MT318_H__
#include "type.h"

#define    P_MT318_FrameStart				0x02//֡ͷ                            
#define    P_MT318_FrameEnd		    		0x03//֡β                           

//������+�������
#define 	P_MT318_FindCard				0x5137//Ѱ��                            
#define 	P_MT318_Pretereatment		0x5136//Ԥ����                          
#define 	P_MT318_StartCharge		    	0x5130//��ʼ�ӵ�                       
#define 	P_MT318_EndCharge		    		0x5134//�����ӵ�                       
#define	P_MT318_RePlay			    		0x5135//���佻��                       
#define 	P_MT318_CalcMac2				0x5133//����MAC2                        
#define 	P_MT318_Random4			    	0x5132//ȡ4�ֽ������                   
#define 	P_MT318_CheckBC			    	0x5131//��֤��վ�㲥                  
#define 	P_MT318_Random8			    	0x5138//ȡ8�ֽ������                  
#define 	P_MT318_MasterCS				0x5139//��վ��֤���洢����              
#define 	P_MT318_ReadESAMPara		0x513A//��ȡESAM�д洢�ĵ�۵Ȳ���      
#define 	P_MT318_ModifyESAMKey		0x5151//ESAM��Կ�޸�                    
#define 	P_MT318_FirstSet					0x5152//��������                       
#define 	P_MT318_Baud4800				0x3632//������ѡ��4800bps               
#define 	P_MT318_Baud9600				0x3633//������ѡ��9600bps              
#define 	P_MT318_Baud19200				0x3634//������ѡ��19200bps              
#define 	P_MT318_Baud38400				0x3635//������ѡ��38400bps              

//����״̬��
#define 	P_MT318_Seccess			    		0x59//�ɹ�                            
#define 	P_MT318_Faild1					0x4E//�ӵ硢�����ӵ硢Ԥ����ʧ��      
#define 	P_MT318_Faild2			    		0x47//PSAM��λʧ��                   
#define 	P_MT318_Faild3			    		0x48//�û�����λʧ��                 
#define 	P_MT318_Busy						0x44//�����û����ڳ��               
#define 	P_MT318_Faild4			   	 	0x43//MAC3У��ʧ�ܻ�δУ��            

//MT318Э��֡�ṹ
typedef  struct __MT318_FramePara__
{
	uint8 ucSTX;//֡ͷ
	uint16 ucDataLen;//�����򳤶�
	uint16 ucCmdCode;//������+�����ֲ���
	uint8 ucState;//״̬��
	uint8 ucDataSpace[100];//������
} MT318_FramePara;

bool MT_PackageFrame(void  *pPara,  void  *pBuf1, uint16  *ucDatalen);
bool MT_ParseFrame(void  *pPara,  void *pBuf, uint16  ucDatalen);
uint8 Check(uint8 *data, uint16 dataLen);
#endif

