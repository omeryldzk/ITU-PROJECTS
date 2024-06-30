// EB
// Implementing Red-Black Tree in C++

#include <iostream>

namespace RBT {
  struct Node {
    int data;
    std::string name;
    Node *parent;
    Node *left;
    Node *right;
    int color;
  };
}

class RedBlackTree {
private:
  RBT::Node* root;


  // Helper function for recursive traversal
  void inorderHelper(RBT::Node* node) {
    if (node != nullptr) {
      inorderHelper(node->left);
      std::cout << node->data << " " << node->name << std::endl;
      inorderHelper(node->right);
    }
  }
  void inorderPopulate(RBT::Node* root, std::pair<std::string, int> orderedData[], int& index) {
        if (root != nullptr) {
            inorderPopulate(root->left, orderedData, index);
            orderedData[index].first = root->name;
            orderedData[index].second = root->data;
            index++;
            inorderPopulate(root->right, orderedData, index);
        }
    }

  void preorderHelper(RBT::Node* node) {
    if (node != nullptr) {
      std::cout << node->data << " " << node->name << std::endl;
      preorderHelper(node->left);
      preorderHelper(node->right);
    }
  }
  void postorderHelper(RBT::Node* node) {
    if (node != nullptr) {
      postorderHelper(node->left);
      postorderHelper(node->right);
      std::cout << node->data << " " << node->name << std::endl;
    }
  }

  // Helper function to find the node with the minimum key
  RBT::Node* findMin(RBT::Node* node) {
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }
  // Helper function to find the node with the minimum key
  RBT::Node* findMax(RBT::Node* node) {
    while (node->right != nullptr) {
      node = node->right;
    }
    return node;
  }

  int getHeightHelper(RBT::Node* node) {
        if (node == nullptr) {
        return -1;
        }
        int leftHeight = getHeightHelper(node->left);
        int rightHeight = getHeightHelper(node->right);
        return 1 + std::max(leftHeight, rightHeight);
    }
    int getTotalNodesHelper(RBT::Node* node) {
        if (node == nullptr) {
        return 0;
        }
        return 1 + getTotalNodesHelper(node->left) + getTotalNodesHelper(node->right);
    }
    // Helper function to perform left rotation
    void leftRotate(RBT::Node* x) {
        // Implement left rotation
        RBT::Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Helper function to perform right rotation
    void rightRotate(RBT::Node* y) {
        // Implement right rotation
        RBT::Node* x = y->left;
        y->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }
    // Helper function to replace one subtree with another
    void transplant(RBT::Node* u, RBT::Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

        // Helper function to fix violations after deletion
    void deleteFixup(RBT::Node* x) {
        // Implement fixup after deletion
        while (x != root && (x != nullptr && x->color == 0)) {
            if (x == x->parent->left) {
                RBT::Node* w = x->parent->right;
                if (w != nullptr && w->color == 1) {
                    w->color = 0; // Case 1
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == 0) &&
                    (w->right == nullptr || w->right->color == 0)) {
                    w->color = 1; // Case 2
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == 0) {
                        if (w->left != nullptr) {
                            w->left->color = 0; // Case 3
                        }
                        w->color = 1;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color; // Case 4
                    x->parent->color = 0;
                    if (w->right != nullptr) {
                        w->right->color = 0;
                    }
                    leftRotate(x->parent);
                    x = root; // Exit loop
                }
            } else {
                RBT::Node* w = x->parent->left;
                if (w != nullptr && w->color == 1) {
                    w->color = 0; // Case 1
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == 0) &&
                    (w->left == nullptr || w->left->color == 0)) {
                    w->color = 1; // Case 2
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == 0) {
                        if (w->right != nullptr) {
                            w->right->color = 0; // Case 3
                        }
                        w->color = 1;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color; // Case 4
                    x->parent->color = 0;
                    if (w->left != nullptr) {
                        w->left->color = 0;
                    }
                    rightRotate(x->parent);
                    x = root; // Exit loop
                }
            }
            x = x->parent;
        }
        if (x != nullptr) {
            x->color = 0; // Ensure the root is black
        }
    }


