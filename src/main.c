#include <stdio.h>
#include <stdlib.h>
//progetto API


//Strutture:

//struct Station: rappresentazione di una Station; le stazioni sono raggruppate in un albero BST
struct Station {
    //int numCars: numero di auto presenti nella stazione
    int numCars;
    //int *cars: vettore in ordine decrescente contenente le autonomie delle auto presenti nella stazione,
    int *cars;
    //int distance: distanza della stazione rispetto a 0
    int distance;
    //struct Station *left: puntatore al figlio sinistro
    struct Station* left;
    //struct Station *right: puntatore al figlio destro
    struct Station* right;
};

//struct auxiliaryStation: rappresentazione di una stazione (ausiliaria); le stazioni sono raggruppate in un vettore ordinato (crescente) al fine di calcolare un percorso minimo
struct auxiliaryStation {
    //int pathCode: posizione (indice) nel vettore
    int pathCode;
    //int distance: distanza della stazione rispetto a 0
    int distance;
    //int distance: massima autonomia dell'auto presente nella stazione
    int km;
};

//Dichiarazione funzioni
struct Station* addStation(struct Station* root, int distance);
struct Station* removeStation(struct Station* root, int distance, int *originalDistance);
struct Station* createStationNode(int distance);

void addCars(struct Station *station);
int addOneCar(struct Station *root, int distance, int km);
int removeCar(struct Station *root, int distance , int km);

void increasingBFS(int size, struct auxiliaryStation *stations, int *parent);
void decreasingBFS(int size, struct auxiliaryStation *stations, int *parent);

void printIncreasingPath(int size, const int *parent, struct auxiliaryStation *stations);
void printDecreasingPath(int size, const int *parent, struct auxiliaryStation *stations);

void browsBST(struct Station* root, struct auxiliaryStation* stations, int x, int y, int* index);
struct auxiliaryStation* getRangedStations(struct Station* root, int x, int y, int* size);

int binarySearch(const int vector[], int size, int value);
void addInDecreasingOrder(int **vector, int *size, int value);

void manager();





















//struct Station* addStation(struct Station* root, int distance): aggiunge una stazione (distanza presa dal parametro) all'albero (root) e ritorna un puntatore alla punta dell'albero; se esiste gia una stazione con tale distanza non aggiunge nulla, e ritorna NULL
struct Station* addStation(struct Station* root, int distance) {
    struct Station* newNode = createStationNode(distance);
     //se l'albero è vuoto
    if (root == NULL) {
        addCars(newNode);
        printf("aggiunta\n");
        return newNode;
    }

    struct Station* curr = root;
    struct Station* father = NULL;


    while (curr != NULL) {
        father = curr;
        if (distance == curr->distance) {
            //esiste già una stazione con tale distance, devo esaurire il numero di auto e le auto stesse scritte comunque dall'utente
            int tmp, dim;
            if(scanf("%d", &dim) == EOF){
                exit(-1);
            }
            for(int i = 0; i <dim; i++){
                if(scanf("%d", &tmp) == EOF){
                    exit(-1);
                }
            }
            printf("non aggiunta\n");
            free(newNode);
            return root;
        }
        if (distance <= curr->distance) {
            //cerco a sinistra
            curr = &(*curr->left);
        } else {
            //cerco a destra
            curr = &(*curr->right);
        }
    }

    if (distance <= father->distance) {
        father->left = newNode;
        addCars(newNode);
        //il nuovo nodo è stato aggiunto correttamente
        printf("aggiunta\n");
    } else {
        father->right = newNode;
        addCars(newNode);
        //il nuovo nodo è stato aggiunto correttamente
        printf("aggiunta\n");
    }
    return root;
}
//struct Station* removeStation(struct Station* root, int distance, int *originalDistance)
struct Station* removeStation(struct Station* root, int distance, int *originalDistance){

    //stazione con tale distanza non trovata
    if (root == NULL) {
        printf("non demolita\n");
        return root;
    }


