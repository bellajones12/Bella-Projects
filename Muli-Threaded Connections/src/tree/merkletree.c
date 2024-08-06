#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "tree/merkletree.h"
#include "chk/pkgchk.h"
#include "crypt/sha256.h"


/**
 * @param root merkle tree root node
 * @param object bpkg_obj with data to be hashed
 * @return new merkle_tree struct
 */
struct merkle_tree* create_tree_struct(struct merkle_tree_node* root, struct bpkg_obj* object) {
    struct merkle_tree* tree = (struct merkle_tree*)malloc(sizeof(struct merkle_tree));
    tree->root = root;
    tree->n_nodes = number_of_nodes(root);
    tree->computed_chunks = number_of_completed_chunks(root);
    return tree;
}


/**
 * function to return total number of nodes in completed tree
 * @param node root of merkle tree
 * @return The total number of nodes in the merkle tree
 */
int number_of_nodes(struct merkle_tree_node* node) {
    if (node == NULL) {
        return 0;
    }

    // recursively searching left and right subtrees
    int left_count =  number_of_nodes(node->left);
    int right_count =  number_of_nodes(node->right);

    return 1 + left_count + right_count;

}

/**
 * function to return number of completed chunks in tree
 * @param node The root node of the merkle tree.
 * @return number of completed chunks (i.e. the number of chunks where the expected and computed hashes match)
 */
int number_of_completed_chunks(struct merkle_tree_node* node) {
    if (node == NULL) {
        return 0;
    }

    int count = 0;

    if (node->is_leaf == 1 && strcmp(node->computed_hash, node->expected_hash) == 0) {
        count++;
    }

    // Recursively count nodes in the left and right subtrees
    count += number_of_completed_chunks(node->left);
    count += number_of_completed_chunks(node->right);

    return count;
}


/**
 * function to traverse tree and return all hashes in an array
 * @param node root of merkle tree
 * @param hash_array array storing hashes
 * @param index current index of array
 */
void return_all_hashes(struct merkle_tree_node* node, char** hash_array, int* index) {
  if (node != NULL) {
    return_all_hashes(node->left, hash_array, index); 

    // adding hashes to array
    if (node->is_leaf == 1) {
        strcpy(hash_array[(*index)++], node->expected_hash);
    }    
    return_all_hashes(node->right, hash_array, index);
  }

}

/**
 * function to traverse tree and return all completed nodes
 * @param node, merkle tree node root
 * @param node_array array to store completed nodes
 * @param index index to keep track of where to allocate new array 
 */
void return_all_completed_hash_nodes(struct merkle_tree_node* node, struct merkle_tree_node** node_array, int* index) {
    if (node == NULL) {
        return;
    }
    
    // checking if node is completed to add to the array
    if (node->is_leaf == 1) {
        if (strcmp(node->computed_hash, node->expected_hash) == 0) {
            node_array[*index] = node;
            (*index)++;
        }
    }  

    return_all_completed_hash_nodes(node->left, node_array, index);
    return_all_completed_hash_nodes(node->right, node_array, index);
    return;
}


/**
 * function to traverse tree and return all completed hashes
 * @param node, merkle tree node root
 * @param hash array array to store completed hashes
 * @param index index to keep track of where to allocate new array 
 */
void return_all_completed_hashes(struct merkle_tree_node* node, char** hash_array, int* index) {
    if (node == NULL) {
        return;
    }
    return_all_completed_hashes(node->left, hash_array, index); 

    // checking if node is completed to add to the array
    if (node->is_leaf == 1) {
        if (strcmp(node->computed_hash, node->expected_hash) == 0) {
            strcpy(hash_array[(*index)], node->expected_hash);
            (*index)++;
        }
    }  
    return_all_completed_hashes(node->right, hash_array, index);
    return;
}


/**
 * function to traverse tree and return the min completed hashes
 * @param node, merkle tree node root
 * @param node_array array to store completed nodes
 * @param index index to keep track of where to allocate new array 
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
void return_min_completed_hashes(struct merkle_tree_node* node, char** hash_array, int* index) {
    if (node == NULL) {
        return;
    }
    return_min_completed_hashes(node->left, hash_array, index); 

    // if node has completed children, add hash to array
    if (node->completed_children == 1) {
        strcpy(hash_array[(*index)], node->expected_hash);
        (*index)++;
    }

    // if nodes children aren't completed, but this node is, add to array
    if (node->completed_children == 0 && strcmp(node->computed_hash, node->expected_hash) == 0 && node->parent->completed_children != 1) {
        strcpy(hash_array[(*index)], node->computed_hash);
        (*index)++;
    } 
    return_min_completed_hashes(node->right, hash_array, index);
    return;
}


/**
 * function to traverse tree and return number of completed hashes
 * @param node, merkle tree node root
 * @return int, returning the number of completed hashes
 */
