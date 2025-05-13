#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

// compilazione: g++ lezione17-grafi-dijkstra.cpp
//
// Obiettivo:
// 1) grafo con archi pesati
// 2) implementazione shortest path

#define INFTY 1000000

int details = 0;
int graph = 0;

/// file di output per grafo
ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

int ct_visit = 0; // contatore durante visita

//////////////////////////////////////////////////
/// Definizione della struttura dati lista
//////////////////////////////////////////////////

/// struct per il nodo della lista
typedef struct node {
    int val; /// prossimo nodo
    float w; /// peso dell'arco
    struct node* next;
} node_t;

/// struct per la lista
typedef struct list {
    node* head;
} list_t;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati lista
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/// Definizione della struttura dati grafo
//////////////////////////////////////////////////

int* V;          // elenco dei nodi del grafo
int* V_visitato; // nodo visitato?
int* V_prev;     // nodo precedente dalla visita
float* V_dist;   // distanza da sorgente

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
list_t** E; /// array di puntatori a le liste di adiacenza per ogni nodo
int n_nodi;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati grafo
//////////////////////////////////////////////////

/// Questo e' un modo per stampare l'indirizzo node relativamente ad un altro di riferimento.
/// Permette di ottenere offset di piccola dimensione per essere facilmente visualizzati
/// Nota: il metodo non e' robusto e potrebbe avere comportamenti indesiderati su architetture diverse
/// L'alternativa corretta' e' utilizzare %p di printf: es. printf("%p\n",(void*) node);
/// con lo svantaggio di avere interi a 64 bit poco leggibili

list_t* global_ptr_ref = NULL; /// usato per memorizzare il puntatore alla prima lista allocata

int get_address(void* node) {
    return (int)((long)node - (long)global_ptr_ref);
}

void node_print(int n) {

    /// calcolo massima distanza (eccetto infinito)
    float max_d = 0;
    for (int i = 0; i < n_nodi; i++)
        if (max_d < V_dist[i] && V_dist[i] < INFTY)
            max_d = V_dist[i];

    output_graph << "node_" << n << "_" << n_operazione << endl;
    output_graph << "[ shape = oval; ";

    if (V_visitato[n] == 1)
        output_graph << "penwidth = 4; ";

    float col = V_dist[n] / max_d; /// distanza in scala 0..1
    output_graph << "fillcolor = \"0.0 0.0 " << col / 2 + 0.5 << "\"; style=filled; ";
    if (V_dist[n] < INFTY)
        output_graph << "label = "
        << "\"Idx: " << n << ", dist: " << V_dist[n] << "\" ];\n";
    else
        output_graph << "label = "
        << "\"Idx: " << n << ", dist: INF\" ];\n";

    node_t* elem = E[n]->head;
    while (elem != NULL) { /// disegno arco
        output_graph << "node_" << n << "_" << n_operazione << " -> ";
        output_graph << "node_" << elem->val << "_" << n_operazione << " [ label=\"" << elem->w << "\", len=" << elem->w / 100 * 10 << " ]\n";
        elem = elem->next;
    }

    if (V_prev[n] != -1) { // se c'e' un nodo precedente visitato -> disegno arco

        float len = 0;
        /*
        //cerco arco V_prev[n] --> V[n]
        node_t* elem=E[ V_prev[n] ]->head;
        while (elem!=NULL){
        int v=elem->val; /// arco u --> v
        if (v == V[n])
          len=elem->w;
        elem=elem->next;
          }
        */

        len = 1;
        output_graph << "node_" << n << "_" << n_operazione << " -> ";
        output_graph << "node_" << V_prev[n] << "_" << n_operazione << " [ color=blue, penwidth=5, len=" << len / 100 * 10 << " ]\n";
    }
}

void graph_print() {
    for (int i = 0; i < n_nodi; i++)
        node_print(i);
    n_operazione++;
}

void list_print(list_t* l) {
    printf("Stampa lista\n");

    if (l->head == NULL) {
        printf("Lista vuota\n");
    }
    else {
        node_t* current = l->head;

        while (current != NULL) {
            if (!details)
                printf("%d w:%f, ", current->val, current->w);
            else { /// stampa completa
                if (current->next == NULL)
                    printf("allocato in %d [Val: %d, W: %f, Next: NULL]\n",
                        get_address(current),
                        current->val,
                        current->w);
                else
                    printf("allocato in %d [Val: %d, W: %f, Next: %d]\n",
                        get_address(current),
                        current->val,
                        current->w,
                        get_address(current->next));
            }
            current = current->next;
        }
        printf("\n");
    }
}

