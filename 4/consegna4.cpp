#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fstream>
#include <iostream>

using namespace std;

// compilazione: g++ consegna4.cpp

#define INFTY 1000000

int details = 0;
int graph = 0;

// Output file for graph visualization
ofstream output_graph;
int n_operazione = 0;  // operation counter to visualize various steps

int ct_visit = 0;  // counter during visit

//////////////////////////////////
// Data structure definition for list
//////////////////////////////////

// struct for the list node
typedef struct node {
  int val;  // next node
  float w;  // edge weight
  struct node* next;
} node_t;

// struct for the list
typedef struct list {
  node* head;
} list_t;

//////////////////////////////////
// End Data structure definition for list
//////////////////////////////////

//////////////////////////////////
// Data structure definition for graph
//////////////////////////////////

int* V;           // list of graph nodes
int* V_visitato;  // visited node
int* V_prev;      // previous node from visit
float* V_dist;    // distance from source

list_t** E;  // array of pointers to adjacency lists for each node
int n_nodi;

//////////////////////////////////
// End Data structure definition for graph
//////////////////////////////////

list_t* global_ptr_ref = NULL;  // used to store the pointer to the first allocated list

int get_address(void* node) {
  return (int)((long)node - (long)global_ptr_ref);
}

void node_print(int n) {
  // calculate maximum distance (except infinity)
  float max_d = 0;
  for (int i = 0; i < n_nodi; i++)
    if (max_d < V_dist[i] && V_dist[i] < INFTY)
      max_d = V_dist[i];

  output_graph << "node_" << n << "_" << n_operazione << endl;
  output_graph << "[ shape = oval; ";

  if (V_visitato[n] == 1)
    output_graph << "penwidth = 4; ";

  float col = V_dist[n] / max_d;  // distance in scale 0..1
  output_graph << "fillcolor = \"0.0 0.0 " << col / 2 + 0.5 << "\"; style=filled; ";
  if (V_dist[n] < INFTY)
    output_graph << "label = "
                 << "\"Idx: " << n << ", dist: " << V_dist[n] << "\" ];\n";
  else
    output_graph << "label = "
                 << "\"Idx: " << n << ", dist: INF\" ];\n";

  node_t* elem = E[n]->head;
  while (elem != NULL) {  // draw edge
    output_graph << "node_" << n << "_" << n_operazione << " -> ";
    output_graph << "node_" << elem->val << "_" << n_operazione << " [ label=\"" << elem->w << "\", len=" << elem->w / 100 * 10 << " ]\n";
    elem = elem->next;
  }

  if (V_prev[n] != -1) {  // if there is a visited previous node -> draw edge
    float len = 0;
    len = 1;
    output_graph << "node_" << V_prev[n] << "_" << n_operazione << " -> ";
    output_graph << "node_" << n << "_" << n_operazione << " [ color=blue, penwidth=5, len=" << len / 100 * 10 << " ]\n";
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

    while (current != NULL) {
      if (!details)
        printf("%d w:%f, ", current->val, current->w);
      else {  // detailed print
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

  l->head = NULL;
  if (details) {
    printf("Imposto a NULL head\n");
  }

  return l;
}

void list_insert_front(list_t* l, int elem, float w) {
  // inserisce un elemento all'inizio della lista
  node_t* new_node = new node_t;
  new_node->next = NULL;

  new_node->val = elem;
  new_node->w = w;

  new_node->next = l->head;

  l->head = new_node;
}
// HEAP //
const int MAX_SIZE = 256;  // static allocation
float heap[MAX_SIZE];
int heapIndex[MAX_SIZE];  // array of indices for the node
int heap_size = 0;        // current size of the heap

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
  return (child_L_idx(n) == -1);  // no need to check the R child
}

/**
 * @brief Inserts an element into the heap at the correct position to maintain the min-heap property.
 * @param elem The element to insert.
 * @param idx The index of the node in the graph.
 */
void heap_insert(float elem, int idx) {
  // insert the new node with content elem
  // in the last position of the array
  // that is, continue to complete the current level

  if (details)
    printf("Inserisco elemento %f in posizione %d\n", elem, heap_size);

  if (heap_size < MAX_SIZE) {
    int i = heap_size;
    heap_size++;

    heap[i] = elem;
    heapIndex[i] = idx;

    while (i != 0) {
      if (heap[parent_idx(i)] <= heap[i]) {  // properties of the heap respected -> exit
        if (details)
          printf("Il genitore ha valore %f <= del nodo %f, esco\n", heap[parent_idx(i)], heap[i]);
        return;
      }

      if (details)
        printf("Il genitore ha valore %f > del nodo %f, swap\n", heap[parent_idx(i)], heap[i]);

      float t = heap[parent_idx(i)];
      int tIdx = heapIndex[parent_idx(i)];
      heap[parent_idx(i)] = heap[i];
      heapIndex[parent_idx(i)] = heapIndex[i];
      heap[i] = t;
      heapIndex[i] = tIdx;

      i = parent_idx(i);
    }
  } else
    printf("Heap pieno!\n");
}

/**
 * @brief Removes the minimum element from the heap and returns it.
 * @return A pair containing the minimum element and its index in the graph.
 */

pair<float, int> heap_remove_min() {
  if (heap_size <= 0) {  // heap is empty
    printf("Errore: heap vuoto\n");
    return {NULL, -1};
  }

  float min = heap[0];
  int idx_min = heapIndex[0];

  if (details)
    printf("Minimo identificato %f\n", min);
  // swap the root with the last element
  // the root is now the last element -- ready for removal
  float t = heap[0];
  int tIdx = heapIndex[0];
  heap[0] = heap[heap_size - 1];
  heapIndex[0] = heapIndex[heap_size - 1];
  heap[heap_size - 1] = t;
  heapIndex[heap_size - 1] = tIdx;

  // remove the last element
  heap_size--;

  int i = 0;  // start from the root

  while (!is_leaf(i)) {  // ensure to stop at the leaf
    if (details)
      printf("Lavoro con il nodo in posizione i = %d, valore %f\n", i, heap[i]);

    int swapIdx = -1;
    // check if the node is greater than its left child
    if (heap[i] > heap[child_L_idx(i)]) {  // the node is greater than the left child
      // swap, the properties of the heap are violated
      swapIdx = child_L_idx(i);
      if (details)
        printf("Figlio L e' piu' piccolo (valore %f)\n", heap[child_L_idx(i)]);

      if (child_R_idx(i) >= 0 &&  // there is a right child
          heap[child_L_idx(i)] > heap[child_R_idx(i)]) {
        swapIdx = child_R_idx(i);
        if (details)
          printf("Figlio R e' ancora piu' piccolo (valore %f)\n", heap[child_R_idx(i)]);
      }
    } else {                                   // the node is smaller than the left child
      if (child_R_idx(i) >= 0) {               // there is a right child
        if (heap[i] > heap[child_R_idx(i)]) {  // the node is greater than the right child
          // swap, the properties of the heap are violated
          swapIdx = child_R_idx(i);
          if (details)
            printf("Figlio R e' piu' piccolo del nodo (valore %f)\n", heap[child_R_idx(i)]);
        } else
          break;
      } else
        break;
    }

    // swap the node with the child that has the minimum value
    float t = heap[i];
    int tIdx = heapIndex[i];
    heap[i] = heap[swapIdx];
    heapIndex[i] = heapIndex[swapIdx];
    heap[swapIdx] = t;
    heapIndex[swapIdx] = tIdx;

    i = swapIdx;
  }

  return {min, idx_min};  // return the minimum value and its index
}

// END HEAP //

/**
 * @brief Implements Dijkstra's algorithm to find the shortest path in a graph.
 * @param n The index of the source node.
 */
void shortest_path(int n) {
  V_dist[n] = 0;
  heap_insert(V_dist[n], n);  // inserisco il nodo sorgente in coda

  graph_print();

  while (heap_size > 0) {
    auto best = heap_remove_min();  // extract the minimum element from the heap
    int best_idx = best.second;
    if (details)
      printf("(1) Estraggo il nodo %d con distanza %f\n", best_idx, best.first);

    // control if the node has already been visited
    int u = best_idx;
    if (V_visitato[u] == 1)
      continue;

    V_visitato[u] = 1;
    // mark the node as visited

    // control the adjacency list of the node
    node_t* elem = E[u]->head;
    while (elem != NULL) {
      int v = elem->val;  // edge u --> v

      // alt ← dist[u] + Graph.Edges(u, v)
      float alt = V_dist[u] + elem->w;  // compute the alternative distance
      if (alt < V_dist[v]) {            // if the alternative distance is less than the current distance
        V_dist[v] = alt;
        V_prev[v] = u;
      }

      heap_insert(V_dist[v], v);  // insert the node v in the heap
      if (details)
        printf("(2) Inserisco in coda il nodo %d con peso %f\n", v, V_dist[v]);

      elem = elem->next;
    }
    graph_print();
  }

  graph_print();
}

/**
 * @brief Implements the Bellman-Ford algorithm to find the shortest path in a graph.
 * @param n The index of the source node.
 * @param ADJ The adjacency list of the graph.
 * @param local_n_nodi The number of nodes in the graph.
 * @return True if there are no negative weight cycles, false otherwise.
 */
bool bellman_ford(int n, list_t** ADJ = E, int local_n_nodi = n_nodi) {
  V_prev[n] = n;
  V_dist[n] = 0;
  for (int i = 1; i <= local_n_nodi - 1; i++) {
    for (int u = 0; u < local_n_nodi; u++) {
      node_t* adj = ADJ[u]->head;
      while (adj != nullptr) {
        int v = adj->val;
        if (V_dist[u] != INFTY && V_dist[u] + adj->w < V_dist[v]) {  // if the distance from the source to the current node + the weight of the edge is less than the distance from the source to the node
          V_dist[v] = V_dist[u] + adj->w;                            // update the distance from the source to the node
          V_prev[v] = u;                                             // update the previous node
        }
        adj = adj->next;
      }
    }
  }

  for (int i = 0; i < local_n_nodi; i++) {  // control for negative weight cycles
    node_t* adj = ADJ[i]->head;
    while (adj != nullptr) {
      int v = adj->val;
      if (V_dist[i] != INFTY && V_dist[i] + adj->w < V_dist[v]) {  // if the distance from the source to the current node + the weight of the edge is less than the distance from the source to the node
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
  // parse command line arguments
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
    // prepare header for graphviz
    output_graph << "digraph g" << endl;
    output_graph << "{ " << endl;
    output_graph << "node [shape=none]" << endl;
    output_graph << "rankdir=\"LR\"" << endl;
    //    output_graph << "edge[tailclip=false,arrowtail=dot];"<<endl;
  }

  int N = 10;
  n_nodi = N * N;

  n_nodi = 10;

  // init nodes
  V = new int[n_nodi];
  V_visitato = new int[n_nodi];
  V_prev = new int[n_nodi];
  V_dist = new float[n_nodi];

  // init edges
  E = new list_t*[n_nodi];
  // graph initialization
  for (int i = 0; i < n_nodi; i++) {
    V[i] = 2 * i;
    V_visitato[i] = 0;  // flag = not visited
    V_prev[i] = -1;     // no predecessor
    V_dist[i] = INFTY;  // infinity

    E[i] = list_new();

    if (i == 0)
      global_ptr_ref = E[i];

    int x = i % N;
    int y = i / N;
  }

  int start = 0;
  int end = n_nodi - 1;
  int w_max = 100;

  for (int i = 0; i < n_nodi - 1; i++) {
    // arco costoso
    list_insert_front(E[i], end, w_max - 2 * i);
    // arco 1
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

  shortest_path(start);
  printf("Distanza minima dal nodo %d al nodo %d e' %f\n", start, end, V_dist[end]);
  graph_print();

  if (bellman_ford(0)) {
    printf("Bellman-Ford: distanza minima dal nodo %d al nodo %d e' %f\n", start, end, V_dist[end]);
    printf("Bellman-Ford: percorso minimo dal nodo %d al nodo %d e' ", start, end);
    int i = end;
    while (i != start) {
      printf("%d ", i);
      i = V_prev[i];
    }
    printf("%d \n", start);
  } else
    printf("Bellman-Ford: esiste un ciclo con peso negativo\n");

  // generate graph with negative cycle
  list_t** E2 = new list_t*[3];
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
  } else
    printf("Bellman-Ford: esiste un ciclo con peso negativo\n");

  if (graph) {
    // footer for graphviz
    output_graph << "}" << endl;
    output_graph.close();
    cout << " File graph.dot scritto" << endl
         << "****** Creare il grafo con: neato graph.dot -Tpdf -o graph.pdf" << endl;
  }

  return 0;
}
