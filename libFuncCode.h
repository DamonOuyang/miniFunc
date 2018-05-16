// 
// libFuncCode.h
// version 1.0
// Created by Jason Tang on 28/03/2017
//
// version 3.9
// Created by Jason Tang on 14/11/2017
//
// version 4.4
// Created by Jason Tang on 02/03/2018
//
//	Copy right @ 2017 Jetinno. All rights reserved.
//

#ifndef _FUNCTIONCODE_H_
#define _FUNCTIONCODE_H_

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned char BYTE;

#define MAX_FRAME_SIZE 256

// Function Code
#define FUNCTION_CODE_STATE_QUERY					0x01
#define FUNCTION_CODE_OPERATION						0x02
#define FUNCTION_CODE_CONFIG_SET						0x03
#define FUNCTION_CODE_CONFIG_QUERY				0x04
#define FUNCTION_CODE_MAKE_PRODUCT				0x05
#define FUNCTION_CODE_CONNECTIN_QUERY		0x06
#define FUNCTION_CODE_RECIPE_SET						0x07
#define FUNCTION_CODE_RECIPE_QUERY					0x08
#define FUNCTION_CODE_PER_ID_PRODUCT			0x09
#define FUNCTION_CODE_RFID_PWD_SET					0x0A
#define FUNCTION_CODE_IAP_UPGRADE					0x0B
#define FUNCTION_CODE_WARNING_QUERY			0x0C
#define FUNCTION_CODE_STATE_SET_QUERY			0x0D

// ES Brewer Control Types
// ���������ƶ�������
#define ES_BREWER_DOSAGE				0
#define ES_BREWER_BREWEING			1
#define ES_BREWER_LASTSQUEEZE		2
#define ES_BREWER_CLEANING			3
#define ES_BREWER_FLUSH					4
#define ES_BREWER_SERVICE_ON		5
#define ES_BREWER_SERVICE_OFF		6
#define ES_BREWER_INIT						7
#define ES_BREWER_CHAMBER			8
#define ES_BREWER_PURIFICATION		9
#define ES_BREWER_DURATION			17

// FB Brewer Control Types
// �ݲ������ƶ�������
#define FB_BREWER_INIT						0
#define FB_BREWER_SEAL						1
#define FB_BREWER_WATER_INPUT	2
#define FB_BREWER_DOSAGE				3
#define FB_BREWER_WIPE_POSITION	4
#define FB_BREWER_SERVICE				5
#define FB_BREWER_WIPE_MOVING	6
#define FB_BREWER_DURATION			7

////////////////////////////////////////////////////////////////////////////////////////////////
//                                             request interfaces                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////
/*
	���Σ� frameStr - ����ָ������
	����ֵ�� return - ����ָ���
*/
// Operation Class 1 : QUERY
// 1.1 State Query
// ��ѯ��¯�¶�
int StateQuery_BoilerTemp(BYTE frameStr[]);				// 0x0A + 0x02
// ��ѯ��ˮ�¶�
int StateQuery_CoolerTemp(BYTE frameStr[]);				// 0x0C + 0x02
// ��ѯ�����¶�
int StateQuery_EnvTemp(BYTE frameStr[]);					// 0x0E + 0x02
// ��ѯ��¯ѹ��
int StateQuery_BoilerPressure(BYTE frameStr[]);			// 0x10 + 0x02
// ��ѯ����
int StateQuery_Flow(BYTE frameStr[]);							// 0x12 + 0x02 
// ��ѯ����״̬
int StateQuery_SystemState(BYTE frameStr[]);				// 0x14 + 0x04
// ��ѯMDB���
int StateQuery_MDB(BYTE frameStr[]);							// 0x18 + 0x04
// ��ѯRFID״̬
int StateQuery_RFID(BYTE frameStr[]);								// 0x26 + 0x14
// ��ѯ���ο�������ʱ��
int StateQuery_Duration(BYTE frameStr[]);						// 0x3C + 0x08
// ��ѯ�̼��汾
int StateQuery_SystemVersion(BYTE frameStr[]);			// 0x44 + 0x08
// ��ѯʹ��״̬
int StateQuery_EnableState(BYTE frameStr[]);				// 0x4C + 0x02
// ��ѯ����������
int StateQuery_Sensor(BYTE frameStr[]);						// 0x4E + 0x04
// ��ѯ�ϱ����ȳ�ˮʱ��
int StateQuery_LastBrewingTime(BYTE frameStr[]);		// 0x52 + 0x02		@ add 17/11/13

