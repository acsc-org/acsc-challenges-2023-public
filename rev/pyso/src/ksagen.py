import random

random.seed(919191)

S_arr = [i for i in range(256)]
random.shuffle(S_arr)

for i in range(256):
    print(f"""@njit('(u1[:],)', inline='never', debug=True)
def _ksa_init{i}(S):
    S[{i}] = {S_arr[i]}
    Z = np.zeros(256, dtype=np.uint8)""")

    r = random.randint(0, 3)
    if r == 0:
        print("""
    for i in range(256):
        Z[i] *= S[i]
    return Z
""")
    elif r == 1:
        print("""
    for i in range(256):
        Z[i] += S[i]
    return Z
""")
    elif r == 2:
        print("""
    for i in range(256):
        Z[i] -= S[i]
    return Z
""")
    elif r == 3:
        print("""
    for i in range(256):
        Z[i] ^= S[i]
    return Z
""")


for i in range(256):
    print(f"""@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ksa{i}(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j""")

for i in range(16):
    print(
f"""@njit('(u1[:],)', inline='never', debug=True)
def _ksa_init_part{i}(S):""")
    for j in range(i*16, (i+1)*16):
        print(f"    _ksa_init{j}(S)")

    print("")

print(
f"""@njit('(u1[:],)', inline='never', debug=True)
def _ksa_init(S):""")
for i in range(16):
    print(f"    _ksa_init_part{i}(S)")

print("")

for i in range(16):
    print(
f"""@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _ksa_part{i}(S, K, flag, j):""")
    for j in range(i*16, (i+1)*16):
        print(f"    j = _ksa{j}(S, K, flag, {j}, j)")

    print("    return j")

print(
f"""@njit('(u1[:],u1[:],u1[:])', inline='never', debug=True)
def _ksa(S, K, flag):
    j = 0""")
for i in range(16):
    print(f"    j = _ksa_part{i}(S, K, flag, j)")

print("")