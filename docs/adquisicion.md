# Adquisicion

## Descripción breve del sistema

### main.c

Main.c se encarga de inicializar la placa, levantar la configuracion y enviar los datos por el pipe.

### card.c

Capa de abstraccion que se encarga del registro, configuracion y comunicacion de la placa ADLINK. leaky abstraction... a mejorar.

### config.c

Capa de abstraccion encargada de levantar la configuración del archivo config.ini.

Expone la función "read_config(char * filename, struct config *config)" y el struct "config".

#### config.ini

llave valor_deseado
