#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node* splay(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key) {
        if (root->left == NULL) return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL) ? root : rightRotate(root);
    } else {
        if (root->right == NULL) return root;

        // Zig-Zig (Right Right)
        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }
        // Zig-Zag (Right Left)
        else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

Node* insert(Node* root, int key) {
    if (root == NULL) return newNode(key);

    root = splay(root, key);

    if (root->key == key) return root;

    Node* new_node = newNode(key);
    if (key < root->key) {
        new_node->right = root;
        new_node->left = root->left;
        root->left = NULL;
    } else {
        new_node->left = root;
        new_node->right = root->right;
        root->right = NULL;
    }

    return new_node;
}

Node* deleteKey(Node* root, int key) {
    if (root == NULL) return NULL;

    root = splay(root, key);

    if (root->key != key) return root;

    if (root->left == NULL) {
        Node* temp = root->right;
        free(root);
        return temp;
    }

    if (root->right == NULL) {
        Node* temp = root->left;
        free(root);
        return temp;
    }

    Node* temp = root;

    root = splay(root->left, key);

    root->right = temp->right;

    free(temp);
    return root;
}

void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// Function to print the tree structure
void printTree(Node* root, int space) {
    if (root == NULL) return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    printTree(root->right, space);

    // Print current node after space
    printf("\n");
    for (int i = 10; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->key);

    // Process left child
    printTree(root->left, space);
}

int main() {
    Node* root = NULL;
    int keys[] = {10, 20, 30, 40, 50, 25};

    for (int i = 0; i < 6; i++)
        root = insert(root, keys[i]);

    printf("Inorder traversal after insertion:\n");
    inorder(root);
    printf("\n");

    printf("Tree structure after insertion:\n");
    printTree(root, 0);

    root = deleteKey(root, 40);
    printf("\nInorder traversal after deleting 40:\n");
    inorder(root);
    printf("\n");

    return 0;
}

//https://github.com/anuraganand789/CLiveCoding/tree/master/datastructures/AVLTree
