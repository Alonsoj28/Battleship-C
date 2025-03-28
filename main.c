#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Celda{
  //Estado, 0 = libre, 1 = ocupada
  int status;
  //Identificador, numero unico de cada nave
  int ident;
  //Impactada, 0 = no ha sido impactada, 1 = si lo ha sido
  int hit;
};

struct Nave{
  //Tipo de nave, String con el nombre de la Nave: portaviones, buque, submarino, crucero, lancha
  char type[20];
  //Orientacion, V = Vertical, H = Horizontal
  char side;
  //Hundida, 0 = falso, 1 = verdadero
  char sunk;
};

//Declaramos todas nuestras funciones
void printMenu(int, int);
void printModeMenu();
void startGame(int, int);
void initBoard(struct Celda *, int);
void getShips(struct Nave *, int);
int isPossible(struct Celda *, int, int, int, char [], char);
int getSize(char []);
void printBoard(struct Celda *, int);
void testMatrix(struct Celda *, int);
void cpuAttack(struct Celda *, struct Celda *, struct Celda *, int);
void userAttack(struct Celda *, struct Celda *, struct Celda *, int);
int checkGame(struct Celda *, int );
void cpuAttackEz(struct Celda *, struct Celda *, int);
void userAttackEz(struct Celda *, struct Celda *, int);




int main(void) {
  //Hacemos que la semilla con la que trabaje srand sea aleatoria dependiendo de nuestro tiempo local
  srand(time(0));
  system("clear");

  // El juego por default inicia con un tablero de 10x10 y en modo Didactico. Modo 1 = didactico, Mode 2 = Facil
  int size = 10, mode = 1;

  int op = 0;
  while(op <= 3){
    // Una funcion la cual nos imprime todo el menu, de forma compacta
    printMenu(size, mode);
    scanf(" %i", &op);
    // Un Switch Case muy simple, para poder facilitar selecionar las opciones del menu
    switch (op){
      case 1:
      printf("Dimension (SxS): ");
      scanf("%i", &size);
      system("clear");  
      break;

      case 2:
      system("clear");
      // La opcion 2 nos abre un submenu con la opcion de escoger entre modo de juego didactico y facil
      printModeMenu();
      scanf("%i", &mode);
      while (mode > 2 || mode < 1){
        printf("Error con los datos introducidos, favor de verificar!! \n ");
        printModeMenu();
        scanf("%i", &mode);
      }
        
      system("clear");  
      break;

      case 3:
      system("clear");  
      // La opcion 3 inicia el juego
      startGame(size, mode);
      op = 4;
      break;

      default:
      printf("Gracias por jugar!");
      break;
      
    }
  }
  return 0;
}

void printMenu(int s, int m){
  
  printf("Tablero: %i x %i \n", s, s);
  if(m == 1){
    printf("Modo de juego: Didactico \n\n");
  }else{
    printf("Modo de juego: Facil \n\n");
  }
  printf("1.- Modificar dimensión del tablero \n");
  printf("2.- Seleccionar modo de juego \n");
  printf("3.- Iniciar juego \n");
  printf("4.- Salir \n"); 
}

void printModeMenu(){
  printf("1.- Didáctico \n");
  printf("2.- Fácil \n");
}