int number_min_completed_hashes(struct merkle_tree_node* node) {
    if (node == NULL) {
        return 0;
    }

    int number_min_hashes = 0;

    // recursively search left subtree
    number_min_hashes += number_min_completed_hashes(node->left); 

    // if nodes are computed, increment counter
    if (node->completed_children == 1) {
        number_min_hashes++;
    }
    if (node->completed_children == 0 && strcmp(node->computed_hash, node->expected_hash) == 0 && node->parent->completed_children != 1) {
        number_min_hashes++;
    } 

    // recursivley search right subtree
    number_min_hashes += number_min_completed_hashes(node->right);
    return number_min_hashes;
}


/**
 * function to traverse tree and destroy all nodes
 * @param node, merkle tree node root
 */
void destroyNode(struct merkle_tree_node* node) {
    if (node == NULL) {
        return;
    }
    
    // recursively search left and right children
    destroyNode(node->left);
    destroyNode(node->right);

    // free node
    free(node);

    return;
}


/**
 * function to compute new hash from concatenation of childrens hashes
 * @param node, merkle tree node parent
 * @param hash variable to store combined hash
 */
void compute_parents_hash(char hash[HASH_MAX_SIZE*2], struct merkle_tree_node* parent) {
    struct sha256_compute_data cdata = { 0 };
    uint8_t hashout[SHA256_INT_SZ];
    char final_hash[65] = { 0 };

    size_t input_length = strlen(hash);

    sha256_update(&cdata, hash, input_length);
    sha256_finalize(&cdata, hashout);
    sha256_output_hex(&cdata, final_hash);

    strcpy(parent->computed_hash, final_hash);

    return;
}

/**
 * function to compute new hash from concatenation of childrens hashes
 * @param node, merkle tree node parent
 * @param left_hash left child's hash
 * @param right_hash right child's hash
 */
void combine_hashes(char left_hash[SHA256_HEXLEN], char right_hash[SHA256_HEXLEN], struct merkle_tree_node* node) {
    // allocating space for both hashes to be copied over plus null terminator
    char new_hash[SHA256_HEXLEN*2 + 1];
    snprintf(new_hash, sizeof(new_hash), "%s%s", left_hash, right_hash);

    // compute hash before assigning
    compute_parents_hash(new_hash, node);

    return;
}


/**
 * creating and returning merkle tree
 * @param object bpkg_obj with data to populate tree
 * @return returning tree struct
 */
struct merkle_tree* create_merkle_tree(struct bpkg_obj* object) {
    // Allocate memory for the Merkle tree structure
    struct merkle_tree* tree = (struct merkle_tree*)malloc(sizeof(struct merkle_tree));

    // Initialize tree attributes
    tree->root = NULL;
    tree->n_nodes = object->nchunks + object->nhashes;
    tree->computed_chunks = 0;

    return tree;
}


/**
 * function to create a stree object
 * @param objectbpkg_obj representing the data to put into tree
 * @return returns root node of tree created
 */
struct merkle_tree_node* creating_tree(struct bpkg_obj* object) {
    struct merkle_tree_node* root = NULL;
    struct merkle_tree_node* node = NULL;
    int number_nodes = object->nchunks + object->nhashes;

    // adding all non-leaf nodes
    for (int i = 0; i < object->nhashes; i++) {    
        node = insert(root, i, "non_leaf", 0, object->hashes[i], object->hashes[i], number_nodes);
        if (i == 0) {
            root = node;
        }
    }

    // adding all leaf nodes
    int i = object->nhashes;
    for (int j = 0; j < object->nchunks; j++) {
        compute_hash(object->filename, object->chunks[j]);
        node = insert(root, i, "leaf", 1, object->chunks[j]->hash, object->chunks[j]->computed_hash, number_nodes);
        i++;
    }

    return root;
}


/**
 * function to create new merkle_tree node
 * @param key Key associated with the node
 * @param value Value associated with the node
 * @param is_leaf Flag indicating if the node is a leaf (1) or internal node (0)
 * @param expected_hash Expected hash value for the node
 * @param computed_hash Computed hash value for the node
 * @return new merkle_tree node
 */
struct merkle_tree_node* create_node(int key, void*value, int is_leaf, char expected_hash[SHA256_HEXLEN], char computed_hash[SHA256_HEXLEN]) {
    struct merkle_tree_node* node = (struct merkle_tree_node*)malloc(sizeof(struct merkle_tree_node));

    node->key = key;
    node->value = value;
    node->is_leaf = is_leaf;

    strncpy(node->expected_hash, expected_hash, SHA256_HEXLEN);
    strncpy(node->computed_hash, computed_hash, SHA256_HEXLEN);

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    node->completed_children = 0;

    return node;

}


/**
 * function to insert a node into the merkle tree in level order
 * @param node node to insert
 * @param key Key associated with the node
 * @param value Value associated with the node
 * @param is_leaf Flag indicating if the node is a leaf (1) or internal node (0)
 * @param expected_hash Expected hash value for the node
 * @param computed_hash Computed hash value for the node
 * @param number_nodes number of nodes in tree
 * @return new node inserted
 */
