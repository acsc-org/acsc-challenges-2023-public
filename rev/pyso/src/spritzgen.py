for i in range(64):
    print(f"""@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _prng{i}(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _xor{i}(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111
""")

for i in range(4):
    print(
f"""@njit('(u1[:], u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _enc_part{i}(S, inp, i, j, k, z):""")
    for j in range(i*16, (i+1)*16):
        print(f"    z, i, j, k = _prng{j}(S, i, j, k, z)")
        print(f"    inp[{j}] = _xor{j}(inp[{j}], z)")
    print("    return z, i, j, k")
    print("")

## spritz
print("@njit('u1[:](u1[:],u1[:])', inline='never', debug=True)")
print("def _spritz(K, inp):")
print("    S = np.zeros(256, dtype=np.uint8)")
print("    _ksa_init(S)")
print("    _ksa(S, K, inp)")

print("    i, j, k, z = 0, 0, 0, 0")

for i in range(4):
    print(f"    z, i, j, k = _enc_part{i}(S, inp, i, j, k, z)")

print("    return inp")
print("")