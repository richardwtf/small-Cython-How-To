#include "whash.h"

  
  
uint32_t 
hash(uint32_t a)
{
  a = (a * 0x44f) << 12;
  a = (~a ^ 0x5f7be) >> 6;
  a = (a + 0x9632) + (a << 3);
  a = (a ^ 0xa21d3) ^ (a << 4);
  a = (a + 0x7337) + (a << 3);
  a = (a ^ 0x9ff7c) >> 6;
  return a;
}


whash_t*
whash_initialize()
{
  int i;
  whash_t* result = malloc(sizeof(whash_t));
  result->table = malloc(sizeof(whash_row_t*) * WH_DEFAULT_HASHTABLE_SIZE);
  result->length = WH_DEFAULT_HASHTABLE_SIZE;
  result->occupied = 0;
  for (i = 0; i < WH_DEFAULT_HASHTABLE_SIZE; i++) {
    result->table[i] = calloc(1, sizeof(whash_row_t));
    result->table[i]->key = WH_NULL_KEY;
    result->table[i]->value = WH_NULL_VALUE;
  }
  return result;
}


void
whash_clear(whash_t* h)
{
  int i;
  uint16_t m = h->length;
  for (i = 0; i < m; i++) {
    free(h->table[i]);
    h->table[i] = NULL;
  }
  free(h->table);
  h->table = NULL;
  h->occupied = 0;
}


uint16_t
whash_size(whash_t* h)
{
  return h->occupied;
}


bool
whash_full(whash_t* h)
{
  return h->length == h->occupied;
}


int
whash_add(whash_t* h, uint32_t key, float value)
{
  whash_row_t* target;
  
  if (whash_full(h)) {
    fprintf(stderr, "Error: hash is full.\n");
    exit(EXIT_FAILURE);
  }
  if (key == WH_NULL_KEY or value == WH_NULL_VALUE) {
    fprintf(stderr, "Warning: not adding: key %d, value %g\n",
        key, value);
    return 2;
  }

  target = whash_find(h, key); /* magic happens here */

  if (target->key == key) {
     /* key already exists in h */
    return 1;
  } else {
    target->key = key;
    target->value = value;
    h->occupied++;
    return 0;
  }
}


whash_row_t*
whash_find(whash_t* h, uint32_t key)
{
  uint32_t m = (uint32_t) (h->length - 1);
  uint32_t pos = hash(key);
  whash_row_t* row = h->table[pos & m];

  while (1) {
    if (row->key == WH_NULL_KEY or row->key == key) {
      return row;
    }
    pos++; /* linear collision strategy */
    row = h->table[pos & m];
  }
}


bool
whash_has(whash_t* h, uint32_t key)
{
  return (whash_find(h, key)->key != WH_NULL_KEY);
}


float
whash_get(whash_t* h, uint32_t key)
{
  whash_row_t* row = whash_find(h, key);
  if (row->key == WH_NULL_KEY)
    return WH_NULL_VALUE;
  else
    return row->value;
}


void 
whash_print(whash_t* h)
{
  int c = 0;
  int i;
  printf("{");
  for (i = 0; c < h->occupied; i++) {
    if (h->table[i]->key != WH_NULL_KEY) {
      printf("%d: %g", h->table[i]->key, h->table[i]->value);
      if (c != h->occupied - 1) printf(", ");
      c++;
    }
  }
  printf("}\n");
}


void
whash_normalize(whash_t* h)
{
  int i, c = 0;
  float squaressum = 0.0;
  float nf;

  for (i = 0; c < h->occupied; i++) {
    if (h->table[i]->key != WH_NULL_KEY) {
      squaressum += SQUARE(h->table[i]->value);
      c++;
    }
  }
  
  if (squaressum == 1.0f) {
    return;
  }

  nf = 1.0f / sqrtf(squaressum);

  c = 0;
  for (i = 0; c < h->occupied; i++) {
    if (h->table[i]->key != WH_NULL_KEY) {
      h->table[i]->value *= nf;
      c++;
    }
  }
}


float
whash_cosine(whash_t* a, whash_t* b)
{
  float result = 0.0;
  int i, c = 0;
  whash_row_t *row_a, *row_b;

  for (i = 0; c < a->occupied; i++) {
    row_a = a->table[i];
    if (row_a->key != WH_NULL_KEY) {
      row_b = whash_find(b, row_a->key);
      if (row_b->key != WH_NULL_KEY)
        result += row_a->value * row_b->value;
      c++;
    }
  }
  return result;
}
