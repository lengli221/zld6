
#ifndef __MT318_H__
#define __MT318_H__
#include "type.h"

#define    P_MT318_FrameStart				0x02//帧头                            
#define    P_MT318_FrameEnd		    		0x03//帧尾                           

//命令字+命令参数
#define 	P_MT318_FindCard				0x5137//寻卡                            
#define 	P_MT318_Pretereatment		0x5136//预处理                          
#define 	P_MT318_StartCharge		    	0x5130//开始加电                       
#define 	P_MT318_EndCharge		    		0x5134//结束加电                       
#define	P_MT318_RePlay			    		0x5135//补充交易                       
#define 	P_MT318_CalcMac2				0x5133//计算MAC2                        
#define 	P_MT318_Random4			    	0x5132//取4字节随机数                   
#define 	P_MT318_CheckBC			    	0x5131//验证主站广播                  
#define 	P_MT318_Random8			    	0x5138//取8字节随机数                  
#define 	P_MT318_MasterCS				0x5139//主站验证并存储数据              
#define 	P_MT318_ReadESAMPara		0x513A//读取ESAM中存储的电价等参数      
#define 	P_MT318_ModifyESAMKey		0x5151//ESAM秘钥修改                    
#define 	P_MT318_FirstSet					0x5152//出厂设置                       
#define 	P_MT318_Baud4800				0x3632//波特率选择4800bps               
#define 	P_MT318_Baud9600				0x3633//波特率选择9600bps              
#define 	P_MT318_Baud19200				0x3634//波特率选择19200bps              
#define 	P_MT318_Baud38400				0x3635//波特率选择38400bps              

//返回状态字
#define 	P_MT318_Seccess			    		0x59//成功                            
#define 	P_MT318_Faild1					0x4E//加电、结束加电、预处理失败      
#define 	P_MT318_Faild2			    		0x47//PSAM复位失败                   
#define 	P_MT318_Faild3			    		0x48//用户卡复位失败                 
#define 	P_MT318_Busy						0x44//其他用户正在充电               
#define 	P_MT318_Faild4			   	 	0x43//MAC3校验失败或未校验            

//MT318协议帧结构
typedef  struct __MT318_FramePara__
{
	uint8 ucSTX;//帧头
	uint16 ucDataLen;//数据域长度
	uint16 ucCmdCode;//命令字+命令字参数
	uint8 ucState;//状态字
	uint8 ucDataSpace[100];//数据域
} MT318_FramePara;

bool MT_PackageFrame(void  *pPara,  void  *pBuf1, uint16  *ucDatalen);
bool MT_ParseFrame(void  *pPara,  void *pBuf, uint16  ucDatalen);
uint8 Check(uint8 *data, uint16 dataLen);
#endif