void startGame(int s, int mode){
  // Informacion importante para startGame: en esta funcion inicializamos los tableros y las naves que van en ellos, al Jugador humano se le denombra Player1 y la computadora como Player2
  
  /*
  // ESTA PARTE DEL CODIGO SOLO SE USA PARA DEBUGGEAR LA FUNCION DE isPossible();
  struct Nave PlayerTest[] = {{"Portaviones", 'H', 0}, {"Portaviones", 'H', 0}};
  struct Nave *pn = PlayerTest;
  struct Celda TableroP1[s][s];
  struct Celda *p1 = &TableroP1[0][0];
  initBoard(p1, s);
  struct Celda *pt = &TableroP1[2][5];
  int check = 0;
  check = isPossible(pt, s, 2, 5, pn->type, pn->side);
  if (check == 1){
    int size = getSize(pn->type);
    for (int i = 0; i < size; i++){
          (pt+i)->ident = 1;
          (pt+i)->status = 1;
        }
  }else{
    printf("No se pudo");
  }
  pt = &TableroP1[2][1];
  check = 0;
  check = isPossible(pt, s, 2, 1, (pn+1)->type, (pn+1)->side);
  if (check == 1){
    int size = getSize((pn+1)->type);
    for (int i = 0; i < size; i++){
          (pt+i)->ident = 2;
          (pt+i)->status = 1;
        }
  }else{
    printf("No se pudo");
  }

  printBoard(p1, s);
  scanf("%i");
  */

  // Se crea un arreglo de tipos de dato Nave con un maximo de 40 posibles naves
  struct Nave Player1[40];
  // Con esta funcion se generan la cantidad de naves necesarias por jugador, para que las naves ocupen entre el 25% y el 35% del tablero.
  getShips(Player1, s);
  struct Nave Player2[40];
  getShips(Player2, s);
  // Aqui inicializamos el tablero del Jugador
  struct Celda TableroP1[s][s];
  struct Celda *p1 = &TableroP1[0][0];
  initBoard(p1, s);
  // Con temp_id llevamos cuenta de cuantas naves se han colocado, de esta manera podemos darles un identificador unico a cada uno
  int temp_id = 1;
  int check = 0;
  // El contador j nos ayuda a llevar cuenta de la nave que estamos colocando dentro del arreglo Player
  int j = 0;
  struct Nave *pn = Player1;
  struct Celda *temp = NULL;
  // La condicion de este ciclo while es que tiene que revisar todo el arreglo hasta que detecte que la siguiente entrada no tiene nombre de Nave, en otras palabras, que no se ha declarado ninguna nave despues
  while(getSize((pn+j)->type) != 0){
    // Se generan coordenadas aleatorias para colocar cada nave
    int x = rand() % s;
    int y = rand() % s;
    temp = &TableroP1[x][y];
    // Mandamos dichas coordenadas y el tipo de nave a la funcion isPossible, que nos permite saber si se va a poder colocar la nave, este criterio es con base en la esquina mas superior/izquierda de la nave, dependiendo su orientacion
    check = isPossible(temp, s, x, y, (pn+j)->type, (pn+j)->side);
    if (check == 1){
      // Si el valor de isPossible regresa como 1, procedemos a colocar la nave en sus respectivas celdas
      int size = getSize((pn+j)->type);
      if(((pn+j)->side) == 'H'){
        for (int i = 0; i < size; i++){
          // Colocamos el identificador unico de cada nave
          (temp+i)->ident = temp_id;
          // Cambiamos el status para declarar que esta ocupada
          (temp+i)->status = 1;
        }
          temp_id++;
          j++;
          check = 0;
      }else{
        if(((pn+j)->side) == 'V'){
          for (int i = 0; i < size; i++){
            (temp+(i*s))->ident = temp_id;
            (temp+(i*s))->status = 1;
          }
          temp_id++;
          j++;
          check = 0;
      }
    }
  }
}
  struct Celda TableroP2[s][s];
  struct Celda *p2 = &TableroP2[0][0];
  initBoard(p2, s);
  temp_id = 1;
  check = 0;
  j = 0;
  struct Nave *pc = Player2;
  temp = NULL;
  while(getSize((pc+j)->type) != 0){
    int x = rand() % s;
    int y = rand() % s;
    temp = &TableroP2[x][y];
    check = isPossible(temp, s, x, y, (pc+j)->type, (pc+j)->side);
    if (check == 1){
      int size = getSize((pc+j)->type);
      if(((pc+j)->side) == 'H'){
        for (int i = 0; i < size; i++){
          (temp+i)->ident = temp_id;
          (temp+i)->status = 1;
        }
          temp_id++;
          j++;
          check = 0;
      }else{
        if(((pc+j)->side) == 'V'){
          for (int i = 0; i < size; i++){
            (temp+(i*s))->ident = temp_id;
            (temp+(i*s))->status = 1;
          }
          temp_id++;
          j++;
          check = 0;
      }
    }
  }
}
  // Definimos esta variable trash para poder pedirle al usuario que presione enter para continuar, por alguna razon no sirve getchar(); con este codigo
  int trash;
  // Esta variable sirve para determinar si el juego sigue activo o si ya ha ganado alguien
  int victory = 0;
  // Ambas coordenadas donde atacara el usuario
  int atX, atY;
  // Usamos un arreglo convertidor como una especie de llave para decidir donde atacara el usuario, esto es porque como el usuario ve el tablero es invertido a como esta almacenado
  int convertArray[s];
  int *convert = convertArray;
  int g = 0;
  for (int i = s-1; i >= 0; i--){
    *(convert+g) = i;
    g++;
  }
  // Esta variable va ser la coordenada en X temporal que va a introducir el usuario, antes de que sea convertida
  int tempX;
  int winner;
  struct Celda *sight = NULL;
  int valid = 1;
  // De manera aleatoria decidimos quien inicia el juego
  int starts = rand() % 2;
  // Este es el modo didactico
  if(mode == 1){
    if (starts == 0){
    int x = rand() % s;
    int y = rand() % s;
    sight = &TableroP1[x][y];
    // Llamamos a esta funcion para registrar el ataque del oponente con la coordenada aleatoria que se genero
    cpuAttack(sight, p1, p2, s);
    // Convertimos el valor de X para que tenga sentido con lo que ve el usuario en pantalla
    x = *(convert+x);
    // Declaramos en donde fue que disparo el oponente
    printf(" Disparo en (%i, %i)! \n", x, y);
    printf("\n Introduzca 1 para continuar > ");
    scanf(" %i", &trash);
    system("clear"); 
    // Al finalizar el turno, revisamos si todavia hay alguna nave que no haya sido hundida, de ser asi, el juego continua
    if (checkGame(p1, s) == 0){
      victory = 1;
      winner = 2;
    }
  }

  while(victory == 0){
    printf("Turno del Jugador! \n");
    printf(" \n\n\n Jugador: \n\n\n");
    printBoard(p1, s);
    printf(" \n\n\n Oponente: \n\n\n");
    printBoard(p2, s);
    printf("\n Coordenada en X para atacar: \n");
    scanf(" %i", &tempX);
    printf("\n Coordenada en Y para atacar: \n");
    scanf(" %i", &atY);
    system("clear"); 
    // Aqui se hace la conversion de la coordenada en X desde antes, porque aqui el usuario introduce lo que ve
    atX = *(convert+tempX);
    // Aqui ocurre lo mismo que con los ataques del oponente, solo que aqui los ataques no son aleatorios, son decididos por el usuario
    sight = &TableroP2[atX][atY];
    userAttack(sight, p1, p2, s);
    printf("\n Introduzca 1 para continuar...");
    scanf(" %i", &trash);
    system("clear"); 
    if (checkGame(p2, s) == 0){
      victory = 1;
      winner = 1;
      break;
    }
    int x = rand() % s;
    int y = rand() % s;
    sight = &TableroP1[x][y];
    cpuAttack(sight, p1, p2, s);
    x = *(convert+x);
    printf(" Disparo en (%i, %i)! \n", x, y);
    printf("\n Introduzca 1 para continuar > ");
    scanf(" %i", &trash);
    system("clear");
    // Todo esto se repite hasta que ya no haya naves que no hayan sido hundidas
    if (checkGame(p1, s) == 0){
      victory = 1;
      winner = 2;
      break;
    }
  }

  // Una vez finaliza el juego y se tiene un ganador decidido, imprimimos el mensaje que corresponde y el programa termina
  if(winner == 1){
    printf("Felicidades! Usted ha vencido a su oponente!! \n");
    printf("\n Introduzca 1 para finalizar el programa > ");
    scanf(" %i", &trash);
  }

  if(winner == 2){
    printf("Oh no! Usted ha perdido contra su oponente!! \n");
    printf("\n Introduzca 1 para finalizar el programa > ");
    scanf(" %i", &trash);
  }
}

  // Este es el modo facil, misma logica y codigo que con el modo didactico, solo que eliminamos cualquier impresion del tablero enemigo
  if(mode == 2){
    if (starts == 0){
    int x = rand() % s;
    int y = rand() % s;
    sight = &TableroP1[x][y];
    cpuAttackEz(sight, p1, s);
    x = *(convert+x);
    printf(" Disparo en (%i, %i)! \n", x, y);
    printf("\n Introduzca 1 para continuar > ");
    scanf(" %i", &trash);
    system("clear"); 
    if (checkGame(p1, s) == 0){
      victory = 1;
      winner = 2;
    }
  }

  while(victory == 0){
    printf("Turno del Jugador! \n");
    printf(" \n\n\n Jugador: \n\n\n");
    printBoard(p1, s);
    printf("\n Coordenada en X para atacar: \n");
    scanf(" %i", &tempX);
    printf("\n Coordenada en Y para atacar: \n");
    scanf(" %i", &atY);
    system("clear"); 
    atX = *(convert+tempX);
    sight = &TableroP2[atX][atY];
    userAttackEz(sight, p1,s);
    printf("\n Introduzca 1 para continuar...");
    scanf(" %i", &trash);
    system("clear"); 
    if (checkGame(p2, s) == 0){
      victory = 1;
      winner = 1;
      break;
    }
    int x = rand() % s;
    int y = rand() % s;
    sight = &TableroP1[x][y];
    cpuAttackEz(sight, p1, s);
    x = *(convert+x);
    printf(" Disparo en (%i, %i)! \n", x, y);
    printf("\n Introduzca 1 para continuar > ");
    scanf(" %i", &trash);
    system("clear");
    if (checkGame(p1, s) == 0){
      victory = 1;
      winner = 2;
      break;
    }
  }

  if(winner == 1){
    printf("Felicidades! Usted ha vencido a su oponente!! \n");
    printf("\n Introduzca 1 para finalizar el programa > ");
    scanf(" %i", &trash);
  }

  if(winner == 2){
    printf("Oh no! Usted ha perdido contra su oponente!! \n");
    printf("\n Introduzca 1 para finalizar el programa > ");
    scanf(" %i", &trash);
  }
  }
  
  /*struct Celda test[s][s];
  struct Celda *pt = &test[s][s];
  testMatrix(pt, s);
  printBoard(pt, s);
  scanf("%i");
  */

  
  
  }

