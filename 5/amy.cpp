/*
 * @file consegna5.cpp
 * @author Simone Bergonzi, Giulia Bettinelli, Amy Malacarne
 * @source GeeksForGeeks
 * @graph https://snap.stanford.edu/data/soc-sign-bitcoin-alpha.html
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>

#define N_NODES 100

struct node;
struct child;

struct edge {
    int u, v, weight;
    edge(int _u, int _v, int w) : u(_u), v(_v), weight(w) {}
    edge() : u(-1), v(-1), weight(0) {}
};

struct child {
    node* value;      
    child* next;      
    bool is_leaf;     
    int id;           
};

struct node {
    int u_id; 
    int v_id; 
    child* childs; 

    node() : u_id(-1), v_id(-1), childs(nullptr) {}
    node(int _uid, int _vid) : u_id(_uid), v_id(_vid), childs(nullptr) {}

    ~node() {
        child* current_child = childs;
        while (current_child != nullptr) {
            child *next_child = current_child->next;
            delete current_child;
            current_child = next_child;
        }
    }
};

node* tree = nullptr;
int tree_count = 0;     
int max_tree_nodes = 0; 

// Global array to store the most recent 'node' in the visualization tree
// that corresponds to a given component root ID.
node* component_head[N_NODES];

void deallocate_tree() {
    if (tree != nullptr) {
        delete[] tree;
        tree = nullptr;
        tree_count = 0;
        max_tree_nodes = 0;
    }
}

// Counts the number of lines (edges) in a file
// int count_lines_in_file(const std::string& filename) {
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Error: Could not open file to count lines: " << filename << std::endl;
//         return 0;
//     }
//     int count = 0;
//     std::string line;
//     while (std::getline(file, line)) {
//         count++;
//     }
//     file.close();
//     return count;
// }


void dfs(node *current, std::ofstream& fout){
    if (!current) {
        std::cerr << "DFS Error: current node is nullptr. Skipping.\n";
        return;
    }

    std::stringstream label;
    label << "\"" << current->u_id << "_" << current->v_id << "\"";

    child* temp_child_ptr = current->childs;
    while (temp_child_ptr != nullptr) {
        std::stringstream child_label;
        if (!temp_child_ptr->is_leaf) {
            
            if (!temp_child_ptr->value) {
                std::cerr << "DFS Critical Error: Non-leaf child has nullptr value! Parent: "
                          << label.str() << ", Child ID: " << temp_child_ptr->id << ". Skipping this child.\n";
                temp_child_ptr = temp_child_ptr->next;
                continue;
            }
            child_label << "\"" << temp_child_ptr->value->u_id << "_" << temp_child_ptr->value->v_id << "\"";
        } else {
            child_label << "\"" << temp_child_ptr->id << "\"";
            fout << "    " << child_label.str() << " [shape=box];\n";
        }
        fout << "    " << label.str() << " -> " << child_label.str() << ";\n";
        if (!temp_child_ptr->is_leaf) {
            dfs(temp_child_ptr->value, fout);
        }
        temp_child_ptr = temp_child_ptr->next;
    }
}

void print_dot_tree_to_file(node *root, const std::string& filename){
    std::ofstream fout(filename);
    if(!fout){
        std::cerr << "Error: Unable to open file " << filename << " for writing.\n";
        return;
    }

    fout << "digraph G {\n";
    fout << "    rankdir=TB; // Top-to-Bottom layout\n";

    if(root != nullptr){
        dfs(root, fout);
    } else {
        std::cerr << "Warning: Root for DOT printing is null. No graph will be generated.\n";
    }
    fout << "}\n";
    fout.close();
}


void makeSets(int parent[], int rank[], int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;        
        rank[i] = 0;          
        component_head[i] = nullptr; 
    }
}

int findParent(int parent[], int component) {
    if (parent[component] == component)
        return component;
    return parent[component] = findParent(parent, parent[component]); 
}


void unionSet(int u_root, int v_root, int parent[], int rank[]) {
    u_root = findParent(parent, u_root);
    v_root = findParent(parent, v_root);

    if (u_root == v_root)
        return;

    if (tree_count >= max_tree_nodes) {
        std::cerr << "Error: 'tree' array full. Cannot add more union nodes.\n";
        return;
    }


    tree[tree_count] = node(u_root, v_root);
    node* current_union_node = &tree[tree_count];
    tree_count++;

    // Add u_root's component as a child
    child* new_child_u = new child;
    new_child_u->next = nullptr;
    new_child_u->id = u_root; 

    if (component_head[u_root] != nullptr) {
        new_child_u->value = component_head[u_root];
        new_child_u->is_leaf = false;
    } else {
        new_child_u->value = nullptr; 
        new_child_u->is_leaf = true;
    }
    // Add to the front of the childs linked list
    new_child_u->next = current_union_node->childs;
    current_union_node->childs = new_child_u;

    // Add v_root's component as a child 
    child* new_child_v = new child;
    new_child_v->next = nullptr;
    new_child_v->id = v_root;

    if (component_head[v_root] != nullptr) {
        new_child_v->value = component_head[v_root];
        new_child_v->is_leaf = false;
    } else {
        new_child_v->value = nullptr;
        new_child_v->is_leaf = true;
    }
    new_child_v->next = current_union_node->childs; 
    current_union_node->childs = new_child_v;

    int new_parent_of_merged_set;
    if (rank[u_root] < rank[v_root]) {
        parent[u_root] = v_root;
        new_parent_of_merged_set = v_root;
    } else if (rank[u_root] > rank[v_root]) {
        parent[v_root] = u_root;
        new_parent_of_merged_set = u_root;
    } else {
        parent[v_root] = u_root;
        rank[u_root]++;
        new_parent_of_merged_set = u_root;
    }
    component_head[new_parent_of_merged_set] = current_union_node;
}


void shellSort(edge arr[], int n) {
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

std::pair<edge*, int> kruskalAlgo(edge edges[], int nEdges) {
    // First we sort the edge array in ascending order
    // so that we can access minimum distances/cost
    shellSort(edges, nEdges);
    std::cout << "Edges sorted." << std::endl;

    int parent[N_NODES];
    int rank[N_NODES];

    // Initialize DSU sets
    makeSets(parent, rank, N_NODES); 
    std::cout << "DSU sets initialized." << std::endl;

    // To store the edges of the MST
    edge* minTree = new edge[N_NODES - 1]; 
    int minTreeIndex = 0;

    // Allocate the 'tree' array for visualization nodes
    // In the worst case, we make N_NODES - 1 union operations.
    max_tree_nodes = N_NODES - 1;
    tree = new node[max_tree_nodes];

    for (int i = 0; i < nEdges && minTreeIndex < N_NODES - 1; i++) {
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

    std::cout << "Minimum spanning tree created with " << minTreeIndex << " edges." << std::endl;

    return {minTree, minTreeIndex};
}

int main() {
    const std::string input_filename = "soc-sign-bitcoinalpha.csv"; 
    int n_edges = 1985;

    // Count the number of edges in the file dynamically
    //int n_edges = count_lines_in_file(input_filename);
    // if (n_edges == 0) {
    //     std::cerr << "Error: No edges found in " << input_filename << ". Exiting.\n";
    //     return 1;
    // }
    // std::cout << "Found " << n_edges << " edges in " << input_filename << std::endl;

    std::ifstream input_data;
    input_data.open(input_filename);

    if (!input_data.is_open()) {
        std::cerr << "Error: Unable to open file " << input_filename << "\n";
        return 1;
    }

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
    input_data.close();

    std::cout << "First edge read: " << edges[0].u << "-" << edges[0].v << " " << edges[0].weight << std::endl;
    std::cout << "Last edge read: " << edges[n_edges - 1].u << "-" << edges[n_edges - 1].v << " " << edges[n_edges - 1].weight << std::endl;

    std::pair<edge*, int> minTree = kruskalAlgo(edges, n_edges);

    if (tree_count > 0) {
        print_dot_tree_to_file(&tree[tree_count - 1], "graph.dot");
        std::cout << "DOT graph generated in graph.dot\n";
    } else {
        std::cout << "No union nodes created for DOT printing (possibly disconnected graph or no edges).\n";
    }

    // --- Memory Deallocation ---
    delete[] edges;
    delete[] minTree.first; // Deallocate the MST edges array
    deallocate_tree();       // Deallocate the 'tree' array and all its children

    return 0;
}