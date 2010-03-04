# Importing other Python modules is possible.
import random # not needed here, just for demonstration

# cdef extern from ...: is used to give access to the specified functions and
# types of the particular C header file.
cdef extern from "whash.h":
  ctypedef struct whash_t # (confusing) way to include the whash_t type
  cdef whash_t* whash_initialize()
  cdef void whash_clear(whash_t* h)
  cdef int whash_add(whash_t* h, unsigned int key, float value)
  cdef bint whash_has(whash_t* h, unsigned int key) # bint is basically Pythons boolean
  cdef short whash_size(whash_t* h)
  cdef float whash_get(whash_t* h, unsigned int key)
  cdef void whash_normalize(whash_t* h)
  cdef float whash_cosine(whash_t* a, whash_t* b)
  cdef void whash_print(whash_t* h)

# One could for example also use
cdef extern from "stdlib.h":
  cdef void* malloc(int x)
  cdef void free(void* ptr)
# to include malloc(3) and free(3).


# Use "def" for normal/slow Python functions (also objects, variables), "cdef"
# for fast functions that will not be callable from outside, and "cpdef" for
# fast functions that will be callable (p stands for 'public').
#
# I use "cpdef" for functions that I want to access from the outside and that
# do return something.
#
# Specify the exacpt type of variables as often as possible! It will make the
# resulting C code much less complex and much faster, because it gets rid of
# complicated Python type --> C type conversions (plus no guessing of types).


# Very simple Vector class, that is basically is only a weights hash for now.
cdef class Vector:
  cdef whash_t* weights
  cdef public list keys # "public" makes this property ... well ... public

  def __init__(Vector self):
    self.weights = whash_initialize()
    self.keys = [] # "list" type is the standard Python list, "dict" is dict


  def __del__(Vector self):
    whash_clear(self.weights)
    free(self.weights)


  def add(Vector self, unsigned int key, float value):
    # Using cdef for simple types like integers is usually a great
    # speed benefit over using normal Python types.
    # e.g. we could also write:
    # res = whash_add(self.weights, key, value)
    # but this would generate much more complicated C code and therefore
    # cost speed.
    cdef int res = whash_add(self.weights, key, value)

    if res == 0: # success
      self.keys.append(key) # keeping track of keys may be useful someday...


  # ... just some simple functions here
  
  cpdef float get(Vector self, unsigned int key):
    return whash_get(self.weights, key)


  cpdef short size(Vector self):
    return whash_size(self.weights)


  cpdef bint has(Vector self, unsigned int key):
    return whash_has(self.weights, key)


  def normalize(Vector self):
    whash_normalize(self.weights)


  def print_weights(Vector self):
    whash_print(self.weights)

  cpdef cosine_sim(Vector self, Vector other):
    return whash_cosine(self.weights, other.weights)

