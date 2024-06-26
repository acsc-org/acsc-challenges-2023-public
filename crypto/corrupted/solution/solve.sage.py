

# This file was *autogenerated* from the file solve.sage
from sage.all_cmdline import *   # import sage library

_sage_const_1 = Integer(1); _sage_const_16 = Integer(16); _sage_const_2 = Integer(2); _sage_const_534 = Integer(534); _sage_const_0p5 = RealNumber('0.5'); _sage_const_0p01 = RealNumber('0.01'); _sage_const_0 = Integer(0)
import sys
n = int(sys.argv[_sage_const_1 ],_sage_const_16 )
p = int(sys.argv[_sage_const_2 ],_sage_const_16 )
a = p % _sage_const_2 **_sage_const_534 
PR = PolynomialRing(Zmod(n), names=('x',)); (x,) = PR._first_ngens(1)
f = ((_sage_const_2 **_sage_const_534 )*x + a)
f = f.monic()
root = f.small_roots(beta=_sage_const_0p5 , epsilon=_sage_const_0p01 )[_sage_const_0 ]
p = int(root) << _sage_const_534  | a
q = n//p
assert(n==p*q)
print(hex(p)[_sage_const_2 :],hex(q)[_sage_const_2 :])

