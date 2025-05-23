/*
  * @file consegna5.cpp
  * @author Simone Bergonzi, Giulia Bettinelli, Amy Malacarne
  * @source GeeksForGeeks
  * @graph https://snap.stanford.edu/data/soc-sign-bitcoin-alpha.html
  *
*/

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>

struct edge {
  int u, v, weight;
  edge(int _u, int _v, int w) : u(_u), v(_v), weight(w) {}
  edge() : u(-1), v(-1), weight(0) {}
};

struct child;

struct node {
  int u_id;
  int v_id;
  child* childs = nullptr;
  node() : u_id(-1), v_id(-1) {}
  node(int _uid, int _vid) : u_id(_uid), v_id(_vid) {}
  ~node() {
    // Destructor to deallocate the childs
    child* current_child = childs;
    while (current_child != nullptr) {
      child* next_child = current_child->next;
      delete current_child;
      current_child = next_child;
    }
  }
};

struct child {
  node* value;
  child* next;
  bool is_leaf;
  int id = -1;
};


node* tree = nullptr;
int tree_count = 0;

void printDfs(node* current, std::ofstream& fout) {
  if (!current) return;

  std::stringstream label;
  label << "\"" << current->u_id << "-" << current->v_id << "\"";

  fout << "    " << label.str() << " [style=filled, fillcolor=red];\n";

  while(current->childs) {
    node* child = current->childs->value;
    std::stringstream child_label;
    if(!current->childs->is_leaf)
      child_label << "\"" << child->u_id << "-" << child->v_id << "\"" << " [color=red]";
    else{
      child_label << "\"" << current->childs->id << "\""<< " [color=green]";
      fout << "    " << child_label.str() << " [style=filled, fillcolor=green];\n";
    }
    fout << "    " << label.str() << " -> " << child_label.str() << ";\n";
    printDfs(child, fout);
    current->childs = current->childs->next;
  }
}

void print_dot_tree_to_file(node* root, const std::string& filename = "graph.dot") {
  std::ofstream fout(filename);
  if (!fout) {
    std::cerr << "Errore: impossibile aprire il file " << filename << " per la scrittura.\n";
    return;
  }

  fout << "digraph G {\n";

  printDfs(root, fout);
  fout << "}\n";
  fout.close();
}

// Initialization of parent[] and rank[] arrays
void makeSets(int parent[], int rank[], int n)
{
  for (int i = 1; i < n; i++) {
    parent[i] = i;
    rank[i] = 0;
  }
}

// Function to find the parent of a node
int findParent(int parent[], int component)
{
  if (parent[component] == component)
    return component;

  return parent[component] = findParent(parent, parent[component]);
}

node* findNode(int id)
{
  // Check if the node already exists
  for (int i = tree_count -1 ; i >= 0; --i) {
    if (tree[i].u_id == id || tree[i].v_id == id) {
      return &tree[i];
    }
  }

  // If the node does not exist, return nullptr
  return nullptr;
}

