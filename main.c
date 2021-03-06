#include "main.h"

struct callback_data *callback_data;
int chunk_index;
HANDLE ch0_pipe; 

int main(){
	struct config *config;
	int pointsperchunk;
	int buffersize;
	I16 cardnumber;
	I16 Id;
	I16 err;
	I16 call;

	chunk_index = 0;

	config = malloc(sizeof(struct config));
	read_config("config.ini", config);
	callback_data = malloc(sizeof(struct callback_data));
	callback_data->config = config;

	cardnumber = register_card();
	callback_data->cardnumber = cardnumber;
	pointsperchunk = config->bins * config->nshotsperchunk;
	buffersize = pointsperchunk * sizeof(I16);
	configure_card(cardnumber);
	allocate_buffers(cardnumber, buffersize, &(callback_data->channel0_buffer), &(callback_data->channel1_buffer));
	setup_buffers(cardnumber, pointsperchunk, callback_data->channel0_buffer, callback_data->channel1_buffer, Id);
	call = WD_AI_EventCallBack_x64(cardnumber, 1, TrigEvent, (ULONG_PTR)callback); // Agregar error checking

	ch0_pipe =  CreateNamedPipe(TEXT("\\\\.\\pipe\\ch0_raw_pipe"),
                            PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                            PIPE_WAIT,
                            1,
                            buffersize,
                            0,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);

	if (ch0_pipe == INVALID_HANDLE_VALUE){
		printf("\nPipe failed ERR=%d", GetLastError());
		_getch();
		exit(-2);
	}

	err = WD_AI_ContReadChannel(cardnumber, 0, 0, pointsperchunk, 1, 1, ASYNCH_OP);
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
	CloseHandle(ch0_pipe);
	printf("\nCount: %d \n Startpos: %d", startpos, count);
	printf("\nPress ENTER to exit the program. ");
	_getch();
	exit(1);
}

void callback(){
	I16 *buffer = callback_data->channel0_buffer;
	int pointsperchunk = callback_data->config->nshotsperchunk * callback_data->config->bins;
	int buffersize = pointsperchunk * sizeof(I16);

	WriteFile(ch0_pipe, tbuffer, tbuffersize, byteswritten, NULL);
	FlushFileBuffers(ch0_pipe);

	printf("\nSent chunk size: %d", buffersize);
}
