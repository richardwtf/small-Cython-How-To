#include <assert.h>
#include "whash.h"

int main() {
  whash_t* w = whash_initialize();
  whash_add(w, 5, 0.2);
  assert(whash_add(w, 5, 0.1) == 1); // already used key
  assert(whash_add(w, 0, 0.1) == 2); // invalid key
  assert(whash_has(w, 5) == true);
  assert(whash_get(w, 5) == 0.2f);

  whash_t* x = whash_initialize();
  whash_add(x, 6, 0.3);
  assert(whash_has(x, 6) == true);
  assert(whash_get(x, 6) == 0.3f);

  assert(whash_has(w, 7) == false);
  assert(whash_get(x, 7) == WH_NULL_VALUE);
  
  whash_print(w);
  whash_print(x);

  assert(whash_cosine(w, x) == 0.0f);

  whash_add(w, 6, 1.0);
  assert(whash_cosine(w, x) == 0.3f);

  whash_normalize(x);
  assert(whash_get(x, 6) == 1.0f);

  whash_clear(w);
  whash_clear(x);

  free(w);
  free(x);
}
