#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "chk/pkgchk.h"
#include "tree/merkletree.h"
#include "crypt/sha256.h"


/**
 * Loads the package for when a valid path is given
 * @param path, path to the bpkg file
 * @return a pointer to a newly allocaged bpkg_obj struct, or NULL on error
 */
struct bpkg_obj* bpkg_load(const char* path) {
    struct bpkg_obj* obj = NULL;
    int fd;
    struct stat stat_b;
    char* mapped = NULL;
    if (path != NULL) {

        // open file
        fd = open(path, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return obj;
        }

        // get file size
        fstat(fd, &stat_b);
        if (fstat(fd, &stat_b) == -1) {
            perror("fstat");
            return obj;
        }

        // memory map the file
        mapped = mmap(NULL, stat_b.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mapped == MAP_FAILED) {
            perror("mmap");
            close(fd);
            return obj;
        }

        char *ptr = mapped;

        char ident[IDENT_MAX_SIZE];
        char filename[FILENAME_MAX_SIZE];



        // assigning ident
        char *ident_start = strstr(ptr, "ident:");
        if (ident_start != NULL) {
            ident_start += strlen("ident:");
            char *newline = strchr(ident_start, '\n');
            if (newline != NULL) {
                size_t ident_length = newline - ident_start; 
                strncpy(ident, ident_start, ident_length);
                ident[ident_length] = '\0';
            }
        }
        obj = (struct bpkg_obj*)malloc(sizeof(struct bpkg_obj));
        strcpy(obj->ident, ident);

        // assigning filename
        char *filename_start = strstr(ptr, "filename:");
        if (filename_start != NULL) {
            filename_start += strlen("filename:");
            char *newline = strchr(filename_start, '\n');
            if (newline != NULL) {
                size_t filename_length = newline - filename_start; 
                strncpy(filename, filename_start, filename_length);
                filename[filename_length] = '\0';
            }
        }
        strcpy(obj->filename, filename);

        // assigning size
        char *size_start = strstr(ptr, "size:");
        if (size_start != NULL) {
            size_start += strlen("size:");

            long size_long = strtol(size_start, &ptr, 10);
            obj->size = (size_t)size_long;
        }

        // assigning nhashes
        char *nhashes_start = strstr(ptr, "nhashes:");
        if (nhashes_start != NULL) {
            nhashes_start += strlen("nhashes:");

            long nhashes_long = strtol(nhashes_start, &ptr, 10);
            obj->nhashes = (size_t)nhashes_long;
        } else {
            perror("no nahshes attribute");
            return NULL;
        }


        // assigning hashes
        char *hashes_start = strstr(ptr, "hashes:");
        char** hashes = NULL;

        if (hashes_start != NULL) {
            hashes_start += strlen("hashes:\n");
            hashes = malloc(obj->nhashes * sizeof(char *));
            obj->hashes = malloc(obj->nhashes * sizeof(char *));

            int num_hashes = 0;

            while (num_hashes < obj->nhashes) {
                char *newline = strchr(hashes_start, '\n');
                size_t line_length = newline - hashes_start;
                hashes[num_hashes] = malloc((line_length+1) * sizeof(char));
                
                strncpy(hashes[num_hashes], hashes_start, line_length);
                hashes[num_hashes][line_length] = '\0';

                obj->hashes[num_hashes] = malloc((line_length+1) * sizeof(char));

                char hash_value[HASH_MAX_SIZE];

                if (sscanf(hashes[num_hashes], "  %[^,]", hash_value) == 1) {
                    strncpy(obj->hashes[num_hashes], hash_value, HASH_MAX_SIZE);
                    obj->hashes[num_hashes][line_length] = '\0';
                }

                num_hashes++;
                hashes_start = newline+1;
            }

            for (int i=0; i < num_hashes; i++) {
                free(hashes[i]);
            }

            free(hashes);
        }

        // assigning nchunks
        char *nchunks_start = strstr(ptr, "nchunks:");
        if (nchunks_start != NULL) {
            nchunks_start += strlen("nchunks:");

            long nchunks_long = strtol(nchunks_start, &ptr, 10);
            obj->nchunks = (size_t)nchunks_long;

        }

        // assigning chunks
        char *chunks_start = strstr(ptr, "chunks:");
        char *chunks_array = NULL;

        if (chunks_start != NULL) {
            chunks_start += strlen("chunks:\n");
            obj->chunks = malloc((obj->nchunks) * sizeof(char *));

            int num_chunks = 0;
            while (num_chunks < obj->nchunks) {
                char *newline = strchr(chunks_start, '\n');
                size_t line_length = newline - chunks_start;

                chunks_array = malloc((line_length+1) * sizeof(char));

                strncpy(chunks_array, chunks_start, line_length);
                chunks_array[line_length] = '\0';
                
                obj->chunks[num_chunks] = malloc(sizeof(struct chunk));

                char hash[HASH_MAX_SIZE];
                uint32_t offset;
                uint32_t chunk_size;

                    if (sscanf(chunks_array, "  %[^,],%u,%u", hash, &offset, &chunk_size) == 3) {
                        strncpy((obj->chunks)[num_chunks]->hash, hash, HASH_MAX_SIZE);
                        ((obj->chunks)[num_chunks])->offset = offset;
                        ((obj->chunks)[num_chunks])->size = chunk_size;
                    }

                free(chunks_array);
                num_chunks++;
                chunks_start = newline+1;
            }

        }
    }
        close(fd);

