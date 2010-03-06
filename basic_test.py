#!/usr/bin/env python

# "vector" is the name of the .so file
from vector import Vector


v = [Vector()]

v[0].add(2, 0.2)
v[0].add(4, 0.4)
v[0].add(6, 0.6)
v[0].add(8, 0.8)
v[0].add(9, 0.9)

print "v[0] before normalize():"
v[0].print_weights()
v[0].normalize()
print "v[0] after normalize():"
v[0].print_weights()

v.append(Vector())
v[1].add(1, 0.1)
v[1].add(2, 0.2)
v[1].add(8, 0.8)

print "v[1] before normalize():"
v[1].print_weights()
v[1].normalize()
print "v[1] after normalize():"
v[1].print_weights()


print "cosine similarity of v[0] and v[1]:"
print v[0].cosine_sim(v[1])