void initBoard(struct Celda *pb, int s){
  // La funcion initBoard, inicializa todas las celdas del tablero con el tamaño deseado, inicia cada celda con todos los valores iguales a 0
  int totalSize = s*s;

  for (int i = 0; i < totalSize; i++){
    (pb+i)->hit = 0;
    (pb+i)->ident = 0;
    (pb+i)->status = 0;
  }
}

void getShips(struct Nave *pn, int s){
  // La funcion getShips va a generarnos de manera aleatoria una cantidad de naves para cada jugador, con el criterio de que la cantidad de naves en el tablero no debe superar el 35% de las celdas disponibles
  int totalSize = s*s;
  // El contador shipSize nos dice cuanto espacio es cubierto por las naves ya generadas
  int shipSize = 0;
  // El contador 'i' lleva cuenta de cuantas naves ya hemos generado, para colocar cada nave en su respectiva posicion dentro del arreglo
  int i = 0;
  // El contador 'o' nos va a generar la orientacion de la nave de manera aleatoria
  int o = 0;
  while(totalSize*0.3 >= shipSize){
    // Este ciclo while se va a repetir mientras no exedamos de el 30% de las celdas totales
    int r = (rand() % 5) + 1;
    // Generamos un numero aleatorio entre el 1 y el 5, esto nos va a decir que tipo de nave se va a generar
    switch (r){
      case 1:
      // El caso 1 nos genera una Lancha, ingresamos su respectiva informacion y le añadimos 1 al contador de shipSize
      if(shipSize+1 > totalSize*0.35){
        // Constantemente revisamos, antes de declarar la nave, si es que esta nave no hace que superen del 35% de las celdas disponibles
        shipSize = totalSize;
        break;
      }else{
        strcpy((pn + i)->type, "Lancha");
        (pn + i)->sunk = 0;
        (pn + i)->side = 'V';
        shipSize += 1;
        i++;
        break;
      }

      case 2:
      if(shipSize+2 > totalSize*0.35){
        break;
      }else{
        strcpy((pn + i)->type, "Crucero");
        (pn + i)->sunk = 0;
        o = rand() % 2;
        if (o == 0){
          (pn + i)->side = 'H';
        }else{
          (pn + i)->side = 'V';
        }
        shipSize += 2;
        i++;
        break;
      }

      case 3:
      if(shipSize+3 > totalSize*0.35){
        break;
      }else{
        strcpy((pn + i)->type, "Submarino");
        (pn + i)->sunk = 0;
        o = rand() % 2;
        if (o == 0){
          (pn + i)->side = 'H';
        }else{
          (pn + i)->side = 'V';
        }
        shipSize += 3;
        i++;
        break;
      }
      
      case 4:
      if(shipSize+4 > totalSize*0.35){
        break;
      }else{
        strcpy((pn + i)->type, "Buque");
        (pn + i)->sunk = 0;
        o = rand() % 2;
        if (o == 0){
          (pn + i)->side = 'H';
        }else{
          (pn + i)->side = 'V';
        }
        shipSize += 4;
        i++;
        break;
      }

      case 5:
      if(shipSize+5 > totalSize*0.35){
        break;
      }else{
        strcpy((pn + i)->type, "Portaviones");
        (pn + i)->sunk = 0;
        o = rand() % 2;
        if (o == 0){
          (pn + i)->side = 'H';
        }else{
          (pn + i)->side = 'V';
        }
        shipSize += 5;
        i++;
        break;
      }
    }
  }
}