    return obj;
}

/**
 * Checks to see if the referenced filename in the bpkg file
 * exists or not.
 * @param bpkg, constructed bpkg object
 * @return query_result, a single string should be
 *      printable in hashes with len sized to 1.
 * 		If the file exists, hashes[0] should contain "File Exists"
 *		If the file does not exist, hashes[0] should contain "File Created"
 */

struct bpkg_query bpkg_file_check(struct bpkg_obj* bpkg) {
    struct bpkg_query query = { 0 };
    struct stat file_exists;
    query.len = 1;
    query.hashes = (char **)malloc((query.len) * sizeof(char*));

    // checking if file exists
    if (stat(bpkg->filename, &file_exists) == 0) {
        query.hashes[0] = strdup("File Exists");
    } 
    else {
        // if doesn't, create new file
        FILE *file;
        file = fopen(bpkg->filename, "w");
        fclose(file);
        query.hashes[0] = strdup("File Created");
    }
    return query;
}

/**
 * Retrieves a list of all hashes within the package/tree
 * @param bpkg, constructed bpkg object
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
struct bpkg_query bpkg_get_all_hashes(struct bpkg_obj* bpkg) {
    struct bpkg_query qry = { 0 };
    struct merkle_tree_node* root = creating_tree(bpkg);
    struct merkle_tree* tree = create_tree_struct(root, bpkg);

    // length is number of hashes
    qry.len = tree->n_nodes;
    qry.hashes = malloc((qry.len) * sizeof(char*));

    // allocating space for hashes + chunks
    for (int i = 0; i < bpkg->nhashes; i++) {    
        qry.hashes[i] = strdup(bpkg->hashes[i]);
    }

    int i = bpkg->nhashes;
    for (int j=0; j < bpkg->nchunks; j++) {
        qry.hashes[i] = strdup(bpkg->chunks[j]->hash);
        i++;
    }

    // freeing resources
    destroyNode(tree->root);
    free(tree);
    return qry;
}

/**
 * Retrieves all completed chunks of a package object
 * @param bpkg, constructed bpkg object
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
struct bpkg_query bpkg_get_completed_chunks(struct bpkg_obj* bpkg) { 
    struct bpkg_query qry = { 0 };
    struct merkle_tree_node* root = creating_tree(bpkg);
    struct merkle_tree* tree = create_tree_struct(root, bpkg);

    // computing hash for chunks
    for (int i = 0; i < bpkg->nchunks; i++) {
        compute_hash(bpkg->filename, bpkg->chunks[i]);
    }

    // completing hashes for non-leaf nodes
    compute_nodes_hashes(root);

    qry.len = tree->computed_chunks;
    qry.hashes = malloc((qry.len) * sizeof(char*));

    char** hash_array = malloc((qry.len) * sizeof(char*));
    for (int i = 0; i < qry.len; i++) {
        hash_array[i] = malloc((SHA256_HEXLEN+1) * sizeof(char*));
    }

    // returning array of all completed hashes
    int index = 0;
    return_all_completed_hashes(root, hash_array, &index);

    for (int i=0; i < qry.len; i++) {
        qry.hashes[i] = strdup(hash_array[i]);
    }

    for (int i=0; i < qry.len; i++) {
        free(hash_array[i]);
    }

    free(hash_array);

    destroyNode(root);
    free(tree);

    return qry;
}


/**
 * Gets the minimum number of hashes required to verify the completeness of a bpkg object.
 *
 * @param bpkg The bpkg object to analyze.
 * @return A bpkg_query struct containing the minimum completed hashes.
 */