list_t* list_new(void) {
    list_t* l = new list;
    if (details) {
        printf("Lista creata\n");
    }

    l->head = NULL; //// perche' non e' l.head ?
    if (details) {
        printf("Imposto a NULL head\n");
    }

    return l;
}

void list_insert_front(list_t* l, int elem, float w) {
    /// inserisce un elemento all'inizio della lista
    node_t* new_node = new node_t;
    new_node->next = NULL;

    new_node->val = elem;
    new_node->w = w;

    new_node->next = l->head;

    l->head = new_node;
}

/// HEAP//
const int MAX_SIZE = 256; /// allocazione statica
float heap[MAX_SIZE];
int heapIndex[MAX_SIZE]; /// array di indici per il nodo
int heap_size = 0; /// dimensione attuale dell'heap

int parent_idx(int n) {
    if (n == 0)
        return -1;
    return (n - 1) / 2;
}

int child_L_idx(int n) {
    if (2 * n + 1 >= heap_size)
        return -1;
    return 2 * n + 1;
}

int child_R_idx(int n) {
    if (2 * n + 2 >= heap_size)
        return -1;
    return 2 * n + 2;
}

int is_leaf(int n) {
    return (child_L_idx(n) == -1); // non c'e' bisogno di controllare il figlio R
}

void heap_insert(float elem, int idx) {
    /// inserisco il nuovo nodo con contenuto elem
    /// nell'ultima posizione dell'array
    /// ovvero continuo a completare il livello corrente

    if (details)
        printf("Inserisco elemento %f in posizione %d\n", elem, heap_size);

    if (heap_size < MAX_SIZE) {
        int i = heap_size;
        heap_size++;

        heap[i] = elem;
        heapIndex[i] = idx;

        while (i != 0) {                          // non sono sulla radice
            if (heap[parent_idx(i)] <= heap[i]) { /// proprieta' dell' heap e' rispettata -> esco
                if (details)
                    printf("Il genitore ha valore %f <= del nodo %f, esco\n", heap[parent_idx(i)], heap[i]);
                return;
            }

            if (details)
                printf("Il genitore ha valore %f > del nodo %f, swap\n", heap[parent_idx(i)], heap[i]);
            /// il nodo ha un genitore sicuramente <   --> swap
            float t = heap[parent_idx(i)];
            int tIdx = heapIndex[parent_idx(i)];
            heap[parent_idx(i)] = heap[i];
            heapIndex[parent_idx(i)] = heapIndex[i];
            heap[i] = t;
            heapIndex[i] = tIdx;

            i = parent_idx(i);
        }
    }
    else
        printf("Heap pieno!\n");
}

pair<float, int> heap_remove_min() {

    if (heap_size <= 0) { /// heap vuoto!
        printf("Errore: heap vuoto\n");
        return { NULL,-1 };
    }

    float minimo = heap[0];
    int idx_min = heapIndex[0];

    if (details)
        printf("Minimo identificato %f\n", minimo);
    /// scambio la radice con l'ultima foglia a destra
    /// il minimo e' stato spostato in fondo --> pronto per l'eliminazione
    float t = heap[0];
    int tIdx = heapIndex[0];
    heap[0] = heap[heap_size - 1];
    heapIndex[0] = heapIndex[heap_size - 1];
    heap[heap_size - 1] = t;
    heapIndex[heap_size - 1] = tIdx;

    // elimino il minimo (ora in fondo all'array)
    heap_size--;

    //    tree_print_graph(0);  // radice

    /// nella radice c'e' un valore grabde (massimo?)
    int i = 0; // indice di lavoro (parto dalla root)

    while (!is_leaf(i)) { /// garantisco di fermarmi alla foglia

        if (details)
            printf("Lavoro con il nodo in posizione i = %d, valore %f\n", i, heap[i]);

        int con_chi_mi_scambio = -1;

        /// controllo il nodo i con il suo figlio L
        if (heap[i] > heap[child_L_idx(i)]) { // il nodo i e' piu' grabde
            /// attivare uno swap (la proprieta' heap non e' rispettata)
            con_chi_mi_scambio = child_L_idx(i);
            if (details)
                printf("Figlio L e' piu' piccolo (valore %f)\n", heap[child_L_idx(i)]);

            if (child_R_idx(i) >= 0 && // esiste il nodo destro
                heap[child_L_idx(i)] > heap[child_R_idx(i)]) {
                con_chi_mi_scambio = child_R_idx(i);
                if (details)
                    printf("Figlio R e' ancora piu' piccolo (valore %f)\n", heap[child_R_idx(i)]);
            }
        }
        else { // il nodo e' piu' piccolo del figlio L

            if (child_R_idx(i) >= 0) {                // esiste il figlio R
                if (heap[i] > heap[child_R_idx(i)]) { /// attivo lo swap
                    con_chi_mi_scambio = child_R_idx(i);
                    if (details)
                        printf("Figlio R e' piu' piccolo del nodo (valore %f)\n", heap[child_R_idx(i)]);
                }
                else
                    break;
            }
            else
                break;
        }

        /// swap tra i e con_chi_mi_scambio
        float t = heap[i];
        int tIdx = heapIndex[i];
        heap[i] = heap[con_chi_mi_scambio];
        heapIndex[i] = heapIndex[con_chi_mi_scambio];
        heap[con_chi_mi_scambio] = t;
        heapIndex[con_chi_mi_scambio] = tIdx;

        i = con_chi_mi_scambio;

        // tree_print_graph(0);  // radice
    }

    return { minimo, idx_min }; /// ritorno il valore e l'indice del nodo
}

