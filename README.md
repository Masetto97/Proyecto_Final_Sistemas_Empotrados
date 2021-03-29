# Proyecto Final Sistemas Empotrados

Se desea hacer un reloj con alarma y medida de temperatura. El funcionamiento del sistema será el siguiente. Cuando se presione el pulsador SA-1, se mostrará en el led de 7 segmentos la temperatura. Cuando se presione el pulsador SA-2 se mostrará la hora.

La hora se mostrará en formato HH:MM.

Será posible la configuración de la hora actual y de la hora de alarma en el dispositivo. Para entrar en el modo de configuración, será necesario presionar el pulsador SA-3 durante 2 segundos. En ese momento, se enviará al PC, por el puerto serie, el siguiente mensaje: “introduzca la hora” Se deberá introducir entonces la hora en formato HH:MM, utilizando para ello los comandos SebdBlock y RecvBlock. Una vez hecho esto se mostrará por el puerto serie el mensaje “Introduzca la hora de la alarma”, se introducirá entonces la hora de la alarma también en formato HH:MM, que se enviará por el puerto serie.

Cuando la hora del reloj coincida con la hora de alarma, se activará de manera intermitente el zumbador del Shield y se iluminarán los cuatro leds del shield de manera intermitente.
