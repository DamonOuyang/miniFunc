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
// 冲泡器控制动作类型
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
// 泡茶器控制动作类型
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
	出参： frameStr - 串口指令数组
	返回值： return - 串口指令长度
*/
// Operation Class 1 : QUERY
// 1.1 State Query
// 查询锅炉温度
int StateQuery_BoilerTemp(BYTE frameStr[]);				// 0x0A + 0x02
// 查询冷水温度
int StateQuery_CoolerTemp(BYTE frameStr[]);				// 0x0C + 0x02
// 查询环境温度
int StateQuery_EnvTemp(BYTE frameStr[]);					// 0x0E + 0x02
// 查询锅炉压力
int StateQuery_BoilerPressure(BYTE frameStr[]);			// 0x10 + 0x02
// 查询流量
int StateQuery_Flow(BYTE frameStr[]);							// 0x12 + 0x02 
// 查询机器状态
int StateQuery_SystemState(BYTE frameStr[]);				// 0x14 + 0x04
// 查询MDB金额
int StateQuery_MDB(BYTE frameStr[]);							// 0x18 + 0x04
// 查询RFID状态
int StateQuery_RFID(BYTE frameStr[]);								// 0x26 + 0x14
// 查询本次开机运行时间
int StateQuery_Duration(BYTE frameStr[]);						// 0x3C + 0x08
// 查询固件版本
int StateQuery_SystemVersion(BYTE frameStr[]);			// 0x44 + 0x08
// 查询使能状态
int StateQuery_EnableState(BYTE frameStr[]);				// 0x4C + 0x02
// 查询传感器配置
int StateQuery_Sensor(BYTE frameStr[]);						// 0x4E + 0x04
// 查询上杯咖啡出水时间
int StateQuery_LastBrewingTime(BYTE frameStr[]);		// 0x52 + 0x02		@ add 17/11/13

// 查询BIB产品使能限制状态码   1-disable; 0-enable
int StateQuery_EnableState_BIB(BYTE frameStr[]);		// 0x54 + 0x02		@ add 18/03/02
// 查询异常单元标志
int StateQuery_ExceptionUnit(BYTE frameStr[]);			// 0x56 + 0x04		@ add 18/03/02
// 查询蒸发器温度
int StateQuery_EvaporatorTemp(BYTE frameStr[]);		// 0x5A + 0x02		@ add 18/03/02
// 查询泠凝器温度
int StateQuery_CondenserTemp(BYTE frameStr[]);		// 0x5C + 0x02		@ add 18/03/02
// 查询气体压力
int StateQuery_GasPressure(BYTE frameStr[]);				// 0x5E + 0x02		@ add 18/03/02
// 查询冷水流量
int StateQuery_ColdWaterFlow(BYTE frameStr[]);			// 0x60 + 0x02		@ add 18/03/02
// 查询机箱湿度
int StateQuery_CaseHumidity(BYTE frameStr[]);			// 0x62 + 0x01		@ add 18/03/02
// 查询BIB开关传感器
int StateQuery_BIBSwitchSensor(BYTE frameStr[]);		// 0x63 + 0x01		@ add 18/03/02
// 查询BIB电机连接状态
int StateQuery_BIBMotorState(BYTE frameStr[]);			// 0x64 + 0x01		@ add 18/03/02
// 查询BIB热水排阀连接状态
int StateQuery_BIBHotWaterValveState(BYTE frameStr[]);			// 0x65 + 0x01		@ add 18/03/02
// 查询BIB冷水排阀连接状态
int StateQuery_BIBColdWaterValveState(BYTE frameStr[]);			// 0x66 + 0x01		@ add 18/03/02
// 查询BIB常规2P2W阀连接状态
int StateQuery_BIB2P2WValveState(BYTE frameStr[]);					// 0x67 + 0x01		@ add 18/03/02
// 查询BIB其他阀和电机连接状态
int StateQuery_BIBValveMotorState(BYTE frameStr[]);				// 0x68 + 0x01		@ add 18/03/02
// 查询BIB控制板软件版本
int StateQuery_BIBControllerVersion(BYTE frameStr[]);				// 0x69 + 0x01		@ add 18/03/02

// 查询警告池
int StateQuery_WarningQuery(BYTE frameStr[]);			// padding 0x00
// 查询基本状态集
int StateQuery_SetQuery(BYTE frameStr[]);					// 0x0A~0x1B & 0x4C & 0x4D
// 查询基本状态集之二
int StateQuery_SetQuery_ext(BYTE frameStr[]);			// 0x0A~0x1B & 0x4C & 0x4D	& 0x54~0x63			@ add 18/03/02
// 查询机器型号
int StateQuery_MachineModel(BYTE frameStr[]);			// padding 0x12~78		@ add 17/11/14

// 用户自定义查询
// startAdd - 查询起始地址
// dataLength - 查询数据长度
int StateQuery_Customized(BYTE frameStr[], int startAddr, int dataLength);			// startAdd + dataLength

