#include "main.h"

struct config{
	int nch;
	int qfreq;
	int bins;
	int nshotsperchunk;
	int nsubchunk;
};

struct callback_data{
	struct config *config;
	I16 cardnumber;
	I16 *channel0_buffer;
	I16 *channel1_buffer;
};

struct callback_data *callback_data;

void callback(void *data){
	printf("\ncallback");
	printf(" %d ", callback_data->cardnumber);
	I16 *buffer = callback_data->channel0_buffer;
	int buffersize = callback_data->config->bins;
	double voltage;
	for(int i = 0; i < buffersize; i++){
		WD_AI_VoltScale(callback_data->cardnumber, AD_B_2_V, buffer[i], &voltage);
		printf(" voltage: %f", voltage);
	}
	//hacer algo con los buffers
	Sleep(100);
	//FILE *file = fopen("file","a");
	//fprintf(file,"callback");
	//fclose(file);
}

void setup_buffers(int cardnumber, int pointsperchunk, I16 *ch0_buff, I16 *ch1_buff, I16 Id){
	I16 err;
	err = WD_AI_ContBufferSetup(cardnumber, ch0_buff, pointsperchunk, &Id);
	if(err!=0){
		printf("Error seting up buffers ch0 err:%d, id:%d card:%d", err, Id, cardnumber);		
		WD_Buffer_Free (cardnumber, ch0_buff);
		WD_Buffer_Free (cardnumber, ch1_buff);
		WD_Release_Card(cardnumber);
		_getch();
    	exit(1);
	}
	err = WD_AI_ContBufferSetup(cardnumber, ch1_buff, pointsperchunk, &Id);
	if(err!=0){
		printf("Error seting up buffers ch1 err:%d", err);
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
		printf("failed allocating channel 0 buffsiz:%d", buffersize);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	*ch1_buff = WD_BufferAllocEx(cardnumber, buffersize);
	if(!*ch1_buff){
		printf("failed allocating channel 1");
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
		printf("WD_AI_Config error = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	//err = WD_AI_Trig_Config( cardnumber, WD_AI_TRGMOD_DELAY, WD_AI_TRGSRC_ExtD, WD_AI_TrgPositive, CH0ATRIG, 0.0, 0, 0, 0, 0);
	err = WD_AI_Trig_Config(cardnumber, WD_AI_TRGMOD_POST, WD_AI_TRGSRC_ANA, WD_AI_TrgNegative, 0, 0.0, 0, 0, 0, 0);
	if(err!=0){
		printf("WD_AI_Trig_Config = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	err = WD_AI_CH_ChangeParam(cardnumber, All_Channels, AI_IMPEDANCE, IMPEDANCE_50Ohm);
	if(err!=0){
		printf("WD_AI_CH_ChangeParam changing impedance failed = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
	err = WD_AI_CH_ChangeParam(cardnumber, All_Channels, AI_RANGE, AD_B_2_V);
	if(err!=0){
		printf("WD_AI_CH_ChangeParam changing voltage failed = %d", err);
		WD_Release_Card(cardnumber);
		_getch();
		exit(1);
	}
}

I16 register_card(){
    I16 card = WD_Register_Card(PCIe_9852, 0);
    if (card < 0){
        printf("Register_Card error=%d", card);
		_getch();
        exit(-1);
    }
    return card;
}

void read_config(char * filename, struct config *config){
    char *line = malloc(sizeof(char) * 200);
    char *value = malloc(sizeof(char) * 200);
    FILE *file_config = fopen(filename, "r");

    while(fscanf(file_config, "%200s %200s", line, value) != EOF){
        if (strcmp(line, "NCh") == 0)
            config->nch = atoi(value);
        if (strcmp(line, "qFreq") == 0)
            config->qfreq = atoi(value);
        if (strcmp(line, "Bins") == 0)
            config->bins = atoi(value);
        if (strcmp(line, "NShotsChk") == 0)
            config->nshotsperchunk = atoi(value);
        if (strcmp(line, "NSubChk") == 0)
            config->nsubchunk = atoi(value);
    }
}

int main(){
	struct config *config;
	int pointsperchunk;
	int buffersize;
	I16 Id;
	I16 err;
	config = malloc(sizeof(struct config));
	read_config("config.ini", config);
	printf("\nConfig has been read");
	callback_data = malloc(sizeof(struct callback_data));
	callback_data->config = config;

	I16 cardnumber = register_card();
	callback_data->cardnumber = cardnumber;
	printf("\nRegistered card");

	pointsperchunk = config->nch * config->bins * config->nshotsperchunk / config->nsubchunk;
	buffersize = pointsperchunk * sizeof(I16);

	allocate_buffers(cardnumber, buffersize, &(callback_data->channel0_buffer), &(callback_data->channel1_buffer));
	printf("\nBuffers allocated");
	printf("\nPointsPerChunk: %d BufferSize: %d", pointsperchunk, buffersize);

	configure_card(cardnumber);
	printf("\nCard configured");

	setup_buffers(cardnumber, pointsperchunk, callback_data->channel0_buffer, callback_data->channel1_buffer, Id);
	printf("\nBuffers setup");

	printf("\nTrying to setup callback");
	//I16 call = WD_AI_EventCallBackEx_x64(cardnumber, 1, TrigEvent, (ULONG_PTR) callback, callback_data);
	I16 call = WD_AI_EventCallBack_x64(cardnumber, 1, TrigEvent, (ULONG_PTR)callback);
	printf("\nCallback registered %d", call);
	
	err = WD_AI_ContReadChannel(cardnumber, 0, 0, config->bins, 1, 1, ASYNCH_OP);
	if(err!=0){
		printf("Error Scanning channels err:%d", err);
		WD_AI_ContBufferReset (cardnumber);
		WD_Buffer_Free (cardnumber, callback_data->channel0_buffer);
		WD_Buffer_Free (cardnumber, callback_data->channel1_buffer);
		WD_Release_Card(cardnumber);
    	exit(1);
	}
	printf("\nScanning channels");

	_getch();
	U64 count, startpos;
	WD_AI_AsyncClear64(cardnumber, &startpos, &count);
   	WD_AI_ContBufferReset(cardnumber);
   	WD_Buffer_Free(cardnumber, callback_data->channel0_buffer);
   	WD_Buffer_Free(cardnumber, callback_data->channel1_buffer);
   	err = WD_Release_Card(cardnumber);
	printf("\nCount: %d \n Startpos: %d", startpos, count);
   	printf("\nPress ENTER to exit the program. ");
	_getch();
	exit(1);
}