int isPossible(struct Celda *pb, int s, int x, int y, char Name[], char side){
  // Possiblemente la funcion mas importante de toda la inicializacion y declaracion de valores, esta funcion recibe varios elementos, hace uso de todos los elementos que recibe y revisa si es que la nave que se planea colocar en una direccion aleatoria tiene espacio ahi. Ya sea porque excede los limites del tablero o porque hay otra nave ocupando ese espacio
  int shipSize = getSize(Name);
  // Llamamos la funcion getSize para obtener el tamaño de la nave que estamos intentando colocar
  
  if (side == 'H'){
    // Si su orientacion es Horizontal, tenemos que revisar que no exceda los limites del tablero, primero
    if((y + shipSize) <= s){
      for(int i=0; i < shipSize; i++){
        // Despues, revisamos que ninguna de las celdas siguientes a su inicio, esten ocupadas por otra nave
        if((pb+i)->status == 1){
          return 0;
        }
      }
      // En caso de que pase todas estas condiciones, nos regresara un 1, que es equivalente a Verdadero, si en cualquier momento se encuentra alguna restriccion, se regresara un 0, que evidentemente es igual a Falso
      return 1;
    }else{
      return 0;
    }
    }
  if (side == 'V'){
    if((x + shipSize) <= s){
      for(int i=0; i < shipSize; i++){
        // Aqui revisamos lo mismo, pero revisamos renglon por renglon para poder verificar que ninguna celda este ocupada por debajo de este
        if((pb+(i*s))->status == 1){
          return 0;
        }
      }
    return 1;
    }else{
      return 0;
    }
  }
}

  