    if (distance < root->distance) {
        //cerco la stazione con tale distance nel sottoAlbero sinistro
        root->left = removeStation(root->left, distance, originalDistance);
    } else if (distance > root->distance) {
        //cerco la stazione con tale distance nel sottoAlbero destra
        root->right = removeStation(root->right, distance, originalDistance);
    } else {
        //è stata trovata la stazione con tale distanza
        if(*originalDistance == distance){
            printf("demolita\n");
        }
        //ricalibro l'albero
        if (root->left == NULL) {
            struct Station* temp = root->right;
            return temp;
        } else if (root->right == NULL) {
            struct Station* temp = root->left;
            return temp;
        }
        struct Station* temp = root->right;
        while (temp->left != NULL) {
            temp = &(*temp->left);
        }

        root->distance = temp->distance;
        root->cars = NULL;
        root->cars = temp->cars;
        root->numCars = temp->numCars;
        root->right = removeStation(root->right, temp->distance, originalDistance);
    }


    return root;
}
//struct Station* createStationNode(int distance): funzione che crea un nodo di tipo struct Station * e lo restituisce
struct Station* createStationNode(int distance) {
    struct Station* newNode = (struct Station*)malloc(sizeof(struct Station));
    if (newNode == NULL) {
        exit(-1);
    }
    newNode->numCars = 0;
    newNode->cars = NULL;
    newNode->distance = distance;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//void addCars(struct Station *station): aggiunge un numero variabile di auto (a discrezione dell'utente) alla stazione (parametro); questa funzione viene chiamato ogni volta che ''struct Station* addStation(struct Station* root, int distance)'' va a buon fine
void addCars(struct Station *station) {
    int km = 0;
    //numero di auto da aggiungere
    int numCars;
    if(scanf("%d", &numCars) == EOF){
        exit(-1);
    }
    station->numCars = numCars;
    if(numCars == 0){
        station->cars = NULL;
        return;
    }
    station->cars = (int *)malloc(sizeof (int) * numCars);
    int *dimensione = (int *)malloc(sizeof (int));
    (*dimensione) = 0;

    for(int i = 0; i < numCars; i++){
        if(scanf("%d", &km) == EOF){
            exit(-1);
        }
        addInDecreasingOrder(&station->cars, dimensione, km);
    }
    free(dimensione);
}
//int addOneCar(struct Station *root, int distance, int km) aggiungi un'auto con autonomia alla stazione con tale distance; se esiste la stazione, aggiunge l'auto e ritorna 1, altrimenti ritorna -1
int addOneCar(struct Station *root, int distance, int km){
    int *vector;

    struct Station *station = root;

    while(station->distance != distance){
        if(distance > station->distance){
            station = &(*station->right);
        }else{
            station = &(*station->left);
        };
        //non esiste una station con tale distance
        if(station == NULL){
            return -1;
        }
    }

    //se la station non aveva auto
    if(station->cars == NULL){
        (vector) = (int *)malloc(sizeof(int));

    } else {

    }

    station->numCars++;
    int numCars = station->numCars;
    if(station->cars != NULL){
        station->cars =(int *) realloc(station->cars, (numCars) * sizeof(int ));
        vector = station->cars;
    }


    if (vector == NULL) {
        exit(-1);
    }


    //aggiunge l'auto in station->cars (in ordine crescente)
    int i = numCars - 1;
    while (i > 0 && km > (vector)[i - 1]) {
        (vector)[i] = (vector)[i - 1];
        i--;
    }
    (vector)[i] = km;
    station->cars = vector;
    return 1;
}
//int removeCar(struct Station *root, int distance, int km): funzione per rimuovere l'auto con autonomia km dalla stazione con tale distanza; se esiste l'auto la rimuove e ritorna 1, altrimenti ritorna -1 (anche se non esiste la stazione)
int removeCar(struct Station *root, int distance, int km){

    //prima controllo se esiste una stazione con tale distanza
    struct Station *stazione = root;
    while(stazione->distance != distance){
        if(distance > stazione->distance){
            stazione = &(*stazione->right);
        }else{
            stazione = &(*stazione->left);
        };

        if(stazione == NULL){
            //non esiste una stazione con tale distanza
            return -1;
        }
    }

    //rimuovo l'auto
    int indice = binarySearch(stazione->cars, stazione->numCars, km);
    if(indice == -1){
        //non esiste tale auto nella stazione
        return -1;
    } else{
        for(int i = indice; i < stazione->numCars - 1; i++){
            stazione->cars[i] = stazione->cars[i+1];
        }
        stazione->numCars--;
        if(stazione->numCars!=0){
            stazione->cars = (int *)realloc(stazione->cars, stazione->numCars * sizeof(int));
        } else {
            stazione->cars = NULL;
        }
        return 1;
    }
}

//void decreasingBFS(int size, struct auxiliaryStation *stations, int *parent): funzione per calcolare il percorso nel caso la stazione di partenza abbia una distanza minore rispetto alla stazione di arrivo; utilizzo l'algoritmo BFS visto a lezione
void increasingBFS(int size, struct auxiliaryStation *stations, int *parent){

    int visited[size];
    for(int i = 0; i < size; i++){
        visited[i] = -1;
        parent[i] = -1;
    }

    int queue[size];
    int front = 0, rear = 0;

    visited[0] = 1;
    queue[rear++] = 0;
    int i ;

    while(front != rear) {
        int current = queue[front++];
        if(current == size - 1){
            return;
        }
        i = 1;
        struct auxiliaryStation *temp = &stations[current + i];

        while(temp != NULL){
            if(abs(temp->distance - stations[current].distance) <= stations[current].km){
                int adjacency = temp->pathCode;

                if(visited[adjacency] == -1) {
                    parent[adjacency] = current;
                    if(adjacency == size - 1) {
                        return;
                    }
                    visited[adjacency] = 1;
                    queue[rear++] = adjacency;
                }
                i++;
                if(i <= size - 1){
                    temp = &stations[current + i];
                } else {
                    temp = NULL;
                }
            } else temp = NULL;

        }
    }
}
//void decreasingBFS(int size, struct auxiliaryStation *stations, int *parent): funzione per calcolare il percorso nel caso la stazione di partenza abbia una distanza maggiore rispetto alla stazione di arrivo; utilizzo l'algoritmo BFS rivisitato in modo tale che siano predilette le stations con la distance minore
void decreasingBFS(int size, struct auxiliaryStation *stations, int *parent){

    int visited[size];
    int queue[size];
    for(int i = 0; i < size; i++){
        visited[i] = -1;
        parent[i] = -1;
        queue[i] = -1;
    }
    int front = 0, rear = 0;
    visited[0] = 1;
    queue[rear++] = 0;
    int i ;

    while(front != rear) {
        int current = queue[front++];
        if(current == -1){
            //nessun percorso disponibile
            parent[size - 1] = -1;
        }

        i = 1;
        //partendo da &stations[current + 1] prediligo le stations più vicine allo 0
        struct auxiliaryStation *temp = &stations[current + 1];

        while(temp != NULL){
            if(abs(temp->distance - stations[current].distance) <= temp->km){
                //se temp può arrivare in stations[current] allora lo segno in parent
                int adjacency = temp->pathCode;
                if(visited[adjacency] == -1) {
                    parent[adjacency] = current;
                    if(temp->pathCode == size - 1) {
                        return;
                    }
                    visited[adjacency] = 1;
                    queue[rear++] = temp->pathCode;
                }
                i++;
                if(current + i <= size - 1){
                    temp = &stations[current + i];
                } else{
                    temp  = NULL;
                }
            } else {
                i++;
                if(current + i <= size - 1){
                    temp = &stations[current + i];
                } else{
                    temp  = NULL;
                }
            }
        }
    }
}

//void printIncreasingPath(int size, const int *parent, struct auxiliaryStation *stations): funzione per stampare il percorso generato precedentemente da increasingBFS
void printIncreasingPath(int size, const int *parent, struct auxiliaryStation *stations) {
    if (0 == size - 1) {
        printf("%d", stations[0].distance);
        return;
    }

    if (parent[size - 1] == -1) {
        printf("nessun percorso");
        return;
    }
    int curr = size - 1;
    int path[100];
    int lung = 0;

    while (curr != 0) {
        path[lung] = curr;
        lung++;
        curr = parent[curr];
    }
    path[lung] = 0;
    for (int i = lung; i >= 0; i--) {
        if(i!= 0){
            printf("%d ", stations[path[i]].distance);
        } else {
            printf("%d", stations[path[i]].distance);
        }

    }
}
//void printDecreasingPath(int size, const int *parent, struct auxiliaryStation *stations): funzione per stampare il percorso generato precedentemente da decreasingBFS
void printDecreasingPath(int size, const int *parent, struct auxiliaryStation *stations) {
    //se la stazione di partenza è anche la stazione di arrivo
    if (0 == size - 1) {
        printf("%d", stations[0].distance);
        return;
    }

    //non esiste nessun percorso
    if (parent[size - 1] == -1) {
        printf("nessun percorso");
        return;
    }
    int path[100];
    int i = 0;
    int curr = size - 1;
    while (curr != 0) {
        path[i] = curr;
        curr = parent[curr];
        i++;
    }
    path[i] = 0;

    //stampo il percorso
    for (int j = 0; j <= i; j++) {
        if(j!= 0){
            printf("%d ", stations[path[j]].distance);
        } else {
            printf("%d ", stations[size - 1].distance);
        }
    }
}

//void browsBST(struct Station* root, struct auxiliaryStation* stations, int x, int y, int* index): esplora l'albero in ordine di distanza crescente, conta le stazioni da inserire nel vettore stations se stations è NULL, se stations != NULL le inserisce
void browsBST(struct Station* root, struct auxiliaryStation* stations, int x, int y, int* index) {
    if (root == NULL) return;

    //esploro il sottoAlbero sinistro
    browsBST(root->left, stations, x, y, index);

    // Se il valore del nodo è compreso tra x e y, aggiunge root al vettore stations
    if (stations != NULL && root->distance >= x && root->distance <= y ) {
        stations[(*index)++].distance = root->distance;
        stations[(*index) - 1].pathCode = (*index) - 1;
        if(root->cars != NULL){
            stations[(*index) - 1].km = root->cars[0];
        } else {
            stations[(*index) - 1].km = 0;
        }
    }
    // conta le lunghezza di stations
    else if(root->distance >= x && root->distance <= y && stations == NULL){
        (*index)++;
    }

    //esploro il sottoAlbero destro
    browsBST(root->right, stations, x, y, index);
}
//struct auxiliaryStation* getRangedStations(struct Station* root, int x, int y, int* size): root è la punta dell'albero, x e y sono le distanze ove le distanze delle stazioni devono essere comprese per essere dentro al vettore ritornato; *size verrà inizializzata con il valore di lunghezza del vettore
struct auxiliaryStation* getRangedStations(struct Station* root, int x, int y, int* size) {
    //prima conto quanti nodi ci sono con x <= distanza <= y (o y <= distanza <= x)
    int count = 0;
    if(x<y){
        browsBST(root, NULL, x, y,  &count);
    } else {
        browsBST(root,NULL, y, x,  &count);
    }
    //creo il vettore
    struct auxiliaryStation* vettore = (struct auxiliaryStation*)malloc(count * sizeof(struct auxiliaryStation));


