#include "Arduino.h"

#include "librerias.h"

// Instanciacion de LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void mostrar_dtc(String dtc)
{
  lcd.setCursor(0,1);
  lcd.print("DTC:" + dtc);
}

void mostrar_lcd(String mensaje, boolean limpiar=false, int fila=0, int columna=0)
{
  if (limpiar) lcd.clear();
  lcd.setCursor(columna, fila);
  lcd.print(mensaje);
}

void enviar_alerta(String tipo_alerta, String valor_alerta)
{
  mostrar_lcd("*", false, 1, 15);
  WiFiClient clienteWIFI;

  HTTPClient http;

  String request = "http://maker.ifttt.com/trigger/";
  request.concat(EVENTO);
  request.concat("/with/key/");
  request.concat(IFTTTKEY);
  request.concat("?value1=");
  request.concat(tipo_alerta);
  request.concat("&value2=");
  request.concat(valor_alerta);

  Serial.println("Enviando request a: "+ request);
  
  if(http.begin(clienteWIFI, request))
  {
    // Se inicia la conexion y se envia un HTTP Header
    int codigo_http = http.GET();

    // codigo_http sera negativo si hay un error
    if (codigo_http < 0)
    {
      Serial.println(codigo_http);
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(codigo_http).c_str());
    }

    http.end();
  }
  else
  {
    Serial.println("No se pudo conectar");
  }
  
  // Delay antes de salir de la funcion
  delay(1000);
  mostrar_lcd(" ", false, 1, 15);
}
