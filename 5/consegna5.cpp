/*
  * @file consegna5.cpp
  * @author Simone Bergonzi (@MurasakiSimema), Giulia Bettinelli, Amy Malacarne
  * @source GeeksForGeeks
  * @graph https://snap.stanford.edu/data/soc-sign-bitcoin-alpha.html
  * 
*/

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>

//              Constants
// Constants for the maximum depth of the tree to be printed
#define PRINT_LEVEL 5 


//              Structs and Classes 

/// @brief Struct to represent an edge in the graph
/// @param u The first vertex of the edge
/// @param v The second vertex of the edge
/// @param weight The weight of the edge
struct edge {
  int u, v, weight;
  edge(int _u, int _v, int w) : u(_u), v(_v), weight(w) {}
  edge() : u(-1), v(-1), weight(0) {}
};

struct node;

/// @brief Struct to represent a child of a node in the tree
/// @param value Pointer to the child node
/// @param next Pointer to the next child
/// @param is_leaf Boolean indicating if the child is a leaf
/// @param id The id of the child if it is a leaf
struct child {
  node* value;
  child* next;
  bool is_leaf;
  int id = -1;
};

/// @brief Struct to represent a node in the tree
/// @param u_id The id of the first vertex
/// @param v_id The id of the second vertex
/// @param childs Pointer to the first child of the node
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


//              Global variables for the tree

//Global variables to store the tree and its count
node* tree = nullptr;
int tree_count = 0;


//              Printing functions and variables

// Functions to print the tree in DOT format
int print_dfs = 0;
/// @brief This function prints the tree in DOT format to a file. It uses a depth-first search approach to traverse the tree and print each node and its children.
/// @param current current node
/// @param fout output file stream
/// @param stop boolean to stop the recursion when the level is too high
void printDfs(node* current, std::ofstream& fout, bool stop = false) {
  if (!current) return;

  std::stringstream label;
  label << "\"" << current->u_id << "-" << current->v_id << "\"";

  fout << "    " << label.str() << " [style=filled, fillcolor=red];\n";

  if (stop) {
    print_dfs--;
    return;
  }

  while (current->childs) {
    node* child = current->childs->value;
    std::stringstream child_label;
    if (!current->childs->is_leaf) {
      child_label << "\"" << child->u_id << "-" << child->v_id << "\"" << " [color=red]";
      print_dfs++;
    }
    else {
      child_label << "\"" << current->childs->id << "\"" << " [color=green]";
      fout << "    " << child_label.str() << " [style=filled, fillcolor=green, shape=box];\n";
    }
    fout << "    " << label.str() << " -> " << child_label.str() << ";\n";

    if (print_dfs > PRINT_LEVEL)
      printDfs(child, fout, true);
    else
      printDfs(child, fout);
    current->childs = current->childs->next;
  }

  print_dfs--;
}

/// @brief Function to print the tree in DOT format to a file
/// @param root Pointer to the root node of the tree
/// @param filename Name of the output file (default is "graph.dot")
void print_dot_tree_to_file(node* root, const std::string& filename = "graph.dot") {
  std::ofstream fout(filename);
  if (!fout) {
    std::cerr << "Errore: impossibile aprire il file " << filename << " per la scrittura.\n";
    return;
  }

  fout << "digraph G {\n";

  print_dfs = 0;
  printDfs(root, fout);
  fout << "}\n";
  fout.close();
}

//              Set functions

/// @brief Function to initialize the parent and rank arrays
/// @param parent Array to store the parent of each node
/// @param rank Array to store the rank of each node
/// @param n Number of nodes
void makeSets(int parent[], int rank[], int n)
{
  for (int i = 0; i < n; i++) {
    parent[i] = i;
    rank[i] = 0;
  }
}

/// @brief Function to find the parent of a node
/// @param parent Array to store the parent of each node
/// @param component The node to find the parent of
/// @return The parent of the node
int findParent(int parent[], int component)
{
  if (parent[component] == component)
    return component;

  return parent[component] = findParent(parent, parent[component]);
}

