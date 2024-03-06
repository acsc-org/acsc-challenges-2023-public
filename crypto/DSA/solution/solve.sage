# https://raw.githubusercontent.com/defund/coppersmith/master/coppersmith.sage
import itertools

def small_roots(f, bounds, m=1, d=None):
	if not d:
		d = f.degree()

	R = f.base_ring()
	N = R.cardinality()
	
	f /= f.coefficients().pop(0)
	f = f.change_ring(ZZ)

	G = Sequence([], f.parent())
	for i in range(m+1):
		base = N^(m-i) * f^i
		for shifts in itertools.product(range(d), repeat=f.nvariables()):
			g = base * prod(map(power, f.variables(), shifts))
			G.append(g)

	B, monomials = G.coefficient_matrix()
	monomials = vector(monomials)

	factors = [monomial(*bounds) for monomial in monomials]
	for i, factor in enumerate(factors):
		B.rescale_col(i, factor)

	B = B.dense_matrix().LLL()

	B = B.change_ring(QQ)
	for i, factor in enumerate(factors):
		B.rescale_col(i, 1/factor)

	H = Sequence([], f.parent().change_ring(QQ))
	for h in filter(None, B*monomials):
		H.append(h)
		I = H.ideal()
		if I.dimension() == -1:
			H.pop()
		elif I.dimension() == 0:
			roots = []
			for root in I.variety(ring=ZZ):
				root = tuple(R(root[var]) for var in f.variables())
				roots.append(root)
			return roots

	return []


# --- main ---

from hashlib import sha256
import ast

def h(m: bytes) -> int:
    return int(sha256(m).hexdigest(), 16)


f = open("output.txt")
g = ast.literal_eval(f.readline().strip().split(" = ")[1])
p1, p2 = ast.literal_eval(f.readline().strip().split(" = ")[1])
y1, y2 = ast.literal_eval(f.readline().strip().split(" = ")[1])
m = ast.literal_eval(f.readline().strip().split(" = ")[1])
r1, s1 = ast.literal_eval(f.readline().strip().split(" = ")[1])
r2, s2 = ast.literal_eval(f.readline().strip().split(" = ")[1])

q1 = (p1 - 1) // 2
q2 = (p2 - 1) // 2

N = q1 * q2
PR.<x, k> = PolynomialRing(Zmod(N))

k1 = CRT([1, 0], [q1, q2])
k2 = CRT([0, 1], [q1, q2])

z = h(m)
f1 = s1*k - (z + r1*x)
f2 = s2*k - (z + r2*x)

f = k1*f1 + k2*f2
roots = small_roots(f, [q1, q1], m=2, d=2)

print(bytes.fromhex(hex(roots[0][0])[2:]))


