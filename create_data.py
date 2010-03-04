#!/usr/bin/env python

import sys
from random import random, randrange
from xml.etree.cElementTree import Element, SubElement, tostring
from xml.dom.minidom import parseString

NR_OF_VECTORS = 5000
VECTOR_SIZE = 25 # number of key's per Vector, should not exceed
                 # WH_DEFAULT_HASHTABLE_SIZE for now (see whash.h)
VECTOR_KEY_MIN = 1
VECTOR_KEY_MAX = 100000
FILENAME = "vectors.xml"

print >> sys.stderr, "Creating %d vectors with %d dimensions each" %\
        (NR_OF_VECTORS, VECTOR_SIZE)

doc = Element("vectorlist")

for i in range(NR_OF_VECTORS):
    vector_element = SubElement(doc, "vector")
    for j in range(VECTOR_SIZE):
        elem_element = SubElement(vector_element, "elem")
        key = randrange(VECTOR_KEY_MIN, VECTOR_KEY_MAX)
        value = random()
        elem_element.set("key", str(key))
        elem_element.set("value", str(value))

out = open(FILENAME, "w")
out.writelines(parseString(tostring(doc)).toprettyxml())