// ��ѯBIB��Ʒʹ������״̬��   1-disable; 0-enable
int StateQuery_EnableState_BIB(BYTE frameStr[]);		// 0x54 + 0x02		@ add 18/03/02
// ��ѯ�쳣��Ԫ��־
int StateQuery_ExceptionUnit(BYTE frameStr[]);			// 0x56 + 0x04		@ add 18/03/02
// ��ѯ�������¶�
int StateQuery_EvaporatorTemp(BYTE frameStr[]);		// 0x5A + 0x02		@ add 18/03/02
// ��ѯ�������¶�
int StateQuery_CondenserTemp(BYTE frameStr[]);		// 0x5C + 0x02		@ add 18/03/02
// ��ѯ����ѹ��
int StateQuery_GasPressure(BYTE frameStr[]);				// 0x5E + 0x02		@ add 18/03/02
// ��ѯ��ˮ����
int StateQuery_ColdWaterFlow(BYTE frameStr[]);			// 0x60 + 0x02		@ add 18/03/02
// ��ѯ����ʪ��
int StateQuery_CaseHumidity(BYTE frameStr[]);			// 0x62 + 0x01		@ add 18/03/02
// ��ѯBIB���ش�����
int StateQuery_BIBSwitchSensor(BYTE frameStr[]);		// 0x63 + 0x01		@ add 18/03/02
// ��ѯBIB�������״̬
int StateQuery_BIBMotorState(BYTE frameStr[]);			// 0x64 + 0x01		@ add 18/03/02
// ��ѯBIB��ˮ�ŷ�����״̬
int StateQuery_BIBHotWaterValveState(BYTE frameStr[]);			// 0x65 + 0x01		@ add 18/03/02
// ��ѯBIB��ˮ�ŷ�����״̬
int StateQuery_BIBColdWaterValveState(BYTE frameStr[]);			// 0x66 + 0x01		@ add 18/03/02
// ��ѯBIB����2P2W������״̬
int StateQuery_BIB2P2WValveState(BYTE frameStr[]);					// 0x67 + 0x01		@ add 18/03/02
// ��ѯBIB�������͵������״̬
int StateQuery_BIBValveMotorState(BYTE frameStr[]);				// 0x68 + 0x01		@ add 18/03/02
// ��ѯBIB���ư�����汾
int StateQuery_BIBControllerVersion(BYTE frameStr[]);				// 0x69 + 0x01		@ add 18/03/02

// ��ѯ�����
int StateQuery_WarningQuery(BYTE frameStr[]);			// padding 0x00
// ��ѯ����״̬��
int StateQuery_SetQuery(BYTE frameStr[]);					// 0x0A~0x1B & 0x4C & 0x4D
// ��ѯ����״̬��֮��
int StateQuery_SetQuery_ext(BYTE frameStr[]);			// 0x0A~0x1B & 0x4C & 0x4D	& 0x54~0x63			@ add 18/03/02
// ��ѯ�����ͺ�
int StateQuery_MachineModel(BYTE frameStr[]);			// padding 0x12~78		@ add 17/11/14

// �û��Զ����ѯ
// startAdd - ��ѯ��ʼ��ַ
// dataLength - ��ѯ���ݳ���
int StateQuery_Customized(BYTE frameStr[], int startAddr, int dataLength);			// startAdd + dataLength

// 1.2 Config Query
// ��ѯ��ˮ�¶�
int ConfigQuery_HotWaterTemp(BYTE frameStr[]);								// 0x00 + 0x02
// ��ѯ��ˮ�¶�
int ConfigQuery_ColdWaterTemp(BYTE frameStr[]);							// 0x02 + 0x02
// ��ѯ��������ѹ��
int ConfigQuery_BrewerPressure(BYTE frameStr[]);								// 0x04 + 0x02
// ��ѯ�Զ���ϴʱ����
int ConfigQuery_AutoFlushTimeInterval(BYTE frameStr[]);					// 0x06 + 0x02
// ��ѯ�������¶�
int ConfigQuery_BrewerTemp(BYTE frameStr[]);									// 0x08 + 0x02
// ��ѯ������������
int ConfigQuery_BrewerSqueezeForce(BYTE frameStr[]);					// 0x0A + 0x02
// ��ѯ����������ʱ��
int ConfigQuery_BrewerSqueezePeriod(BYTE frameStr[]);					// 0x0C + 0x02
// ��ѯ�������س�ʱ��
int ConfigQuery_BrewerRestorationPeriod(BYTE frameStr[]);			// 0x0E + 0x02
// ��ѯ�������ټ�ʱ��
int ConfigQuery_BrewerLastSqueezePeriod(BYTE frameStr[]);			// 0x10 + 0x02
// ��ѯ���̱��Ӽ����ֵ
int ConfigQuery_CatcherThreshold(BYTE frameStr[]);							// 0x14 + 0x01
// ��ѯ�ֱ������Ӽ����ֵ
int ConfigQuery_DispenserThreshold(BYTE frameStr[]);						// 0x15 + 0x01
// ��ѯ�ٴηֱ�����
int ConfigQuery_RedispensingTimes(BYTE frameStr[]);						// 0x17 + 0x01
// ��ѯ���ֱù�ͣ��ʱ
int ConfigQuery_ValveClosingDelayTime(BYTE frameStr[]);				// 0x1A + 0x01
// ��ѯ���ֱ������
int ConfigQuery_GearPumpMaxPower(BYTE frameStr[]);					// 0x1B + 0x01
// ��ѯ���ſ����������ʱ��
int ConfigQuery_MixerOpenningDelayTime(BYTE frameStr[]);			// 0x1C + 0x01
// ��ѯͣˮ���������ʱֹͣ
int ConfigQuery_MixerClosingDelayTime(BYTE frameStr[]);				// 0x1D + 0x01
// ��ѯ�ŷ����ٶ�
int ConfigQuery_FanSpeed(BYTE frameStr[]);										// 0x1E + 0x01
// ��ѯ�ݲ����������ٶ�
int ConfigQuery_FBBrewerAirPumpSpeed(BYTE frameStr[]);				// 0x1F + 0x01
// ��ѯ�ݲ���2�γ�ˮʱ���϶
int ConfigQuery_FBBrewerEffluentTimeInterval(BYTE frameStr[]);		// 0x20 + 0x01
// ��ѯ�����ô���ʱ��
int ConfigQuery_AirPumpRunningTime(BYTE frameStr[]);					// 0x21 + 0x01
// ��ѯĥ����ȥ�з�ʱ��
int ConfigQuery_ResidualPowderRemoveTime(BYTE frameStr[]);									// 0x22 + 0x01		@ add 18/04/08
// ��ѯ��ϴʱĥ����ȥ�зۼ��ʱ��
int ConfigQuery_ResidualPowderRemoveTimeInterval(BYTE frameStr[]);					// 0x23 + 0x01		@ add 18/04/08
// ��ѯRFIDʹ�ܹ���
int ConfigQuery_RFIDEnable(BYTE frameStr[]);										// 0x27 + 0x01
// ��ѯ����ƿ�����ʱ��
int ConfigQuery_UVLightOnPeriod(BYTE frameStr[]);							// 0x28 + 0x01
// ��ѯ����ƿ�����ʱ��
int ConfigQuery_UVLightOffPeriod(BYTE frameStr[]);							// 0x29 + 0x01
// ��ѯ����Ԥ����ˮ������
int ConfigQuery_PrebrewingWaterScale(BYTE frameStr[]);					// 0x31 + 0x01
// ��ѯ����Ԥ����ʱ��
int ConfigQuery_PrebrewingPeriod(BYTE frameStr[]);							// 0x32 + 0x01
// ��ѯ�����Ƿ���ϴ
int ConfigQuery_FlushOnBoot(BYTE frameStr[]);									// 0x33 + 0x01

