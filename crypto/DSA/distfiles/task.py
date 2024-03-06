import os
from hashlib import sha256
from Crypto.Util.number import getPrime, isPrime, getRandomNBitInteger, inverse


flag = os.environ.get("FLAG", "neko{cat_are_the_most_powerful_beings_in_fact}")


def h(m: bytes) -> int:
    return int(sha256(m).hexdigest(), 16)


def gen_prime():
    while True:
        q = getPrime(520)
        p = 2*q + 1
        if isPrime(p):
            return p, q


p1, q1 = gen_prime()
p2, q2 = gen_prime()

if q1 > q2:
    (p1, q1), (p2, q2) = (p2, q2), (p1, q1)

x = int((os.urandom(512 // 8 - len(flag) - 1) + flag.encode()).hex(), 16)
g = 4
y1 = pow(g, x, p1)
y2 = pow(g, x, p2)


def sign(m: bytes):
    z = h(m)
    k = getRandomNBitInteger(512)
    r1 = pow(g, k, p1)
    r2 = pow(g, k, p2)
    s1 = inverse(k, q1) * (z + r1*x) % q1
    s2 = inverse(k, q2) * (z + r2*x) % q2

    return (r1, s1), (r2, s2)


def verify(m: bytes, sig1, sig2):
    z = h(m)
    r1, s1 = sig1
    r2, s2 = sig2

    s1inv = inverse(s1, q1)
    s2inv = inverse(s2, q2)
    gk1 = pow(g, s1inv*z, p1) * pow(y1, s1inv*r1, p1) % p1
    gk2 = pow(g, s2inv*z, p2) * pow(y2, s2inv*r2, p2) % p2

    return r1 == gk1 and r2 == gk2


m = b"omochi mochimochi mochimochi omochi"
sig1, sig2 = sign(m)

print(f"g = {g}")
print(f"p1, p2 = {p1}, {p2}")
print(f"y1, y2 = {y1}, {y2}")

print(f"m = {m}")
print(f"r1, s1 = {sig1}")
print(f"r2, s2 = {sig2}")