  // Helper function to fix violations after insertion
  void insertFixup(RBT::Node* z) {
      // Implement fixup after insertion
      while (z != nullptr && z->parent != nullptr && z->parent->color == 1) {
          if (z->parent == z->parent->parent->left) {
              RBT::Node* y = z->parent->parent->right;
              if (y != nullptr && y->color == 1) {
                  z->parent->color = 0; // Flip colors
                  y->color = 0;
                  z->parent->parent->color = 1;
                  z = z->parent->parent;
              } else {
                  if (z == z->parent->right) {
                      z = z->parent;
                      leftRotate(z);
                  }
                  z->parent->color = 0; // Flip colors
                  z->parent->parent->color = 1;
                  rightRotate(z->parent->parent);
              }
          } else {
              RBT::Node* y = z->parent->parent->left;
              if (y != nullptr && y->color == 1) {
                  z->parent->color = 0; // Flip colors
                  y->color = 0;
                  z->parent->parent->color = 1;
                  z = z->parent->parent;
              } else {
                  if (z == z->parent->left) {
                      z = z->parent;
                      rightRotate(z);
                  }
                  z->parent->color = 0; // Flip colors
                  z->parent->parent->color = 1;
                  leftRotate(z->parent->parent);
              }
          }
      }
      root->color = 0; // Ensure the root is black
  }

public:
  RedBlackTree() {
  }
    void inorder(std::pair<std::string, int> orderedData[], int index) {
        inorderPopulate(root,orderedData,index);
    }
    void inorder() {
        inorderHelper(root);
    }
    void preorder() {
        preorderHelper(root);
    }

    void postorder() {
        postorderHelper(root);
    }


    RBT::Node* searchTree(int key) {
        RBT::Node* node = root;

        while (node != nullptr && node->data != key) {
            if (key < node->data) {
                node = node->left;
            } else {
                node = node->right;
            }
        }

        return node;
    }

    // Find the successor of a given node
  RBT::Node* successor(RBT::Node* node) {
      if (node->right != nullptr) {
          return findMin(node->right);
      }
      RBT::Node* y = node->parent;
      while (y != nullptr && node == y->right) {
          node = y;
          y = y->parent;
      }
      return y;
  }

  // Find the predecessor of a given node
  RBT::Node* predecessor(RBT::Node* node) {
      if (node->left != nullptr) {
          return findMax(node->left);
      }
      RBT::Node* y = node->parent;
      while (y != nullptr && node == y->left) {
          node = y;
          y = y->parent;
      }
      return y;
  }


  // Insert a node with a given key and name
    void insert(const std::string& name,int key) {
        RBT::Node* z = new RBT::Node();
        z->data = key;
        z->name = name;
        z->left = nullptr;
        z->right = nullptr;
        z->color = 1; // New node is always red

        RBT::Node* y = nullptr;
        RBT::Node* x = root;
        // Finding the Correct Position
        while (x != nullptr) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        // Setting the Parent and Inserting the Node
        z->parent = y;
        if (y == nullptr) {
            root = z; // Tree was empty
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }

        insertFixup(z);
    }

    // Function to delete a node with the given key from the Red-Black Tree
void deleteNode(int key) {
    // Search for the node with the given key in the Red-Black Tree
    RBT::Node* z = searchTree(key);

    // Check if the node with the given key is found
    if (z == nullptr) {
        std::cout << "Node with key " << key << " not found for deletion." << std::endl;
        return;
    }

    // Initialize variables for replacement node, original color, and successor node
    RBT::Node* y = z;
    int yOriginalColor = y->color;
    RBT::Node* x;

    // Case 1: If the node to be deleted has at most one child
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right); // Replace z with its right child
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left); // Replace z with its left child
    } else {
        // Case 2: If the node to be deleted has two children
        y = findMin(z->right); // Find the successor (minimum node in the right subtree)
        yOriginalColor = y->color;
        x = y->right;

        // Update the pointers to replace y with its right child
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        // Replace z with its successor y
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    // If the original color of y was black, fix the Red-Black Tree properties
    if (yOriginalColor == 0) {
        deleteFixup(x);
    }

    // Deallocate memory for the deleted node
    delete z;
}

   int getHeight() {
        return getHeightHelper(root);
    }

    RBT::Node* getMaximum() {
        return findMax(root);
    }

    RBT::Node* getMinimum() {
        return findMin(root);
    }

    int getTotalNodes() {
        return getTotalNodesHelper(root);
    }

};