/// END HEAP //

void shortest_path(int n) {

    /*      V_visitato[i]=0;  // flag = non visitato
      V_prev[i]=-1;  // non c'e' precedente
      V_dist[i]=INFTY;  // infinito
    */

    V_dist[n] = 0;
    heap_insert(V_dist[n], n); /// inserisco il nodo sorgente in coda

    graph_print();

    while (heap_size > 0) {
        auto best = heap_remove_min(); /// estraggo il nodo con distanza minima
        int best_idx = best.second;
        if (details)
            printf("(1) Estraggo il nodo %d con distanza %f\n", best_idx, best.first);

        /// estrai dalla coda
        int u = best_idx;
        if (V_visitato[u] == 1)
            continue;

        V_visitato[u] = 1;

        /// esploro la lista di adiacenza
        node_t* elem = E[u]->head;
        while (elem != NULL) {
            int v = elem->val; /// arco u --> v

            /// alt ← dist[u] + Graph.Edges(u, v)
            float alt = V_dist[u] + elem->w; /// costo per arrivare al nuovo nodo passando per u
            if (alt < V_dist[v]) {           // il percorso sorgente ---> u --> v migliora il percorso attuale sorgente --> v
                V_dist[v] = alt;
                V_prev[v] = u;
            }


            heap_insert(V_dist[v], v); /// inserisco il nuovo nodo in coda
            if (details)
                printf("(2) Inserisco in coda il nodo %d con peso %f\n", v, V_dist[v]);

            elem = elem->next;
        }
        graph_print();
    }

    graph_print();
}

bool bellman_ford(int n, list_t** ADJ = E, int local_n_nodi = n_nodi) {
    V_prev[n] = n;
    V_dist[n] = 0;
    for (int i = 1; i <= local_n_nodi - 1; i++) {
        for (int u = 0; u < local_n_nodi; u++) {
            node_t* adj = ADJ[u]->head;
            while (adj != nullptr) {
                int v = adj->val;
                if (V_dist[u] != INFTY && V_dist[u] + adj->w < V_dist[v]) {  // se la distanza dalla sorgente al nodo corrente + il peso dell'arco è minore della distanza dalla sorgente al nodo
                    V_dist[v] = V_dist[u] + adj->w;                          // aggiorno la distanza dalla sorgente al nodo
                    V_prev[v] = u;                                           // aggiorno il nodo precedente
                }
                adj = adj->next;
            }
        }
    }

    for (int i = 0; i < local_n_nodi; i++) {  // controllo se esiste un ciclo con peso negativo
        node_t* adj = ADJ[i]->head;
        while (adj != nullptr) {
            int v = adj->val;
            if (V_dist[i] != INFTY && V_dist[i] + adj->w < V_dist[v]) {  // se la distanza dalla sorgente al nodo corrente + il peso dell'arco è minore della distanza dalla sorgente al nodo
                if (details)
                    printf("Esiste un ciclo con peso negativo\n");
                return false;
            }
            adj = adj->next;
        }
    }
    return true;
}


