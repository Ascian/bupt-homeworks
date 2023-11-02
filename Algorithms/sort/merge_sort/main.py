from random import shuffle

import sys

N = 10000000

if __name__ == '__main__':
    if len(sys.argv) > 1:
        N = int(eval(sys.argv[1]))
    l = list(range(N))
    shuffle(l)
    l = [x % 100000 for x in l]
    f = open("mergesort.in", "w")
    f.write(str(len(l)) + '\n')
    # for i in range(0, int(N / 100)):
    #     l[i * 100:(i + 1) * 100] = sorted(l[i * 100:(i + 1) * 100])

    l.sort(reverse=True)
    for x in l:
        f.write(str(x) + " ")