int getSize(char Name[]){
  // Una funcion muy simple que nos permite saber la longitud, los espacios que abarca cada nave con base en su nombre, en caso de que reciba algun valor que no tenga un nombre valido, la funcion regresa un 0
  if(strcmp(Name, "Portaviones") == 0){
    return 5;
  }
  
  if(strcmp(Name, "Buque") == 0){
    return 4;
  }
  
  if(strcmp(Name, "Submarino") == 0){
    return 3;
  }
  
  if(strcmp(Name, "Crucero") == 0){
    return 2;
  }

  if(strcmp(Name, "Lancha") == 0){
    return 1;
  }

  return 0;
}

void printBoard(struct Celda *pb, int s){
  // Tambien una funcion bastante simple que solo nos imprime renglon por renglon toda la matriz y su estado de la celda, si es que esta ocupada, nos pone el identificador de cada nave, para poder diferenciar entre una nave y otra. En caso de haber sido impactada, coloca una X y en caso de estar vacia, coloca un guion.
    int j = 0;
    for (int i = s; i > 0; i--){
      printf(" %d", i-1);
        for (j = 0; j < s; j++){
          if (pb->hit == 0){
              if (pb->status == 1){
                  printf("\t %i", pb->ident);
              }
              else{
                printf("\t -");
              }
          }
          else{
              printf("\t X");
          }
          pb++;
        }
        
        printf(" \n");
    }
    for (int g = -1; g <s; g++){
      if (g == -1){
            printf("  ");
          }else{
        printf("\t %d", g);
          }
    }
  printf(" \n");
}

