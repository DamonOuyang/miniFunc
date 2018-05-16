#include "stdafx.h"
#include "libFuncCode.h"
#include "codeDef.h"
#include <string>
using namespace std;

static CANISTER canister_set[8] = {0};
static char RspBuffer[MAX_FRAME_SIZE];
int packReqQuery( BYTE frameStr[], int funcCode, int dataAddr, int dataLength, int addrReserve=0, int lengthReserve=0 );	// @ add/mod	2017/11/14

UINT16 usMBCRC16( UINT8 * pucFrame, UINT16 usLen )
{
	UINT8 ucCRCHi = 0xFF;
	UINT8 ucCRCLo = 0xFF;
	UINT16 iIndex;
	while( usLen-- )
	{
		iIndex = ucCRCLo ^ *( pucFrame++ );
		ucCRCLo = ( UINT8 )( ucCRCHi ^ aucCRCHi[iIndex] );
		ucCRCHi = aucCRCLo[iIndex];
	}
	return ( UINT16 )( ucCRCHi << 8 | ucCRCLo );
}

bool checkSign(UINT8 * frame_ptr, UINT16 usLen)
{
	UINT16 check_sum = usMBCRC16((UINT8 *)frame_ptr, usLen);
	UINT8 * csByte_ptr = (UINT8 *)&check_sum;
	UINT8 a1 = *(frame_ptr+usLen) ;
	UINT8 a2 = *(frame_ptr+usLen+1);
	UINT8 b1 = *csByte_ptr;
	UINT8 b2 = *(csByte_ptr+1);
	if (*(frame_ptr+usLen) == *(csByte_ptr+1) && *(frame_ptr+usLen+1) == *csByte_ptr)
		return true;
	else
		return false;
}

void buildHeader(BYTE *frame_ptr, int funcCode)
{
	frame_header Header;
	memset( &Header, 0, sizeof(Header) );
	Header.start_of_frame = 0xFE;
	Header.request_code = funcCode;

	memcpy(frame_ptr, &Header, sizeof(frame_header));
	frame_ptr += sizeof(frame_header);
}

void buildTailer(BYTE *frame_ptr, BYTE *frameStr)
{
	frame_tailer Tailer;
	UINT16 check_sum = usMBCRC16((UINT8 *)frameStr, frame_ptr - frameStr);
	memcpy(Tailer.check_sum, &check_sum, sizeof(UINT16));

	memcpy(frame_ptr, &Tailer.check_sum[1], sizeof(BYTE));
	memcpy(frame_ptr+sizeof(BYTE), &Tailer.check_sum[0], sizeof(BYTE));
}

char * buildExceptionString( UINT16 rcCode )
{
	memset( RspBuffer, 0, sizeof(RspBuffer) );

	BYTE exceptCode[2];
	memcpy(exceptCode, &rcCode, sizeof(UINT16));

	int functionCode = (int)exceptCode[0] - 0x80;
	int exCode = (int)exceptCode[1];
	string exString = "";
	switch(exCode)
	{
		case EXCEPTION_CODE_UNDEFINED:
			exString = "Undefined error.";
			break;
		case EXCEPTION_CODE_UNIDENTIFIED:
			exString = "Unidentified error.";
			break;
		case EXCEPTION_CODE_CRC_ERROR:
			exString = "CRC error.";
			break;
		case EXCEPTION_CODE_ADDR_ERROR:
			exString = "Address error.";
			break;
		case EXCEPTION_CODE_LENGTH_ERROR:
			exString = "Data length error.";
			break;
		case EXCEPTION_CODE_ACCESS_VIOLATION:
			exString = "Access violation.";
			break;
		case EXCEPTION_CODE_SYSTEM_BUSY:
			exString = "System busy.";
			break;
		case EXCEPTION_CODE_SYSTEM_ERROR:
			exString = "System error.";
			break;
		default:
			break;
	}
	sprintf_s(RspBuffer,"{\"cmd\":\"exception\",\"description\":\"%s\",\"functionCode\":%d}",exString.c_str(), functionCode);
	return RspBuffer;
}

int parseExceptionFrame( BYTE frameStr[] )
{
	UINT16 rcCode;
	memcpy(&rcCode, &frameStr[1], sizeof(UINT16));

	return rcCode;
}

char * parseRspQuery(BYTE frameStr[])
{
	memset( RspBuffer, 0, sizeof(RspBuffer) );

	UINT8 functionCode;
	UINT8 dataLength;
	BYTE data[MAX_FRAME_SIZE] = {0}; 
	memcpy(&functionCode, &frameStr[1], sizeof(UINT8));
	if (functionCode == 0xA8)	{
		dataLength = 3;
	}	else	{
		memcpy(&dataLength, &frameStr[5], sizeof(UINT8));
	}
	memcpy(data, &frameStr[6], (int)dataLength);

	if(!checkSign(frameStr, 6+(int)dataLength))
	{
		sprintf_s(RspBuffer,"{\"cmd\":\"exception\",\"description\":\"checksum failed!\",\"functionCode\":%d}", functionCode);
		return RspBuffer;
	}

	string tmpStr;
	char tmpCh[4]={0};
	for (int i = 0; i<(int)dataLength; i++)
	{
		sprintf_s(tmpCh,"%02X",data[i]);
		tmpStr +=tmpCh;
	}
	
	sprintf_s(RspBuffer,"{\"cmd\":\"query\",\"data\":\"%s\",\"functionCode\":%d,\"result\":\"success\"}",tmpStr.c_str(), functionCode);
	return RspBuffer;
}