// 1.2 Config Query
// 查询热水温度
int ConfigQuery_HotWaterTemp(BYTE frameStr[]);								// 0x00 + 0x02
// 查询冷水温度
int ConfigQuery_ColdWaterTemp(BYTE frameStr[]);							// 0x02 + 0x02
// 查询咖啡酿造压力
int ConfigQuery_BrewerPressure(BYTE frameStr[]);								// 0x04 + 0x02
// 查询自动清洗时间间隔
int ConfigQuery_AutoFlushTimeInterval(BYTE frameStr[]);					// 0x06 + 0x02
// 查询冲泡器温度
int ConfigQuery_BrewerTemp(BYTE frameStr[]);									// 0x08 + 0x02
// 查询冲泡器挤饼力
int ConfigQuery_BrewerSqueezeForce(BYTE frameStr[]);					// 0x0A + 0x02
// 查询冲泡器挤饼时间
int ConfigQuery_BrewerSqueezePeriod(BYTE frameStr[]);					// 0x0C + 0x02
// 查询冲泡器回程时间
int ConfigQuery_BrewerRestorationPeriod(BYTE frameStr[]);			// 0x0E + 0x02
// 查询冲泡器再挤时间
int ConfigQuery_BrewerLastSqueezePeriod(BYTE frameStr[]);			// 0x10 + 0x02
// 查询托盘杯子检测阈值
int ConfigQuery_CatcherThreshold(BYTE frameStr[]);							// 0x14 + 0x01
// 查询分杯器杯子检测阈值
int ConfigQuery_DispenserThreshold(BYTE frameStr[]);						// 0x15 + 0x01
// 查询再次分杯次数
int ConfigQuery_RedispensingTimes(BYTE frameStr[]);						// 0x17 + 0x01
// 查询齿轮泵关停延时
int ConfigQuery_ValveClosingDelayTime(BYTE frameStr[]);				// 0x1A + 0x01
// 查询齿轮泵最大功率
int ConfigQuery_GearPumpMaxPower(BYTE frameStr[]);					// 0x1B + 0x01
// 查询阀门开后搅拌器延时打开
int ConfigQuery_MixerOpenningDelayTime(BYTE frameStr[]);			// 0x1C + 0x01
// 查询停水后搅拌器延时停止
int ConfigQuery_MixerClosingDelayTime(BYTE frameStr[]);				// 0x1D + 0x01
// 查询排风扇速度
int ConfigQuery_FanSpeed(BYTE frameStr[]);										// 0x1E + 0x01
// 查询泡茶器空气泵速度
int ConfigQuery_FBBrewerAirPumpSpeed(BYTE frameStr[]);				// 0x1F + 0x01
// 查询泡茶器2段出水时间间隙
int ConfigQuery_FBBrewerEffluentTimeInterval(BYTE frameStr[]);		// 0x20 + 0x01
// 查询空气泵吹起时间
int ConfigQuery_AirPumpRunningTime(BYTE frameStr[]);					// 0x21 + 0x01
// 查询磨豆器去残粉时长
int ConfigQuery_ResidualPowderRemoveTime(BYTE frameStr[]);									// 0x22 + 0x01		@ add 18/04/08
// 查询清洗时磨豆器去残粉间隔时间
int ConfigQuery_ResidualPowderRemoveTimeInterval(BYTE frameStr[]);					// 0x23 + 0x01		@ add 18/04/08
// 查询RFID使能功能
int ConfigQuery_RFIDEnable(BYTE frameStr[]);										// 0x27 + 0x01
// 查询紫外灯控制亮时间
int ConfigQuery_UVLightOnPeriod(BYTE frameStr[]);							// 0x28 + 0x01
// 查询紫外灯控制灭时间
int ConfigQuery_UVLightOffPeriod(BYTE frameStr[]);							// 0x29 + 0x01
// 查询咖啡预冲泡水量比例
int ConfigQuery_PrebrewingWaterScale(BYTE frameStr[]);					// 0x31 + 0x01
// 查询咖啡预冲泡时间
int ConfigQuery_PrebrewingPeriod(BYTE frameStr[]);							// 0x32 + 0x01
// 查询开机是否清洗
int ConfigQuery_FlushOnBoot(BYTE frameStr[]);									// 0x33 + 0x01

