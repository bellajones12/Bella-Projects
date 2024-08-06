#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <stddef.h>
#include <chk/pkgchk.h>
#include <crypt/sha256.h>

#define SHA256_HEXLEN (65)

#define MAX_Q_SIZE (4000)


struct merkle_tree_node {
    int key;
    void* value;
    struct merkle_tree_node* left;
    struct merkle_tree_node* right;
    struct merkle_tree_node* parent;
    int is_leaf;
    char expected_hash[SHA256_HEXLEN];
    char computed_hash[SHA256_HEXLEN];
    int completed_children;
    int completed;
};


struct merkle_tree {
    struct merkle_tree_node* root;
    size_t n_nodes;
    size_t computed_chunks;
};


struct merkle_tree* create_tree_struct(struct merkle_tree_node* root, struct bpkg_obj* object);

struct merkle_tree_node* insert(struct merkle_tree_node* node, int key, void*value, int is_leaf, char expected_hash[SHA256_HEXLEN], char computed_hash[SHA256_HEXLEN], int number_nodes);

struct merkle_tree_node* create_node(int key, void*value, int is_leaf, char expected_hash[SHA256_HEXLEN], char computed_hash[SHA256_HEXLEN]);

struct merkle_tree_node* creating_tree(struct bpkg_obj* object);

void combine_hashes(char left_hash[SHA256_HEXLEN], char right_hash[SHA256_HEXLEN], struct merkle_tree_node* node);

void destroyNode(struct merkle_tree_node* node);

int number_of_nodes(struct merkle_tree_node* node);

void return_all_hashes(struct merkle_tree_node* node, char** hash_array, int* index);

void return_all_completed_hash_nodes(struct merkle_tree_node* node, struct merkle_tree_node** node_array, int* index);

void return_all_completed_hashes(struct merkle_tree_node* node, char** hash_array, int* index);

int number_of_completed_chunks(struct merkle_tree_node* node);

void return_min_completed_hashes(struct merkle_tree_node* node, char** hash_array, int* index);

char* compute_nodes_hashes(struct merkle_tree_node* node);

int number_min_completed_hashes(struct merkle_tree_node* node);

void printLevelOrder(struct merkle_tree_node* root);

void compute_new_hash(struct merkle_tree_node* node1, struct merkle_tree_node* node2);

int iterate_leaf_nodes(struct merkle_tree_node* node);

#endif