// ��ѯ�������
int ConfigQuery_MachineID(BYTE frameStr[]);													// 0x34 + 0x14		@ add 2017/11/14
// ��ѯ��ˮ����ʱ���ۣ� 0~3.0s
int ConfigQuery_PowderOutDelay(BYTE frameStr[]);										// 0x48 + 0x01		@ add 2017/11/14
// ��ѯ��ˮǰ��ǰͣ�ۣ� 0~2.0s
int ConfigQuery_PowderStopAhead(BYTE frameStr[]);									// 0x49 + 0x01		@ add 2017/11/14
// ��ѯ�������ٵ���ֵ�� 0.2~10.0 ml/s
int ConfigQuery_CurrentSpeedThreshold(BYTE frameStr[]);							// 0x4A + 0x01		@ add 2017/11/14
// ��ѯ�������ٵͼ��ʱ���� 5.0~20.0 s
int ConfigQuery_LowSpeedCheckPeriod(BYTE frameStr[]);							// 0x4B + 0x01		@ add 2017/11/14
// ��ѯ��ת���ʱ���� 1.5~4.0 s
int ConfigQuery_BlockCheckPeriod(BYTE frameStr[]);										// 0x4C + 0x01		@ add 2017/11/14
// ��ѯˮ�������ʱ���� 30~250 s
int ConfigQuery_WaterTankFillingCheckPeriod(BYTE frameStr[]);					// 0x4D + 0x01		@ add 2017/11/14
// ��ѯES2��������ѹ��
// press - ѹ��ֵ����λmbar
int ConfigQuery_BrewerPressure_2(BYTE frameStr[]);										// 0x4E + 0x02		@ add 2017/11/14
// ��ѯES2������������
// force - ��������1~127
int ConfigQuery_BrewerSqueezeForce_2(BYTE frameStr[]);							// 0x50 + 0x02		@ add 2017/11/14
// ��ѯES2����������ʱ��
// period - ʱ�䣬0~25.5s
int ConfigQuery_BrewerSqueezePeriod_2(BYTE frameStr[]);							// 0x52 + 0x02		@ add 2017/11/14
// ��ѯES2�������س�ʱ��
// period - ʱ�䣬0~25.5s
int ConfigQuery_BrewerRestorationPeriod_2(BYTE frameStr[]);					// 0x54 + 0x02		@ add 2017/11/14
// ��ѯES2�������ټ�ʱ��
// period - ʱ�䣬0~25.5s
int ConfigQuery_BrewerLastSqueezePeriod_2(BYTE frameStr[]);					// 0x56 + 0x02		@ add 2017/11/14
// ��ѯFB2�ݲ����������ٶ�
// speed - �ٶȣ�1~127
int ConfigQuery_FBBrewerAirPumpSpeed_2(BYTE frameStr[]);						// 0x58 + 0x01		@ add 2017/11/14
// ��ѯFB2�ݲ���2�γ�ˮʱ���϶
// timeInterval - ʱ�䣬0~25.5s
int ConfigQuery_FBBrewerEffluentTimeInterval_2(BYTE frameStr[]);				// 0x59 + 0x01		@ add 2017/11/14
// ��ѯFB2�����ô���ʱ��
// time - ʱ�䣬0~25.5s
int ConfigQuery_AirPumpRunningTime_2(BYTE frameStr[]);							// 0x5A + 0x01		@ add 2017/11/14
// ��ѯES2����Ԥ����ˮ������
// scale - ������0~100
int ConfigQuery_PrebrewingWaterScale_2(BYTE frameStr[]);							// 0x5B + 0x01		@ add 2017/11/14
// ��ѯES2����Ԥ����ʱ��
// period - ʱ�䣬0~25.5s
int ConfigQuery_PrebrewingPeriod_2(BYTE frameStr[]);									// 0x5C + 0x01		@ add 2017/11/14