int parse_cmd(int argc, char** argv) {
    /// controllo argomenti
    int ok_parse = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'v') {
            details = 1;
            ok_parse = 1;
        }
        if (argv[i][1] == 'g') {
            graph = 1;
            ok_parse = 1;
        }
    }

    if (argc > 1 && !ok_parse) {
        printf("Usage: %s [Options]\n", argv[0]);
        printf("Options:\n");
        printf("  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
        printf("  -graph: creazione file di dot con il grafo dell'esecuzione (forza d=1 t=1)\n");
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    int i, test;

    // init random
    srand((unsigned)time(NULL));

    if (parse_cmd(argc, argv))
        return 1;

    if (graph) {
        output_graph.open("graph.dot");
        /// preparo header
        output_graph << "digraph g" << endl;
        output_graph << "{ " << endl;
        output_graph << "node [shape=none]" << endl;
        output_graph << "rankdir=\"LR\"" << endl;
        ;
        //    output_graph << "edge[tailclip=false,arrowtail=dot];"<<endl;
    }

    int N = 10;
    n_nodi = N * N;

    n_nodi = 10;

    // init nodi
    V = new int[n_nodi];
    V_visitato = new int[n_nodi];
    V_prev = new int[n_nodi];
    V_dist = new float[n_nodi];

    // init archi
    E = new list_t * [n_nodi]; //(list_t**)malloc(n_nodi*sizeof(list_t*));

    // costruzione grafo
    for (int i = 0; i < n_nodi; i++) {
        V[i] = 2 * i;
        V_visitato[i] = 0; // flag = non visitato
        V_prev[i] = -1;    // non c'e' precedente
        V_dist[i] = INFTY; // infinito

        E[i] = list_new();

        if (i == 0)
            global_ptr_ref = E[i];

        int x = i % N;
        int y = i / N;
    }

    int partenza = 0;
    int arrivo = n_nodi - 1;
    int w_max = 100;

    for (int i = 0; i < n_nodi - 1; i++) {
        /// arco costoso
        list_insert_front(E[i], arrivo, w_max - 2 * i);
        /// arco 1
        if (i > 0)
            list_insert_front(E[i - 1], i, 1);
    }

    graph_print();

    if (details)
        for (int i = 0; i < n_nodi; i++) {
            printf("Sono il nodo di indice %d nell'array\n", i);
            printf("Il valore del nodo e' %d\n", V[i]);
            printf("La lista di adiacenza e'\n");
            list_print(E[i]);
        }

    shortest_path(partenza);
    printf("Distanza minima dal nodo %d al nodo %d e' %f\n", partenza, arrivo, V_dist[arrivo]);
    graph_print();


    if (bellman_ford(0)) {
        printf("Bellman-Ford: distanza minima dal nodo %d al nodo %d e' %f\n", partenza, arrivo, V_dist[arrivo]);
        printf("Bellman-Ford: percorso minimo dal nodo %d al nodo %d e' ", partenza, arrivo);
        int i = arrivo;
        while (i != partenza) {
            printf("%d ", i);
            i = V_prev[i];
        }
        printf("%d \n", partenza);
    }
    else
        printf("Bellman-Ford: esiste un ciclo con peso negativo\n");

    //generate graph with negative cycle
    list_t** E2 = new list_t * [3];
    for (int i = 0; i < 3; i++) {
        E2[i] = list_new();
        if (i < 2)
            list_insert_front(E2[i], i + 1, -1);
        else
            list_insert_front(E2[i], 0, -1);
    }
    if (bellman_ford(0, E2, 3)) {
        printf("Bellman-Ford: distanza minima dal nodo %d al nodo %d e' %f\n", 0, 2, V_dist[2]);
        printf("Bellman-Ford: percorso minimo dal nodo %d al nodo %d e' ", 0, 2);
        int i = 2;
        while (i != 0) {
            printf("%d ", i);
            i = V_prev[i];
        }
    }
    else
        printf("Bellman-Ford: esiste un ciclo con peso negativo\n");

    if (graph) {
        /// preparo footer e chiudo file
        output_graph << "}" << endl;
        output_graph.close();
        cout << " File graph.dot scritto" << endl
            << "****** Creare il grafo con: neato graph.dot -Tpdf -o graph.pdf" << endl;
    }

    return 0;
}