// 查询机器编号
int ConfigQuery_MachineID(BYTE frameStr[]);													// 0x34 + 0x14		@ add 2017/11/14
// 查询进水后延时掉粉， 0~3.0s
int ConfigQuery_PowderOutDelay(BYTE frameStr[]);										// 0x48 + 0x01		@ add 2017/11/14
// 查询关水前提前停粉， 0~2.0s
int ConfigQuery_PowderStopAhead(BYTE frameStr[]);									// 0x49 + 0x01		@ add 2017/11/14
// 查询咖啡流速低阈值， 0.2~10.0 ml/s
int ConfigQuery_CurrentSpeedThreshold(BYTE frameStr[]);							// 0x4A + 0x01		@ add 2017/11/14
// 查询咖啡流速低检测时长， 5.0~20.0 s
int ConfigQuery_LowSpeedCheckPeriod(BYTE frameStr[]);							// 0x4B + 0x01		@ add 2017/11/14
// 查询堵转检测时长， 1.5~4.0 s
int ConfigQuery_BlockCheckPeriod(BYTE frameStr[]);										// 0x4C + 0x01		@ add 2017/11/14
// 查询水箱填充检测时长， 30~250 s
int ConfigQuery_WaterTankFillingCheckPeriod(BYTE frameStr[]);					// 0x4D + 0x01		@ add 2017/11/14
// 查询ES2咖啡酿造压力
// press - 压力值，单位mbar
int ConfigQuery_BrewerPressure_2(BYTE frameStr[]);										// 0x4E + 0x02		@ add 2017/11/14
// 查询ES2冲泡器挤饼力
// force - 挤饼力，1~127
int ConfigQuery_BrewerSqueezeForce_2(BYTE frameStr[]);							// 0x50 + 0x02		@ add 2017/11/14
// 查询ES2冲泡器挤饼时间
// period - 时间，0~25.5s
int ConfigQuery_BrewerSqueezePeriod_2(BYTE frameStr[]);							// 0x52 + 0x02		@ add 2017/11/14
// 查询ES2冲泡器回程时间
// period - 时间，0~25.5s
int ConfigQuery_BrewerRestorationPeriod_2(BYTE frameStr[]);					// 0x54 + 0x02		@ add 2017/11/14
// 查询ES2冲泡器再挤时间
// period - 时间，0~25.5s
int ConfigQuery_BrewerLastSqueezePeriod_2(BYTE frameStr[]);					// 0x56 + 0x02		@ add 2017/11/14
// 查询FB2泡茶器空气泵速度
// speed - 速度，1~127
int ConfigQuery_FBBrewerAirPumpSpeed_2(BYTE frameStr[]);						// 0x58 + 0x01		@ add 2017/11/14
// 查询FB2泡茶器2段出水时间间隙
// timeInterval - 时间，0~25.5s
int ConfigQuery_FBBrewerEffluentTimeInterval_2(BYTE frameStr[]);				// 0x59 + 0x01		@ add 2017/11/14
// 查询FB2空气泵吹气时间
// time - 时间，0~25.5s
int ConfigQuery_AirPumpRunningTime_2(BYTE frameStr[]);							// 0x5A + 0x01		@ add 2017/11/14
// 查询ES2咖啡预冲泡水量比例
// scale - 比例，0~100
int ConfigQuery_PrebrewingWaterScale_2(BYTE frameStr[]);							// 0x5B + 0x01		@ add 2017/11/14
// 查询ES2咖啡预冲泡时间
// period - 时间，0~25.5s
int ConfigQuery_PrebrewingPeriod_2(BYTE frameStr[]);									// 0x5C + 0x01		@ add 2017/11/14

// 查询滑动门开门电流
// current - 电流，10~90
int ConfigQuery_SlidingdoorOpenCurrent(BYTE frameStr[]);							// 0x5D + 0x01   	@ add 18/04/08
// 查询滑动门关门电流
// current - 电流，10~90
int ConfigQuery_SlidingdoorShutCurrent(BYTE frameStr[]);							// 0x5E + 0x01   	@ add 18/04/08

// 用户自定义查询
// startAdd - 查询起始地址
// dataLength - 查询数据长度
int ConfigQuery_Customized(BYTE frameStr[], int startAddr, int dataLength);	// startAdd + dataLength

// 1.3 Enable Query
// 查询元件连接状态
int ConnQuery(BYTE frameStr[],  int dataAddr, int dataLength);							// 0x00  connection
// 查询元件使能状态
int EnableQuery(BYTE frameStr[],  int dataAddr, int dataLength);						    // 0x01  enable

// 1.4 Recipe Query
// 查询产品配方
// productId - 产品id
int RecipeQuery(BYTE frameStr[],  int productId, int defaultRecipe = 0);	

// Operation Class 2 : OPERATION
// 2.1 Machine Operation
// 入参：
// motorId - 电机id
// current - 电机电流

// 打开相应搅拌电机
int Operation_MixerMotorPowerOn(BYTE frameStr[], int motorId, int current, int dir/*1:positive, 0:reverse*/);				// 0x00 ~ 0x05
// 打开空气泵
int Operation_AirPumpPowerOn(BYTE frameStr[], int current);												// 0x06
// 打开相应料盒电机
int Operation_CanisterMotorPowerOn(BYTE frameStr[], int motorId, int current);			// 0x08 ~ 0x0D
// 打开ES挤饼电机
int Operation_ESPistonMotorPowerOn(BYTE frameStr[], int current);									// 0x10
// 打开ES密封电机
int Operation_ESFilterMotorPowerOn(BYTE frameStr[], int current);									// 0x11
// 打开FB刮片电机
int Operation_FBWiperMotorPowerOn(BYTE frameStr[], int current);									// 0x12
// 打开FB密封电机
int Operation_FBFilterMotorPowerOn(BYTE frameStr[], int current);									// 0x13
// 打开齿轮水泵
int Operation_GearPumpPowerOn(BYTE frameStr[], int current);											// 0x16
// 打开分杯电机
int Operation_CupSeparatorMotorPowerOn(BYTE frameStr[], int current);						// 0x18
// 打开杯桶电机
int Operation_CupStackMotorPowerOn(BYTE frameStr[], int current);								// 0x19
// 打开运杯电机
int Operation_CupCatcherMotorPowerOn(BYTE frameStr[], int current);							// 0x1A
// 打开大门电机
int Operation_BigDoorMotorPowerOn(BYTE frameStr[], int current);									// 0x1B
// 打开小门电机
int Operation_OutletDoorMotorPowerOn(BYTE frameStr[], int current);								// 0x1C