// ��ѯ�����ſ��ŵ���
// current - ������10~90
int ConfigQuery_SlidingdoorOpenCurrent(BYTE frameStr[]);							// 0x5D + 0x01   	@ add 18/04/08
// ��ѯ�����Ź��ŵ���
// current - ������10~90
int ConfigQuery_SlidingdoorShutCurrent(BYTE frameStr[]);							// 0x5E + 0x01   	@ add 18/04/08

// �û��Զ����ѯ
// startAdd - ��ѯ��ʼ��ַ
// dataLength - ��ѯ���ݳ���
int ConfigQuery_Customized(BYTE frameStr[], int startAddr, int dataLength);	// startAdd + dataLength

// 1.3 Enable Query
// ��ѯԪ������״̬
int ConnQuery(BYTE frameStr[],  int dataAddr, int dataLength);							// 0x00  connection
// ��ѯԪ��ʹ��״̬
int EnableQuery(BYTE frameStr[],  int dataAddr, int dataLength);						    // 0x01  enable

// 1.4 Recipe Query
// ��ѯ��Ʒ�䷽
// productId - ��Ʒid
int RecipeQuery(BYTE frameStr[],  int productId, int defaultRecipe = 0);	

// Operation Class 2 : OPERATION
// 2.1 Machine Operation
// ��Σ�
// motorId - ���id
// current - �������

// ����Ӧ������
int Operation_MixerMotorPowerOn(BYTE frameStr[], int motorId, int current, int dir/*1:positive, 0:reverse*/);				// 0x00 ~ 0x05
// �򿪿�����
int Operation_AirPumpPowerOn(BYTE frameStr[], int current);												// 0x06
// ����Ӧ�Ϻе��
int Operation_CanisterMotorPowerOn(BYTE frameStr[], int motorId, int current);			// 0x08 ~ 0x0D
// ��ES�������
int Operation_ESPistonMotorPowerOn(BYTE frameStr[], int current);									// 0x10
// ��ES�ܷ���
int Operation_ESFilterMotorPowerOn(BYTE frameStr[], int current);									// 0x11
// ��FB��Ƭ���
int Operation_FBWiperMotorPowerOn(BYTE frameStr[], int current);									// 0x12
// ��FB�ܷ���
int Operation_FBFilterMotorPowerOn(BYTE frameStr[], int current);									// 0x13
// �򿪳���ˮ��
int Operation_GearPumpPowerOn(BYTE frameStr[], int current);											// 0x16
// �򿪷ֱ����
int Operation_CupSeparatorMotorPowerOn(BYTE frameStr[], int current);						// 0x18
// �򿪱�Ͱ���
int Operation_CupStackMotorPowerOn(BYTE frameStr[], int current);								// 0x19
// ���˱����
int Operation_CupCatcherMotorPowerOn(BYTE frameStr[], int current);							// 0x1A
// �򿪴��ŵ��
int Operation_BigDoorMotorPowerOn(BYTE frameStr[], int current);									// 0x1B
// ��С�ŵ��
int Operation_OutletDoorMotorPowerOn(BYTE frameStr[], int current);								// 0x1C

// �򿪳��½�ˮ��
int Operation_OpenInletValve(BYTE frameStr[]);																		// 0x20
// ����ˮ��ˮ��
int Operation_OpenColdWaterInletValve(BYTE frameStr[]);													// 0x21
// ��ES��λ��ͨ��
int Operation_OpenES_2P3W_Valve(BYTE frameStr[]);																// 0x22
// ��ES2�������
int Operation_ESPistonMotorPowerOn_2(BYTE frameStr[], int current);								// 0x24		@ add 17/11/13
// ��ES2�ܷ���
int Operation_ESFilterMotorPowerOn_2(BYTE frameStr[], int current);								// 0x25		@ add 17/11/13
// ��FB2��Ƭ���
int Operation_FBWiperMotorPowerOn_2(BYTE frameStr[], int current);								// 0x26		@ add 17/11/13
// ��FB2�ܷ���
int Operation_FBFilterMotorPowerOn_2(BYTE frameStr[], int current);								// 0x27		@ add 17/11/13
// ����Ӧ��λ��ͨ��
int Operation_Open_2P2W_Valve(BYTE frameStr[], int valveId);												// 0x28 ~ 0x2F

