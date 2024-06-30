#include <iostream>
#include <string>

namespace BST {
  struct Node {
    int data;
    std::string name;
    Node *parent;
    Node *left;
    Node *right;
  };
}

class BinarySearchTree {
private:
  BST::Node* root;
  void inorderPopulate(BST::Node* root, std::pair<std::string, int> orderedData[], int& index) {
        if (root != nullptr) {
            inorderPopulate(root->left, orderedData, index);
            orderedData[index].first = root->name;
            orderedData[index].second = root->data;
            index++;
            inorderPopulate(root->right, orderedData, index);
        }
    }
  // Helper function for recursive traversal
  void inorderHelper(BST::Node* node) {
    if (node != nullptr) {
      inorderHelper(node->left);
      std::cout << node->data << " " << node->name << std::endl;
      inorderHelper(node->right);
    }
  }
  void preorderHelper(BST::Node* node) {
    if (node != nullptr) {
      std::cout << node->data << " " << node->name << std::endl;
      preorderHelper(node->left);
      preorderHelper(node->right);
    }
  }
  void postorderHelper(BST::Node* node) {
    if (node != nullptr) {
      postorderHelper(node->left);
      postorderHelper(node->right);
      std::cout << node->data << " " << node->name << std::endl;
    }
  }

  // Helper function to find the node with the minimum key
  BST::Node* findMin(BST::Node* node) {
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }
  // Helper function to find the node with the minimum key
  BST::Node* findMax(BST::Node* node) {
    while (node->right != nullptr) {
      node = node->right;
    }
    return node;
  }

  // Helper function for recursive deletion
  BST::Node* deleteNodeHelper(BST::Node* root, int key) {
    if (root == nullptr) {
      return root;
    }

    if (key < root->data) {
      root->left = deleteNodeHelper(root->left, key);
    } else if (key > root->data) {
      root->right = deleteNodeHelper(root->right, key);
    } else {
      // Node with only one child or no child
      if (root->left == nullptr) {
        BST::Node* temp = root->right;
        delete root;
        return temp;
      } else if (root->right == nullptr) {
        BST::Node* temp = root->left;
        delete root;
        return temp;
      }

      // Node with two children: Get the inorder successor (smallest
      // in the right subtree)
      BST::Node* temp = findMin(root->right);

      // Copy the inorder successor's content to this node
      root->data = temp->data;
      root->name = temp->name;

      // Delete the inorder successor
      root->right = deleteNodeHelper(root->right, temp->data);
    }
    return root;
  }
   int getHeightHelper(BST::Node* node) {
        if (node == nullptr) {
        return -1;
        }
        int leftHeight = getHeightHelper(node->left);
        int rightHeight = getHeightHelper(node->right);
        return 1 + std::max(leftHeight, rightHeight);
    }
    BST::Node* insertRecursive(BST::Node* node, int key, const std::string& name, BST::Node* parent = nullptr) {
        if (node == nullptr) {
            BST::Node* newNode = new BST::Node;
            newNode->data = key;
            newNode->name = name;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->parent = parent;
            return newNode;
        }

        if (key < node->data) {
            node->left = insertRecursive(node->left, key, name, node);
        } else {
            node->right = insertRecursive(node->right, key, name, node);
        }

        return node;
    }
    int getTotalNodesHelper(BST::Node* node) {
        if (node == nullptr) {
        return 0;
        }
        return 1 + getTotalNodesHelper(node->left) + getTotalNodesHelper(node->right);
    }

public:
    BinarySearchTree() : root(nullptr) {}
     void inorder(std::pair<std::string, int> orderedData[], int index) {
        inorderPopulate(root,orderedData,index);
    }
    // Public wrapper for the inorder traversal
    void inorder() {
        inorderHelper(root);
    }

    void preorder() {
        preorderHelper(root);
    }

    void postorder() {
        postorderHelper(root);
    }

           // Search for a node with a given key
    BST::Node* searchTree(int key) {
      BST::Node* node = root;

      while (node != nullptr && node->data != key) {
          if (key < node->data) {
              node = node->left;
          } else {
              node = node->right;
          }
      }

      return node;
  }

    BST::Node* successor(BST::Node* node) {
        if (node->right != nullptr) {
        return findMin(node->right);
        }

        BST::Node* successor = nullptr;
        BST::Node* current = root;

        while (current != nullptr) {
        if (node->data < current->data) {
            successor = current;
            current = current->left;
        } else if (node->data > current->data) {
            current = current->right;
        } else {
            break;
        }
        }

        return successor;
    }

    BST::Node* predecessor(BST::Node* node) {
        // Implementation for finding the predecessor of a given node
        // Return the predecessor node if exists, otherwise return nullptr
        if (node->left != nullptr) {
            return findMax(node->left);
        }

        BST::Node* predecessor = nullptr;
        BST::Node* current = root;

        while (current != nullptr) {
            if (node->data < current->data) {
                current = current->left;
            } else if (node->data > current->data) {
                predecessor = current;
                current = current->right;
            } else {
                break;
            }
        }

        return predecessor;
    }
    void insert(const std::string& name,int key ) {
        root = insertRecursive(root, key, name);
}


    void deleteNode(int key) {
        root = deleteNodeHelper(root, key);
  }

   

    int getHeight() {
        return getHeightHelper(root);
    }

    BST::Node* getMaximum() {
        return findMax(root);
    }

    BST::Node* getMinimum() {
        return findMin(root);
    }

    int getTotalNodes() {
        return getTotalNodesHelper(root);
    }
    };