void testMatrix(struct Celda *pb , int s){
  // Una funcion para probar el como se imprimen los tableros, no tiene impacto en el programa para el usuario
  int totalSize = s*s;

  for (int i = 0; i < totalSize; i++){
    (pb+i)->hit = 0;
    (pb+i)->ident = i;
    (pb+i)->status = 1;
  }
}
  
void cpuAttack(struct Celda *sight, struct Celda *p1, struct Celda *p2, int s){
  // En esta funcion, recibimos la direccion en memoria de donde decidio atacar el oponente, revisamos si ya ha sido golpeada antes, si no ha sido golpeada, la marca como hundida, de otra manera, el tiro falla
    if(sight->status == 1){
        if(sight->hit == 0){
          sight->hit = 1;
          printf("Turno del Oponente! \n");
          printf(" \n\n\n Jugador: \n\n\n");
          printBoard(p1, s);
          printf(" \n\n\n Oponente: \n\n\n");
          printBoard(p2, s);
          printf("\n Ha golpeado tu Nave! \n");
        }else{
          printf("\n Ha fallado! \n");
        }
      }else{
        printf("Turno del Oponente! \n");
        printf(" \n\n\n Jugador: \n\n\n");
        printBoard(p1, s);
        printf(" \n\n\n Oponente: \n\n\n");
        printBoard(p2, s);
        printf("\n Ha fallado! \n");
      }
}

void userAttack(struct Celda *sight, struct Celda *p1, struct Celda *p2, int s){
  // En esta funcion recibimos la direccion de donde quiere atacar el jugador, si se encuentra una flota que no ha sido hundida previamente, marcamos como golpe el disparo y cambiamos su estado a hundida, de lo contrario, el disparo falla
  if(sight->status == 1){
        if(sight->hit == 0){
          sight->hit = 1;
          printf("Turno del Jugador! \n");
          printf(" \n\n\n Jugador: \n\n\n");
          printBoard(p1, s);
          printf(" \n\n\n Oponente: \n\n\n");
          printBoard(p2, s);
          printf("\n Has golpeado su Nave! \n");
        }
      }else{
        printf("Turno del Jugador! \n");
        printf(" \n\n\n Jugador: \n\n\n");
        printBoard(p1, s);
        printf(" \n\n\n Oponente: \n\n\n");
        printBoard(p2, s);
        printf("\n Ha fallado! \n");
      }
}

int checkGame(struct Celda *p1, int s){
  // Esta funcion checa todas las celdas del tablero del jugador que se envie, si encuentra al menos una que siga flotando, regresa un valor de 1, de lo contrario, es 0
  int size = s*s;
  for (int i = 0; i < size; i++){
    if((p1+i)->status == 1 && (p1+i)->hit == 0){
      return 1;
    }
  }
  return 0;
  
}

void cpuAttackEz(struct Celda *sight, struct Celda *p1, int s){
  // La misma funcion que el ataque normal, solo que esta no imprime el tablero enemigo
    if(sight->status == 1){
        if(sight->hit == 0){
          sight->hit = 1;
          printf("Turno del Oponente! \n");
          printf(" \n\n\n Jugador: \n\n\n");
          printBoard(p1, s);
          printf("\n Ha golpeado tu Nave! \n");
        }else{
          printf("\n Ha fallado! \n");
        }
      }else{
        printf("Turno del Oponente! \n");
        printf(" \n\n\n Jugador: \n\n\n");
        printBoard(p1, s);
        printf("\n Ha fallado! \n");
      }
}

void userAttackEz(struct Celda *sight, struct Celda *p1, int s){
  // La misma funcion que el ataque normal, solo que esta no imprime el tablero enemigo
  if(sight->status == 1){
        if(sight->hit == 0){
          sight->hit = 1;
          printf("Turno del Jugador! \n");
          printf(" \n\n\n Jugador: \n\n\n");
          printBoard(p1, s);
          printf("\n Has golpeado su Nave! \n");
        }
      }else{
        printf("Turno del Jugador! \n");
        printf(" \n\n\n Jugador: \n\n\n");
        printBoard(p1, s);
        printf("\n Ha fallado! \n");
      }
}