struct bpkg_query bpkg_get_min_completed_hashes(struct bpkg_obj* bpkg) {
    struct bpkg_query qry = { 0 };
    
    // computing all chunks
    for (int i = 0; i < bpkg->nchunks; i++) {
        compute_hash(bpkg->filename, bpkg->chunks[i]);
    }

    // creating tree object
    struct merkle_tree_node* root = creating_tree(bpkg);
    struct merkle_tree* tree = create_tree_struct(root, bpkg);
    
    // compute internal nodes hashes
    printLevelOrder(root);

    // compute corresponding hashes
    qry.len = (size_t)number_min_completed_hashes(root);

    int index = 0;
    char** hash_array = malloc(qry.len * sizeof(char*));

    qry.hashes = malloc((qry.len) * sizeof(char*));

    for (int i = 0; i < qry.len; i++) {
        hash_array[i] = malloc((SHA256_HEXLEN+1) * sizeof(char*));
    }  

    return_min_completed_hashes(root, hash_array, &index);

    for (int i = 0; i < qry.len; i++) {
        qry.hashes[i] = strdup(hash_array[i]);
    }

    for (int i=0; i < qry.len; i++) {
        free(hash_array[i]);
    }

    free(hash_array);

    destroyNode(root);
    free(tree);

    return qry;
}


/**
 * Computes hash of chunk
 * @param filename, .data file to get data from
 * @param chunk, chunk structto store computed hashes in
 * @return int, return status of success
 */
int compute_hash(char filename[FILENAME_MAX_SIZE], struct chunk* chunk) {
    uint32_t size = chunk->size;
    uint32_t offset = chunk->offset;
    FILE* file = NULL;
    struct sha256_compute_data cdata = { 0 };
    char buf[4096];
    size_t nbytes = 0;
    uint8_t hashout[SHA256_INT_SZ];
    char final_hash[65] = { 0 };

    file = fopen(filename, "r");
    if (file == NULL) {
        // perror("opening file error");
        return-1;
    }

    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("seek error");
        fclose(file);
        return -1;
    }

    sha256_compute_data_init(&cdata);

    size_t bytes_left = size;
    size_t bytes_to_read;

    // reading .data file
    while (bytes_left > 0 && (bytes_to_read = fread(buf, 1, 4096, file)) > 0) {
        if (bytes_to_read > bytes_left) {
            bytes_to_read = bytes_left;
        }
        sha256_update(&cdata, buf, bytes_to_read);
        bytes_left -= bytes_to_read;
    }

    // computing hash
    sha256_update(&cdata, buf, nbytes);
    sha256_finalize(&cdata, hashout);
    sha256_output_hex(&cdata, final_hash);


    strcpy(chunk->computed_hash, final_hash);
    fclose(file);
    return 1;
}