// 打开常温进水阀
int Operation_OpenInletValve(BYTE frameStr[]);																		// 0x20
// 打开冷水进水阀
int Operation_OpenColdWaterInletValve(BYTE frameStr[]);													// 0x21
// 打开ES二位三通阀
int Operation_OpenES_2P3W_Valve(BYTE frameStr[]);																// 0x22
// 打开ES2挤饼电机
int Operation_ESPistonMotorPowerOn_2(BYTE frameStr[], int current);								// 0x24		@ add 17/11/13
// 打开ES2密封电机
int Operation_ESFilterMotorPowerOn_2(BYTE frameStr[], int current);								// 0x25		@ add 17/11/13
// 打开FB2刮片电机
int Operation_FBWiperMotorPowerOn_2(BYTE frameStr[], int current);								// 0x26		@ add 17/11/13
// 打开FB2密封电机
int Operation_FBFilterMotorPowerOn_2(BYTE frameStr[], int current);								// 0x27		@ add 17/11/13
// 打开相应二位二通阀
int Operation_Open_2P2W_Valve(BYTE frameStr[], int valveId);												// 0x28 ~ 0x2F

// 机器清洗
int Operation_CleanMachine(BYTE frameStr[]);																			// 0x30
// 机器消毒
int Operation_Desinfection(BYTE frameStr[]);																			// 0x31
// 分杯测试
int Operation_CupDispenserTest(BYTE frameStr[], int cupCount);										// 0x32
// 出热水
int Operation_PutHotWater(BYTE frameStr[], int WaterVolume);											// 0x33
// MDB设备控制
int Operation_MDBControl(BYTE frameStr[], int OnOff/*1:on  0:off*/);									// 0x34
// 退币
// CoinCount -  退币数量
int Operation_CoinReturn(BYTE frameStr[], int CoinCount);													// 0x35
// 大门控制
int Operation_DoorControl(BYTE frameStr[], int OpenShut/*1:open  0:shut*/);					// 0x36
// 机器复位
// 0 - 恢复故障
// 1 - I/O板软重启
int Operation_SystemReset(BYTE frameStr[], int resetType = 0);											// 0x37		@ mod 18/05/15  V4.6
// 冲泡器控制
// controlType - 详见冲泡器控制动作类型
int Operation_ESBrewerControl(BYTE frameStr[], int controlType, int ES_No = 0/*0:ES_1  0:ES_2*/);			// 0x38		@ mod 17/11/13
// 料盒掉粉测试
// canisterId - 料盒编号
// time - 掉粉时间（1~25.5s）
int Operation_PowderOutTest(BYTE frameStr[], int canisterId, int time);								// 0x39
// 磨豆测试
// time - 磨豆时间（1~25.5s）
int Operation_GrinderTest(BYTE frameStr[], int time);																// 0x3A
// 杯托移动
int Operation_CupHolderMove(BYTE frameStr[], int direction/*1:out  0:in*/);					// 0x3B
// 泡茶器掉茶测试
// time - 掉茶时间（1~25.5s）
int Operation_DropTeaTest(BYTE frameStr[], int time);															// 0x3C
// 泡茶器动作
// controlType - 详见泡茶器控制动作类型
int Operation_FBBrewerControl(BYTE frameStr[], int controlType, int FB_No = 0/*0:FB_1  0:FB_2*/);		// 0x3D		@ mod 17/11/13
// 运杯系统测试
// cupCount - 运杯数量
int Operation_CupCatcherTest(BYTE frameStr[], int cupCount);											// 0x3E
// 小门控制
int Operation_SmallDoorControl(BYTE frameStr[], int OpenShut/*1:open  0:shut*/);		// 0x3F
// 清空水箱
int Operation_AirbreakEmpty(BYTE frameStr[]);																		// 0x40
// 清空锅炉
int Operation_BoilerEmpty(BYTE frameStr[]);																				// 0x41
// 空气泵测试
// current - 电流值
int Operation_AirPumpTest(BYTE frameStr[], int current);														// 0x42
// 退出测试
int Operation_ExitTest(BYTE frameStr[]);																						// 0x43
// 紫外灯和照明灯控制
int Operation_LampController(BYTE frameStr[], int lampType, int OnOff/*1:On  0:Off*/);	// 0x46				@ add 18/03/02