// ������ϴ
int Operation_CleanMachine(BYTE frameStr[]);																			// 0x30
// ��������
int Operation_Desinfection(BYTE frameStr[]);																			// 0x31
// �ֱ�����
int Operation_CupDispenserTest(BYTE frameStr[], int cupCount);										// 0x32
// ����ˮ
int Operation_PutHotWater(BYTE frameStr[], int WaterVolume);											// 0x33
// MDB�豸����
int Operation_MDBControl(BYTE frameStr[], int OnOff/*1:on  0:off*/);									// 0x34
// �˱�
// CoinCount -  �˱�����
int Operation_CoinReturn(BYTE frameStr[], int CoinCount);													// 0x35
// ���ſ���
int Operation_DoorControl(BYTE frameStr[], int OpenShut/*1:open  0:shut*/);					// 0x36
// ������λ
// 0 - �ָ�����
// 1 - I/O��������
int Operation_SystemReset(BYTE frameStr[], int resetType = 0);											// 0x37		@ mod 18/05/15  V4.6
// ����������
// controlType - ������������ƶ�������
int Operation_ESBrewerControl(BYTE frameStr[], int controlType, int ES_No = 0/*0:ES_1  0:ES_2*/);			// 0x38		@ mod 17/11/13
// �Ϻе��۲���
// canisterId - �Ϻб��
// time - ����ʱ�䣨1~25.5s��
int Operation_PowderOutTest(BYTE frameStr[], int canisterId, int time);								// 0x39
// ĥ������
// time - ĥ��ʱ�䣨1~25.5s��
int Operation_GrinderTest(BYTE frameStr[], int time);																// 0x3A
// �����ƶ�
int Operation_CupHolderMove(BYTE frameStr[], int direction/*1:out  0:in*/);					// 0x3B
// �ݲ����������
// time - ����ʱ�䣨1~25.5s��
int Operation_DropTeaTest(BYTE frameStr[], int time);															// 0x3C
// �ݲ�������
// controlType - ����ݲ������ƶ�������
int Operation_FBBrewerControl(BYTE frameStr[], int controlType, int FB_No = 0/*0:FB_1  0:FB_2*/);		// 0x3D		@ mod 17/11/13
// �˱�ϵͳ����
// cupCount - �˱�����
int Operation_CupCatcherTest(BYTE frameStr[], int cupCount);											// 0x3E
// С�ſ���
int Operation_SmallDoorControl(BYTE frameStr[], int OpenShut/*1:open  0:shut*/);		// 0x3F
// ���ˮ��
int Operation_AirbreakEmpty(BYTE frameStr[]);																		// 0x40
// ��չ�¯
int Operation_BoilerEmpty(BYTE frameStr[]);																				// 0x41
// �����ò���
// current - ����ֵ
int Operation_AirPumpTest(BYTE frameStr[], int current);														// 0x42
// �˳�����
int Operation_ExitTest(BYTE frameStr[]);																						// 0x43
// ����ƺ������ƿ���
int Operation_LampController(BYTE frameStr[], int lampType, int OnOff/*1:On  0:Off*/);	// 0x46				@ add 18/03/02

// ��������ϴ
// cleanTimes - �������ݣ���1~10����ʾ��ϴ������90ml�̶�ˮ����11~255����ʾ��ϴһ�Σ�ˮ��Ϊ����ֵ��
int Operation_ModuleESClean(BYTE frameStr[], int cleanTimes, int ES_No = 0/*0:ES_1  0:ES_2*/);						// 0x47 - 0xAA:0xBB		@ mod 17/11/13
// �ݲ�����ϴ
// cleanTimes - �������ݣ���1~10����ʾ��ϴ������90ml�̶�ˮ����11~255����ʾ��ϴһ�Σ�ˮ��Ϊ����ֵ��
int Operation_ModuleFBClean(BYTE frameStr[], int cleanTimes, int FB_No = 0/*0:FB_1  0:FB_2*/);						// 0x47 - 0xCC:0xDD	@ mod 17/11/13
// ��������ϴ
// mixerId - ���������
int Operation_ModuleMixerClean(BYTE frameStr[], int mixerId, int cleanTimes);													// 0x47 - 0x01~0x04

int Operation_IceController(BYTE frameStr[], int controlType, int duration=0);				// 0x48		@ add 17/11/13   @ Del 18/03/02

// BIB���� �����������ȣ�
// controlType - ��1: ����; 2:����ˮ; 3:������ϣ�4:������̼��5:��ˮ��
// duration - ���ã�����ʱ����ˮ�� ( ����������ã�����+0 = ���� )		@ 18/05/15  V4.6
int Operation_BIBController(BYTE frameStr[], int controlType, int duration=0);					// 0x48		@ add 18/03/02

// BIB��ϴ
// pipID - 1-8:�ܵ���; 9:ȫ�ܵ�; 10:�Ʊ�ϵͳ
// cleanTimes - �������ݣ���1~10����ʾ��ϴ������30ml�̶�ˮ����11~255����ʾ��ϴһ�Σ�ˮ��Ϊ����ֵ��
// ( ����������ã��Ʊ�ϵͳ��ϴ+0 = ��ˮ )		@ 18/05/15  V4.6
int Operation_ModuleBIBClean(BYTE frameStr[], int pipID, int cleanTimes);						// 0x49 		@ add 18/03/02
// BIBԪ������
// componentID - 1-8:BIB�������;11-18:��ˮ�ŷ�; 21-28:��ˮ�ŷ�; 31-38:�����ŷ�; 41:���߹�; 42:��ˮ��; 43:��ˮ��
// current - 13~120, ���̶�ֵ: 0
int Operation_BIBComponentTest(BYTE frameStr[], int componentID, int current=0);		// 0x4A 		@ add 18/03/02
// BIB������������
// BIBId ȡֵ��Χ�� 1~8    *******
int Operation_BIBQuantifyTest(BYTE frameStr[], int BIBId, int current, int duration);			// 0x4B ~ 0x52		@ add 18/03/02
// ģ����������
int Operation_ModuleLifeTest(BYTE frameStr[], int moduleId, int testTimes);					// 0x53		@ add 18/03/02
// ���涯̬����
int Operation_GeneralDynamicTest(BYTE frameStr[], int orderType, int time = 0);			// 0x54		@ add 18/03/02

// 2.2 Make Product
// ���Ϻг��ϴ���������Ʒ
// call steps:  AddIngredient -->AddIngredient --> ... -->Operation_MakeProduct
int Operation_MakeProduct(BYTE frameStr[]);																			// @ Mod 18/03/02

// ���Ϻг��ϴ���������Ʒ�����ֱ�������
// call steps:  AddIngredient -->AddIngredient --> ... -->Operation_MakeProductWithoutDispensing
int Operation_MakeProductWithoutDispensing(BYTE frameStr[]);										// @ add 18/03/02

