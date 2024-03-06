from Crypto.Util.number import *
import gmpy2
from hashlib import sha512

q = open("output.txt").read()
x = []
m = []

n = -1

# e*d = k * phi + 1
# 0 = k * phi + 1 (mod e)
# k * (p-1) * (q-1) = -1 (mod e)
# k * n - k*p - k*q + k = -1 (mod e)
# -k * (p+q) = -1 - k*n - k (mod e)
# p+q = (1/k) * (k*n + k + 1) (mod e)

for buf in q.split("\n"):
  if n == -1:
    n = int(buf.split(" ")[2])
    continue
  if len(buf) == 0:break
  e,k = map(int,buf.split(":"))
  z = inverse(k,e) * (k*n + k + 1)
  z %= e
  x += [z]
  m += [e]

r = crt(x,m)

print(r)

x_2 = [int(r)]
m_2 = [int(prod(m))]

print(size(m_2[0]))

# p*q = n
# p+q = r
# q = (r-p)
# p*(r-p) = n
# p^2 - p*r + n = 0

# p+q will be 1025 bit
# we have 1008 bit. we need bruteforce.

for k in range(131101):
  r = crt(x_2 + [k],m_2 + [131101])
  dis = r*r - 4*n
  if is_square(dis):
    upper = r + isqrt(dis)
    if upper % 2 == 0:
      p = upper // 2
      q = n // p
      print("found")
      print(p)
      print(q)
      print(n==p*q)
      break     

if p > q:p,q = q,p
flag = "ACSC{" + sha512( f"{p}{q}".encode() ).hexdigest() + "}" 
print(flag)

