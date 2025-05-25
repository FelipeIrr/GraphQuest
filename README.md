======================================================================

        GraphQuest

======================================================================

▶Descripción

◜----------------------------------------------------------------------------------------------------------◝
  Este programa permite jugar un laberinto el cual el objetivo es pasar por diferentes salas 
  las cuales tienen ítems con puntos y peso y para lograr ganar el jugador debe llegar a la salida
  obteniendo la mayor cantidad de puntos por objetos antes de que se le termine su tiempo

◟----------------------------------------------------------------------------------------------------------◞


▶Cómo compilar y ejecutar

◜------------------------------------------------------------------------------------------------------------------------◝
 Este programa ha sido desarrollado en el lenguaje C y puede ser ejecutado a través del programa visual studio code
 junto con sus respectivas extensiones como C/C++ Extensión Pack de Microsoft para empezar a operar el programa realicé
 las siguientes acciones paso a paso.

 ●Requisitos previos:
  
  ○ Tener en el sistema instalado Visual Studio Code.
  ○ Instalar las extensión C/C++ (Microsoft).
  ○ Tener instalado un compilador de C por ejemplo gcc.
    Si se utiliza windows para ejecutar este programa
    se recomienda instalar MinGW o utilizar el entorno WSL.

 ●Pasos para compilar y ejecutar:

  ▷ Descarga y descomprime el archivo .zip en una carpeta que estime conveniente

  ▷ Abre el proyecto en Visual Studio Code
   ○ Inicia Visual Sudio Code.
   ○ Selecciona [Archivo > Abrir carpeta...] y elige la carpeta donde se descomprimió el proyecto.

  ▷ Compila el código
   ○ Abre el archivo principal [tarea3.c].
   ○ Abre la terminal integrada [Terminal > Nueva terminal].
   ○ En la terminal, compila el programa con el siguiente comando:
    
    ◸
        gcc tdas/*.c tarea3.c -o tarea
                                            ◿
  
  ▷ Ejecuta el programa
   ○ Una vez compilado, se puede ejecutar el programa con:

    ◸ 
        ./tarea.exe
                     ◿
◟------------------------------------------------------------------------------------------------------------------------◞


▶Funcionalidades

◜----------------------------------------------------------------------------------------------------------------------------------◝
 ●Funcionando Correctamente:
  ○ El ingreso de datos del csv para el laberinto funciona correctamente
  ○ Al recojer items se agregan correctamente al usuario
  ○ Al Soltar objetos se quedan correctamente en el escenario actual del jugador

 ●Problemas conocidos:
  ○ Al recojer todos los items disponibles no se quita el tiempo de forma correcta
  ○ Al reinciar la partida se reinicia el jugador de forma correcta pero no los escenarios ni la pocicion del jugador
  ○ Se puede abusar del recojer todos los objetos y dejarlos en una sala para sacar el maximo de puntos sin perder
 
 ●A mejorar:
  ○ Hacer que funcione correctamente el reiniciar
  ○ Hacer que funcione correctamente el recojer todos los items de la sala
◟----------------------------------------------------------------------------------------------------------------------------------◞


▶Ejemplo de uso

◜-----------------------------------------------------------------------------------------------------------------------------------------------------------------------◝
 ●Paso 1 : Cargar Laberinto
 se cargan los datos del laberinto para poder jugar
 ◤                                                             ◥
  Opción seleccionada: 1) Cargar Laberinto
  Se han cargado los escenarios correctamente.
 ◣                                                             ◢
 □ Al seleccionar cargar laberinto se cargan todos los datos del csv para actualizar y ingresar los datos al estado

 ●Paso 2 : Iniciar Partida
 Se incia la partida y muestra las diferentes opciones junto al estado inicial del jugador
 ◤                                                                  ◥
  Opción seleccionada: 2) Iniciar Partida
  ========================================
  jugador: 0 pts, 0 peso, 10.00 tiempo
  escenario actual: Entrada principal
  Descripción:Una puerta rechinante abre paso a esta mansion
  olvidada por los dioses y los conserjes. El aire huele a humedad
  y a misterios sin resolver.
  ========================================
  1) Recoger Item(s)
  2) Descartar Item(s)
  3) Avanzar en una Direccion
  4) Reiniciar Partida
  5) Salir del Juego
 ◣                                                                  ◢
 □ Se muestra informacion respecto al jugador y al escenario en el que esta junto con la informacion
 de las opciones disponibles

 ●Paso 2.1 : Recoger Item(s)
 Se selecciona de la variedad de items dentro de la sala en la que este el jugador
 ◤                                                                                ◥
  Opción seleccionada: 2.1) Recoger Item(s)
  Recoger item(s)
  Estás en: "lugar"
  "Descripcion"
  Items disponibles para recoger:
  1) Cuchillo (Puntos: 3, Peso: 1)
  2) Pan (Puntos:2, Peso: 1)
  a) Agarrar todos
  q) Salir
  Seleccione el número del item a recoger, 'a' para todos o 'q' para salir: 1

  Item Seleccionado: Cuchillo,3,1
  Item Cuchillo,3,1 recogido.

  Estado del jugador actualizado:
  Puntos: 3
  Peso: 1
  Tiempo: 8.00
  Inventario:
  1) Cuchillo,3,1
 ◣                                                                                ◢
 □ Se muestra las diferentes opciones de items disponibles para ser seleccionados por el jugador o seleccionar todos los items o ninguno

 ●Paso 2.2 : Descartar Item(s)
 Se selecciona de los diferentes items dentro del imventario del jugador para ser descartados
 ◤                                                                  ◥
  Opción seleccionada: 2.2) Descartar Item(s)
  Descartar Item(s)
  1) Cuchillo,3,1
  q) Salir
  Seleccione el numero del item a descartar: 1 
  Item Cuchillo,3,1descartado y dejado en el escenario.
 ◣                                                                  ◢
 □ Se selecciona de los diferentes items del inventario del jugador para ser descartado y al jugador se le agrega +1 al
 tiempo y si el jugador no quiere soltar nada tiene la opcion de salir de este menu

 ●Paso 2.3 : Avanzar en una dirección
 ◤                                                      ◥
  Opción seleccionada: 2.3) Avanzar en una dirección
  Avanzar en una Dirección
  Direcciones disponibles:
  w) Arriba
  s) Abajo
  d) Derecha
  q) Salir
  Seleccione una direcci├│n (w/a/s/d) o 'q' para salir: s
  Te has movido al escenario: Jardin
  Tiempo restante: 8.00
 ◣                                                      ◢
 □ De las diferentes Direcciones el jugador puede ir en estos casos Arriba, Abajo, Derecha o si no quiere seleccionar ninguna
  Salir para volver al menu

 
 ●Paso 2.4 : Reiniciar Partida
 ◤                                                                                                                                                           ◥
  Opción seleccionada: 2.4) Reiniciar Partida
   Reiniciar Partida
  Partida reiniciada. Ahora est├ís en el escenario inicial.
  ========================================
  jugador: 0 pts, 0 peso, 10.00 tiempo
  escenario actual: Entrada principal
  Descripci├│n: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.
  ========================================
  1) Recoger ├ìtem(s)
  2) Descartar ├ìtem(s)
  3) Avanzar en una Direcci├│n
  4) Reiniciar Partida
  5) Salir del Juego
 ◣                                                                                                                                                           ◢
 □ Se reiniciar al punto inicial del juego para volver a iniciar desde 0 (se requiere solucion al reiniciar por mal funcionamiento)

 ●Paso 2.5 : Salir del Juego
 permite volver al menu de inicio
 ◤                                                 ◥
  Opción seleccionada: 2.5) Salir del Juego
  1) Recoger ├ìtem(s)
  2) Descartar ├ìtem(s)
  3) Avanzar en una Direcci├│n
  4) Reiniciar Partida
  5) Salir del Juego
  Ingrese su opci├│n: 5

  ========================================
     GraphQuest
  ========================================
  1) Cargar Laberinto
  2) Iniciar Partida
  3) Salir
 ◣                                                 ◢
 □ Vuelves al punto de inicio donde se puede cargar el Laberinto y Iniciar Partida
◟-----------------------------------------------------------------------------------------------------------------------------------------------------------------------◞
