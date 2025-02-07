#ifndef PKGCHK_H
#define PKGCHK_H

#include <stddef.h>
#include <stdint.h>
#include <crypt/sha256.h>

#define IDENT_MAX_SIZE (1025)
#define FILENAME_MAX_SIZE (257)
#define HASH_MAX_SIZE (65)
#define SHA256_BFLEN (1024)




/**
 * Query object, allows you to assign
 * hash strings to it.
 * Typically: malloc N number of strings for hashes
 *    after malloc the space for each string
 *    Make sure you deallocate in the destroy function
 */
struct bpkg_query {
	char** hashes;
	size_t len;
};

struct chunk {
	char hash[HASH_MAX_SIZE];
	uint32_t offset;
	uint32_t size;
	char computed_hash[HASH_MAX_SIZE];
};

//TODO: Provide a definition
struct bpkg_obj {
	int h;
	char ident[IDENT_MAX_SIZE];
	char filename[FILENAME_MAX_SIZE];
	uint32_t size;
	uint32_t nhashes;
	char **hashes;
	uint32_t nchunks;
	struct chunk **chunks;
};


/**
 * Loads the package for when a value path is given
 */
struct bpkg_obj* bpkg_load(const char* path);

/**
 * Checks to see if the referenced filename in the bpkg file
 * exists or not.
 * @param bpkg, constructed bpkg object
 * @return query_result, a single string should be
 *      printable in hashes with len sized to 1.
 * 		If the file exists, hashes[0] should contain "File Exists"
 *		If the file does not exist, hashes[0] should contain "File Created"
 */
struct bpkg_query bpkg_file_check(struct bpkg_obj* bpkg);

/**
 * Retrieves a list of all hashes within the package/tree
 * @param bpkg, constructed bpkg object
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
struct bpkg_query bpkg_get_all_hashes(struct bpkg_obj* bpkg);

/**
 * Retrieves all completed chunks of a package object
 * @param bpkg, constructed bpkg object
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
struct bpkg_query bpkg_get_completed_chunks(struct bpkg_obj* bpkg);


/**
 * Gets the mininum of hashes to represented the current completion state
 * Example: If chunks representing start to mid have been completed but
 * 	mid to end have not been, then we will have (N_CHUNKS/2) + 1 hashes
 * 	outputted
 *
 * @param bpkg, constructed bpkg object
 * @return query_result, This structure will contain a list of hashes
 * 		and the number of hashes that have been retrieved
 */
struct bpkg_query bpkg_get_min_completed_hashes(struct bpkg_obj* bpkg); 


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
struct bpkg_query bpkg_get_all_chunk_hashes_from_hash(struct bpkg_obj* bpkg, char* hash);


/**
 * Deallocates the query result after it has been constructed from
 * the relevant queries above.
 * @param query to destroy
 */
void bpkg_query_destroy(struct bpkg_query* qry);

/**
 * Deallocates memory at the end of the program
 * @param object to destroy
 */
void bpkg_obj_destroy(struct bpkg_obj* obj);

struct merkle_tree_node* find_hash(struct merkle_tree_node* node, char* hash);

int find_leaf_nodes(struct merkle_tree_node* node, struct bpkg_query* qry, int size, int allocated_size);

int compute_hash(char filename[FILENAME_MAX_SIZE], struct chunk* chunk);

int return_completed_hash(struct bpkg_obj* bpkg);

#endif