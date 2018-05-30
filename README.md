# SistMushrooms
Proyecto de diseño de un sistema automatizado de monitoreo y control para un cultivo de champiñones empleando un microcontrolador ARM Cortex M4 de la empresa ST Microelectronics en su tarjeta de desarrollo Nucleo-F401RE

Se encarga del control de las variables mas importantes de la siembra a traves de diferentes actuadores y se adquiere la data a partir de una serie de sensores los cuales se conectan con el microcontrolador por medio de distintos protocolos de comunicacion.
Se realiza un envio de datos a traves de un modulo 2G GPRS/GSM de la empresa SIMCOM el cual envia mensajes de texto con datos solicitados de la siembra y envia paquete de datos a una pagina web de IoT llamada Thingspeak en donde se encuentra un canal publico con diferentes campos en los cuales se alojan las diferentes variables de la siembra
