namespace Tree {
  struct node {
    int value;
    node* left = nullptr;
    node* right = nullptr;
    node* father = nullptr;
    int depth = 0;
    int height = 0;
  };

  /**
    * @brief Flips the left and right children of a binary tree recursively.
    * @param n Pointer to the root node of the tree to be flipped.
   */
  void flip(node* n) {
    if (n == nullptr)
      return;
    node* tmp = n->left;
    n->left = n->right;
    n->right = tmp;
    flip(n->left);
    flip(n->right);
  }

  /**
    * @brief Calculates the height of a binary tree recursively.
    * @param n Pointer to the root node of the tree.
    * @return The height of the tree.
   */
  void depth(node* n, int d) {
    if (n == nullptr)
      return;
    n->depth = d;
    depth(n->left, d + 1);
    depth(n->right, d + 1);
  }

  /**
    * @brief Calculates if a binary tree is balanced.
    * @param n Pointer to the root node of the tree.
    * @return True if the tree is balanced, false otherwise.
   */
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

  /**
    * @brief Checks if a binary tree is complete.
    * @param n Pointer to the root node of the tree.
    * @return True if the tree is complete, false otherwise.
   */
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

  /**
   * @brief Calculates the depth of each node in the tree and assigns the father pointer.
   * @param n Pointer to the root node of the tree.
   * @param d The current depth in the tree.
   */
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

  /**
   * @brief Finds the lowest common ancestor of two nodes in a binary tree.
   * @param root Pointer to the root node of the tree.
   * @param p Pointer to the first node.
   * @param q Pointer to the second node.
   * @return Pointer to the lowest common ancestor node if found, nullptr otherwise.
   */
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