struct merkle_tree_node* insert(struct merkle_tree_node* node, int key, void*value, int is_leaf, char expected_hash[SHA256_HEXLEN], char computed_hash[SHA256_HEXLEN], int number_nodes) {
    // if tree is empty, create a new node and return it as the root
    if (node == NULL) {
        node = create_node(key, value, is_leaf, expected_hash, computed_hash);;
        return node;
    }

    // creating queue to perform level-order traversal
    struct merkle_tree_node** queue = (struct merkle_tree_node**)malloc(sizeof(struct merkle_tree_node*) * number_nodes);
    
    // initialising indices to keep track of elements in queue
    int queue_front = 0;
    int queue_rear = -1;
    
    // queueing root nood
    queue_rear++;
    queue[queue_rear] = node;

    // level order traversal until empty child is found
    while (queue_front <= queue_rear) {
        // dequeue current node
        struct merkle_tree_node* current = queue[queue_front];
        queue_front++;

        if (current->left == NULL) {
            // if no left child, create a new node
            current->left = create_node(key, value, is_leaf, expected_hash, computed_hash);
            current->left->parent = current;
            free(queue);
            return node;
        } else {
            // enquueue left child for further processing
            queue_rear++;
            queue[queue_rear] = current->left;
        }

        if (current->right == NULL) {
            // if no right child, create a new node
            current->right = create_node(key, value, is_leaf, expected_hash, computed_hash);
            current->right->parent = current;
            free(queue);
            return node;
        } else {
            // enqueue right child for further processing
            queue_rear++;
            queue[queue_rear] = current->right;
        }
    }

    free(queue);
    return node;
}


/**
 * Calculating hash of non-leaf nodes
 * @param node, merkle tree node to compute hash for
 * @return computed hash
 */
char* compute_nodes_hashes(struct merkle_tree_node* node) {
    if (node == NULL) {
        return NULL;
    }

    // have reached leaf node --> computed hash is already defined
    if (node->is_leaf == 1) {
        return node->computed_hash;
    }

    // recursively calcualte values of children
    char* left_hash = compute_nodes_hashes(node->left);
    char* right_hash = compute_nodes_hashes(node->right);

    // calculate node's computed hash
    combine_hashes(left_hash, right_hash, node);

    // if children have completed hashes, assigned completed children attriute to node
    if (strcmp(node->left->computed_hash, node->left->expected_hash) == 0 && strcmp(node->right->computed_hash, node->right->expected_hash) == 0) {
        node->completed_children = 1;
    }

    return node->computed_hash;
}



/**
 * function to compute hashes of all non-leaf nodes
 * @param root root of merkle tree
 */
void printLevelOrder(struct merkle_tree_node* root) {
    int rear = -1;
    int front = 0;
    struct merkle_tree_node* temp_node = root;
    struct merkle_tree_node* queue[MAX_Q_SIZE];

    // returning if tree is empty
    if (!temp_node) {
        return;
    }

    // Enqueue root node
    queue[++rear] = temp_node;


    while (front <= rear) {
        temp_node = queue[front++];

        if (temp_node->left) {
            queue[++rear] = temp_node->left;
        }

        if (temp_node->right) {
            queue[++rear] = temp_node->right;
        }
    }
    front = 0;

    // accessing elements in reverse (as need to move from bottom to top in order to combine and calculate non-leaf node hashes)
    for (int i = rear; i >= front; i--) {
        if (i-1 < 0) {
            break;
        }
        compute_new_hash(queue[i], queue[i-1]);
        i--;
    }
}



/**
 * function to compute hash of parent node based on children
 * @param node1 keft chid node
 * @param node2 right child node
 */
void compute_new_hash(struct merkle_tree_node* node1, struct merkle_tree_node* node2) {

    if (node1->is_leaf == 1 && node2->is_leaf == 1) {
        if (strcmp(node1->computed_hash, node1->expected_hash) == 0 && strcmp(node2->computed_hash, node2->expected_hash) == 0) {
            // if both children are complete, can simply make parent complete
            node1->completed = 1;
            node2->completed = 1;
            node1->parent->completed_children = 1;
            node1->parent->completed = 1;
            node1->completed_children = 2;
            node2->completed_children = 2;

        }
    } 
    else {
        if (node1->completed == 1 && node2->completed == 1) {
            node1->parent->completed_children = 1;
            node1->parent->completed = 1;
            node1->completed_children = 2;
            node2->completed_children = 2;
        }
    }

    // calling function to combine hashes and get new hash
    combine_hashes(node1->computed_hash, node2->computed_hash, node1->parent);
}



/**
 * Function to return number of leaf nodes in tree
 * @param node root of merkle tree
 * @return total number of leaf nodes in merkle tree
 */
int iterate_leaf_nodes(struct merkle_tree_node* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->is_leaf == 1) {
        return 1;
    } else {
        // recursively search left and right subtrees
        int left_leaves = iterate_leaf_nodes(node->left);
        int right_leaves = iterate_leaf_nodes(node->right);
        return left_leaves + right_leaves;
    }
}