// 冲泡器清洗
// cleanTimes - 复用数据：（1~10，表示清洗次数，90ml固定水量；11~255，表示清洗一次，水量为变量值）
int Operation_ModuleESClean(BYTE frameStr[], int cleanTimes, int ES_No = 0/*0:ES_1  0:ES_2*/);						// 0x47 - 0xAA:0xBB		@ mod 17/11/13
// 泡茶器清洗
// cleanTimes - 复用数据：（1~10，表示清洗次数，90ml固定水量；11~255，表示清洗一次，水量为变量值）
int Operation_ModuleFBClean(BYTE frameStr[], int cleanTimes, int FB_No = 0/*0:FB_1  0:FB_2*/);						// 0x47 - 0xCC:0xDD	@ mod 17/11/13
// 搅拌器清洗
// mixerId - 搅拌器编号
int Operation_ModuleMixerClean(BYTE frameStr[], int mixerId, int cleanTimes);													// 0x47 - 0x01~0x04

int Operation_IceController(BYTE frameStr[], int controlType, int duration=0);				// 0x48		@ add 17/11/13   @ Del 18/03/02

// BIB控制 （包含出冰等）
// controlType - （1: 出冰; 2:出冷水; 3:清除故障；4:二氧化碳；5:热水）
// duration - 复用，出料时间或出水量 ( 特殊参数复用：出冰+0 = 搅拌 )		@ 18/05/15  V4.6
int Operation_BIBController(BYTE frameStr[], int controlType, int duration=0);					// 0x48		@ add 18/03/02

// BIB清洗
// pipID - 1-8:管道号; 9:全管道; 10:制冰系统
// cleanTimes - 复用数据：（1~10，表示清洗次数，30ml固定水量；11~255，表示清洗一次，水量为变量值）
// ( 特殊参数复用：制冰系统清洗+0 = 排水 )		@ 18/05/15  V4.6
int Operation_ModuleBIBClean(BYTE frameStr[], int pipID, int cleanTimes);						// 0x49 		@ add 18/03/02
// BIB元件测试
// componentID - 1-8:BIB果浆电机;11-18:热水排阀; 21-28:冷水排阀; 31-38:常规排阀; 41:螺线管; 42:冷水泵; 43:进水阀
// current - 13~120, 阀固定值: 0
int Operation_BIBComponentTest(BYTE frameStr[], int componentID, int current=0);		// 0x4A 		@ add 18/03/02
// BIB果浆定量测试
// BIBId 取值范围： 1~8    *******
int Operation_BIBQuantifyTest(BYTE frameStr[], int BIBId, int current, int duration);			// 0x4B ~ 0x52		@ add 18/03/02
// 模组寿命测试
int Operation_ModuleLifeTest(BYTE frameStr[], int moduleId, int testTimes);					// 0x53		@ add 18/03/02
// 常规动态测试
int Operation_GeneralDynamicTest(BYTE frameStr[], int orderType, int time = 0);			// 0x54		@ add 18/03/02

// 2.2 Make Product
// 按料盒出料次序制作产品
// call steps:  AddIngredient -->AddIngredient --> ... -->Operation_MakeProduct
int Operation_MakeProduct(BYTE frameStr[]);																			// @ Mod 18/03/02

// 按料盒出料次序制作产品（不分杯制作）
// call steps:  AddIngredient -->AddIngredient --> ... -->Operation_MakeProductWithoutDispensing
int Operation_MakeProductWithoutDispensing(BYTE frameStr[]);										// @ add 18/03/02

// 2.3 Make Product by id
// This 'productId' is set by RecipeSetting() in operation class 3.2
// 按产品id制作产品
int Operation_MakeProductById(BYTE frameStr[], int productId);