// 2.3 Make Product by id
// This 'productId' is set by RecipeSetting() in operation class 3.2
// ����Ʒid������Ʒ
int Operation_MakeProductById(BYTE frameStr[], int productId);

// Operation Class 3 : SETTING
// 3.1 Config Setting
// ������ˮ�¶�
// temp - �¶�ֵ����λ0.1��
int ConfigSetting_HotWaterTemp(BYTE frameStr[], int temp, int temporary = 0);											// 0x00 + 0x02		@ mod 18/05/15  V4.6
// ������ˮ�¶�
// temp - �¶�ֵ����λ0.1��
int ConfigSetting_ColdWaterTemp(BYTE frameStr[], int temp, int temporary = 0);											// 0x02 + 0x02		@ mod 18/05/15  V4.6
// ���ÿ�������ѹ��
// press - ѹ��ֵ����λmbar
int ConfigSetting_BrewerPressure(BYTE frameStr[], int press, int temporary = 0);											// 0x04 + 0x02		@ mod 18/05/15  V4.6
// �����Զ���ϴʱ���϶
// timeInterval - ʱ�䣬��λ����
int ConfigSetting_AutoFlushTimeInterval(BYTE frameStr[], int timeInterval, int temporary = 0);					// 0x06 + 0x02		@ mod 18/05/15  V4.6
// ���ó������¶�
// temp - �¶�ֵ����λ0.1��
int ConfigSetting_BrewerTemp(BYTE frameStr[], int temp, int temporary = 0);													// 0x08 + 0x02		@ mod 18/05/15  V4.6
// ���ó�����������
// force - ��������1~127
int ConfigSetting_BrewerSqueezeForce(BYTE frameStr[], int force, int temporary = 0);									// 0x0A + 0x02		@ mod 18/05/15  V4.6
// ���ó���������ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_BrewerSqueezePeriod(BYTE frameStr[], int period, int temporary = 0);								// 0x0C + 0x02		@ mod 18/05/15  V4.6
// ���ó������س�ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_BrewerRestorationPeriod(BYTE frameStr[], int period, int temporary = 0);						// 0x0E + 0x02		@ mod 18/05/15  V4.6
// ���ó������ټ�ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_BrewerLastSqueezePeriod(BYTE frameStr[], int period, int temporary = 0);						// 0x10 + 0x02		@ mod 18/05/15  V4.6
// �������̱��Ӽ����ֵ
// threshold - ��ֵ��1~127
int ConfigSetting_CatcherThreshold(BYTE frameStr[], int threshold, int temporary = 0);									// 0x14 + 0x01		@ mod 18/05/15  V4.6
// ���÷ֱ������Ӽ����ֵ
// threshold - ��ֵ��1~127
int ConfigSetting_DispenserThreshold(BYTE frameStr[], int threshold, int temporary = 0);								// 0x15 + 0x01		@ mod 18/05/15  V4.6
// �����ٴηֱ���
// times - ������0~10
int ConfigSetting_RedispensingTimes(BYTE frameStr[], int times, int temporary = 0);										// 0x17 + 0x01		@ mod 18/05/15  V4.6
// ���ó��ֱù�ͣ��ʱ
// time - ʱ�䣬0~25.5s
int ConfigSetting_ValveClosingDelayTime(BYTE frameStr[], int time, int temporary = 0);								// 0x1A + 0x01		@ mod 18/05/15  V4.6
// ���ó��ֱ������
// maxPower - ���ʣ�1~127
int ConfigSetting_GearPumpMaxPower(BYTE frameStr[], int maxPower, int temporary = 0);							// 0x1B + 0x01		@ mod 18/05/15  V4.6
// ���÷��ſ����������ʱ��ʱ��
// time - ʱ�䣬0~25.5s
int ConfigSetting_MixerOpenningDelayTime(BYTE frameStr[], int time, int temporary = 0);							// 0x1C + 0x01		@ mod 18/05/15  V4.6
// ����ͣˮ���������ʱֹͣʱ��
// time - ʱ�䣬0~25.5s
int ConfigSetting_MixerClosingDelayTime(BYTE frameStr[], int time, int temporary = 0);								// 0x1D + 0x01		@ mod 18/05/15  V4.6
// �����ŷ����ٶ�
// speed - �ٶȣ�1~127
int ConfigSetting_FanSpeed(BYTE frameStr[], int speed, int temporary = 0);														// 0x1E + 0x01		@ mod 18/05/15  V4.6
// �����ݲ����������ٶ�
// speed - �ٶȣ�1~127
int ConfigSetting_FBBrewerAirPumpSpeed(BYTE frameStr[], int speed, int temporary = 0);							// 0x1F + 0x01		@ mod 18/05/15  V4.6
// �����ݲ���2�γ�ˮʱ���϶
// timeInterval - ʱ�䣬0~25.5s
int ConfigSetting_FBBrewerEffluentTimeInterval(BYTE frameStr[], int timeInterval, int temporary = 0);		// 0x20 + 0x01		@ mod 18/05/15  V4.6
// ���ÿ����ô���ʱ��
// time - ʱ�䣬0~25.5s
int ConfigSetting_AirPumpRunningTime(BYTE frameStr[], int time, int temporary = 0);									// 0x21 + 0x01		@ mod 18/05/15  V4.6