char * parseRspExecution(BYTE frameStr[])
{
	memset( RspBuffer, 0, sizeof(RspBuffer) );

	UINT8 functionCode;
	UINT8 action;
	memcpy(&functionCode, &frameStr[1], sizeof(UINT8));
	memcpy(&action, &frameStr[3], sizeof(UINT8));

	if(!checkSign(frameStr, 6))
	{
		sprintf_s(RspBuffer,"{\"cmd\":\"exception\",\"description\":\"checksum failed!\",\"functionCode\":%d}", functionCode);
		return RspBuffer;
	}

	sprintf_s(RspBuffer,"{\"cmd\":\"execution\",\"action\":\"%02X\",\"functionCode\":%d,\"result\":\"success\"}", action, functionCode);
	return RspBuffer;
}

char * parseRspSetting(BYTE frameStr[])
{
	memset( RspBuffer, 0, sizeof(RspBuffer) );

	UINT8 functionCode;
	memcpy(&functionCode, &frameStr[1], sizeof(UINT8));

	if(!checkSign(frameStr, 6))
	{
		sprintf_s(RspBuffer,"{\"cmd\":\"exception\",\"description\":\"checksum failed!\",\"functionCode\":%d}", functionCode);
		return RspBuffer;
	}

	sprintf_s(RspBuffer,"{\"cmd\":\"setting\",\"functionCode\":%d,\"result\":\"success\"}", functionCode);
	return RspBuffer;
}

char * ResponseHandler( BYTE frameStr[] )
{
	if (frameStr[0] != 0xFE)
	{
		return "{\"cmd\":\"exception\",\"description\":\"message format error!\",\"functionCode\":0}";
	}

	if (frameStr[1] > 0x80 && frameStr[1] != 0xA8)
	{
		UINT16 rcCode =  parseExceptionFrame(frameStr);
		return buildExceptionString( rcCode );
	}

	switch(frameStr[1])
	{
	case 0x01:
	case 0x04:
	case 0x06:
	case 0x08:
	case 0x0C:
	case 0x0D:
	case 0xA8:
		return parseRspQuery(frameStr);
	case 0x02:
	case 0x05:
	case 0x0E:		// @ add 18/03/02
	case 0x09:
		return parseRspExecution(frameStr);
	case 0x03:
	case 0x07:
	case 0x0A:
	case 0x0B:
		return parseRspSetting(frameStr);
	}
	return "{\"cmd\":\"undefined\",\"description\":\"undefined message!\",\"functionCode\":0}";
}

int IAPResponseHandler(BYTE frameStr[], int unitType )		// @ add 18/03/02
{
	int rc = 0;
	switch(unitType)
	{
	case 0x08:
		rc = frameStr[4] == 0x02;
		break;
	case 0x09:
		rc = frameStr[4] == 0x04;
		break;
	case 0x0A:
		rc = frameStr[4] == 0x0E;
		break;
	case 0x10:
		rc = frameStr[4] == 0x05;
		break;
	case 0x13:
		rc = frameStr[4] == 0x14;
		break;
	default:
		rc = frameStr[4] == 0x05;
		break;
	}
	return rc;
}

/////////////////////////////////////////////////////////////////
//                                    request                                          //
/////////////////////////////////////////////////////////////////

