#!/usr/bin/python -O

# timing test:
# 1) read "big" XML file from disk
# 2) create n Vector objects
# 3) euclidian normalization of Vector objects
# 4) compute n x n similarity matrix for Vector objects

import sys, os, time
from xml.etree.cElementTree import ElementTree
from vector import Vector as Vector_c
from math import sqrt

FILENAME = "vectors.xml"

# create Vector class that is similar to the C extension, using a standard
# Python dict for the weights
class Vector:
    def __init__(self):
        self.keys = []
        self.weights = {}

    def add(self, key, value):
        if not key in self.weights:
            self.weights[key] = value
            self.keys.append(key) # keeping track of keys may be useful someday...

    def get(self, key):
        return self.weights[key]

    def size(self):
        return len(self.weights)

    def has(self, key):
      return key in self.weights

    def normalize(self):
        sum = 0.0
        for value in self.weights.values():
            sum += value**2
        if sum == 1.0:
            return
        nf = 1.0 / sqrt(sum)
        for key, value in self.weights.items():
            self.weights[key] = value * nf

    def cosine_sim(self, other):
        sim = 0.0
        for key, value in self.weights.items():
            if key in other.weights:
                sim += value * other.weights[key]
        return sim



vectorspace_c = []
vectorspace_p = []

try:
    doc = ElementTree(file="vectors.xml")
except:
    print >> sys.stderr, "Error: couldn't read data from %s" % FILENAME
    sys.exit(1)
vector_elements = doc.findall("vector")

c = 0
for vector_element in vector_elements:
    c += 1
    vectorspace_c.append(Vector_c())
    vectorspace_p.append(Vector())
    for elem_element in vector_element.getchildren():
        key = int(elem_element.get("key"))
        value = float(elem_element.get("value"))
        vectorspace_c[-1].add(key, value)
        vectorspace_p[-1].add(key, value)

print "%d vectors read" % c
print
print "Timing C extension:"
t0 = time.time()
for vector in vectorspace_c:
    vector.normalize()
print "%g seconds for normalization of vectorspace" % (time.time() - t0)
t0 = time.time()
for i in range(c):
    for j in range(c):
        vectorspace_c[i].cosine_sim(vectorspace_c[j])
print "%g seconds for n x n matrix using C extension" % (time.time() - t0)
print
print "Timing pure Python Vector class:"
t0 = time.time()
for vector in vectorspace_p:
    vector.normalize()
print "%g seconds for normalization of vectorspace" % (time.time() - t0)
t0 = time.time()
for i in range(c):
    for j in range(c):
        vectorspace_p[i].cosine_sim(vectorspace_p[j])
print "%g seconds for n x n matrix using pure Python class" % (time.time() - t0)
