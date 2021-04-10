# Acuario Axolote

Este proyecto busca establecer un controlador de las variables criticas para el desarrollo de un ambystoma mexicanum en un acuario.

Variables de interes
- Temperatura
- Amoniaco en el agua
- Nitritos y nitratos
- PH

Se busca que pueda proveer de informacion en tiempo real del estado del acuario mediante el protocolo IFTTT

# Notas importantes:

Visual Studio Code tiene un glitch al seleccionar el puerto serial de Arduino, para configurarlo, se debe añadir en el archivo arduino.json como "port": "COMX"

El archivo arduino.json se crea automaticamente en la carpeta .vscode al configurar los otros parametros (Programador y tarjeta) en VSCode.
