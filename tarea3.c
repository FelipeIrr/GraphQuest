#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>

// Definición de la estructura para el estado del puzzle

typedef struct {
    List* items; // Lista de items
    int pts; // Puntos
    int peso; // Peso
    float tiempo; // Tiempo (ahora float)
} Jugador;

typedef struct {
    int id;
    char nombre[50];
    char descripcion[200];
    List* items; // Lista de items
    int arriba;
    int abajo;
    int izquierda;
    int derecha;
    int is_final; // 1 si es final, 0 si no
} Escenario;

typedef struct {
    List* escenario;
    Jugador* jugador;
} Estados;

Escenario* iniciar_escenario() {
    // Inicializa un nuevo escenario
    Escenario* nuevo_escenario = (Escenario*)malloc(sizeof(Escenario));
    nuevo_escenario->items = list_create();
    return nuevo_escenario;
}

void leer_escenarios(Estados* estado) {
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos;
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Escenario* nuevo_escenario = (Escenario*)malloc(sizeof(Escenario));
    if (nuevo_escenario == NULL) {
        perror("Error al asignar memoria para escenario");
        continue;
    }
    nuevo_escenario->items = list_create();
    nuevo_escenario->id = atoi(campos[0]);
    strcpy(nuevo_escenario->nombre, campos[1]);
    strcpy(nuevo_escenario->descripcion, campos[2]);
    
    List* items = split_string(campos[3], ";");

    // Agregar los ítems a la lista del escenario
    for(char *item = list_first(items); item != NULL; item = list_next(items)){
        // Copiar el string para evitar problemas de memoria
        char* item_copia = strdup(item);
        list_pushBack(nuevo_escenario->items, item_copia);
    }

    nuevo_escenario->arriba = atoi(campos[4]);
    nuevo_escenario->abajo = atoi(campos[5]);
    nuevo_escenario->izquierda = atoi(campos[6]);
    nuevo_escenario->derecha = atoi(campos[7]);
    nuevo_escenario->is_final = (strcmp(campos[8], "Si") == 0) ? 1 : 0;

    list_pushBack(estado->escenario, nuevo_escenario);

    list_clean(items);
    free(items);
  }
  printf("Se han cargado los escenarios correctamente.\n");
  fclose(archivo);
}

void iniciar_jugador(Jugador* jugador) {
    jugador->items = list_create();
    jugador->pts = 0;
    jugador->peso = 0;
    jugador->tiempo = 10.0f; // Tiempo inicial como float
}