// ������ĥ����ȥ�з�ʱ��
// time - ʱ�䣬0.5~10.0s
int ConfigSetting_ResidualPowderRemoveTime(BYTE frameStr[], int time, int temporary = 0);						// 0x22 + 0x01		@ mod 18/05/15  V4.6
// ������ϴʱ��ĥ��ȥ�зۼ��ʱ��
// timeInterval - ʱ�䣬0~100h
int ConfigSetting_ResidualPowderRemoveTimeInterval(BYTE frameStr[], int timeInterval, int temporary = 0);			// 0x23 + 0x01		@ mod 18/05/15  V4.6

// ��������ƿ�����ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_UVLightOnPeriod(BYTE frameStr[], int period, int temporary = 0);										// 0x28 + 0x01		@ mod 18/05/15  V4.6
// ��������ƿ�����ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_UVLightOffPeriod(BYTE frameStr[], int period, int temporary = 0);										// 0x29 + 0x01		@ mod 18/05/15  V4.6
// ���ÿ���Ԥ����ˮ������
// scale - ������0~100
int ConfigSetting_PrebrewingWaterScale(BYTE frameStr[], int scale, int temporary = 0);								// 0x31 + 0x01		@ mod 18/05/15  V4.6
// ���ÿ���Ԥ����ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_PrebrewingPeriod(BYTE frameStr[], int period, int temporary = 0);										// 0x32 + 0x01		@ mod 18/05/15  V4.6
// ���ÿ����Ƿ���ϴ
int ConfigSetting_FlushOnBoot(BYTE frameStr[], int isFlush, int temporary = 0);												// 0x33 + 0x01		@ mod 18/05/15  V4.6

// ���û������
int ConfigSetting_MachineID(BYTE frameStr[], BYTE midStr[], int temporary = 0);											// 0x34 + 0x14		@ mod 18/05/15  V4.6
// ���ý�ˮ����ʱ���ۣ� 0~3.0s
int ConfigSetting_PowderOutDelay(BYTE frameStr[], int time, int temporary = 0);											// 0x48 + 0x01		@ mod 18/05/15  V4.6
// ���ù�ˮǰ��ǰͣ�ۣ� 0~2.0s
int ConfigSetting_PowderStopAhead(BYTE frameStr[], int time, int temporary = 0);										// 0x49 + 0x01		@ mod 18/05/15  V4.6
// ���ÿ������ٵ���ֵ�� 0.2~10.0 ml/s
int ConfigSetting_CurrentSpeedThreshold(BYTE frameStr[], int speed, int temporary = 0);							// 0x4A + 0x01		@ mod 18/05/15  V4.6
// ���ÿ������ٵͼ��ʱ���� 5.0~20.0 s
int ConfigSetting_LowSpeedCheckPeriod(BYTE frameStr[], int time, int temporary = 0);								// 0x4B + 0x01		@ mod 18/05/15  V4.6
// ���ö�ת���ʱ���� 1.5~4.0 s
int ConfigSetting_BlockCheckPeriod(BYTE frameStr[], int time, int temporary = 0);											// 0x4C + 0x01		@ mod 18/05/15  V4.6
// ����ˮ�������ʱ���� 30~250 s
int ConfigSetting_WaterTankFillingCheckPeriod(BYTE frameStr[], int time, int temporary = 0);						// 0x4D + 0x01		@ mod 18/05/15  V4.6
// ����ES2��������ѹ��
// press - ѹ��ֵ����λmbar
int ConfigSetting_BrewerPressure_2(BYTE frameStr[], int press, int temporary = 0);										// 0x4E + 0x02		@ mod 18/05/15  V4.6
// ����ES2������������
// force - ��������1~127
int ConfigSetting_BrewerSqueezeForce_2(BYTE frameStr[], int force, int temporary = 0);								// 0x50 + 0x02		@ mod 18/05/15  V4.6
// ����ES2����������ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_BrewerSqueezePeriod_2(BYTE frameStr[], int period, int temporary = 0);							// 0x52 + 0x02		@ mod 18/05/15  V4.6
// ����ES2�������س�ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_BrewerRestorationPeriod_2(BYTE frameStr[], int period, int temporary = 0);					// 0x54 + 0x02		@ mod 18/05/15  V4.6
// ����ES2�������ټ�ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_BrewerLastSqueezePeriod_2(BYTE frameStr[], int period, int temporary = 0);					// 0x56 + 0x02		@ mod 18/05/15  V4.6
//����FB2�ݲ����������ٶ�
// speed - �ٶȣ�1~127
int ConfigSetting_FBBrewerAirPumpSpeed_2(BYTE frameStr[], int speed, int temporary = 0);						// 0x58 + 0x01		@ mod 18/05/15  V4.6
// ����FB2�ݲ���2�γ�ˮʱ���϶
// timeInterval - ʱ�䣬0~25.5s
int ConfigSetting_FBBrewerEffluentTimeInterval_2(BYTE frameStr[], int timeInterval, int temporary = 0);	// 0x59 + 0x01		@ mod 18/05/15  V4.6
// ����FB2�����ô���ʱ��
// time - ʱ�䣬0~25.5s
int ConfigSetting_AirPumpRunningTime_2(BYTE frameStr[], int time, int temporary = 0);								// 0x5A + 0x01		@ mod 18/05/15  V4.6
// ����ES2����Ԥ����ˮ������
// scale - ������0~100
int ConfigSetting_PrebrewingWaterScale_2(BYTE frameStr[], int scale, int temporary = 0);							// 0x5B + 0x01		@ mod 18/05/15  V4.6
// ����ES2����Ԥ����ʱ��
// period - ʱ�䣬0~25.5s
int ConfigSetting_PrebrewingPeriod_2(BYTE frameStr[], int period, int temporary = 0);									// 0x5C + 0x01		@ mod 18/05/15  V4.6

