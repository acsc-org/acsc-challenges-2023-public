import sys
n = int(sys.argv[1],16)
p = int(sys.argv[2],16)
a = p % 2^534
PR.<x> = PolynomialRing(Zmod(n))
f = ((2^534)*x + a)
f = f.monic()
root = f.small_roots(beta=0.5, epsilon=0.01)[0]
p = int(root) << 534 | a
q = n//p
assert(n==p*q)
print(hex(p)[2:],hex(q)[2:])