void recoger_items(Jugador* jugador, List* escenarios) {
    Escenario* escenario_actual = list_first(escenarios);
    if (escenario_actual == NULL || escenario_actual->items == NULL){
        puts("No hay items para recoger en este escenario.");
        return;
    }

    // Mostrar el escenario actual
    printf("Estás en: %s\n", escenario_actual->nombre);
    printf("%s\n", escenario_actual->descripcion);
    
    // Mostrar los ítems con detalles
    printf("Items disponibles para recoger:\n");
    List* values = NULL;
    int idx = 1;
    char* item = NULL;
    for (item = list_first(escenario_actual->items); item != NULL; item = list_next(escenario_actual->items)) {
        char* item_copia = strdup(item); // Hacemos una copia para no modificar el original
        List* values = split_string(item_copia, ",");
        char* nombre = list_first(values);
        char* puntos = list_next(values);
        char* peso = list_next(values);
        printf("%d) %s (Puntos: %s, Peso: %s)\n", idx, nombre ? nombre : "-", puntos ? puntos : "-", peso ? peso : "-");
        list_clean(values);
        free(values);
        free(item_copia); // Liberamos la copia
        idx++;
    }
    if (idx == 1) {
        printf("No hay ítems en este escenario.\n");
    }
    printf("a) Agarrar todos\n");
    printf("q) Salir\n");

    item = NULL;

    char opcion[10];
    do {
        printf("Seleccione el número del item a recoger, 'a' para todos o 'q' para salir: ");
        scanf("%s", opcion);

        if (strcmp(opcion, "q") == 0) break;

        if (strcmp(opcion, "a") == 0) {
            // Recoger todos los ítems correctamente
            item = list_first(escenario_actual->items);
            while (item != NULL) {
                char* item_copia = strdup(item); // Copia para split_string
                List* values = split_string(item_copia, ",");
                char* nombre = list_first(values);
                char* puntos_str = list_next(values);
                char* peso_str = list_next(values);
                int puntos = puntos_str ? atoi(puntos_str) : 0;
                int peso = peso_str ? atoi(peso_str) : 0;
                jugador->pts += puntos;
                jugador->peso += peso;
                list_pushBack(jugador->items, item);
                printf("Item %s recogido.\n", nombre ? nombre : item);
                list_clean(values);
                free(values);
                free(item_copia); // Libera la copia
                item = list_next(escenario_actual->items);
                list_popFront(escenario_actual->items); // Elimina el ítem recogido; // Descontar tiempo por recoger el ítem
            }
            break;
        }

        int seleccion = atoi(opcion);
        if (seleccion > 0 && seleccion < idx) { 
            int actual = 1;
            char* item_a_recoger = NULL;
            for (char* item = list_first(escenario_actual->items); item != NULL; item = list_next(escenario_actual->items)) {
                if (actual == seleccion) {
                    item_a_recoger = item;
                    break;
                }
                actual++;
            }
            printf("Item seleccionado: %s\n", item_a_recoger);
            if (item_a_recoger) {
                char* item_copia = strdup(item_a_recoger); // Copia para split_string
                List* values = split_string(item_copia, ",");
                char* nombre = list_first(values);
                char* puntos_str = list_next(values);
                char* peso_str = list_next(values);
                int puntos = puntos_str ? atoi(puntos_str) : 0;
                int peso = peso_str ? atoi(peso_str) : 0;
                jugador->pts += puntos;
                jugador->peso += peso;
                jugador->tiempo --; // Descontar tiempo por recoger el ítem
                list_pushBack(jugador->items, item_a_recoger);
                list_clean(values);
                free(values);
                free(item_copia); // Libera la copia
                printf("Item %s recogido.\n", item_a_recoger);
                // Eliminar el item del escenario
                list_popCurrent(escenario_actual->items);
            }
        } else {
            printf("Opción inválida.\n");
        }
    } while (1);

    // Mostrar estado actualizado del jugador
    printf("\nEstado del jugador actualizado:\n");
    printf("Puntos: %d\n", jugador->pts);
    printf("Peso: %d\n", jugador->peso);
    printf("Tiempo: %.2f\n", jugador->tiempo);
    printf("Inventario:\n");
    int i = 1;
    for (char* item = list_first(jugador->items); item != NULL; item = list_next(jugador->items)) {
        printf("%d) %s\n", i, item);
        i++;
    }
}

void descartar_items(Jugador* jugador, List* escenario) {
    if (jugador->items == NULL || list_size(jugador->items) == 0) {
        puts("No tienes items para descartar.");
        return;
    }

    char opcion[10];
    do {
        printf("Items en tu inventario:\n");
        int idx = 1;
        for (char* item = list_first(jugador->items); item != NULL; item = list_next(jugador->items)) {
            printf("%d) %s\n", idx, item);
            idx++;
        }
        printf("q) Salir\n");

        printf("Seleccione el número del item a descartar: ");
        scanf("%s", opcion);

        if (strcmp(opcion, "q") == 0) break;

        int seleccion = atoi(opcion);
        if (seleccion > 0 && seleccion < idx) {
            int actual = 1;
            char* item_a_descartar = NULL;
            for (char* item = list_first(jugador->items); item != NULL; item = list_next(jugador->items)) {
                if (actual == seleccion) {
                    item_a_descartar = item;
                    break;
                }
                actual++;
            }
            char* item_copia = strdup(item_a_descartar); // Copia para split_string
            if (item_copia) {
                // Obtener el escenario actual (primer elemento de la lista de escenarios)
                Escenario* escenario_actual = list_first(escenario);
                if (escenario_actual != NULL) {
                    list_pushBack(escenario_actual->items, item_a_descartar);
                }
                // Obtener los valores del item (nombre, puntos, peso)
                List* values = split_string(item_copia, ",");
                char* item_name = list_first(values);
                char* puntos_str = list_next(values);
                char* peso_str = list_next(values);
                int puntos = puntos_str ? atoi(puntos_str) : 0;
                int peso = peso_str ? atoi(peso_str) : 0;
                jugador->pts -= puntos;
                jugador->peso -= peso;
                jugador->tiempo += 1;
                list_clean(values);
                free(values);

                list_popCurrent(jugador->items);
                printf("Item %s descartado y dejado en el escenario.\n", item_a_descartar);
            }
        } else {
            printf("Opción inválida.\n");
        }
    } while (1);
}

