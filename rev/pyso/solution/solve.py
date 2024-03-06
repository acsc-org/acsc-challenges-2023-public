import random

random.seed(919191)

P = [141, 109, 174, 83, 181, 39, 15, 49, 226, 110, 18, 245, 75, 130, 20, 68, 232, 199, 32, 239, 33, 12, 61, 27, 22, 132, 145, 245, 137, 209, 27, 134, 138, 239, 173, 242, 218, 203, 88, 210, 176, 81, 42, 99, 147, 198, 231, 176, 17, 153, 149, 243, 53, 14, 209, 88, 132, 124, 85, 81, 183, 207, 167, 87]

## ksa
S = [i for i in range(256)]
random.shuffle(S)
K = [0 for i in range(256)]

ijs = []

j = 0
for i in range(256):
    j = (j + S[i] + K[i]) % 256
    S[i], S[j] = S[j], S[i]
    ijs.append((i, j))

# part 1: reverse the prng
C = []
i = 0
j = 0
k = 0
z = 0
for a in range(len(P)):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]

    C.append((P[a] - z) % 0x100)

# print(C)

## part 2: reverse the swapping
for i, j in ijs[::-1]:
    C[i // 4] ^= i
    C[j // 4] ^= j
    if i // 4 != j // 4:
        C[i // 4] ^= C[j // 4]

print(C)
print(bytes(C))