int packReqQuery( BYTE frameStr[], int funcCode, int dataAddr, int dataLength, int addrReserve, int lengthReserve )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	state_query_frame_body body;
	body.request_addr[0] = addrReserve;
	body.request_addr[1] = (UINT8)dataAddr;
	body.data_length[0] = lengthReserve;
	body.data_length[1] = (UINT8)dataLength;
	memcpy(frame_ptr, &body, sizeof(state_query_frame_body));
	frame_ptr += sizeof(state_query_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packEnableQuery( BYTE frameStr[], int stateCode, int dataAddr, int dataLength )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x06;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	enable_query_frame_body body;
	body.state_code = (UINT8)stateCode;
	body.request_addr= (UINT8)dataAddr;
	body.request_length[0] = 0x00;
	body.request_length[1] = (UINT8)dataLength;
	memcpy(frame_ptr, &body, sizeof(enable_query_frame_body));
	frame_ptr += sizeof(enable_query_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packRecipeQuery( BYTE frameStr[], int productId, int defaultRecipe )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x08;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	recipe_query_frame_body body;
	body.product_id = (UINT8)productId;
	body.reserve_recipe_id= (UINT8)defaultRecipe;
	body.reverve_padding[0] = 0x00;
	body.reverve_padding[1] = 0x00;
	memcpy(frame_ptr, &body, sizeof(recipe_query_frame_body));
	frame_ptr += sizeof(recipe_query_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packMachineOperation( BYTE frameStr[], int dataAddr, int op1, int op2 )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x02;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	operation_frame_body body;
	body.request_addr[0] = 0x00;
	body.request_addr[1] = (UINT8)dataAddr;
	body.operation[0] = (UINT8)op1;
	body.operation[1] = (UINT8)op2;
	memcpy(frame_ptr, &body, sizeof(operation_frame_body));
	frame_ptr += sizeof(operation_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int parseCanisterParams( BYTE data_field[], CANISTER canister_set[] )
{
	BYTE *data_field_ptr = data_field;
	int item = 0;
	for (; item<8; item++)
	{
		if (!canister_set[item].is_set_flag)
			break;
		memcpy(data_field_ptr, &(canister_set[item].canister_id), 8);
		data_field_ptr += 8;
	}
	return item;
}

// @ mod 18/03/02
int packMakeProduct( BYTE frameStr[], CANISTER canister_set[], int type )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = type;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	make_product_frame_body body;
	UINT8 canisterCount = parseCanisterParams(body.data_field, canister_set);
	body.canister_count[0] = 0x00;
	body.canister_count[1] = canisterCount;
	body.data_length[0] = 0x00;
	body.data_length[1] = canisterCount * 8;
	memcpy(frame_ptr, &body, 4);
	frame_ptr += 4;
	memcpy(frame_ptr, body.data_field, body.data_length[1]);
	frame_ptr += body.data_length[1];

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packMakeProductById( BYTE frameStr[], int productId )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x09;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	per_id_product_frame_body body;
	body.product_id = (UINT8)productId;
	body.reserve_recipe_id= 0x00;
	body.reverve_padding[0] = 0x00;
	body.reverve_padding[1] = 0x00;
	memcpy(frame_ptr, &body, sizeof(per_id_product_frame_body));
	frame_ptr += sizeof(per_id_product_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packConfigSetting( BYTE frameStr[], int startAddr, int dataLength, int data, int temporary = 0 )		// 0 - ÓÀ¾Ã£»1- ÔÝ´æ @ mod 18/05/15  V4.6
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x03;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	config_set_frame_body body;
	body.request_addr[0] = temporary;
	body.request_addr[1] = (UINT8)startAddr;
	body.data_length[0] = 0x00;
	body.data_length[1] = (UINT8)dataLength;
	memset(body.data_field, 0, MAX_FRAME_SIZE);
	memcpy(body.data_field, &data, dataLength);
	int bodyLength = dataLength + 4;
	memcpy(frame_ptr, &body, bodyLength);
	frame_ptr += bodyLength;

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packConfigSetting( BYTE frameStr[], int startAddr, int dataLength, BYTE data[], int temporary = 0 )		// 0 - ÓÀ¾Ã£»1- ÔÝ´æ @ mod 18/05/15  V4.6
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x03;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	config_set_frame_body body;
	body.request_addr[0] = temporary;
	body.request_addr[1] = (UINT8)startAddr;
	body.data_length[0] = 0x00;
	body.data_length[1] = (UINT8)dataLength;
	memset(body.data_field, 0, MAX_FRAME_SIZE);
	memcpy(body.data_field, data, dataLength);
	int bodyLength = dataLength + 4;
	memcpy(frame_ptr, &body, bodyLength);
	frame_ptr += bodyLength;

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packRecipeSetting( BYTE frameStr[], int productId, CANISTER canister_set[] )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x07;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	recipe_set_frame_body body;
	UINT8 canisterCount = parseCanisterParams(body.data_field, canister_set);
	body.product_id = (UINT8)productId;
	body.canister_count = canisterCount;
	body.data_length[0] = 0x00;
	body.data_length[1] = canisterCount * 8;
	memcpy(frame_ptr, &body, 4);
	frame_ptr += 4;
	memcpy(frame_ptr, body.data_field, body.data_length[1]);
	frame_ptr += body.data_length[1];

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packRFIDSetting( BYTE frameStr[], int devType, BYTE password[] )
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x0A;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	rfid_pwd_set_frame_body body;
	body.selector = (UINT8)devType;
	body.reserve_padding = 0x00;
	body.data_length[0] = 0x00;
	body.data_length[1] = 0x06;
	memcpy(body.data_field, password, 6);
	memcpy(frame_ptr, &body, sizeof(rfid_pwd_set_frame_body));
	frame_ptr += sizeof(rfid_pwd_set_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}

int packIAPUpgrade( BYTE frameStr[], int selector )							// @ add 18/03/02
{
	BYTE *frame_ptr = frameStr;

	// 1.Build header
	int funcCode = 0x0B;
	buildHeader(frame_ptr, funcCode);
	frame_ptr += sizeof(frame_header);

	// 2.Build body
	iap_upgrade_frame_body body;
	body.selector = (UINT8)selector;
	body.reserve_padding= 0x00;
	body.data_length[0] = 0x00;
	body.data_length[1] = 0x00;
	memcpy(frame_ptr, &body, sizeof(iap_upgrade_frame_body));
	frame_ptr += sizeof(iap_upgrade_frame_body);

	// 3. Finish all
	buildTailer(frame_ptr, frameStr);
	frame_ptr += sizeof(frame_tailer);

	return frame_ptr - frameStr;
}


int AddIngredient( int canisterId, int effluentInterval, int effluent_volume, int dischargePeriod, int dischargeMotorSpeed, int mixerMotorSpeed, int hotCoolChoice )
{
	int item = 0;
	for (; item<sizeof(canister_set)/sizeof(CANISTER); item++)
	{
		if (!canister_set[item].is_set_flag)
		{
			canister_set[item].is_set_flag = 0x31;
			canister_set[item].canister_id = canisterId;
			canister_set[item].effluent_interval = effluentInterval;
			canister_set[item].effluent_volume = effluent_volume;
			canister_set[item].discharge_period = dischargePeriod;
			canister_set[item].discharge_motor_speed = dischargeMotorSpeed;
			canister_set[item].mixer_motor_speed = mixerMotorSpeed;
			canister_set[item].hot_cool_choice = hotCoolChoice;
			break;
		}
	}
	return item;
}

/*
#######################
##             State Query            ##
#######################
*/
int StateQuery_BoilerTemp(BYTE frameStr[])
{
	int funcCode = 0x01;
	int startAddr = 0x0A;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_CoolerTemp( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x0C;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_EnvTemp( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x0E;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BoilerPressure( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x10;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_Flow( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x12;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_SystemState( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x14;
	int dataLength = 0x04;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_MDB( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x18;
	int dataLength = 0x04;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_RFID( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x26;
	int dataLength = 0x14;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_Duration( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x3C;
	int dataLength = 0x08;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_SystemVersion( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x44;
	int dataLength = 0x08;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_EnableState( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x4C;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_Sensor( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x4E;
	int dataLength = 0x04;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_LastBrewingTime( BYTE frameStr[] )
{
	int funcCode = 0x01;
	int startAddr = 0x52;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_EnableState_BIB(BYTE frameStr[])			//	@ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x54;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_ExceptionUnit(BYTE frameStr[])			// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x56;
	int dataLength = 0x04;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_EvaporatorTemp(BYTE frameStr[])		// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x5A;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_CondenserTemp(BYTE frameStr[])		// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x5C;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_GasPressure(BYTE frameStr[])				// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x5E;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_ColdWaterFlow(BYTE frameStr[])			// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x60;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_CaseHumidity(BYTE frameStr[])				// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x62;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIBSwitchSensor(BYTE frameStr[])		// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x63;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIBMotorState(BYTE frameStr[])			// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x64;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIBHotWaterValveState(BYTE frameStr[])			// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x65;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIBColdWaterValveState(BYTE frameStr[])			// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x66;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIB2P2WValveState(BYTE frameStr[])					// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x67;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIBValveMotorState(BYTE frameStr[])					// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x68;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_BIBControllerVersion(BYTE frameStr[])				// @ add 18/03/02
{
	int funcCode = 0x01;
	int startAddr = 0x69;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}


int StateQuery_WarningQuery( BYTE frameStr[] )
{
	int funcCode = 0x0C;
	int startAddr = 0x00;
	int dataLength = 0x00;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_SetQuery( BYTE frameStr[] )
{
	int funcCode = 0x0D;
	int startAddr = 0x00;
	int dataLength = 0x14;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_SetQuery_ext(BYTE frameStr[])				// @ add 18/03/02
{
	int funcCode = 0x0D;
	int startAddr = 0x00;
	int dataLength = 0x24;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int StateQuery_MachineModel( BYTE frameStr[] )
{
	int funcCode = 0xA8;
	int startAddr = 0x34;
	int addrReserve = 0x12;
	int dataLength = 0x78;
	int lengthReserve = 0x56;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength, addrReserve, lengthReserve );
	return frameLength;
}

int StateQuery_Customized( BYTE frameStr[], int startAddr, int dataLength )
{
	int funcCode = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}
// ^_^ end of state query
/*#############################################################################
#############################################################################*/
/*
#######################
##             Config Query           ##
#######################
*/
int ConfigQuery_HotWaterTemp(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x00;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_ColdWaterTemp(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x02;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerPressure(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x04;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_AutoFlushTimeInterval(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x06;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerTemp(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x08;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerSqueezeForce(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x0A;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerSqueezePeriod(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x0C;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerRestorationPeriod(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x0E;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerLastSqueezePeriod(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x10;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_CatcherThreshold(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x14;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_DispenserThreshold(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x15;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_RedispensingTimes(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x17;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_ValveClosingDelayTime(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x1A;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_GearPumpMaxPower(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x1B;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_MixerOpenningDelayTime(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x1C;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_MixerClosingDelayTime(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x1D;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_FanSpeed(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x1E;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_FBBrewerAirPumpSpeed(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x17;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_FBBrewerEffluentTimeInterval(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x20;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_AirPumpRunningTime(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x21;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_ResidualPowderRemoveTime(BYTE frameStr[])							// @ add 18/04/08
{
	int funcCode = 0x04;
	int startAddr = 0x22;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_ResidualPowderRemoveTimeInterval(BYTE frameStr[])				// @ add 18/04/08
{
	int funcCode = 0x04;
	int startAddr = 0x23;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_RFIDEnable(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x27;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_UVLightOnPeriod(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x28;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_UVLightOffPeriod(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x29;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_PrebrewingWaterScale(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x31;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_PrebrewingPeriod(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x32;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_FlushOnBoot(BYTE frameStr[])
{
	int funcCode = 0x04;
	int startAddr = 0x33;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

// @ add  2017/11/14
int ConfigQuery_MachineID( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x34;
	int dataLength = 0x14;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_PowderOutDelay( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x48;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_PowderStopAhead( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x49;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_CurrentSpeedThreshold( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x4A;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_LowSpeedCheckPeriod( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x4B;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BlockCheckPeriod( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x4C;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_WaterTankFillingCheckPeriod( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x4D;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerPressure_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x4E;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerSqueezeForce_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x50;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerSqueezePeriod_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x52;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerRestorationPeriod_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x54;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_BrewerLastSqueezePeriod_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x56;
	int dataLength = 0x02;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_FBBrewerAirPumpSpeed_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x58;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_FBBrewerEffluentTimeInterval_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x59;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_AirPumpRunningTime_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x5A;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_PrebrewingWaterScale_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x5B;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_PrebrewingPeriod_2( BYTE frameStr[] )
{
	int funcCode = 0x04;
	int startAddr = 0x5C;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_SlidingdoorOpenCurrent( BYTE frameStr[] )				// @ add 18/04/08
{
	int funcCode = 0x04;
	int startAddr = 0x5D;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_SlidingdoorShutCurrent( BYTE frameStr[] )					// @ add 18/04/08
{
	int funcCode = 0x04;
	int startAddr = 0x5E;
	int dataLength = 0x01;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}

int ConfigQuery_Customized(BYTE frameStr[], int startAddr, int dataLength)
{
	int funcCode = 0x04;
	int frameLength = packReqQuery( frameStr, funcCode, startAddr, dataLength );
	return frameLength;
}
// @_@ end of config query
/*#############################################################################
#############################################################################*/
/*
#######################
##            Enable Query           ##
#######################
*/
int ConnQuery(BYTE frameStr[], int dataAddr,  int dataLength)
{
	int stateCode = 0x00;
	int frameLength = packEnableQuery( frameStr, stateCode, dataAddr, dataLength );
	return frameLength;
}

int EnableQuery(BYTE frameStr[], int dataAddr, int dataLength)
{
	int stateCode = 0x01;
	int frameLength = packEnableQuery( frameStr, stateCode, dataAddr, dataLength );
	return frameLength;
}
// @_@ end of enable query
/*#############################################################################
#############################################################################*/
/*
#######################
##            Recipe Query           ##
#######################
*/
int RecipeQuery(BYTE frameStr[],  int productId, int defaultRecipe)
{
	int frameLength = packRecipeQuery( frameStr, productId, defaultRecipe);
	return frameLength;
}
// @_@ end of recipe query
/*#############################################################################
#############################################################################*/


/*
#######################
##      Machine Operation       ##
#######################
*/
int Operation_MixerMotorPowerOn(BYTE frameStr[], int motorId, int current, int dir)
{
	int startAddr = motorId;
	int op1 = current;
	int op2 = dir;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_AirPumpPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x06;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}
int Operation_CanisterMotorPowerOn(BYTE frameStr[], int motorId, int current)
{
	int startAddr = motorId;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ESPistonMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x10;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ESFilterMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x11;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_FBWiperMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x12;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_FBFilterMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x13;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_GearPumpPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x16;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CupSeparatorMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x18;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CupStackMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x19;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CupCatcherMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x1A;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_BigDoorMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x1B;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_OutletDoorMotorPowerOn(BYTE frameStr[], int current)
{
	int startAddr = 0x1C;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}


int Operation_OpenInletValve(BYTE frameStr[])
{
	int startAddr = 0x20;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_OpenColdWaterInletValve(BYTE frameStr[])
{
	int startAddr = 0x21;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_OpenES_2P3W_Valve(BYTE frameStr[])
{
	int startAddr = 0x22;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ESPistonMotorPowerOn_2(BYTE frameStr[], int current)
{
	int startAddr = 0x24;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ESFilterMotorPowerOn_2(BYTE frameStr[], int current)
{
	int startAddr = 0x25;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_FBWiperMotorPowerOn_2(BYTE frameStr[], int current)
{
	int startAddr = 0x26;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_FBFilterMotorPowerOn_2(BYTE frameStr[], int current)
{
	int startAddr = 0x27;
	int op1 = current;
	int op2 = 0x01;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_Open_2P2W_Valve(BYTE frameStr[], int valveId)
{
	int startAddr = valveId + 0x27;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}


int Operation_CleanMachine(BYTE frameStr[])
{
	int startAddr = 0x30;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_Desinfection(BYTE frameStr[])
{
	int startAddr = 0x31;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CupDispenserTest(BYTE frameStr[], int cupCount)
{
	int startAddr = 0x32;
	int op1 = cupCount % 256;
	int op2 = cupCount / 256;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_PutHotWater(BYTE frameStr[], int WaterVolume)
{
	int startAddr = 0x33;
	int op1 = WaterVolume % 256;
	int op2 = WaterVolume / 256;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_MDBControl(BYTE frameStr[], int OnOff)
{
	int startAddr = 0x34;
	int op1 = 0x00;
	int op2 = OnOff;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CoinReturn(BYTE frameStr[], int CoinCount)
{
	int startAddr = 0x35;
	int op1 = CoinCount % 256;
	int op2 = CoinCount / 256;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_DoorControl(BYTE frameStr[], int OpenShut)
{
	int startAddr = 0x36;
	int op1 = 0x00;
	int op2 = OpenShut;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_SystemReset(BYTE frameStr[], int resetType)		//@ mod 18/05/15  V4.6
{
	int startAddr = 0x37;
	int op1 = 0x00;
	int op2 = resetType;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ESBrewerControl(BYTE frameStr[], int controlType, int ES_No)
{
	int startAddr = 0x38;
	int op1 = ES_No;
	int op2 = controlType;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_PowderOutTest(BYTE frameStr[], int canisterId, int time)
{
	int startAddr = 0x39;
	int op1 = canisterId;
	int op2 = time;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_GrinderTest(BYTE frameStr[], int time)
{
	int startAddr = 0x3A;
	int op1 = 0x00;
	int op2 = time;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CupHolderMove(BYTE frameStr[], int direction)
{
	int startAddr = 0x3B;
	int op1 = 0x00;
	int op2 = direction;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_DropTeaTest(BYTE frameStr[], int time)
{
	int startAddr = 0x3C;
	int op1 = 0x00;
	int op2 = time;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_FBBrewerControl(BYTE frameStr[], int controlType, int FB_No)
{
	int startAddr = 0x3D;
	int op1 = FB_No;
	int op2 = controlType;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_CupCatcherTest(BYTE frameStr[], int cupCount)
{
	int startAddr = 0x3E;
	int op1 = cupCount % 256;
	int op2 = cupCount / 256;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_SmallDoorControl(BYTE frameStr[], int OpenShut)
{
	int startAddr = 0x3F;
	int op1 = 0x00;
	int op2 = OpenShut;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_AirbreakEmpty(BYTE frameStr[])
{
	int startAddr = 0x40;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_BoilerEmpty(BYTE frameStr[])
{
	int startAddr = 0x41;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_AirPumpTest(BYTE frameStr[], int current)
{
	int startAddr = 0x42;
	int op1 = current;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ExitTest(BYTE frameStr[])
{
	int startAddr = 0x43;
	int op1 = 0x00;
	int op2 = 0x00;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_LampController(BYTE frameStr[], int lampType, int OnOff)		// @ add 18/03/02
{
	int startAddr = 0x46;
	int op1 = lampType;
	int op2 = OnOff;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ModuleESClean(BYTE frameStr[], int cleanTimes, int ES_No)
{
	int startAddr = 0x47;
	int op1;
	if (ES_No == 0){
		op1 = 0xAA;
	}else{
		op1 = 0xBB;
	}
	int op2 = cleanTimes;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ModuleFBClean(BYTE frameStr[], int cleanTimes, int FB_No)
{
	int startAddr = 0x47;
	int op1;
	if (FB_No == 0){
		op1 = 0xCC;
	}else{
		op1 = 0xDD;
	}
	int op2 = cleanTimes;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ModuleMixerClean(BYTE frameStr[], int mixerId, int cleanTimes)
{
	int startAddr = 0x47;
	int op1 = mixerId;
	int op2 = cleanTimes;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_IceController(BYTE frameStr[], int controlType, int duration)					// @ add 18/03/02   ***** ¿É±» Operation_BIBController ½Ó¿Ú¸²¸Ç
{
	int startAddr = 0x48;
	int op1 = controlType;
	int op2 = duration;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_BIBController(BYTE frameStr[], int controlType, int duration)					// @ add 18/03/02
{
	int startAddr = 0x48;
	int op1 = controlType;
	int op2 = duration;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ModuleBIBClean(BYTE frameStr[], int pipID, int cleanTimes)							// @ add 18/03/02
{
	int startAddr = 0x49;
	int op1 = pipID;
	int op2 = cleanTimes;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_BIBComponentTest(BYTE frameStr[], int componentID, int current)		// @ add 18/03/02
{
	int startAddr = 0x4A;
	int op1 = componentID;
	int op2 = current;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_BIBQuantifyTest(BYTE frameStr[], int BIBId, int current, int duration)		// 0x4B ~ 0x52		@ add 18/03/02
{
	int startAddr = BIBId + 0x4A;
	int op1 = current;
	int op2 = duration;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_ModuleLifeTest(BYTE frameStr[], int moduleId, int testTimes)					// 0x53		@ add 18/03/02
{
	int startAddr = 0x53;
	int op1 = moduleId;
	int op2 = testTimes;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}

int Operation_GeneralDynamicTest(BYTE frameStr[], int orderType, int time)				// 0x54		@ add 18/03/02
{
	int startAddr = 0x54;
	int op1 = orderType;
	int op2 = time;
	int frameLength = packMachineOperation( frameStr, startAddr, op1, op2);
	return frameLength;
}
// @_@ end of machine operation
/*#############################################################################
#############################################################################*/
/*
#######################
##            Make Product          ##
#######################
*/
int Operation_MakeProduct(BYTE frameStr[])															// @ Mod 18/03/02
{
	int type = 0x05;
	int frameLength = packMakeProduct(frameStr, canister_set, type);
	memset(canister_set, 0, sizeof(canister_set));
	return frameLength;
}


int Operation_MakeProductWithoutDispensing(BYTE frameStr[])							// @ add 18/03/02
{
	int type = 0x0E;
	int frameLength = packMakeProduct(frameStr, canister_set, type);
	memset(canister_set, 0, sizeof(canister_set));
	return frameLength;
}
// @_@ end of make product
/*#############################################################################
#############################################################################*/
/*
#######################
##    Make Product by id        ##
#######################
*/
int Operation_MakeProductById( BYTE frameStr[], int productId )
{
	int frameLength = packMakeProductById(frameStr, productId);
	memset(canister_set, 0, sizeof(canister_set));
	return frameLength;
}
// @_@ end of make product by id
/*#############################################################################
#############################################################################*/



/*
#######################
##          Config Setting           ##
#######################
*/
int ConfigSetting_HotWaterTemp( BYTE frameStr[], int temp, int temporary )
{
	int startAddr = 0x00;
	int dataLength = 0x02;
	int data = temp;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_ColdWaterTemp(BYTE frameStr[], int temp, int temporary)
{
	int startAddr = 0x02;
	int dataLength = 0x02;
	int data = temp;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerPressure(BYTE frameStr[], int press, int temporary)
{
	int startAddr = 0x04;
	int dataLength = 0x02;
	int data = press;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_AutoFlushTimeInterval(BYTE frameStr[], int timeInterval, int temporary)
{
	int startAddr = 0x06;
	int dataLength = 0x02;
	int data = timeInterval;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerTemp(BYTE frameStr[], int temp, int temporary)
{
	int startAddr = 0x08;
	int dataLength = 0x02;
	int data = temp;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerSqueezeForce(BYTE frameStr[], int force, int temporary)
{
	int startAddr = 0x0A;
	int dataLength = 0x02;
	int data = force;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerSqueezePeriod(BYTE frameStr[], int period, int temporary)
{
	int startAddr = 0x0C;
	int dataLength = 0x02;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerRestorationPeriod(BYTE frameStr[], int period, int temporary)
{
	int startAddr = 0x0E;
	int dataLength = 0x02;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerLastSqueezePeriod(BYTE frameStr[], int period, int temporary)
{
	int startAddr = 0x10;
	int dataLength = 0x02;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_CatcherThreshold(BYTE frameStr[], int threshold, int temporary)
{
	int startAddr = 0x14;
	int dataLength = 0x01;
	int data = threshold;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_DispenserThreshold(BYTE frameStr[], int threshold, int temporary)
{
	int startAddr = 0x15;
	int dataLength = 0x01;
	int data = threshold;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_RedispensingTimes(BYTE frameStr[], int times, int temporary)
{
	int startAddr = 0x17;
	int dataLength = 0x01;
	int data = times;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_ValveClosingDelayTime(BYTE frameStr[], int time, int temporary)
{
	int startAddr = 0x1A;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_GearPumpMaxPower(BYTE frameStr[], int maxPower, int temporary)
{
	int startAddr = 0x1B;
	int dataLength = 0x01;
	int data = maxPower;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_MixerOpenningDelayTime(BYTE frameStr[], int time, int temporary)
{
	int startAddr = 0x1C;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_MixerClosingDelayTime(BYTE frameStr[], int time, int temporary)
{
	int startAddr = 0x1D;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FanSpeed(BYTE frameStr[], int speed, int temporary)
{
	int startAddr = 0x1E;
	int dataLength = 0x01;
	int data = speed;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FBBrewerAirPumpSpeed(BYTE frameStr[], int speed, int temporary)
{
	int startAddr = 0x1F;
	int dataLength = 0x01;
	int data = speed;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FBBrewerEffluentTimeInterval(BYTE frameStr[], int timeInterval, int temporary)
{
	int startAddr = 0x20;
	int dataLength = 0x01;
	int data = timeInterval;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_AirPumpRunningTime(BYTE frameStr[], int time, int temporary)
{
	int startAddr = 0x21;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_ResidualPowderRemoveTime(BYTE frameStr[], int time, int temporary)										// 0x22 + 0x01   	@ add 18/03/02
{
	int startAddr = 0x22;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_ResidualPowderRemoveTimeInterval(BYTE frameStr[], int timeInterval, int temporary)				// 0x23 + 0x01   	@ add 18/03/02
{
	int startAddr = 0x23;
	int dataLength = 0x01;
	int data = timeInterval;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_UVLightOnPeriod(BYTE frameStr[], int period, int temporary)
{
	int startAddr = 0x28;
	int dataLength = 0x01;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_UVLightOffPeriod(BYTE frameStr[], int period, int temporary)
{
	int startAddr = 0x29;
	int dataLength = 0x01;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_PrebrewingWaterScale(BYTE frameStr[], int scale, int temporary)
{
	int startAddr = 0x31;
	int dataLength = 0x01;
	int data = scale;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_PrebrewingPeriod(BYTE frameStr[], int period, int temporary)
{
	int startAddr = 0x32;
	int dataLength = 0x01;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FlushOnBoot(BYTE frameStr[], int isFlush, int temporary)
{
	int startAddr = 0x33;
	int dataLength = 0x01;
	int data = isFlush;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

// @add 2017/11/14
int ConfigSetting_MachineID( BYTE frameStr[], BYTE midStr[], int temporary )
{
	static BYTE strBuff[20];
	memset(strBuff,0,20);
	memcpy(strBuff, midStr, 20);
	int startAddr = 0x34;
	int dataLength = 0x14;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, strBuff, temporary);
	return frameLength;
}

int ConfigSetting_PowderOutDelay( BYTE frameStr[], int time, int temporary )
{
	int startAddr = 0x48;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_PowderStopAhead( BYTE frameStr[], int time, int temporary )
{
	int startAddr = 0x49;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_CurrentSpeedThreshold( BYTE frameStr[], int speed, int temporary )
{
	int startAddr = 0x4A;
	int dataLength = 0x01;
	int data = speed;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_LowSpeedCheckPeriod( BYTE frameStr[], int time, int temporary )
{
	int startAddr = 0x4B;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BlockCheckPeriod( BYTE frameStr[], int time, int temporary )
{
	int startAddr = 0x4C;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_WaterTankFillingCheckPeriod( BYTE frameStr[], int time, int temporary )
{
	int startAddr = 0x4D;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerPressure_2( BYTE frameStr[], int press, int temporary )
{
	int startAddr = 0x4E;
	int dataLength = 0x02;
	int data = press;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerSqueezeForce_2( BYTE frameStr[], int force, int temporary )
{
	int startAddr = 0x50;
	int dataLength = 0x02;
	int data = force;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerSqueezePeriod_2( BYTE frameStr[], int period, int temporary )
{
	int startAddr = 0x52;
	int dataLength = 0x02;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerRestorationPeriod_2( BYTE frameStr[], int period, int temporary )
{
	int startAddr = 0x54;
	int dataLength = 0x02;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_BrewerLastSqueezePeriod_2( BYTE frameStr[], int period, int temporary )
{
	int startAddr = 0x56;
	int dataLength = 0x02;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FBBrewerAirPumpSpeed_2( BYTE frameStr[], int speed, int temporary )
{
	int startAddr = 0x58;
	int dataLength = 0x01;
	int data = speed;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FBBrewerEffluentTimeInterval_2( BYTE frameStr[], int timeInterval, int temporary )
{
	int startAddr = 0x59;
	int dataLength = 0x01;
	int data = timeInterval;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_AirPumpRunningTime_2( BYTE frameStr[], int time, int temporary )
{
	int startAddr = 0x5A;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_PrebrewingWaterScale_2( BYTE frameStr[], int scale, int temporary )
{
	int startAddr = 0x5B;
	int dataLength = 0x01;
	int data = scale;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_PrebrewingPeriod_2( BYTE frameStr[], int period, int temporary )
{
	int startAddr = 0x5C;
	int dataLength = 0x01;
	int data = period;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_SlidingdoorOpenCurrent(BYTE frameStr[], int current, int temporary)						// 0x5D + 0x01   	@ add 18/03/02
{
	int startAddr = 0x5D;
	int dataLength = 0x01;
	int data = current;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_SlidingdoorShutCurrent(BYTE frameStr[], int current, int temporary)							// 0x5E + 0x01   	@ add 18/03/02
{
	int startAddr = 0x5E;
	int dataLength = 0x01;
	int data = current;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_LEDColor(BYTE frameStr[], int r, int g, int b, int temporary)											// 0x5F + 0x03   	@ add 18/05/15  V4.6
{
	static BYTE dataBuff[3];
	memset(dataBuff,0,3);
	// b,r,g
	memcpy(dataBuff, &b, 1);
	memcpy(dataBuff+1, &r, 1);
	memcpy(dataBuff+2, &g, 1);
	int startAddr = 0x5F;
	int dataLength = 0x03;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, dataBuff, temporary);
	return frameLength;
}

int ConfigSetting_LEDSegment(BYTE frameStr[], int segment, int temporary)											// 0x62 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x62;
	int dataLength = 0x01;
	int data = segment;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FB1WaterSection1Ratio(BYTE frameStr[], int ratio, int temporary)								// 0x63 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x63;
	int dataLength = 0x01;
	int data = ratio;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FB2WaterSection1Ratio(BYTE frameStr[], int ratio, int temporary)								// 0x64 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x64;
	int dataLength = 0x01;
	int data = ratio;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FBBrewTime2Enable(BYTE frameStr[], int enable, int temporary)									// 0x65 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x65;
	int dataLength = 0x01;
	int data = enable;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FB1BrewTime2(BYTE frameStr[], int time, int temporary)												// 0x66 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x66;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FB2BrewTime2(BYTE frameStr[], int time, int temporary)												// 0x67 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x67;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_FBWipingSpeed(BYTE frameStr[], int speed, int temporary)											// 0x68 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x68;
	int dataLength = 0x01;
	int data = speed;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_IceMakerEnable(BYTE frameStr[], int enable, int temporary)											// 0x69 + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x69;
	int dataLength = 0x01;
	int data = enable;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_DoorOpenDelayTime(BYTE frameStr[], int time, int temporary)									// 0x6A + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x6A;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

int ConfigSetting_NullCupCheckFlashingTime(BYTE frameStr[], int time, int temporary)							// 0x6B + 0x01   	@ add 18/05/15  V4.6
{
	int startAddr = 0x6B;
	int dataLength = 0x01;
	int data = time;
	int frameLength = packConfigSetting( frameStr, startAddr, dataLength, data, temporary);
	return frameLength;
}

// @_@ end of config setting
/*#############################################################################
#############################################################################*/
/*
#######################
##          Recipe Setting           ##
#######################
*/
int RecipeSetting(BYTE frameStr[], int productId)
{
	int frameLength = packRecipeSetting(frameStr, productId, canister_set);
	memset(canister_set, 0, sizeof(canister_set));
	return frameLength;
}
// @_@ end of recipe setting
/*#############################################################################
#############################################################################*/
/*
#######################
##            RFID  Setting            ##
#######################
*/
int RFIDSetting(BYTE frameStr[], int devType, BYTE password[])
{
	int frameLength = packRFIDSetting(frameStr, devType, password);
	return frameLength;
}

// @_@ end of recipe setting
/*#############################################################################
#############################################################################*/
/*
#######################
##            IAP  Upgrade            ##
#######################
*/
int PreUpgradeOrder(BYTE frameStr[])													// @ add 18/03/02
{
	int selector = 0x00;
	int frameLength = packIAPUpgrade( frameStr, selector );
	return frameLength;
}

int IAPUpgrade(BYTE frameStr[], int unitType)										// @ add 18/03/02
{
	BYTE orderFrame[] = { 0xEE, 0x00, 0x01, 0x00, 0x00, 0xC5, 0xAD }; 
	orderFrame[4] = (BYTE)unitType;
	int dataLength = sizeof(orderFrame);
	memcpy(frameStr, orderFrame, dataLength);
	return dataLength;
}

// @_@ end of IAP upgrade
/*#############################################################################
#############################################################################*/