void avanzar_direccion(Jugador* jugador, List* escenario) {
    Escenario* escenario_actual = list_first(escenario);
    if (escenario == NULL || escenario_actual == NULL) {
        puts("No hay escenarios disponibles.");
        return;
    }

    char opcion;
    int salir = 0;
    do {
        printf("Direcciones disponibles:\n");
        if (escenario_actual->arriba != -1)    printf("w) Arriba\n");
        if (escenario_actual->abajo != -1)     printf("s) Abajo\n");
        if (escenario_actual->izquierda != -1) printf("a) Izquierda\n");
        if (escenario_actual->derecha != -1)   printf("d) Derecha\n");
        printf("q) Salir\n");

        printf("Seleccione una dirección (w/a/s/d) o 'q' para salir: ");
        scanf(" %c", &opcion);

        if (opcion == 'q') {
            salir = 1;
            continue;
        }

        int nuevo_id = -1;
        switch (opcion) {
            case 'w':
                nuevo_id = escenario_actual->arriba;
                break;
            case 's':
                nuevo_id = escenario_actual->abajo;
                break;
            case 'a':
                nuevo_id = escenario_actual->izquierda;
                break;
            case 'd':
                nuevo_id = escenario_actual->derecha;
                break;
            default:
                puts("Dirección no válida.");
                continue; // No salir del bucle
        }

        if (nuevo_id == -1) {
            puts("No hay escenario en esa dirección.");
            continue; // No salir del bucle
        }

        // Buscar el escenario con el id correspondiente
        Escenario* nuevo_escenario = NULL;
        for (Escenario* esc = list_first(escenario); esc != NULL; esc = list_next(escenario)) {
            if (esc->id == nuevo_id) {
                nuevo_escenario = esc;
                break;
            }
        }

        if (nuevo_escenario == NULL) {
            puts("No se encontró el escenario destino.");
            continue; // No salir del bucle
        }

        // Mover el escenario actual al final y poner el nuevo al inicio
        list_popFront(escenario); // Quita el actual
        list_pushFront(escenario, nuevo_escenario);

        printf("Te has movido al escenario: %s\n", nuevo_escenario->nombre);

        // Descontar tiempo según la ecuación T(tiempo) = ((pesototal + 1) / 10)
        int descuento = (jugador->peso + 1.0) / 10;
        if (descuento < 1) descuento = 1;
        jugador->tiempo -= (float)descuento;

        printf("Tiempo restante: %.2f\n", jugador->tiempo);

        salir = 1; // Salir del bucle tras moverse exitosamente
    } while (!salir);
}

void reiniciar_Partida(Jugador* jugador, List* escenario, Escenario* escenario_inicial) {
    // Reinicia el jugador
    iniciar_jugador(jugador);

    // Buscar el escenario con id 1 (escenario inicial)
    Escenario* inicial = NULL;
    for (Escenario* esc = list_first(escenario); esc != NULL; esc = list_next(escenario)) {
        if (esc->id == 1) {
            inicial = esc;
            break;
        }
    }

    if (inicial) {
        // Quitar el escenario inicial de su posición actual si no está al frente
        for (Escenario* esc = list_first(escenario); esc != NULL; esc = list_next(escenario)) {
            if (esc == inicial) {
                list_popCurrent(escenario);
                break;
            }
        }
        // Poner el escenario inicial al frente de la lista
        list_pushFront(escenario, inicial);
    }

    printf("Partida reiniciada. Ahora estás en el escenario inicial.\n");
}

