# Overview

El sistema funciona mediante modulos intercomunicados por named pipes.

Adquisicion -> Almacenamiento -> Procesamiento

* Adquisicion : es el encargado de comunicarse con la placa y enviar chunks, no acepta ningun tipo de comunicacion.

* Almacenamiento : es el encargado de almacenar y enviar los datos a procesar, si le llega una alarma debe guardar los datos crudos.

* Procesamiento : es el encargado de procesar los chunk y generar alarmas.