// Function to unite two sets
void unionSet(int u, int v, int parent[], int rank[])
{
  // Finding the parents
  u = findParent(parent, u);
  v = findParent(parent, v);

  node* node_u = findNode(u);
  node* node_v = findNode(v);

  if (node_u == nullptr && node_v == nullptr) {
    tree[tree_count] = node(u, v);
    child* new_child = new child;
    new_child->value = nullptr;
    new_child->next = nullptr;
    new_child->is_leaf = true;
    new_child->id = u;
    tree[tree_count].childs = new_child;

    new_child = new child;
    new_child->value = nullptr;
    new_child->next = nullptr;
    new_child->is_leaf = true;
    new_child->id = v;
    tree[tree_count].childs->next = new_child;

    tree_count++;
  }
  else if (node_u == nullptr) {
    tree[tree_count] = node(u, v);

    child* new_child = new child;
    new_child->value = node_v;
    new_child->next = tree[tree_count].childs;
    tree[tree_count].childs = new_child;

    new_child = new child;
    new_child->value = nullptr;
    new_child->next = nullptr;
    new_child->is_leaf = true;
    new_child->id = u;
    new_child->next = tree[tree_count].childs;
    tree[tree_count].childs = new_child;

    tree_count++;
  }
  else if (node_v == nullptr) {
    tree[tree_count] = node(u, v);

    child* new_child = new child;
    new_child->value = node_u;
    new_child->next = tree[tree_count].childs;
    tree[tree_count].childs = new_child;

    new_child = new child;
    new_child->value = nullptr;
    new_child->next = nullptr;
    new_child->is_leaf = true;
    new_child->id = v;
    new_child->next = tree[tree_count].childs;
    tree[tree_count].childs = new_child;
    
    tree_count++;
  }
  else {
    tree[tree_count] = node(u, v);

    child* new_child = new child;
    new_child->value = node_u;
    new_child->next = tree[tree_count].childs;
    tree[tree_count].childs = new_child;

    new_child = new child;
    new_child->value = node_v;
    new_child->next = tree[tree_count].childs;
    tree[tree_count].childs = new_child;

    tree_count++;
  }

  if (rank[u] < rank[v]) {
    parent[u] = v;
  }
  else if (rank[u] > rank[v]) {
    parent[v] = u;
  }
  else {
    parent[v] = u;

    // Since the rank increases if
    // the ranks of two sets are same
    rank[u]++;
  }
}

void shellSort(edge arr[], int n)
{
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i += 1) {
      edge temp = arr[i];

      int j;
      for (j = i; j >= gap && arr[j - gap].weight > temp.weight; j -= gap)
        arr[j] = arr[j - gap];

      arr[j] = temp;
    }
  }
}

// Function to find the MST
std::pair<edge*, int> kruskalAlgo(int nNodes, edge edges[], int nEdges)
{
  // First we sort the edge array in ascending order
  // so that we can access minimum distances/cost
  shellSort(edges, nEdges);
  std::cout << "Edges sorted" << std::endl;

  int parent[nNodes];
  int rank[nNodes];

  // Function to initialize parent[] and rank[]
  makeSets(parent, rank, nNodes);
  std::cout << "Sets created" << std::endl;

  // To store the minimun cost
  edge* minTree = new edge[nNodes - 1];
  int minTreeIndex = 0;
  for (int i = 0; i < nEdges && minTreeIndex < nNodes; i++) {
    int v1 = findParent(parent, edges[i].u);
    int v2 = findParent(parent, edges[i].v);

    // If the parents are different that
    // means they are in different sets so
    // union them
    if (v1 != v2) {
      unionSet(v1, v2, parent, rank);
      minTree[minTreeIndex] = edges[i];
      ++minTreeIndex;
    }
  }

  std::cout << "Minimum spanning tree created with " << minTreeIndex << " edges" << std::endl;

  return { minTree, minTreeIndex };
}

void printEdges(edge* edges, int nEdges)
{
  for (int i = 0; i < nEdges; i++) {
    std::cout << edges[i].u << "-" << edges[i].v << " " << edges[i].weight << std::endl;
  }
}

int main() {
  std::ifstream input_data;
  input_data.open("soc-sign-bitcoinalpha.csv");

  int n_nodes = 100;
  int n_edges = 1985;
  edge* edges = new edge[n_edges];

  for (int i = 0; i < n_edges; i++) {
    char comma;
    int u, v, w;
    input_data >> u;
    input_data >> comma;
    input_data >> v;
    input_data >> comma;
    input_data >> w;

    edges[i] = edge(u, v, w + 10);
  }

  std::cout << "First edge: " << edges[0].u << "-" << edges[0].v << " " << edges[0].weight << std::endl;
  std::cout << "Last edge: " << edges[n_edges - 1].u << "-" << edges[n_edges - 1].v << " " << edges[n_edges - 1].weight << std::endl;

  tree = new node[n_nodes - 1];
  std::pair<edge*, int> minTree = kruskalAlgo(n_nodes, edges, n_edges);

  print_dot_tree_to_file(&tree[tree_count - 1], "graph.dot");

  delete[] edges;
  delete[] tree;
}