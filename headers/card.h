#ifndef STDLIBS_H
#define STDLIBS_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <conio.h>
#include <windows.h>

#ifndef WD_DASK_H
#define WD_DASK_H
#include "WD-dask64.h"
#endif


/*
 * setup buffer: inicializa los buffers de la placa.
 * cardnumber: la placa cuyos buffers queremos inicializar.
 * pointsperchunk: tamanio del buffer (habria que renombrarlo a buffersize)
 * ch0_buff: puntero al buffer
 * ch1_buff: puntero al segundo buffer (podriamos sacarlo y que tengas que llamarlo dos veces)
 * Id: id del buffer
 */
void setup_buffers(int cardnumber, int pointsperchunk, I16 *ch0_buff, I16 *ch1_buff, I16 Id);

/*
 * allocate buffers: aloca la memoria para los buffers de la placa.
 * cardnumber: la placa que queremos inicializar.
 * buffersize: tamanio del buffer.
 * ch0_buff: puntero al puntero del buffer.
 * ch1_buff: puntero al puntero del segundo buffer (podriamos sacarlo y que tengas que llamarlo dos veces).
 */
void allocate_buffers(int cardnumber, int buffersize, I16 **ch0_buff, I16 **ch1_buff);

/*
 * configura la placa 
 * cardnumber: la placa que queremos configurar
 */
void configure_card(I16 cardnumber);

/*
 * register card: registra la placa
 * actualmente solo registra la placa 0, deberiamos hacer que pida que placa queremos como para en un futuro soportar varias placas.
 */
I16 register_card();