/// @brief Function to find the last node in the tree with the given id
/// @param id The id of the node to find
/// @return Pointer to the node if found, nullptr otherwise
node* findNode(int id)
{
  // Check if the node already exists
  for (int i = tree_count - 1; i >= 0; --i) {
    if (tree[i].u_id == id || tree[i].v_id == id) {
      return &tree[i];
    }
  }

  // If the node does not exist, return nullptr
  return nullptr;
}

/// @brief Function to unite two sets
/// @param u The first node
/// @param v The second node
/// @param parent Array to store the parent of each node
/// @param rank Array to store the rank of each node
/// @note This function also creates a new node in the tree and adds the two nodes as children of the new node.
void unionSet(int u, int v, int parent[], int rank[])
{
  // Finding the parents
  u = findParent(parent, u);
  v = findParent(parent, v);

  // Finding the last node in the tree with the given id
  node* node_u = findNode(u);
  node* node_v = findNode(v);

  if (node_u == nullptr && node_v == nullptr) {
    // If both nodes are not found, create new nodes for both u and v
    // and add them as children of the new node (u,v)

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
    // If node_u is not found, create a new node for u
    // and add it as a child of the new node (u,v) with node_v

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
    // If node_v is not found, create a new node for v
    // and add it as a child of the new node (u,v) with node_u

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
    // If both nodes are found, add both nodes as children of the new node (u,v)

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

//              Kruskal's algorithm

/// @brief Function to sort the edges using Shell Sort
/// @param arr Array of edges to be sorted
/// @param n Number of edges in the array
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

/// @brief Function to find the minimum spanning tree using Kruskal's algorithm
/// @param nNodes Number of nodes in the graph
/// @param edges Array of edges in the graph
/// @param nEdges Number of edges in the graph
/// @return A pair containing a pointer to the minimum spanning tree and the number of edges in the tree
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
    int u_root = findParent(parent, edges[i].u);
    int v_root = findParent(parent, edges[i].v);

    // If the parents are different that
    // means they are in different sets so
    // union them
    if (u_root != v_root) {
      unionSet(u_root, v_root, parent, rank);
      minTree[minTreeIndex] = edges[i];
      ++minTreeIndex;
    }
  }

  std::cout << "Minimum spanning tree created with " << minTreeIndex << " edges" << std::endl;

  return { minTree, minTreeIndex };
}

//              Utility functions

/// @brief Function to print the edges of the graph
/// @param edges Array of edges in the graph
/// @param nEdges Number of edges in the graph
void printEdges(edge* edges, int nEdges)
{
  for (int i = 0; i < nEdges; i++) {
    std::cout << edges[i].u << "-" << edges[i].v << " " << edges[i].weight << std::endl;
  }
}

int main() {
  std::ifstream input_data;
  input_data.open("soc-sign-bitcoinalpha.csv"); // Open the input file

  int n_nodes = 100; // Number of nodes in the graph
  int n_edges = 1985; // Number of edges in the graph
  edge* edges = new edge[n_edges];

  // Read the edges from the input file
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
  input_data.close(); // Close the input file

  //Test the input
  std::cout << "First edge: " << edges[0].u << "-" << edges[0].v << " " << edges[0].weight << std::endl;
  std::cout << "Last edge: " << edges[n_edges - 1].u << "-" << edges[n_edges - 1].v << " " << edges[n_edges - 1].weight << std::endl;

  // Initialize the tree
  tree = new node[n_nodes - 1]; 
  tree_count = 0;

  // Kruskal's algorithm
  std::pair<edge*, int> minTree = kruskalAlgo(n_nodes, edges, n_edges);

  // Print the edges of the minimum spanning tree
  print_dot_tree_to_file(&tree[tree_count - 1], "graph.dot");

  // Print the edges of the minimum spanning tree
  delete[] edges;
  delete[] minTree.first;
  delete[] tree;
}