// ���û����ſ��ŵ���
// current - ������10~90
int ConfigSetting_SlidingdoorOpenCurrent(BYTE frameStr[], int current, int temporary = 0);						// 0x5D + 0x01		@ mod 18/05/15  V4.6
// ���û����Ź��ŵ���
// current - ������10~90
int ConfigSetting_SlidingdoorShutCurrent(BYTE frameStr[], int current, int temporary = 0);							// 0x5E + 0x01		@ mod 18/05/15  V4.6

// ����LED�ƴ���ɫ
// ����Ϊ B-R-G ���� (0~31)
int ConfigSetting_LEDColor(BYTE frameStr[], int r, int g, int b, int temporary = 0);											// 0x5F + 0x03   	@ add 18/05/15  V4.6
// ����LED�ƴ�����
int ConfigSetting_LEDSegment(BYTE frameStr[], int segment, int temporary = 0);											// 0x62 + 0x01   	@ add 18/05/15  V4.6
// ����FB1��һ��ˮ����  (10~90)
int ConfigSetting_FB1WaterSection1Ratio(BYTE frameStr[], int ratio = 50, int temporary = 0);						// 0x63 + 0x01   	@ add 18/05/15  V4.6
// ����FB2��һ��ˮ����  (10~90)
int ConfigSetting_FB2WaterSection1Ratio(BYTE frameStr[], int ratio = 50, int temporary = 0);						// 0x64 + 0x01   	@ add 18/05/15  V4.6
// �����ݲ�ʱ��2�Ƿ�����
// 0 - �����ã� 1 - ����
int ConfigSetting_FBBrewTime2Enable(BYTE frameStr[], int enable, int temporary = 0);									// 0x65 + 0x01   	@ add 18/05/15  V4.6
// ����FB1�ݲ�ʱ��2  (0~100)
int ConfigSetting_FB1BrewTime2(BYTE frameStr[], int time, int temporary = 0);												// 0x66 + 0x01   	@ add 18/05/15  V4.6
// ����FB2�ݲ�ʱ��2  (0~100)
int ConfigSetting_FB2BrewTime2(BYTE frameStr[], int time, int temporary = 0);												// 0x67 + 0x01   	@ add 18/05/15  V4.6
// �����ݲ��������ٶ�  (30~90)
int ConfigSetting_FBWipingSpeed(BYTE frameStr[], int speed, int temporary = 0);											// 0x68 + 0x01   	@ add 18/05/15  V4.6
// �����Ʊ����Ƿ�����
// 0 - �����ã� 1 - ����
int ConfigSetting_IceMakerEnable(BYTE frameStr[], int enable, int temporary = 0);											// 0x69 + 0x01   	@ add 18/05/15  V4.6
// ���ò�Ʒ��ɺ���ʱ����ʱ��  (10~100, 0.1s) 
int ConfigSetting_DoorOpenDelayTime(BYTE frameStr[], int time = 25, int temporary = 0);							// 0x6A + 0x01   	@ add 18/05/15  V4.6
// �����ޱ�������ʱ����ʱ��  (10~100, 0.1s)
int ConfigSetting_NullCupCheckFlashingTime(BYTE frameStr[], int time = 25, int temporary = 0);				// 0x6B + 0x01   	@ add 18/05/15  V4.6

// 3.2 Recipe Setting
// �����䷽
// call steps:  AddIngredient -->AddIngredient --> ... -->RecipeSetting
int RecipeSetting(BYTE frameStr[], int productId);

// 3.3 RFID Setting
// ����RFID����
int RFIDSetting(BYTE frameStr[], int devType, BYTE password[]);		// devType - 0:reader, 1:card;   password[6]

// Operation Class 4 : UPGRADE
// 4.1 pre-Upgrade Order
int PreUpgradeOrder(BYTE frameStr[]);										// @ add 18/03/02

// 4.2 IAP Upgrade 
// unitType - ��Ҫ�����İ忨����
// 0x08 - IO ��
// 0x09 - CUP ��
// 0x0A - ϵͳת�Ӱ�  *Ԥ��
// 0x10 - ES brewer ��
// 0x13 - BIB controller ��
int IAPUpgrade(BYTE frameStr[], int unitType);							// @ add 18/03/02


// ��ӳ��ϲ֣��������ϡ����ȶ�������BIB������
// Add ingredient for recipe
int AddIngredient( int canisterId, int effluentInterval, int effluent_volume, 
				  int dischargePeriod, int dischargeMotorSpeed, int mixerMotorSpeed, int hotCoolChoice/*1:hot, 0:cool*/ );

////////////////////////////////////////////////////////////////////////////////////////////////
//                                             response handler                                                             //
////////////////////////////////////////////////////////////////////////////////////////////////

// ���ڷ���ָ�����
/*
	��Σ� frameStr - ���ڷ���ָ��
	����ֵ�� ָ������ַ���
*/
char * ResponseHandler(BYTE frameStr[]);

/*
	��Σ� frameStr - ���ڷ���ָ��
				unitType - ������������
	����ֵ�� �����ɹ�����ʶ
				1 - �ɹ�
				0 - ʧ��
*/
int IAPResponseHandler(BYTE frameStr[], int unitType = 0x00 );		// @ add 18/03/02

#endif