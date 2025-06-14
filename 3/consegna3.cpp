#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fstream>
#include <iostream>

using namespace std;

int details = 0;
int graph = 0;

int n = 0;  /// array dimension

/// output file for graph
ofstream output_graph;
int n_operazione = 0;  /// operation counter to visualize various steps

//////////////////////////////////////////////////
/// Definition of the list data structure
//////////////////////////////////////////////////

/// struct for the list node
typedef struct node {
  int val;
  struct node* next;
} node_t;

/// struct for the list
typedef struct list {
  node* head;
} list_t;

//////////////////////////////////////////////////
/// End Definition of the list data structure
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/// Definition of the graph data structure
//////////////////////////////////////////////////

int* V;           // list of graph nodes
int* V_visitato;  // visited node?
int* V_distanza;  // distance from the starting node
int* G_visitato;  // global visited?

list_t** E;  /// array of pointers to adjacency lists for each node
int n_nodi;

int maxCycleLength = 0;    /// maximum cycle length found
bool findedCycle = false;  /// flag to indicate if a new max cycle has been found
int startNode = -1;        /// starting node for the cycle search
list_t maxCycle;           /// list to store the maximum cycle found

//////////////////////////////////////////////////
/// End Definition of the graph data structure
//////////////////////////////////////////////////

list_t* global_ptr_ref = NULL;  /// used to store the pointer to the first allocated list

int get_address(void* node) {
  return (int)((long)node - (long)global_ptr_ref);
}

void node_print(int n) {
  output_graph << "node_" << n << "_" << n_operazione << endl;
  output_graph << "[ shape = oval; ";

  if (G_visitato[n] == 1)
    output_graph << "fillcolor = \"#555555\"; style=filled; ";
  else if (V_visitato[n] == 1)
    output_graph << "fillcolor = \"#bbbbbb\"; style=filled; ";
  output_graph << "label = "
               << "\"Idx: " << n << ", val: " << V[n] << "\" ];\n";

  node_t* elem = E[n]->head;
  while (elem != NULL) {  /// disegno arco
    output_graph << "node_" << n << "_" << n_operazione << " -> ";
    output_graph << "node_" << elem->val << "_" << n_operazione << " [  color=gray ]\n";
    elem = elem->next;
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
  } else {
    node_t* current = l->head;

    bool first = true;  // flag to print the first element differently
    while (current != NULL) {
      if (!details) {
        if (first) {
          printf("%d", current->val);
          first = false;  // after the first element, print with a comma
        } else
          printf(", %d", current->val);
      } else {  /// detailed print
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

list_t* list_new(void) {
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

void list_insert_front(list_t* l, int elem) {
  /// inserisce un elemento all'inizio della lista
  node_t* new_node = new node_t;
  new_node->next = NULL;

  new_node->val = elem;

  new_node->next = l->head;

  l->head = new_node;
}

int findCycle(int n, int depth = 0) {
  graph_print();

  if (details)
    printf("DFS: lavoro sul nodo %d (visitato %d)\n", n, V_visitato[n]);

  if (V_visitato[n] == 1) {
    return depth - V_distanza[n];  // there is a cycle
  }

  if (V_visitato[n] == 2)
    return 0;  // found a new alternative path (no cycle)

  V_visitato[n] = 1;      // first time I encounter this node
  V_distanza[n] = depth;  // distance from the starting node

  if (details)
    printf("Visito il nodo %d (val %d)\n", n, V[n]);

  int t = 0;
  node_t* elem = E[n]->head;
  while (elem != NULL) {
    int temp = findCycle(elem->val, depth + 1);
    if (temp > t) {
      t = temp;
    }
    if (temp > maxCycleLength) {
      maxCycleLength = temp;
      findedCycle = true;       // found a cycle
      maxCycle.head = nullptr;  // delete the previous maximum cycle
      startNode = elem->val;    // store the starting node of the cycle
    }

    elem = elem->next;
  }

  V_visitato[n] = 2;
  G_visitato[n] = 1;

  if (findedCycle) {
    list_insert_front(&maxCycle, n);  // insert the adjacent node in the cycle
  }
  if (startNode == n) {
    // if we are at the starting node of the cycle, we can stop
    findedCycle = false;
  }

  return t;
}

/**
 * @brief Find the longest cycle in the graph.
 * @return The length of the longest cycle found in the graph.
 */
void fullFindCycle() {
  int t = 0;  // cycle length

  maxCycleLength = 0;
  findedCycle = false;
  startNode = -1;
  maxCycle.head = nullptr;

  for (int i = 0; i < n_nodi; i++) {
    G_visitato[i] = 0;  // flag = not visited
  }

  for (int i = 0; i < n_nodi; i++) {
    if (G_visitato[i] == 1) {
      if (details)
        printf("nodo %d gia' visitato\n", i);
      continue;  // node already visited
    }

    for (int j = 0; j < n_nodi; j++) {
      V_visitato[j] = 0;  // flag = not visited
      V_distanza[j] = 0;  // flag = not visited
    }
    t = findCycle(i);
    graph_print();
    if (details)
      printf("test %d: ciclo = %d\n", i, t);
  }
}

int parse_cmd(int argc, char** argv) {
  /// parse command line arguments
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
  E = new list_t*[n_nodi];

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

  fullFindCycle();
  printf("Ciclo più lungo = %d\n", maxCycleLength);
  list_print(&maxCycle);

  if (graph) {
    /// preparo footer e chiudo file
    output_graph << "}" << endl;
    output_graph.close();
    cout << " File graph.dot scritto" << endl
         << "Creare il grafo con: dot graph.dot -Tpdf -o graph.pdf" << endl;
  }

  return 0;
}
