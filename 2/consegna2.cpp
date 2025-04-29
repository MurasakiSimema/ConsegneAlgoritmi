namespace Tree {
  struct node {
    int value;
    node* left = nullptr;
    node* right = nullptr;
    node* father = nullptr;
    int depth = 0;
    int height = 0;
  };

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

  bool isBalanced(node* n, int d = 0) {
    if (n == nullptr)
      return true;
    n->depth = d;
    bool left = isBalanced(n->left, d + 1);
    bool right = isBalanced(n->right, d + 1);

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

  void depthWithFather(node* n, int d) {
    if (n == nullptr)
      return;
    n->depth = d;

    if (n->left != nullptr) {
      n->left->father = n;
      depthWithFather(n->left, d + 1);
    }
    if (n->right != nullptr) {
      n->right->father = n;
      depthWithFather(n->right, d + 1);
    }
  }

  node* lowestCommonAnchestor(node* root, node* p, node* q) {
    depthWithFather(root, 0);
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
}  // namespace Tree