void iniciar_Partida(Estados* estado) {
    iniciar_jugador(estado->jugador);
    Jugador* jugador = estado->jugador;
    if (jugador == NULL) {
        puts("Error al inicializar el jugador.");
        return;
    }
    if (list_first(estado->escenario) == NULL) {
        puts("No hay escenarios disponibles.");
        return;
    }
    char opcion;
    int fin_partida = 0;
    Escenario* escenario_inicial = list_first(estado->escenario);

    do{
        limpiarPantalla();

        Escenario* escenario_actual = list_first(estado->escenario);

        printf("========================================\n");
        printf("jugador: %d pts, %d peso, %.2f tiempo\n", jugador->pts, jugador->peso, jugador->tiempo);
        printf("escenario actual: %s\n", escenario_actual->nombre);
        printf("Descripción: %s\n", escenario_actual->descripcion);
        printf("========================================\n");
        
        // Verifica si el tiempo se acabó
        if (jugador->tiempo <= 0.0f) {
            printf("\n¡Derrota! Se acabó el tiempo.\n");
            printf("Puntos finales: %d\n", jugador->pts);
            printf("Peso final: %d\n", jugador->peso);
            fin_partida = 1;
            break;
        }
        // Verifica si llegó al escenario final
        if (escenario_actual->is_final) {
            limpiarPantalla();
            printf("==================================================\n");
            printf("\n¡Victoria! Has llegado a la salida.\n");
            printf("Puntos finales: %d\n", jugador->pts);
            printf("Peso final: %d\n", jugador->peso);
            printf("Tiempo restante: %.2f\n", jugador->tiempo);
            printf("==================================================\n");
            fin_partida = 1;
            break;
        }

        puts("1) Recoger Ítem(s)");
        puts("2) Descartar Ítem(s)");
        puts("3) Avanzar en una Dirección");
        puts("4) Reiniciar Partida");
        puts("5) Salir del Juego");
        
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case '1':
                limpiarPantalla();
                printf("Recoger Ítem(s)\n");
                recoger_items(jugador, estado->escenario);
                break;
            case '2':
                limpiarPantalla();
                printf("Descartar Ítem(s)\n");
                descartar_items(jugador, estado->escenario);
                break;
            case '3':
                limpiarPantalla();
                printf("Avanzar en una Dirección\n");
                avanzar_direccion(jugador, estado->escenario);
                break;
            case '4':
                limpiarPantalla();
                printf("Reiniciar Partida\n");
                reiniciar_Partida(jugador, estado->escenario, escenario_inicial);
                break;
        }
        if (!fin_partida) {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }
    }while(opcion != '5' && !fin_partida);
}

// void mostrar_escenarios(Estados* estado) {
//     printf("Escenarios:\n");
//     List * lista = estado->escenario;
//     for(Escenario* escenario = list_first(lista);
//         escenario != NULL;
//         escenario = list_next(lista)){
//         printf("ID: %d\n", escenario->id);
//         printf("Nombre: %s\n", escenario->nombre);
//         printf("Descripción: %s\n", escenario->descripcion);
//         printf("Arriba: %d\n", escenario->arriba);
//         printf("Abajo: %d\n", escenario->abajo);
//         printf("Izquierda: %d\n", escenario->izquierda);
//         printf("Derecha: %d\n", escenario->derecha);
//         if(escenario->is_final) {
//             printf("Es final\n");
//         }
//     }
// }

int main() {
    limpiarPantalla();
    Estados* escenarios = (Estados*)malloc(sizeof(Estados));
    if(escenarios == NULL) {
        perror("Error al asignar memoria para escenarios");
        return 1;
    }
    escenarios->escenario = list_create();
    escenarios->jugador = (Jugador*)malloc(sizeof(Jugador));
    if (escenarios->jugador == NULL) {
        perror("Error al asignar memoria para jugador");
        free(escenarios);
        return 1;
    }
    escenarios->jugador->items = list_create();
    escenarios->jugador->pts = 0;
    escenarios->jugador->peso = 0;

    char opcion;
    do {
        puts("========================================");
        puts("     GraphQuest");
        puts("========================================");
        
        puts("1) Cargar Laberinto");
        puts("2) Iniciar Partida");
        puts("3) Salir");
        
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        
        switch (opcion) {
            case '1':
            leer_escenarios(escenarios);
            break;
            case '2':
            //mostrar_escenarios(escenarios);
            iniciar_Partida(escenarios);
            break;
            default:
                puts("Opción no válida. Por favor, intente de nuevo.");
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != '3');

  return 0;
}