/**
 * Retrieves all chunk hashes given a certain an ancestor hash (or itself)
 * Example: If the root hash was given, all chunk hashes will be outputted
 * 	If the root's left child hash was given, all chunks corresponding to
 * 	the first half of the file will be outputted
 * 	If the root's right child hash was given, all chunks corresponding to
 * 	the second half of the file will be outputted
 * @param bpkg, constructed bpkg object
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
struct bpkg_query bpkg_get_all_chunk_hashes_from_hash(struct bpkg_obj* bpkg, 
    char* hash) {
    
    struct bpkg_query qry = { 0 };

    struct merkle_tree_node* root = creating_tree(bpkg);
    if (strcmp(root->expected_hash, hash) == 0) {
        struct merkle_tree* tree = create_tree_struct(root, bpkg);

        qry.len = (tree->n_nodes + 1) / 2;
        qry.hashes = malloc((qry.len) * sizeof(char*));

        // add all leaf nodes to qry.hashes if node is hash
        for (int i=0; i < bpkg->nchunks; i++) {
            qry.hashes[i] = strdup(bpkg->chunks[i]->hash);
        }
 
        destroyNode(root);
        free(tree);
        return qry;
    }
    
    struct merkle_tree_node* node = find_hash(root, hash);

    qry.len = iterate_leaf_nodes(node);
    qry.hashes = malloc((qry.len) * sizeof(char*));

    char** hash_array = malloc(qry.len * sizeof(char*));

    for (int i = 0; i < qry.len; i++) {
        hash_array[i] = malloc((SHA256_HEXLEN+1) * sizeof(char*)); 
    }  
    int index = 0;

    return_all_hashes(node, hash_array, &index);

    for (int i = 0; i < qry.len; i++) {
        qry.hashes[i] = strdup(hash_array[i]);
    }

    for (int i=0; i < qry.len; i++) {
        free(hash_array[i]);
    }

    free(hash_array);

    destroyNode(root);
    return qry;
}


/**
 * traverse tree until node with specified hash has been found
 *
 * @param node The current node in the search.
 * @param hash The hash to search for.
 * @return A pointer to the node with the matching hash, or NULL if not found.
 */
struct merkle_tree_node* find_hash(struct merkle_tree_node* node, char* hash) {
  if (node == NULL) {
    return NULL;
  }

  // Check current node
  if (strcmp(node->expected_hash, hash) == 0) {
    return node;
  }

  // Check left subtree
  struct merkle_tree_node* left_result = find_hash(node->left, hash);
  if (left_result != NULL) {
    return left_result; // Found in left subtree
  }

  // Check right subtree (only if left subtree didn't find it)
  return find_hash(node->right, hash);
}


/**
 * Finds all leaf nodes in a subtree and adds their computed hashes to the query.
 *
 * @param node The current node in the subtree.
 * @param qry The query to add the leaf node hashes to.
 * @param size The current number of hashes added to the query.
 * @param allocated_size The allocated size of the query's hash array.
 * @return The updated number of hashes added to the query.
 */
int find_leaf_nodes(struct merkle_tree_node* node, struct bpkg_query* qry, int size, int allocated_size) {
    if (node == NULL) {
        return size;
    }
    if (node->is_leaf == 1) {
        // resizing array for each leaf node
        allocated_size++;
        char** tmp = realloc(qry->hashes, (allocated_size) * sizeof(char*));
        qry->hashes = tmp;
        // add leaf node's computed hash to query
        qry->hashes[size] = node->computed_hash;
        size ++;

    } 
    else {
        // search left subtree
        if (node->left != NULL) {
            find_leaf_nodes(node->left, qry, size, allocated_size);
        } 
        // search right subtree
        if (node->right != NULL) {
            find_leaf_nodes(node->right, qry, size, allocated_size);
        }
    }
    return size;
}

/**
 * Deallocates the query result after it has been constructed from
 * the relevant queries above.
 * @param query to destroy
 */
void bpkg_query_destroy(struct bpkg_query* qry) {
    for (int i = 0; i < qry->len; i++) {
        free(qry->hashes[i]);
    }
    free(qry->hashes);
    qry->len = 0;
}

/**
 * Deallocates memory at the end of the program
 * @param object to destroy
 */
void bpkg_obj_destroy(struct bpkg_obj* obj) {
    for (int i = 0; i < (obj->nhashes); i++) {
        free(obj->hashes[i]);
    }
    free(obj->hashes);

    for (int i = 0; i < (obj->nchunks); i++) {
        free(obj->chunks[i]);
    }
    free(obj->chunks);

    free (obj);


}

/**
 * Returns a completed hash if so.
 *
 * @param bpkg The bpkg object to check.
 * @return status determining if hash is computed or not
 */
int return_completed_hash(struct bpkg_obj* bpkg) { 
    struct merkle_tree_node* root = creating_tree(bpkg);
    struct merkle_tree* tree = create_tree_struct(root, bpkg);

    // computing hashes for all internal nodes
    printLevelOrder(root);


    if (root->completed == 1) {
        destroyNode(root);
        free(tree);
        return 0;
    } else {
        destroyNode(root);
        free(tree);
        return 1;
    }
    destroyNode(root);
    free(tree);
    return -1;
}