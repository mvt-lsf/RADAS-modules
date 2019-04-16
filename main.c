#include <main.h>

struct config{
    int nch;
    int qfreq;
    int bins;
    int nshotsperchunk;
    int nsubchunk;
};

struct callback_data{
	struct config *config;
	I16 *channel0_buffer;
	I16 *channel1_buffer;
};

void callback(){
	//hacer algo con los buffers
}

int main(){
    struct config *config;
	struct callback_data *callback_data;
	int pointsperchunk;
	int buffersize;
    read_config("config.ini", config);

	
	callback_data = malloc(sizeof(callback_data));
	callback_data->config = config;

    I16 cardnumber = register_card();
    configure_card(cardnumber);
    pointsperchunk = config->nch * config->bins * config->nshotsperchunk / config->nsubchunk;
    buffersize = pointsperchunk * sizeof(I16);
    callback_data->channel0_buffer = WD_Buffer_Alloc(cardnumber, buffersize);
    callback_data->channel1_buffer = WD_Buffer_Alloc(cardnumber, buffersize);
	WD_AI_ContBufferSetup64(cardnumber, callback_data->channel0_buffer, pointsperchunk, Id);
	WD_AI_ContBufferSetup64(cardnumber, callback_data->channel1_buffer, pointsperchunk, Id);
	WD_AI_EventCallBackEx_x64(cardnumber, 1, DAQEnd, callback, callback_data);
	WD_AI_ContScanChannels64(cardnumber, 1, 0, config->bins, config->qfreq, config->qfreq, ASYNCH_OP);
}

void setup_buffers(int cardnumber, int buffersize, I16 *ch0_buff, I16 *ch1_buff){
	ch0_buff = WD_Buffer_Alloc(cardnumber, buffersize);
    ch1_buff = WD_Buffer_Alloc(cardnumber, buffersize);
}

void configure_card(I16 cardnumber){
    WD_AI_Config(cardnumber, WD_IntTimeBase, TRUE, WD_AI_ADCONVSRC_TimePacer, FALSE, TRUE);
    WD_AI_Trig_Config(cardnumber, WD_AI_TRGMOD_DELAY, WD_AI_TRGSRC_ExtD, WD_AI_TrgPositive, CH0ATRIG, 0.0, 0, 0, 0, 0);
    WD_AI_CH_ChangeParam(cardnumber, All_Channels, AI_IMPEDANCE, IMPEDANCE_50Ohm);
    WD_AI_CH_ChangeParam(cardnumber, 0, AI_RANGE, AD_B_2_V);
	WD_AI_CH_ChangeParam(cardnumber, 1, AI_RANGE, AD_B_2_V);
}

I16 register_card(){
    I16 card = WD_Register_Card(PCIe_9852, 0);
    if (card < 0){
        printf("Register_Card error=%d", card);
        exit(-1);
    }
    return card;
}

void read_config(char * filename, struct config *config){
    config = malloc(sizeof(struct config));
    char *line = malloc(sizeof(char) * 200);
    char *value = malloc(sizeof(char) * 200);
    FILE *file_config = fopen(filename, "r");

    while(fscanf(file_config, "%200s %200s", line, value) != EOF){
        if (strcmp(line,"NCh") == 0)
            config->nch = atoi(value);
        if (strcmp(line,"qFreq") == 0)
            config->qfreq = atoi(value);
        if (strcmp(line,"Bins") == 0)
            config->bins = atoi(value);
        if (strcmp(line,"NShotsChk") == 0)
            config->nshotsperchunk = atoi(value);
        if (strcmp(line,"NSubChk") == 0)
            config->nsubchunk = atoi(value);
    }
}
