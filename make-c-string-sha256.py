#!/usr/bin/env python

from hashlib import sha256
from sys import argv

def make_hash_values(value):
    val = sha256(value).hexdigest();
    res = ''
    j = 0;
    for i in range(2,65,2):
        res += "0x%s" %  val[j:i]
        if (i != 64):
            res+=", "
        j = i
    return res


def make_string(value):
    res =  "{"
    res += make_hash_values(value)
    res += "}"
    return res

if __name__ == "__main__":
    if len(argv) == 1:
        print "Need some arguments"

    for i in argv[1:]:
        print make_string(i)



