from pwn import *
from task import SusCipher
import itertools
import os
import sys

if len(sys.argv) == 1:
    r = process(['python3', 'task.py'])
else:
    r = remote(sys.argv[1], int(sys.argv[2]))

patterns = [
    [0b110011, 0b010110, 0b111000, 2],
    [0b101001, 0b101000, 0b101000, 4],
    [0b110101, 0b011000, 0b000110, 6],
    [0b010010, 0b111000, 0b111111, 7],
    [0b110011, 0b010110, 0b111000, 5],
    [0b110101, 0b011000, 0b000010, 0],
    [0b010010, 0b111000, 0b111111, 3],
    [0b101001, 0b101000, 0b111111, 1]
]

def rand():
    return int.from_bytes(os.urandom(6), 'big')

cands = [[] for _ in range(8)]
test_vector = []
for iidx, pat in enumerate(patterns):
    print(iidx)
    counter = [0] * 64
    idiff, iexp, odiff, oidx = pat

    pos = []
    for x in range(64):
        y = idiff ^ x
        z = SusCipher.S[x] ^ SusCipher.S[y]
        if z == iexp:
            pos.append(x)
    
    tmp = [0] * 8
    tmp[oidx] = odiff
    oexp = SusCipher._combine(SusCipher._perm(tmp))

    for _ in range(1024):
        batch1 = [rand() for _ in range(64)]
        r.sendlineafter(b'> ', ",".join(str(v) for v in batch1).encode())
        res1 = [int(v.strip()) for v in r.recvline().decode().split(', ')]

        batch2 = [v ^ (idiff << (6 * (7 - iidx))) for v in batch1]
        r.sendlineafter(b'> ', ",".join(str(v) for v in batch2).encode())
        res2 = [int(v.strip()) for v in r.recvline().decode().split(', ')]

        for i in range(64):
            if res1[i] ^ res2[i] != oexp:
                continue
        
            for p in pos:
                v = (batch1[i] >> (6 * (7 - iidx))) & 0b111111
                counter[v ^ p] += 1
        
        mx = max(counter)
        if max(counter) >= 10:
            break
    
    for i in range(64):
        if counter[i] == mx:
            cands[iidx].append(i)

    test_vector.append((batch1[0], res1[0]))

print(cands)
for key_blocks in itertools.product(*cands):
    key = SusCipher._combine(key_blocks)
    cipher = SusCipher(key)

    for i, o in test_vector:
        if cipher.encrypt(i) != o:
            break
    else:
        r.sendlineafter(b'> ', str(key).encode())
        r.interactive()

