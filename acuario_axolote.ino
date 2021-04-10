#include "constantes_privadas.h"
#include "constantes.h"
#include "librerias.h"
#include "utils.h"

// Se envia la referencia OneWire a DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensores(&oneWire); 

// * Variables globales *********************************************************************************************
int cuenta_alerta = 0;
int contador_trigger = 0;

// Inicializaion de banderas
boolean bandera_alerta = true;
boolean alerta_dtc_temperatura = false;
boolean bandera_controlador_temperatura = false;

ESP8266WiFiMulti wifi_manager;

void setup(void)
{
  // Declaracion de pushbutton para limpiar DTC
  pinMode(CLEAR_DTC, INPUT);

  // Declaracion de salida controlador temperatura
  pinMode(OUTPUT_TEMP, OUTPUT);

  // Inicio de comunicacion serial, para leer datos de diagnostico
  Serial.begin(115200);

  // Inicio display lcd
  lcd.init();
  lcd.backlight();
  
  // Configuracion WIFI
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WIFI");
  lcd.setCursor(0, 1);
  lcd.print(WIFISID);

  Serial.print("Conectandose a red: ");
  Serial.println(WIFISID);
  
  // Se intenta conectar a la red WIFI seleccionada:
  wifi_manager.addAP(WIFISID, WIFIPASS);
  while (wifi_manager.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Muestra en LCD conexion exitosa e inicializa DTC a 0
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectado :D");

  Serial.println("Conexion exitosa");
  
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("DTC:" + String(NO_ERROR));
}

void loop(void)
{ 
  delay(1000); 
  
  // * Medicion de temperatura **************************************************************************************
  
  sensores.requestTemperatures();              
  float temperatura = sensores.getTempCByIndex(0);

  // La temperatura está por encima del umbral
  if (temperatura > TEMPERATURA_ALERTA) 
  {  
    // Incrementa el contador para la alerta
    cuenta_alerta++;
  }
  else 
  {
    // Si esta por debajo de la temperatura maxima reninicia los contadores
    cuenta_alerta = 0;
    contador_trigger =0; 
    bandera_alerta = true;
  }

  // La temperatura ha estado por encima del umbral de alerta el tiempo requerido para lanzar una alerta
  if (cuenta_alerta >= LECTURAS_PARA_ALERTA)
  {  
    if (bandera_alerta)
    {
      // Configura los contadores para enviar la siguiente alerta
      bandera_alerta = false;
      contador_trigger = 0;

      // Envia la alerta por a IFTTT para enviar mensaje de Telegram
      enviar_alerta("Alerta%20temperatura", String(temperatura) + "%20C");
    }
    
    // Si ha pasasdo el intervalo entre alertas, vuelve a acativar la bandera para enviar otra alerta
    if (contador_trigger >= INTERVALO_ENTRE_ALERTAS) bandera_alerta = true;
    else contador_trigger++;
  }
  
  // * Manejo de regulador de temperatura ***************************************************************************

  // La temperatura ha alcanzado el umbral bajo del regulador, este se apagará
  if (temperatura < TEMP_CONTROLADOR_OFF) bandera_controlador_temperatura = false;
  
  // Setea bandera para encender regulador de temperatura  
  if (temperatura >= TEMP_CONTROLADOR_ON) bandera_controlador_temperatura = true;

  // Controla el regulador de temperatura de acuerdo al estado de la bandera, el relevador se maneja por logica inversa
  if (bandera_controlador_temperatura) digitalWrite(OUTPUT_TEMP, HIGH);
  else digitalWrite(OUTPUT_TEMP, LOW);

  
  // * Manejo de DTCs ***********************************************************************************************
  
  // DTC 1 - Conexion de red perdida
  if (wifi_manager.run() != WL_CONNECTED) mostrar_dtc(ERROR_NO_RED);
  
  // DTC 3 - Error de sensor de temperatura
  if (temperatura == -127.00 || temperatura == 85.00) 
  {
    mostrar_dtc(ERROR_SENSOR_TEMP);

    // Envia una alerta si no se ha hecho antes
    if (!alerta_dtc_temperatura)
    { 
      enviar_alerta("Error%20Sensor", "Sensor%20Teperatura");
      alerta_dtc_temperatura = true;
    }  
  }

  // Se presionó el boton de CLEAR_DTC, se borra el DTC guardado
  if (digitalRead(CLEAR_DTC))
  {
    alerta_dtc_temperatura = false;
    mostrar_dtc(NO_ERROR);
  }
 
  // * Impresion de variables *************************************************************************************** 
  
  Serial.println("Temperaturatura: " + String(sensores.getTempCByIndex(0)));   
  mostrar_lcd("T:" + String(temperatura) + "  ");
}
