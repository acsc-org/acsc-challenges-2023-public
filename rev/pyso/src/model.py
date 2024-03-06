import random
random.seed(919191)

## ksa
S = [i for i in range(256)]
random.shuffle(S)
K = [0 for i in range(256)]

flag = b"ACSC{pyth0n_numb4_0n3___s0_m4ny_functi0ns_s0_l1ttl3_us3ful_c0d3}"
flag = list(flag)

j = 0
for i in range(256):
    j = (j + S[i] + K[i]) % 256
    S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]
    flag[i // 4] ^= i
    flag[j // 4] ^= j


C = []
i = 0
j = 0
k = 0
z = 0
for a in range(len(flag)):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    # print(S, z, i, j, k)
    # C.append(flag[a] ^ z)
    C.append((flag[a] + z) % 256)

print(C)
print(bytes(C))