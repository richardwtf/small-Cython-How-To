/*
 * Very simple implementation of an associative array for mapping floats (saves
 * space and is pretty much sufficient here) to integers, e.g. for saving
 * termid's and their respective weights. Therefore the name w(eights)hash.
 * TODO
 *   - implement growing and shrinking of the hashtable
 *   - ...
 */

#ifndef _WHASH_H_
#define _WHASH_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <iso646.h>

#define SQUARE(x) x*x

#define WH_NULL_KEY 0
#define WH_NULL_VALUE 0.0 // value of key's that are not in the hash table
#define WH_DEFAULT_HASHTABLE_SIZE 64


/* Type definitions */

typedef struct whash_row_s whash_row_t;
typedef struct whash_s whash_t;

struct whash_row_s {
  uint32_t key;
  float value;
};

struct whash_s {
  uint16_t length; // complete size of `table`
  uint16_t occupied; // number of fields that are non-NULL
  whash_row_t** table;
};


/* Function prototypes */

/*
 * Hash function for mapping any integer in a near pseudo-random way to another
 * integer in a way that grants uniqueness and uniformness on integers in the
 * range [1, 1000000] (approximately).
 */
uint32_t hash(uint32_t);


/*
 * Return pointer to a newly allocated whash_t. Initialize `length` with
 * WH_DEFAULT_HASHTABLE_SIZE and `occupied` with 0. `table` consists of
 * pointers to 0 initialized whash_row_t's.
 */
whash_t* whash_initialize();


/* 
 * free() every row in `table` of _h_, then free() `table` itself.
 */
void whash_clear(whash_t*);


/*
 * Returns the size of the hash, where the size is defined as the number of
 * occupied rows in the hash table.
 */
uint16_t whash_size(whash_t* h);


/*
 * Returns `length` == `occupied`.
 */
bool whash_full(whash_t*);


/*
 * Try to add an integer, float tuple. Return 0 on success, 1 if this integer
 * is already present in the hashtable and 2 if the key or the value is
 * WH_NULL_KEY or WH_NULL_VALUE.
 */
int whash_add(whash_t*, uint32_t, float);


/* 
 * Returns the exact whash_row_t where the query key is/should be if it
 * is/would be part of the hash table. 
 * The returned whash_row_t* can only have a key with the value of the query or
 * WH_NULL_KEY depending on wether or not the key actually exists in the table.
 */
whash_row_t* whash_find(whash_t*, uint32_t);


/*
 * Returns true if the query key is member of the hash table.
 * I.e. if whash_find yields a whash_row_t* with a key != WH_NULL_KEY.
 */
bool whash_has(whash_t*, uint32_t);


/*
 * Returns the value of the given key if it exists in the hash table, or
 * WH_NULL_VALUE if it doesn't.
 */
float whash_get(whash_t* h, uint32_t key);


/*
 * Print content of the hash to stdout, in a Python/Ruby like way.
 */
void whash_print(whash_t* h);


/*
 * Normalize the values of the so that the squares of the values
 * sum up to 1.0 -- useful for a Python Vector class...
 */
void whash_normalize(whash_t* h);

/*
 * Return the cosine similarity of two whash_t*'s _a_ and _b_
 */
float whash_cosine(whash_t* a, whash_t* b);


#endif