    int index = 0;

    //ora inserisco le stazioni nel vettore
    if(y>x){
        browsBST(root, vettore, x, y, &index);
    } else {
        browsBST(root,vettore, y, x,  &index);
    }

    //inizializzo il valore di *size a count, ovvero la lunghezza del vettore
    *size = count;


    return vettore;
}

//int binarySearch(const int vector[], int size, int value): trova il valore value nel vettore vector[size], il quale è ordinato in ordine decrescente; ritorna -1 se non è stato trovato, altrimenti ritorna l'indice ove si trova value
int binarySearch(const int vector[], int size, int value) {
    int lx = 0;
    int rx = size - 1;
    while (lx <= rx) {
        int center = (lx + rx) / 2;
        if (vector[center] == value) {
            return center;
        } else if (vector[center] < value) {
            rx = center - 1;
        } else {
            lx = center + 1;
        }
    }
    return -1; // L'elemento non è stato trovato
}
//void addInDecreasingOrder(const int vector[][], int *size, int value): aggiunge al vettore *vector (di dimensione *size) l'intero value, in modo tale che sia ordinato in ordine decrescente; l'allocazione del vettore è gestita esternamente
void addInDecreasingOrder(int **vector, int *size, int value) {
    int position = *size;
    while (position > 0 && value > (*vector)[position - 1]) {
        (*vector)[position] = (*vector)[position - 1];
        position--;
    }
    (*size)++;
    (*vector)[position] = value;
}

//void manager(): funzione che gestisce le richieste dell'utente, chiamando le opportune funzioni
void manager(){
    char request[30];
    struct Station *stationThree = NULL;
    while(scanf("%s", request) != EOF){

        if(request[7] == '-'){
            //caso "rottama-auto"
            //"rottama-auto" case

            //int distance: distanze della stazione ove si trova l'eventuale auto da rottamare
            //int km: autonomia dell'auto da rottamare
            int distance,km;
            if(scanf("%d", &distance) == EOF){
                exit(-1);
            }
            if(scanf("%d", &km) == EOF){
                exit(-1);
            }
            if(removeCar(stationThree, distance, km) == -1){
                printf("non rottamata\n");
            } else{
                printf("rottamata\n");
            }
        } else if(request[8] == '-'){
            //caso "aggiungi-stazione"
            //"aggiungi-stazione" case

            //int distance: distanza della stazione che si vuole aggiungere, o distanza della stazione ove si vuole aggiungere un'auto
            int distance;
            if(scanf("%d", &distance) == EOF){
                exit(-1);
            }
            if(request[9] == 's'){
                stationThree = addStation(stationThree, distance);

            } else {
                //caso "aggiungi-auto"
                //"aggiungi-auto" case

                //int km: autonomia dell'auto da aggiungere
                int km;
                if(scanf("%d", &km) == EOF){
                    exit(-1);
                }
                if(addOneCar(stationThree, distance, km) == -1){
                    printf("non aggiunta\n");
                } else {
                    printf("aggiunta\n");
                }
            }
        } else{
            if(request[10] == 's'){
                //caso "demolisci-stazione"
                //"demolisci-stazione" case

                //int distance: distanza della stazione che si vuole demolire
                int distance;
                if(scanf("%d", &distance) == EOF){
                    exit(-1);
                }
                stationThree = removeStation(stationThree, distance, &distance);

            } else{
                //caso "pianifica-percorso"
                //"pianifica-percorso" case

                //int startDistance: distanza della stazione ove inizia l'eventuale percorso
                //int endDistance: distanza della stazione ove termina l'eventuale percorso
                int startDistance, endDistance;
                if(scanf("%d", &startDistance) == EOF){
                    exit(-1);
                }
                if(scanf("%d", &endDistance) == EOF){
                    exit(-1);
                }

                //int size: lunghezza del vettore struct auxiliaryStation *stazioni
                //int *parent: vettore per salvare le tappe dell'eventuale percorso
                int size, *parent;

                //struct auxiliaryStation *stazioni: vettore ove vengono salvate in ordine crescente tutte le stazioni con distanza compresa fra starDistance ed endDistance
                struct auxiliaryStation *stazioni = getRangedStations(stationThree, startDistance, endDistance, &size);
                parent = (int *)malloc(sizeof(int) * size);
                if(startDistance < endDistance){
                    //calcolo le tappe
                    increasingBFS(size, stazioni, parent);
                    //stampo le tappe
                    printIncreasingPath(size, parent, stazioni);
                    printf("\n");
                } else {
                    //calcolo le tappe
                    decreasingBFS(size, stazioni, parent);
                    //stampo le tappe
                    printDecreasingPath(size, parent, stazioni);
                    printf("\n");
                }
                //libero la memoria da stazioni
                free(stazioni);
            }
        }


    }
}



//MAIN
int main() {
    manager();
    return 0;
}