// Operation Class 3 : SETTING
// 3.1 Config Setting
// 设置热水温度
// temp - 温度值，单位0.1度
int ConfigSetting_HotWaterTemp(BYTE frameStr[], int temp, int temporary = 0);											// 0x00 + 0x02		@ mod 18/05/15  V4.6
// 设置冷水温度
// temp - 温度值，单位0.1度
int ConfigSetting_ColdWaterTemp(BYTE frameStr[], int temp, int temporary = 0);											// 0x02 + 0x02		@ mod 18/05/15  V4.6
// 设置咖啡酿造压力
// press - 压力值，单位mbar
int ConfigSetting_BrewerPressure(BYTE frameStr[], int press, int temporary = 0);											// 0x04 + 0x02		@ mod 18/05/15  V4.6
// 设置自动清洗时间间隙
// timeInterval - 时间，单位分钟
int ConfigSetting_AutoFlushTimeInterval(BYTE frameStr[], int timeInterval, int temporary = 0);					// 0x06 + 0x02		@ mod 18/05/15  V4.6
// 设置冲泡器温度
// temp - 温度值，单位0.1度
int ConfigSetting_BrewerTemp(BYTE frameStr[], int temp, int temporary = 0);													// 0x08 + 0x02		@ mod 18/05/15  V4.6
// 设置冲泡器挤饼力
// force - 挤饼力，1~127
int ConfigSetting_BrewerSqueezeForce(BYTE frameStr[], int force, int temporary = 0);									// 0x0A + 0x02		@ mod 18/05/15  V4.6
// 设置冲泡器挤饼时间
// period - 时间，0~25.5s
int ConfigSetting_BrewerSqueezePeriod(BYTE frameStr[], int period, int temporary = 0);								// 0x0C + 0x02		@ mod 18/05/15  V4.6
// 设置冲泡器回程时间
// period - 时间，0~25.5s
int ConfigSetting_BrewerRestorationPeriod(BYTE frameStr[], int period, int temporary = 0);						// 0x0E + 0x02		@ mod 18/05/15  V4.6
// 设置冲泡器再挤时间
// period - 时间，0~25.5s
int ConfigSetting_BrewerLastSqueezePeriod(BYTE frameStr[], int period, int temporary = 0);						// 0x10 + 0x02		@ mod 18/05/15  V4.6
// 设置托盘杯子检测阈值
// threshold - 阈值，1~127
int ConfigSetting_CatcherThreshold(BYTE frameStr[], int threshold, int temporary = 0);									// 0x14 + 0x01		@ mod 18/05/15  V4.6
// 设置分杯器杯子检测阈值
// threshold - 阈值，1~127
int ConfigSetting_DispenserThreshold(BYTE frameStr[], int threshold, int temporary = 0);								// 0x15 + 0x01		@ mod 18/05/15  V4.6
// 设置再次分杯数
// times - 次数，0~10
int ConfigSetting_RedispensingTimes(BYTE frameStr[], int times, int temporary = 0);										// 0x17 + 0x01		@ mod 18/05/15  V4.6
// 设置齿轮泵关停延时
// time - 时间，0~25.5s
int ConfigSetting_ValveClosingDelayTime(BYTE frameStr[], int time, int temporary = 0);								// 0x1A + 0x01		@ mod 18/05/15  V4.6
// 设置齿轮泵最大功率
// maxPower - 功率，1~127
int ConfigSetting_GearPumpMaxPower(BYTE frameStr[], int maxPower, int temporary = 0);							// 0x1B + 0x01		@ mod 18/05/15  V4.6
// 设置阀门开后搅拌器延时打开时间
// time - 时间，0~25.5s
int ConfigSetting_MixerOpenningDelayTime(BYTE frameStr[], int time, int temporary = 0);							// 0x1C + 0x01		@ mod 18/05/15  V4.6
// 设置停水后搅拌器延时停止时间
// time - 时间，0~25.5s
int ConfigSetting_MixerClosingDelayTime(BYTE frameStr[], int time, int temporary = 0);								// 0x1D + 0x01		@ mod 18/05/15  V4.6
// 设置排风扇速度
// speed - 速度，1~127
int ConfigSetting_FanSpeed(BYTE frameStr[], int speed, int temporary = 0);														// 0x1E + 0x01		@ mod 18/05/15  V4.6
// 设置泡茶器空气泵速度
// speed - 速度，1~127
int ConfigSetting_FBBrewerAirPumpSpeed(BYTE frameStr[], int speed, int temporary = 0);							// 0x1F + 0x01		@ mod 18/05/15  V4.6
// 设置泡茶器2段出水时间间隙
// timeInterval - 时间，0~25.5s
int ConfigSetting_FBBrewerEffluentTimeInterval(BYTE frameStr[], int timeInterval, int temporary = 0);		// 0x20 + 0x01		@ mod 18/05/15  V4.6
// 设置空气泵吹气时间
// time - 时间，0~25.5s
int ConfigSetting_AirPumpRunningTime(BYTE frameStr[], int time, int temporary = 0);									// 0x21 + 0x01		@ mod 18/05/15  V4.6

// 设置研磨器器去残粉时长
// time - 时间，0.5~10.0s
int ConfigSetting_ResidualPowderRemoveTime(BYTE frameStr[], int time, int temporary = 0);						// 0x22 + 0x01		@ mod 18/05/15  V4.6
// 设置清洗时研磨器去残粉间隔时间
// timeInterval - 时间，0~100h
int ConfigSetting_ResidualPowderRemoveTimeInterval(BYTE frameStr[], int timeInterval, int temporary = 0);			// 0x23 + 0x01		@ mod 18/05/15  V4.6

