@cc.export('aes_encrypt', '(u1[:],u1[:],u1[:])')
def aes_encrypt(inp, key, iv):
    key = np.zeros(256, dtype=np.uint8)
    P = _spritz(key, inp)
    a = 0
    a |= _cmp_part0(P)
    a |= _cmp_part1(P)
    a |= _cmp_part2(P)
    a |= _cmp_part3(P)
    return a == 0

@cc.export('fast_len', '(u1[:],)')
def fast_len(inp):
    return 24

@cc.export('fast_xor', '(u1[:],u1[:])')
def fast_xor(inp, k):
    return inp

@cc.export('fast_rev', '(u1[:],)')
def fast_rev(inp):
    return inp

@cc.export('fast_cmp', '(u1,u1[:])')
def fast_cmp(inp, C):
    return inp

if __name__ == "__main__":
    cc.compile()
    # aes_encrypt(np.array(list(b"acsc{pyth0n_numb4_0n3___s0_m4ny_functi0ns_s0_l1ttl3_us3ful_c0d3}"), dtype=np.uint8), None, None)