@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng0(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng1(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng2(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng3(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng4(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng5(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng6(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng7(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng8(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng9(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng10(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng11(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng12(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng13(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng14(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng15(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng16(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng17(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng18(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng19(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng20(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng21(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng22(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng23(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng24(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng25(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng26(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng27(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng28(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng29(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng30(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng31(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng32(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng33(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng34(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng35(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng36(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng37(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng38(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng39(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng40(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng41(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng42(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng43(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng44(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng45(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng46(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng47(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng48(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng49(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng50(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng51(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng52(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng53(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng54(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng55(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng56(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng57(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng58(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng59(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng60(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng61(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng62(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('(u1[:], u1, u1, u1, u1)', inline='never')
def _prng63(S, i, j, k, z):
    i = (i + 5) % 256
    j = (k + S[(j + S[i]) % 256]) % 256
    k = (k + i + S[j]) % 256
    S[i], S[j] = S[j], S[i]
    z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
    return z, i, j, k
@njit('u1[:](u1[:],u1[:])', inline='never')
def _spritz(K, inp):
    S = np.zeros(256, dtype=np.uint8)
    _ksa_init0(S)
    _ksa_init1(S)
    _ksa_init2(S)
    _ksa_init3(S)
    _ksa_init4(S)
    _ksa_init5(S)
    _ksa_init6(S)
    _ksa_init7(S)
    _ksa_init8(S)
    _ksa_init9(S)
    _ksa_init10(S)
    _ksa_init11(S)
    _ksa_init12(S)
    _ksa_init13(S)
    _ksa_init14(S)
    _ksa_init15(S)
    _ksa_init16(S)
    _ksa_init17(S)
    _ksa_init18(S)
    _ksa_init19(S)
    _ksa_init20(S)
    _ksa_init21(S)
    _ksa_init22(S)
    _ksa_init23(S)
    _ksa_init24(S)
    _ksa_init25(S)
    _ksa_init26(S)
    _ksa_init27(S)
    _ksa_init28(S)
    _ksa_init29(S)
    _ksa_init30(S)
    _ksa_init31(S)
    _ksa_init32(S)
    _ksa_init33(S)
    _ksa_init34(S)
    _ksa_init35(S)
    _ksa_init36(S)
    _ksa_init37(S)
    _ksa_init38(S)
    _ksa_init39(S)
    _ksa_init40(S)
    _ksa_init41(S)
    _ksa_init42(S)
    _ksa_init43(S)
    _ksa_init44(S)
    _ksa_init45(S)
    _ksa_init46(S)
    _ksa_init47(S)
    _ksa_init48(S)
    _ksa_init49(S)
    _ksa_init50(S)
    _ksa_init51(S)
    _ksa_init52(S)
    _ksa_init53(S)
    _ksa_init54(S)
    _ksa_init55(S)
    _ksa_init56(S)
    _ksa_init57(S)
    _ksa_init58(S)
    _ksa_init59(S)
    _ksa_init60(S)
    _ksa_init61(S)
    _ksa_init62(S)
    _ksa_init63(S)
    _ksa_init64(S)
    _ksa_init65(S)
    _ksa_init66(S)
    _ksa_init67(S)
    _ksa_init68(S)
    _ksa_init69(S)
    _ksa_init70(S)
    _ksa_init71(S)
    _ksa_init72(S)
    _ksa_init73(S)
    _ksa_init74(S)
    _ksa_init75(S)
    _ksa_init76(S)
    _ksa_init77(S)
    _ksa_init78(S)
    _ksa_init79(S)
    _ksa_init80(S)
    _ksa_init81(S)
    _ksa_init82(S)
    _ksa_init83(S)
    _ksa_init84(S)
    _ksa_init85(S)
    _ksa_init86(S)
    _ksa_init87(S)
    _ksa_init88(S)
    _ksa_init89(S)
    _ksa_init90(S)
    _ksa_init91(S)
    _ksa_init92(S)
    _ksa_init93(S)
    _ksa_init94(S)
    _ksa_init95(S)
    _ksa_init96(S)
    _ksa_init97(S)
    _ksa_init98(S)
    _ksa_init99(S)
    _ksa_init100(S)
    _ksa_init101(S)
    _ksa_init102(S)
    _ksa_init103(S)
    _ksa_init104(S)
    _ksa_init105(S)
    _ksa_init106(S)
    _ksa_init107(S)
    _ksa_init108(S)
    _ksa_init109(S)
    _ksa_init110(S)
    _ksa_init111(S)
    _ksa_init112(S)
    _ksa_init113(S)
    _ksa_init114(S)
    _ksa_init115(S)
    _ksa_init116(S)
    _ksa_init117(S)
    _ksa_init118(S)
    _ksa_init119(S)
    _ksa_init120(S)
    _ksa_init121(S)
    _ksa_init122(S)
    _ksa_init123(S)
    _ksa_init124(S)
    _ksa_init125(S)
    _ksa_init126(S)
    _ksa_init127(S)
    _ksa_init128(S)
    _ksa_init129(S)
    _ksa_init130(S)
    _ksa_init131(S)
    _ksa_init132(S)
    _ksa_init133(S)
    _ksa_init134(S)
    _ksa_init135(S)
    _ksa_init136(S)
    _ksa_init137(S)
    _ksa_init138(S)
    _ksa_init139(S)
    _ksa_init140(S)
    _ksa_init141(S)
    _ksa_init142(S)
    _ksa_init143(S)
    _ksa_init144(S)
    _ksa_init145(S)
    _ksa_init146(S)
    _ksa_init147(S)
    _ksa_init148(S)
    _ksa_init149(S)
    _ksa_init150(S)
    _ksa_init151(S)
    _ksa_init152(S)
    _ksa_init153(S)
    _ksa_init154(S)
    _ksa_init155(S)
    _ksa_init156(S)
    _ksa_init157(S)
    _ksa_init158(S)
    _ksa_init159(S)
    _ksa_init160(S)
    _ksa_init161(S)
    _ksa_init162(S)
    _ksa_init163(S)
    _ksa_init164(S)
    _ksa_init165(S)
    _ksa_init166(S)
    _ksa_init167(S)
    _ksa_init168(S)
    _ksa_init169(S)
    _ksa_init170(S)
    _ksa_init171(S)
    _ksa_init172(S)
    _ksa_init173(S)
    _ksa_init174(S)
    _ksa_init175(S)
    _ksa_init176(S)
    _ksa_init177(S)
    _ksa_init178(S)
    _ksa_init179(S)
    _ksa_init180(S)
    _ksa_init181(S)
    _ksa_init182(S)
    _ksa_init183(S)
    _ksa_init184(S)
    _ksa_init185(S)
    _ksa_init186(S)
    _ksa_init187(S)
    _ksa_init188(S)
    _ksa_init189(S)
    _ksa_init190(S)
    _ksa_init191(S)
    _ksa_init192(S)
    _ksa_init193(S)
    _ksa_init194(S)
    _ksa_init195(S)
    _ksa_init196(S)
    _ksa_init197(S)
    _ksa_init198(S)
    _ksa_init199(S)
    _ksa_init200(S)
    _ksa_init201(S)
    _ksa_init202(S)
    _ksa_init203(S)
    _ksa_init204(S)
    _ksa_init205(S)
    _ksa_init206(S)
    _ksa_init207(S)
    _ksa_init208(S)
    _ksa_init209(S)
    _ksa_init210(S)
    _ksa_init211(S)
    _ksa_init212(S)
    _ksa_init213(S)
    _ksa_init214(S)
    _ksa_init215(S)
    _ksa_init216(S)
    _ksa_init217(S)
    _ksa_init218(S)
    _ksa_init219(S)
    _ksa_init220(S)
    _ksa_init221(S)
    _ksa_init222(S)
    _ksa_init223(S)
    _ksa_init224(S)
    _ksa_init225(S)
    _ksa_init226(S)
    _ksa_init227(S)
    _ksa_init228(S)
    _ksa_init229(S)
    _ksa_init230(S)
    _ksa_init231(S)
    _ksa_init232(S)
    _ksa_init233(S)
    _ksa_init234(S)
    _ksa_init235(S)
    _ksa_init236(S)
    _ksa_init237(S)
    _ksa_init238(S)
    _ksa_init239(S)
    _ksa_init240(S)
    _ksa_init241(S)
    _ksa_init242(S)
    _ksa_init243(S)
    _ksa_init244(S)
    _ksa_init245(S)
    _ksa_init246(S)
    _ksa_init247(S)
    _ksa_init248(S)
    _ksa_init249(S)
    _ksa_init250(S)
    _ksa_init251(S)
    _ksa_init252(S)
    _ksa_init253(S)
    _ksa_init254(S)
    _ksa_init255(S)

    j = 0
    j = _ksa0(S, K, 0, j)
    j = _ksa1(S, K, 1, j)
    j = _ksa2(S, K, 2, j)
    j = _ksa3(S, K, 3, j)
    j = _ksa4(S, K, 4, j)
    j = _ksa5(S, K, 5, j)
    j = _ksa6(S, K, 6, j)
    j = _ksa7(S, K, 7, j)
    j = _ksa8(S, K, 8, j)
    j = _ksa9(S, K, 9, j)
    j = _ksa10(S, K, 10, j)
    j = _ksa11(S, K, 11, j)
    j = _ksa12(S, K, 12, j)
    j = _ksa13(S, K, 13, j)
    j = _ksa14(S, K, 14, j)
    j = _ksa15(S, K, 15, j)
    j = _ksa16(S, K, 16, j)
    j = _ksa17(S, K, 17, j)
    j = _ksa18(S, K, 18, j)
    j = _ksa19(S, K, 19, j)
    j = _ksa20(S, K, 20, j)
    j = _ksa21(S, K, 21, j)
    j = _ksa22(S, K, 22, j)
    j = _ksa23(S, K, 23, j)
    j = _ksa24(S, K, 24, j)
    j = _ksa25(S, K, 25, j)
    j = _ksa26(S, K, 26, j)
    j = _ksa27(S, K, 27, j)
    j = _ksa28(S, K, 28, j)
    j = _ksa29(S, K, 29, j)
    j = _ksa30(S, K, 30, j)
    j = _ksa31(S, K, 31, j)
    j = _ksa32(S, K, 32, j)
    j = _ksa33(S, K, 33, j)
    j = _ksa34(S, K, 34, j)
    j = _ksa35(S, K, 35, j)
    j = _ksa36(S, K, 36, j)
    j = _ksa37(S, K, 37, j)
    j = _ksa38(S, K, 38, j)
    j = _ksa39(S, K, 39, j)
    j = _ksa40(S, K, 40, j)
    j = _ksa41(S, K, 41, j)
    j = _ksa42(S, K, 42, j)
    j = _ksa43(S, K, 43, j)
    j = _ksa44(S, K, 44, j)
    j = _ksa45(S, K, 45, j)
    j = _ksa46(S, K, 46, j)
    j = _ksa47(S, K, 47, j)
    j = _ksa48(S, K, 48, j)
    j = _ksa49(S, K, 49, j)
    j = _ksa50(S, K, 50, j)
    j = _ksa51(S, K, 51, j)
    j = _ksa52(S, K, 52, j)
    j = _ksa53(S, K, 53, j)
    j = _ksa54(S, K, 54, j)
    j = _ksa55(S, K, 55, j)
    j = _ksa56(S, K, 56, j)
    j = _ksa57(S, K, 57, j)
    j = _ksa58(S, K, 58, j)
    j = _ksa59(S, K, 59, j)
    j = _ksa60(S, K, 60, j)
    j = _ksa61(S, K, 61, j)
    j = _ksa62(S, K, 62, j)
    j = _ksa63(S, K, 63, j)
    j = _ksa64(S, K, 64, j)
    j = _ksa65(S, K, 65, j)
    j = _ksa66(S, K, 66, j)
    j = _ksa67(S, K, 67, j)
    j = _ksa68(S, K, 68, j)
    j = _ksa69(S, K, 69, j)
    j = _ksa70(S, K, 70, j)
    j = _ksa71(S, K, 71, j)
    j = _ksa72(S, K, 72, j)
    j = _ksa73(S, K, 73, j)
    j = _ksa74(S, K, 74, j)
    j = _ksa75(S, K, 75, j)
    j = _ksa76(S, K, 76, j)
    j = _ksa77(S, K, 77, j)
    j = _ksa78(S, K, 78, j)
    j = _ksa79(S, K, 79, j)
    j = _ksa80(S, K, 80, j)
    j = _ksa81(S, K, 81, j)
    j = _ksa82(S, K, 82, j)
    j = _ksa83(S, K, 83, j)
    j = _ksa84(S, K, 84, j)
    j = _ksa85(S, K, 85, j)
    j = _ksa86(S, K, 86, j)
    j = _ksa87(S, K, 87, j)
    j = _ksa88(S, K, 88, j)
    j = _ksa89(S, K, 89, j)
    j = _ksa90(S, K, 90, j)
    j = _ksa91(S, K, 91, j)
    j = _ksa92(S, K, 92, j)
    j = _ksa93(S, K, 93, j)
    j = _ksa94(S, K, 94, j)
    j = _ksa95(S, K, 95, j)
    j = _ksa96(S, K, 96, j)
    j = _ksa97(S, K, 97, j)
    j = _ksa98(S, K, 98, j)
    j = _ksa99(S, K, 99, j)
    j = _ksa100(S, K, 100, j)
    j = _ksa101(S, K, 101, j)
    j = _ksa102(S, K, 102, j)
    j = _ksa103(S, K, 103, j)
    j = _ksa104(S, K, 104, j)
    j = _ksa105(S, K, 105, j)
    j = _ksa106(S, K, 106, j)
    j = _ksa107(S, K, 107, j)
    j = _ksa108(S, K, 108, j)
    j = _ksa109(S, K, 109, j)
    j = _ksa110(S, K, 110, j)
    j = _ksa111(S, K, 111, j)
    j = _ksa112(S, K, 112, j)
    j = _ksa113(S, K, 113, j)
    j = _ksa114(S, K, 114, j)
    j = _ksa115(S, K, 115, j)
    j = _ksa116(S, K, 116, j)
    j = _ksa117(S, K, 117, j)
    j = _ksa118(S, K, 118, j)
    j = _ksa119(S, K, 119, j)
    j = _ksa120(S, K, 120, j)
    j = _ksa121(S, K, 121, j)
    j = _ksa122(S, K, 122, j)
    j = _ksa123(S, K, 123, j)
    j = _ksa124(S, K, 124, j)
    j = _ksa125(S, K, 125, j)
    j = _ksa126(S, K, 126, j)
    j = _ksa127(S, K, 127, j)
    j = _ksa128(S, K, 128, j)
    j = _ksa129(S, K, 129, j)
    j = _ksa130(S, K, 130, j)
    j = _ksa131(S, K, 131, j)
    j = _ksa132(S, K, 132, j)
    j = _ksa133(S, K, 133, j)
    j = _ksa134(S, K, 134, j)
    j = _ksa135(S, K, 135, j)
    j = _ksa136(S, K, 136, j)
    j = _ksa137(S, K, 137, j)
    j = _ksa138(S, K, 138, j)
    j = _ksa139(S, K, 139, j)
    j = _ksa140(S, K, 140, j)
    j = _ksa141(S, K, 141, j)
    j = _ksa142(S, K, 142, j)
    j = _ksa143(S, K, 143, j)
    j = _ksa144(S, K, 144, j)
    j = _ksa145(S, K, 145, j)
    j = _ksa146(S, K, 146, j)
    j = _ksa147(S, K, 147, j)
    j = _ksa148(S, K, 148, j)
    j = _ksa149(S, K, 149, j)
    j = _ksa150(S, K, 150, j)
    j = _ksa151(S, K, 151, j)
    j = _ksa152(S, K, 152, j)
    j = _ksa153(S, K, 153, j)
    j = _ksa154(S, K, 154, j)
    j = _ksa155(S, K, 155, j)
    j = _ksa156(S, K, 156, j)
    j = _ksa157(S, K, 157, j)
    j = _ksa158(S, K, 158, j)
    j = _ksa159(S, K, 159, j)
    j = _ksa160(S, K, 160, j)
    j = _ksa161(S, K, 161, j)
    j = _ksa162(S, K, 162, j)
    j = _ksa163(S, K, 163, j)
    j = _ksa164(S, K, 164, j)
    j = _ksa165(S, K, 165, j)
    j = _ksa166(S, K, 166, j)
    j = _ksa167(S, K, 167, j)
    j = _ksa168(S, K, 168, j)
    j = _ksa169(S, K, 169, j)
    j = _ksa170(S, K, 170, j)
    j = _ksa171(S, K, 171, j)
    j = _ksa172(S, K, 172, j)
    j = _ksa173(S, K, 173, j)
    j = _ksa174(S, K, 174, j)
    j = _ksa175(S, K, 175, j)
    j = _ksa176(S, K, 176, j)
    j = _ksa177(S, K, 177, j)
    j = _ksa178(S, K, 178, j)
    j = _ksa179(S, K, 179, j)
    j = _ksa180(S, K, 180, j)
    j = _ksa181(S, K, 181, j)
    j = _ksa182(S, K, 182, j)
    j = _ksa183(S, K, 183, j)
    j = _ksa184(S, K, 184, j)
    j = _ksa185(S, K, 185, j)
    j = _ksa186(S, K, 186, j)
    j = _ksa187(S, K, 187, j)
    j = _ksa188(S, K, 188, j)
    j = _ksa189(S, K, 189, j)
    j = _ksa190(S, K, 190, j)
    j = _ksa191(S, K, 191, j)
    j = _ksa192(S, K, 192, j)
    j = _ksa193(S, K, 193, j)
    j = _ksa194(S, K, 194, j)
    j = _ksa195(S, K, 195, j)
    j = _ksa196(S, K, 196, j)
    j = _ksa197(S, K, 197, j)
    j = _ksa198(S, K, 198, j)
    j = _ksa199(S, K, 199, j)
    j = _ksa200(S, K, 200, j)
    j = _ksa201(S, K, 201, j)
    j = _ksa202(S, K, 202, j)
    j = _ksa203(S, K, 203, j)
    j = _ksa204(S, K, 204, j)
    j = _ksa205(S, K, 205, j)
    j = _ksa206(S, K, 206, j)
    j = _ksa207(S, K, 207, j)
    j = _ksa208(S, K, 208, j)
    j = _ksa209(S, K, 209, j)
    j = _ksa210(S, K, 210, j)
    j = _ksa211(S, K, 211, j)
    j = _ksa212(S, K, 212, j)
    j = _ksa213(S, K, 213, j)
    j = _ksa214(S, K, 214, j)
    j = _ksa215(S, K, 215, j)
    j = _ksa216(S, K, 216, j)
    j = _ksa217(S, K, 217, j)
    j = _ksa218(S, K, 218, j)
    j = _ksa219(S, K, 219, j)
    j = _ksa220(S, K, 220, j)
    j = _ksa221(S, K, 221, j)
    j = _ksa222(S, K, 222, j)
    j = _ksa223(S, K, 223, j)
    j = _ksa224(S, K, 224, j)
    j = _ksa225(S, K, 225, j)
    j = _ksa226(S, K, 226, j)
    j = _ksa227(S, K, 227, j)
    j = _ksa228(S, K, 228, j)
    j = _ksa229(S, K, 229, j)
    j = _ksa230(S, K, 230, j)
    j = _ksa231(S, K, 231, j)
    j = _ksa232(S, K, 232, j)
    j = _ksa233(S, K, 233, j)
    j = _ksa234(S, K, 234, j)
    j = _ksa235(S, K, 235, j)
    j = _ksa236(S, K, 236, j)
    j = _ksa237(S, K, 237, j)
    j = _ksa238(S, K, 238, j)
    j = _ksa239(S, K, 239, j)
    j = _ksa240(S, K, 240, j)
    j = _ksa241(S, K, 241, j)
    j = _ksa242(S, K, 242, j)
    j = _ksa243(S, K, 243, j)
    j = _ksa244(S, K, 244, j)
    j = _ksa245(S, K, 245, j)
    j = _ksa246(S, K, 246, j)
    j = _ksa247(S, K, 247, j)
    j = _ksa248(S, K, 248, j)
    j = _ksa249(S, K, 249, j)
    j = _ksa250(S, K, 250, j)
    j = _ksa251(S, K, 251, j)
    j = _ksa252(S, K, 252, j)
    j = _ksa253(S, K, 253, j)
    j = _ksa254(S, K, 254, j)
    j = _ksa255(S, K, 255, j)

    z, i, j, k = _prng0(S, 0, 0, 0, 0)
    inp[0] ^= z
    z, i, j, k = _prng1(S, i, j, k, z)
    inp[1] ^= z
    z, i, j, k = _prng2(S, i, j, k, z)
    inp[2] ^= z
    z, i, j, k = _prng3(S, i, j, k, z)
    inp[3] ^= z
    z, i, j, k = _prng4(S, i, j, k, z)
    inp[4] ^= z
    z, i, j, k = _prng5(S, i, j, k, z)
    inp[5] ^= z
    z, i, j, k = _prng6(S, i, j, k, z)
    inp[6] ^= z
    z, i, j, k = _prng7(S, i, j, k, z)
    inp[7] ^= z
    z, i, j, k = _prng8(S, i, j, k, z)
    inp[8] ^= z
    z, i, j, k = _prng9(S, i, j, k, z)
    inp[9] ^= z
    z, i, j, k = _prng10(S, i, j, k, z)
    inp[10] ^= z
    z, i, j, k = _prng11(S, i, j, k, z)
    inp[11] ^= z
    z, i, j, k = _prng12(S, i, j, k, z)
    inp[12] ^= z
    z, i, j, k = _prng13(S, i, j, k, z)
    inp[13] ^= z
    z, i, j, k = _prng14(S, i, j, k, z)
    inp[14] ^= z
    z, i, j, k = _prng15(S, i, j, k, z)
    inp[15] ^= z
    z, i, j, k = _prng16(S, i, j, k, z)
    inp[16] ^= z
    z, i, j, k = _prng17(S, i, j, k, z)
    inp[17] ^= z
    z, i, j, k = _prng18(S, i, j, k, z)
    inp[18] ^= z
    z, i, j, k = _prng19(S, i, j, k, z)
    inp[19] ^= z
    z, i, j, k = _prng20(S, i, j, k, z)
    inp[20] ^= z
    z, i, j, k = _prng21(S, i, j, k, z)
    inp[21] ^= z
    z, i, j, k = _prng22(S, i, j, k, z)
    inp[22] ^= z
    z, i, j, k = _prng23(S, i, j, k, z)
    inp[23] ^= z
    z, i, j, k = _prng24(S, i, j, k, z)
    inp[24] ^= z
    z, i, j, k = _prng25(S, i, j, k, z)
    inp[25] ^= z
    z, i, j, k = _prng26(S, i, j, k, z)
    inp[26] ^= z
    z, i, j, k = _prng27(S, i, j, k, z)
    inp[27] ^= z
    z, i, j, k = _prng28(S, i, j, k, z)
    inp[28] ^= z
    z, i, j, k = _prng29(S, i, j, k, z)
    inp[29] ^= z
    z, i, j, k = _prng30(S, i, j, k, z)
    inp[30] ^= z
    z, i, j, k = _prng31(S, i, j, k, z)
    inp[31] ^= z
    z, i, j, k = _prng32(S, i, j, k, z)
    inp[32] ^= z
    z, i, j, k = _prng33(S, i, j, k, z)
    inp[33] ^= z
    z, i, j, k = _prng34(S, i, j, k, z)
    inp[34] ^= z
    z, i, j, k = _prng35(S, i, j, k, z)
    inp[35] ^= z
    z, i, j, k = _prng36(S, i, j, k, z)
    inp[36] ^= z
    z, i, j, k = _prng37(S, i, j, k, z)
    inp[37] ^= z
    z, i, j, k = _prng38(S, i, j, k, z)
    inp[38] ^= z
    z, i, j, k = _prng39(S, i, j, k, z)
    inp[39] ^= z
    z, i, j, k = _prng40(S, i, j, k, z)
    inp[40] ^= z
    z, i, j, k = _prng41(S, i, j, k, z)
    inp[41] ^= z
    z, i, j, k = _prng42(S, i, j, k, z)
    inp[42] ^= z
    z, i, j, k = _prng43(S, i, j, k, z)
    inp[43] ^= z
    z, i, j, k = _prng44(S, i, j, k, z)
    inp[44] ^= z
    z, i, j, k = _prng45(S, i, j, k, z)
    inp[45] ^= z
    z, i, j, k = _prng46(S, i, j, k, z)
    inp[46] ^= z
    z, i, j, k = _prng47(S, i, j, k, z)
    inp[47] ^= z
    z, i, j, k = _prng48(S, i, j, k, z)
    inp[48] ^= z
    z, i, j, k = _prng49(S, i, j, k, z)
    inp[49] ^= z
    z, i, j, k = _prng50(S, i, j, k, z)
    inp[50] ^= z
    z, i, j, k = _prng51(S, i, j, k, z)
    inp[51] ^= z
    z, i, j, k = _prng52(S, i, j, k, z)
    inp[52] ^= z
    z, i, j, k = _prng53(S, i, j, k, z)
    inp[53] ^= z
    z, i, j, k = _prng54(S, i, j, k, z)
    inp[54] ^= z
    z, i, j, k = _prng55(S, i, j, k, z)
    inp[55] ^= z
    z, i, j, k = _prng56(S, i, j, k, z)
    inp[56] ^= z
    z, i, j, k = _prng57(S, i, j, k, z)
    inp[57] ^= z
    z, i, j, k = _prng58(S, i, j, k, z)
    inp[58] ^= z
    z, i, j, k = _prng59(S, i, j, k, z)
    inp[59] ^= z
    z, i, j, k = _prng60(S, i, j, k, z)
    inp[60] ^= z
    z, i, j, k = _prng61(S, i, j, k, z)
    inp[61] ^= z
    z, i, j, k = _prng62(S, i, j, k, z)
    inp[62] ^= z
    z, i, j, k = _prng63(S, i, j, k, z)
    inp[63] ^= z
    return inp