// 设置紫外灯控制亮时间
// period - 时间，0~25.5s
int ConfigSetting_UVLightOnPeriod(BYTE frameStr[], int period, int temporary = 0);										// 0x28 + 0x01		@ mod 18/05/15  V4.6
// 设置紫外灯控制灭时间
// period - 时间，0~25.5s
int ConfigSetting_UVLightOffPeriod(BYTE frameStr[], int period, int temporary = 0);										// 0x29 + 0x01		@ mod 18/05/15  V4.6
// 设置咖啡预冲泡水量比例
// scale - 比例，0~100
int ConfigSetting_PrebrewingWaterScale(BYTE frameStr[], int scale, int temporary = 0);								// 0x31 + 0x01		@ mod 18/05/15  V4.6
// 设置咖啡预冲泡时间
// period - 时间，0~25.5s
int ConfigSetting_PrebrewingPeriod(BYTE frameStr[], int period, int temporary = 0);										// 0x32 + 0x01		@ mod 18/05/15  V4.6
// 设置开机是否清洗
int ConfigSetting_FlushOnBoot(BYTE frameStr[], int isFlush, int temporary = 0);												// 0x33 + 0x01		@ mod 18/05/15  V4.6

// 设置机器编号
int ConfigSetting_MachineID(BYTE frameStr[], BYTE midStr[], int temporary = 0);											// 0x34 + 0x14		@ mod 18/05/15  V4.6
// 设置进水后延时掉粉， 0~3.0s
int ConfigSetting_PowderOutDelay(BYTE frameStr[], int time, int temporary = 0);											// 0x48 + 0x01		@ mod 18/05/15  V4.6
// 设置关水前提前停粉， 0~2.0s
int ConfigSetting_PowderStopAhead(BYTE frameStr[], int time, int temporary = 0);										// 0x49 + 0x01		@ mod 18/05/15  V4.6
// 设置咖啡流速低阈值， 0.2~10.0 ml/s
int ConfigSetting_CurrentSpeedThreshold(BYTE frameStr[], int speed, int temporary = 0);							// 0x4A + 0x01		@ mod 18/05/15  V4.6
// 设置咖啡流速低检测时长， 5.0~20.0 s
int ConfigSetting_LowSpeedCheckPeriod(BYTE frameStr[], int time, int temporary = 0);								// 0x4B + 0x01		@ mod 18/05/15  V4.6
// 设置堵转检测时长， 1.5~4.0 s
int ConfigSetting_BlockCheckPeriod(BYTE frameStr[], int time, int temporary = 0);											// 0x4C + 0x01		@ mod 18/05/15  V4.6
// 设置水箱填充检测时长， 30~250 s
int ConfigSetting_WaterTankFillingCheckPeriod(BYTE frameStr[], int time, int temporary = 0);						// 0x4D + 0x01		@ mod 18/05/15  V4.6
// 设置ES2咖啡酿造压力
// press - 压力值，单位mbar
int ConfigSetting_BrewerPressure_2(BYTE frameStr[], int press, int temporary = 0);										// 0x4E + 0x02		@ mod 18/05/15  V4.6
// 设置ES2冲泡器挤饼力
// force - 挤饼力，1~127
int ConfigSetting_BrewerSqueezeForce_2(BYTE frameStr[], int force, int temporary = 0);								// 0x50 + 0x02		@ mod 18/05/15  V4.6
// 设置ES2冲泡器挤饼时间
// period - 时间，0~25.5s
int ConfigSetting_BrewerSqueezePeriod_2(BYTE frameStr[], int period, int temporary = 0);							// 0x52 + 0x02		@ mod 18/05/15  V4.6
// 设置ES2冲泡器回程时间
// period - 时间，0~25.5s
int ConfigSetting_BrewerRestorationPeriod_2(BYTE frameStr[], int period, int temporary = 0);					// 0x54 + 0x02		@ mod 18/05/15  V4.6
// 设置ES2冲泡器再挤时间
// period - 时间，0~25.5s
int ConfigSetting_BrewerLastSqueezePeriod_2(BYTE frameStr[], int period, int temporary = 0);					// 0x56 + 0x02		@ mod 18/05/15  V4.6
//设置FB2泡茶器空气泵速度
// speed - 速度，1~127
int ConfigSetting_FBBrewerAirPumpSpeed_2(BYTE frameStr[], int speed, int temporary = 0);						// 0x58 + 0x01		@ mod 18/05/15  V4.6
// 设置FB2泡茶器2段出水时间间隙
// timeInterval - 时间，0~25.5s
int ConfigSetting_FBBrewerEffluentTimeInterval_2(BYTE frameStr[], int timeInterval, int temporary = 0);	// 0x59 + 0x01		@ mod 18/05/15  V4.6
// 设置FB2空气泵吹气时间
// time - 时间，0~25.5s
int ConfigSetting_AirPumpRunningTime_2(BYTE frameStr[], int time, int temporary = 0);								// 0x5A + 0x01		@ mod 18/05/15  V4.6
// 设置ES2咖啡预冲泡水量比例
// scale - 比例，0~100
int ConfigSetting_PrebrewingWaterScale_2(BYTE frameStr[], int scale, int temporary = 0);							// 0x5B + 0x01		@ mod 18/05/15  V4.6
// 设置ES2咖啡预冲泡时间
// period - 时间，0~25.5s
int ConfigSetting_PrebrewingPeriod_2(BYTE frameStr[], int period, int temporary = 0);									// 0x5C + 0x01		@ mod 18/05/15  V4.6

