#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

int details = 0;
int graph = 0;

int n = 0; /// dimensione dell'array

/// file di output per grafo
ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

//////////////////////////////////////////////////
/// Definizione della struttura dati lista
//////////////////////////////////////////////////

/// struct per il nodo della lista
typedef struct node
{
    int val;
    struct node* next;
} node_t;

/// struct per la lista
typedef struct list
{
    node* head;
    // node* tail; /// per lista doubly linked
} list_t;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati lista
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/// Definizione della struttura dati grafo
//////////////////////////////////////////////////

int* V;          // elenco dei nodi del grafo
int* V_visitato; // nodo visitato?
int* V_distanza; // distanza dal nodo di partenza
int* G_visitato; // global visitato?

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

int get_address(void* node)
{
    return (int)((long)node - (long)global_ptr_ref);
}

void node_print(int n)
{

    output_graph << "node_" << n << "_" << n_operazione << endl;
    output_graph << "[ shape = oval; ";

    if (G_visitato[n] == 1)
        output_graph << "fillcolor = \"#555555\"; style=filled; ";
    else if (V_visitato[n] == 1)
        output_graph << "fillcolor = \"#bbbbbb\"; style=filled; ";
    output_graph << "label = "
        << "\"Idx: " << n << ", val: " << V[n] << "\" ];\n";

    node_t* elem = E[n]->head;
    while (elem != NULL) { /// disegno arco
        output_graph << "node_" << n << "_" << n_operazione << " -> ";
        output_graph << "node_" << elem->val << "_" << n_operazione << " [  color=gray ]\n";
        elem = elem->next;
    }
}

void graph_print()
{
    for (int i = 0; i < n_nodi; i++)
        node_print(i);
    n_operazione++;
}

void list_print(list_t* l)
{
    printf("Stampa lista\n");

    if (l->head == NULL) {
        printf("Lista vuota\n");
    }
    else {
        node_t* current = l->head;

        while (current != NULL) {
            if (!details)
                printf("%d, ", current->val);
            else { /// stampa completa
                if (current->next == NULL)
                    printf("allocato in %d [Val: %d, Next: NULL]\n",
                        get_address(current),
                        current->val);
                else
                    printf("allocato in %d [Val: %d, Next: %d]\n",
                        get_address(current),
                        current->val,
                        get_address(current->next));
            }
            current = current->next;
        }
        printf("\n");
    }
}

list_t* list_new(void)
{
    list_t* l = new list;
    if (details) {
        printf("Lista creata\n");
    }

    l->head = NULL;
    if (details) {
        printf("Imposto a NULL head\n");
    }

    return l;
}

void list_insert_front(list_t* l, int elem)
{
    /// inserisce un elemento all'inizio della lista
    node_t* new_node = new node_t;
    new_node->next = NULL;

    new_node->val = elem;

    new_node->next = l->head;

    l->head = new_node;
}

int find_cycle(int n, int depth = 0)
{
    graph_print();

    if (details)
        printf("DFS: lavoro sul nodo %d (visitato %d)\n", n, V_visitato[n]);

    if (V_visitato[n] == 1)
        return depth - V_distanza[n]; // c'e' il ciclo

    if (V_visitato[n] == 2)
        return 0; // trovato un nuovo percorso alternativo (non c'e' il ciclo)

    V_visitato[n] = 1; // prima volta che incontro questo nodo
    V_distanza[n] = depth; // distanza dal nodo di partenza

    if (details)
        printf("Visito il nodo %d (val %d)\n", n, V[n]);

    int t = 0;
    node_t* elem = E[n]->head;
    while (elem != NULL) {
        int temp = find_cycle(elem->val, depth + 1);
        if (temp > t)
            t = temp;
        elem = elem->next;
    }

    V_visitato[n] = 2;
    G_visitato[n] = 1;

    return t;
}

int full_find_cycle() {
    for (int i = 0; i < n_nodi; i++) {
        G_visitato[i] = 0; // flag = non visitato
    }
    int max = 0;
    int t = 0;

    for (int i = 0; i < n_nodi; i++) {
        if (G_visitato[i] == 1) {
            if (details)
                printf("nodo %d gia' visitato\n", i);
            continue; // nodo gia' visitato
        }

        for (int j = 0; j < n_nodi; j++) {
            V_visitato[j] = 0; // flag = non visitato
            V_distanza[j] = 0; // flag = non visitato
        }
        t = find_cycle(i);
        graph_print();
        if (t > max)
            max = t;
        printf("test %d: ciclo = %d\n", i, t);
    }

    return max;
}

int parse_cmd(int argc, char** argv)
{
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

int main(int argc, char** argv)
{
    int i, test;

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

    n_nodi = 12;
    V = new int[n_nodi];
    V_visitato = new int[n_nodi];
    V_distanza = new int[n_nodi];
    G_visitato = new int[n_nodi];
    E = new list_t * [n_nodi];

    for (int i = 0; i < n_nodi; i++) {
        V[i] = 3 * i;
        V_visitato[i] = 0;  // flag = non visitato
        E[i] = list_new();
    }

    // Creazione del primo grafo sconnesso
    list_insert_front(E[0], 1);
    list_insert_front(E[1], 2);
    list_insert_front(E[2], 3);
    list_insert_front(E[3], 2);

    // Creazione del secondo grafo sconnesso
    list_insert_front(E[4], 5);
    list_insert_front(E[5], 6);
    list_insert_front(E[6], 7);
    list_insert_front(E[7], 6);

    // Creazione del terzo grafo sconnesso
    list_insert_front(E[8], 9);
    list_insert_front(E[9], 10);
    list_insert_front(E[10], 11);
    list_insert_front(E[10], 9);
    list_insert_front(E[11], 8);

    graph_print();

    if (details)
        for (int i = 0; i < n_nodi; i++) {
            printf("Sono il nodo di indice %d nell'array\n", i);
            printf("Il valore del nodo e' %d\n", V[i]);
            printf("La lista di adiacenza e'\n");
            list_print(E[i]);
        }

    //int t = 0;
    //for (int i = 0; i < n_nodi; i++) {
    //    t += has_cycle(i);
    //    // printf("test %d: ciclo = %d\n", i, t);
    //}

    int t = full_find_cycle();
    printf("Ciclo più lungo = %d\n", t);

    if (graph) {
        /// preparo footer e chiudo file
        output_graph << "}" << endl;
        output_graph.close();
        cout << " File graph.dot scritto" << endl
            << "Creare il grafo con: dot graph.dot -Tpdf -o graph.pdf" << endl;
    }

    return 0;
}
