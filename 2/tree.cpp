#include <iostream>
#include <fstream>

struct node {
  int value;
  node* left = nullptr;
  node* right = nullptr;
  node* father = nullptr;
  int depth = 0;
  int height = 0;
};

int n_operazione = 0;
std::ofstream output_graph;
node* global_ptr_ref = nullptr; /// usato per memorizzare il puntatore alla prima lista allocata

int get_address(void* node) {
  if (node == nullptr)
    return 0;
  return abs((int)((long)node - (long)global_ptr_ref));
}

/// stampa il codice del nodo per dot
void print_node_code(node* n) {
  output_graph << "node_" << get_address(n) << "_" << n_operazione;
}

void node_print_graph(node* n) {

  print_node_code(n);
  output_graph << "\n[label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\" >\n<TR> <TD CELLPADDING=\"3\" BORDER=\"0\"  ALIGN=\"LEFT\" bgcolor=\"#f0f0f0\" PORT=\"id\">";
  output_graph << get_address(n) << "</TD> </TR><TR>\n<TD PORT=\"val\" bgcolor=\"#a0FFa0\">";
  output_graph << n->value << "_" << n->depth << "_" << n->height << "</TD>\n <TD PORT=\"L\" ";
  if (n->left == nullptr)
    output_graph << "bgcolor=\"#808080\"> NULL";
  else
    output_graph << "> " << get_address(n->left);
  output_graph << "</TD>\n <TD PORT=\"R\" ";
  if (n->right == nullptr)
    output_graph << "bgcolor=\"#808080\"> NULL";
  else
    output_graph << "> " << get_address(n->right);
  output_graph << "</TD>\n</TR></TABLE>>];\n";

  /// visualizzazione figli sullo stesso piano
  if (n->left != nullptr && n->right != nullptr) {
    output_graph << "rank = same; ";
    print_node_code(n);
    output_graph << ";";
    print_node_code(n->left);
    output_graph << ";\n";
  }

  // mostro archi uscenti

  if (n->left != nullptr) { /// disegno arco left
    print_node_code(n);
    output_graph << ":L:c -> ";
    print_node_code(n->left);
    output_graph << ":id ;\n";
  }

  if (n->right != nullptr) { /// disegno arco R
    print_node_code(n);
    output_graph << ":R:c -> ";
    print_node_code(n->right);
    output_graph << ":id ;\n";
  }
}

void tree_print_rec_graph(node* n) {
  if (n != NULL) {
    node_print_graph(n);
    tree_print_rec_graph(n->left);
    tree_print_rec_graph(n->right);
  }
}

void tree_print_graph(node* n) {
  /// stampa ricorsiva del nodo
  tree_print_rec_graph(n);
  n_operazione++;
}

void node_print(node* n) {
  if (n == NULL)
    printf("Puntatore vuoto\n");
  else
    printf("allocato in %d [Val: %d, L: %d, R: %d]\n",
      get_address(n),
      n->value,
      get_address(n->right),
      get_address(n->left));
}

void flip(node* n) {
  if (n == nullptr)
    return;
  node* tmp = n->left;
  n->left = n->right;
  n->right = tmp;
  flip(n->left);
  flip(n->right);
}

void depth(node* n, int d) {
  if (n == nullptr)
    return;
  n->depth = d;
  depth(n->left, d + 1);
  depth(n->right, d + 1);
}

bool isBalanced(node* n) {
  if (n == nullptr)
    return true;

  bool left = isBalanced(n->left);
  bool right = isBalanced(n->right);

  if (!left || !right)
    return false;

  if (n->left == nullptr && n->right == nullptr)
    return true;

  if (n->left == nullptr && n->right != nullptr)
    return n->right->depth == 0;

  if (n->left != nullptr && n->right == nullptr)
    return n->left->depth == 1;

  if (n->left->depth - n->right->depth > 1)
    return false;

  return true;
}

bool isComplete(node* n) {
  if (n == nullptr)
    return true;
  if (n->left == nullptr && n->right == nullptr)
    return true;
  if (n->left == nullptr && n->right != nullptr)
    return false;
  if (n->left != nullptr && n->right == nullptr)
    return false;
  return isComplete(n->left) && isComplete(n->right);
}

node* lowestCommonAnchestor(node* root, node* p, node* q) {
  depth(root, 0);
  node* tmp1 = p;
  node* tmp2 = q;
  while (tmp1 != tmp2) {
    if (tmp1->depth > tmp2->depth)
      tmp1 = tmp1->father;
    else if (tmp1->depth < tmp2->depth)
      tmp2 = tmp2->father;
    else {
      tmp1 = tmp1->father;
      tmp2 = tmp2->father;
    }
  }

  return tmp1;
}

void initialize(node*& n) {
  n = new node;
  n->value = 1;
  n->left = new node;
  n->left->father = n;
  n->left->value = 2;
  n->left->left = new node;
  n->left->left->father = n->left;
  n->left->left->value = 3;
  n->left->left->left = nullptr;
  n->left->left->right = nullptr;
  n->left->right = new node;
  n->left->right->father = n->left;
  n->left->right->value = 4;
  n->left->right->left = nullptr;
  n->left->right->right = nullptr;
  n->right = new node;
  n->right->father = n;
  n->right->value = 5;
  n->right->left = new node;
  n->right->left->father = n->right;
  n->right->left->value = 6;
  n->right->left->left = nullptr;
  n->right->left->right = nullptr;
  n->right->right = new node;
  n->right->right->father = n->right;
  n->right->right->value = 7;
  n->right->right->left = nullptr;
  n->right->right->right = nullptr;
}

int main() {

  output_graph.open("graph.dot");
  /// preparo header
  output_graph << "digraph g" << std::endl;
  output_graph << "{ " << std::endl;
  output_graph << "node [shape=none]" << std::endl;
  output_graph << "rankdir=\"TB\"" << std::endl;
  ;
  output_graph << "edge[tailclip=false,arrowtail=dot];" << std::endl;

  node* root = nullptr;
  initialize(root);
  global_ptr_ref = root;

  depth(root, 0);
  tree_print_graph(root);

  node* lca = lowestCommonAnchestor(root, root->left->left, root->left->right);
  std::cout << "LCA (" << root->left->left->value << ", " << root->left->right->value << "): " << lca->value << std::endl;

  output_graph << "}" << std::endl;
  output_graph.close();

  flip(root);

  output_graph.open("graph_flip.dot");
  /// preparo header
  output_graph << "digraph g" << std::endl;
  output_graph << "{ " << std::endl;
  output_graph << "node [shape=none]" << std::endl;
  output_graph << "rankdir=\"TB\"" << std::endl;
  output_graph << "edge[tailclip=false,arrowtail=dot];" << std::endl;

  tree_print_graph(root);

  output_graph << "}" << std::endl;
  output_graph.close();

  std::cout << "isBalanced: " << isBalanced(root) << std::endl;
  std::cout << "isComplete: " << isComplete(root) << std::endl;
  node* left = root->left;
  root->left = nullptr;
  std::cout << "isBalanced: " << isBalanced(root) << std::endl;
  std::cout << "isComplete: " << isComplete(root) << std::endl;
  root->left = left;

  return 0;
}