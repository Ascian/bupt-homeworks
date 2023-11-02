from random import shuffle

import sys

N = 10000000

if __name__ == '__main__':
    if len(sys.argv) > 1:
        N = int(eval(sys.argv[1]))
    l = list(range(N))
    shuffle(l)
    l = [x % 100000 for x in l]
    f = open("quicksort.in", "w")
    f.write(str(len(l)) + '\n')
    for x in l:
        f.write(str(x) + " ")
