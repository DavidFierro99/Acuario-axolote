// Enumeracion para los niveles de DTC
#define NO_ERROR          "0"
#define ERROR_NO_RED      "1"
#define ERROR_NO_INTERNET "2"
#define ERROR_SENSOR_TEMP "3"

// Pin D3 Sensor temperatura
#define ONE_WIRE_BUS D3

// Pin boton clear DTC
#define CLEAR_DTC D6

// Pin boton salida controlador
#define OUTPUT_TEMP D8

// Constantes 
#define LECTURAS_PARA_ALERTA 15
#define INTERVALO_ENTRE_ALERTAS 300
#define TEMPERATURA_ALERTA 20
#define TEMP_CONTROLADOR_ON 19.5
#define TEMP_CONTROLADOR_OFF 17.5
