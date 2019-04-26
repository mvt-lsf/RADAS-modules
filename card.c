#include "card.h"

void setup_buffers(int cardnumber, int pointsperchunk, I16 *ch0_buff, I16 *ch1_buff, I16 Id){
	I16 err;
	err = WD_AI_ContBufferSetup(cardnumber, ch0_buff, pointsperchunk, &Id);
	if(err!=0){
		printf("\nError seting up buffers ch0 err:%d, id:%d card:%d", err, Id, cardnumber);		
		WD_Buffer_Free (cardnumber, ch0_buff);
		WD_Buffer_Free (cardnumber, ch1_buff);
		WD_Release_Card(cardnumber);
		_getch();
    	exit(1);
	}
	err = WD_AI_ContBufferSetup(cardnumber, ch1_buff, pointsperchunk, &Id);
	if(err!=0){
		printf("\nError seting up buffers ch1 err:%d", err);
		WD_AI_ContBufferReset (cardnumber);
		WD_Buffer_Free (cardnumber, ch0_buff);
		WD_Buffer_Free (cardnumber, ch1_buff);
		WD_Release_Card(cardnumber);
		_getch();
    	exit(1);
	}
}

void allocate_buffers(int cardnumber, int buffersize, I16 **ch0_buff, I16 **ch1_buff){
	*ch0_buff = WD_BufferAllocEx(cardnumber, buffersize);
	if(!*ch0_buff){
		printf("\nfailed allocating channel 0 buffsiz:%d", buffersize);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	*ch1_buff = WD_BufferAllocEx(cardnumber, buffersize);
	if(!*ch1_buff){
		printf("\nfailed allocating channel 1");
		WD_Buffer_Free(cardnumber, *ch0_buff);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	memset(*ch0_buff, 0, buffersize);
	memset(*ch1_buff, 0, buffersize);
}

void configure_card(I16 cardnumber){
	I16 err = WD_AI_Config(cardnumber, WD_IntTimeBase, 1, WD_AI_ADCONVSRC_TimePacer, 0, 1);
	if(err!=0){
		printf("\nWD_AI_Config error = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	//err = WD_AI_Trig_Config( cardnumber, WD_AI_TRGMOD_DELAY, WD_AI_TRGSRC_ExtD, WD_AI_TrgPositive, CH0ATRIG, 0.0, 0, 0, 0, 0);
	err = WD_AI_Trig_Config(cardnumber, WD_AI_TRGMOD_POST, WD_AI_TRGSRC_ExtD, WD_AI_TrgPositive, 0, 0.0, 0, 0, 0, 0);
	if(err!=0){
		printf("\nWD_AI_Trig_Config = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	err = WD_OutTrig_Config(cardnumber, 0, 0, WD_OutTrgPWidth_200ns);
	if (err!=0){
		printf("\nWD_OutTrig_Config = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	err = WD_AI_CH_ChangeParam(cardnumber, All_Channels, AI_IMPEDANCE, IMPEDANCE_50Ohm);
	if(err!=0){
		printf("\nWD_AI_CH_ChangeParam changing impedance failed = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	err = WD_AI_CH_ChangeParam(cardnumber, All_Channels, AI_RANGE, AD_B_2_V);
	if(err!=0){
		printf("\nWD_AI_CH_ChangeParam changing voltage failed = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
}

I16 register_card(){
    I16 card = WD_Register_Card(PCIe_9852, 0);
    if (card < 0){
        printf("\nRegister_Card error=%d", card);
		_getch();
        exit(-1);
    }
    return card;
}