// 设置滑动门开门电流
// current - 电流，10~90
int ConfigSetting_SlidingdoorOpenCurrent(BYTE frameStr[], int current, int temporary = 0);						// 0x5D + 0x01		@ mod 18/05/15  V4.6
// 设置滑动门关门电流
// current - 电流，10~90
int ConfigSetting_SlidingdoorShutCurrent(BYTE frameStr[], int current, int temporary = 0);							// 0x5E + 0x01		@ mod 18/05/15  V4.6

// 设置LED灯带颜色
// 参数为 B-R-G 分量 (0~31)
int ConfigSetting_LEDColor(BYTE frameStr[], int r, int g, int b, int temporary = 0);											// 0x5F + 0x03   	@ add 18/05/15  V4.6
// 设置LED灯带段数
int ConfigSetting_LEDSegment(BYTE frameStr[], int segment, int temporary = 0);											// 0x62 + 0x01   	@ add 18/05/15  V4.6
// 设置FB1第一段水比例  (10~90)
int ConfigSetting_FB1WaterSection1Ratio(BYTE frameStr[], int ratio = 50, int temporary = 0);						// 0x63 + 0x01   	@ add 18/05/15  V4.6
// 设置FB2第一段水比例  (10~90)
int ConfigSetting_FB2WaterSection1Ratio(BYTE frameStr[], int ratio = 50, int temporary = 0);						// 0x64 + 0x01   	@ add 18/05/15  V4.6
// 设置泡茶时间2是否启用
// 0 - 不启用； 1 - 启用
int ConfigSetting_FBBrewTime2Enable(BYTE frameStr[], int enable, int temporary = 0);									// 0x65 + 0x01   	@ add 18/05/15  V4.6
// 设置FB1泡茶时间2  (0~100)
int ConfigSetting_FB1BrewTime2(BYTE frameStr[], int time, int temporary = 0);												// 0x66 + 0x01   	@ add 18/05/15  V4.6
// 设置FB2泡茶时间2  (0~100)
int ConfigSetting_FB2BrewTime2(BYTE frameStr[], int time, int temporary = 0);												// 0x67 + 0x01   	@ add 18/05/15  V4.6
// 设置泡茶器刮渣速度  (30~90)
int ConfigSetting_FBWipingSpeed(BYTE frameStr[], int speed, int temporary = 0);											// 0x68 + 0x01   	@ add 18/05/15  V4.6
// 设置制冰机是否启用
// 0 - 不启用； 1 - 启用
int ConfigSetting_IceMakerEnable(BYTE frameStr[], int enable, int temporary = 0);											// 0x69 + 0x01   	@ add 18/05/15  V4.6
// 设置产品完成后延时开门时间  (10~100, 0.1s) 
int ConfigSetting_DoorOpenDelayTime(BYTE frameStr[], int time = 25, int temporary = 0);							// 0x6A + 0x01   	@ add 18/05/15  V4.6
// 设置无杯检测完成时灯闪时间  (10~100, 0.1s)
int ConfigSetting_NullCupCheckFlashingTime(BYTE frameStr[], int time = 25, int temporary = 0);				// 0x6B + 0x01   	@ add 18/05/15  V4.6

// 3.2 Recipe Setting
// 设置配方
// call steps:  AddIngredient -->AddIngredient --> ... -->RecipeSetting
int RecipeSetting(BYTE frameStr[], int productId);

// 3.3 RFID Setting
// 设置RFID密码
int RFIDSetting(BYTE frameStr[], int devType, BYTE password[]);		// devType - 0:reader, 1:card;   password[6]

// Operation Class 4 : UPGRADE
// 4.1 pre-Upgrade Order
int PreUpgradeOrder(BYTE frameStr[]);										// @ add 18/03/02

// 4.2 IAP Upgrade 
// unitType - 需要升级的板卡类型
// 0x08 - IO 板
// 0x09 - CUP 板
// 0x0A - 系统转接板  *预留
// 0x10 - ES brewer 板
// 0x13 - BIB controller 板
int IAPUpgrade(BYTE frameStr[], int unitType);							// @ add 18/03/02


// 添加出料仓（包含粉料、咖啡豆、冰、BIB果浆）
// Add ingredient for recipe
int AddIngredient( int canisterId, int effluentInterval, int effluent_volume, 
				  int dischargePeriod, int dischargeMotorSpeed, int mixerMotorSpeed, int hotCoolChoice/*1:hot, 0:cool*/ );

////////////////////////////////////////////////////////////////////////////////////////////////
//                                             response handler                                                             //
////////////////////////////////////////////////////////////////////////////////////////////////

// 串口返回指令解析
/*
	入参： frameStr - 串口返回指令
	返回值： 指令解析字符串
*/
char * ResponseHandler(BYTE frameStr[]);

/*
	入参： frameStr - 串口返回指令
				unitType - 升级部件种类
	返回值： 升级成功与否标识
				1 - 成功
				0 - 失败
*/
int IAPResponseHandler(BYTE frameStr[], int unitType = 0x00 );		// @ add 18/03/02

#endif