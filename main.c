#include "main.h"

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
}

int main(){
	struct config *config;
	int pointsperchunk;
	int buffersize;
	I16 cardnumber;
	I16 Id;
	I16 err;
	config = malloc(sizeof(struct config));
	read_config("config.ini", config);
	printf("\nConfig has been read");
	callback_data = malloc(sizeof(struct callback_data));
	callback_data->config = config;

	cardnumber = register_card();
	callback_data->cardnumber = cardnumber;
	printf("\nRegistered card");

	pointsperchunk = config->nch * config->bins * config->nshotsperchunk / config->nsubchunk;
	buffersize = pointsperchunk * sizeof(I16);

	allocate_buffers(cardnumber, buffersize, &(callback_data->channel0_buffer), &(callback_data->channel1_buffer));
	printf("\nBuffers allocated");

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