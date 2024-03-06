import base64
from Crypto.PublicKey import RSA
import subprocess
text = open("../challenge/corrupted.pem","r").read()

# Get N
text = text[32:-30]
tmp = text[:(65*5)+40].replace('?','').replace('\n','')
n = int(base64.b64decode(tmp).hex()[24:-4],16)
print(hex(n))

# Get bits of p,q
text = text[715:-512]
text = text[:64] + text[65:65+64] + text[65+65:65+65+64] + text[65*3:(65*3)+64] + text[65*4:(65*4)+64] + text[65*5:(65*5)+64]
print(text[12:172+12])
print(text[12+176:-24])
# Replace unknown bytes with 'A'
text = text.replace('?','A')
p = base64.b64decode(text[12:172+12]).hex()
q = base64.b64decode(text[12+176:-24]).hex()
print(p)
print(q)

p = int(p,16)
q = int(q,16)

# Recover p and q start from LSB
# Using the formula N=pq mod 2^i
for t in range(89):
	if t % 2:
		for i in range(1+(t*6),1+((t+1)*6)):
			for j in range(0,2):
				temp = q|(j*2**(i-1))
				if (p * temp) % 2**i == n % 2**i:
					q = temp
	else:
		for i in range(1+(t*6),1+((t+1)*6)):
			for j in range(0,2):
				temp = p|(j*2**(i-1))
				# print(bin(temp%2**6))
				if (q * temp) % 2**i == n % 2**i:
					p = temp
# Recovered 534bits of p,q (89*6bits)
print(hex(p%2**534))
print(hex(q%2**534))

# Pass the n,p to sage script to calculate the small roots (coppersmith)
output = subprocess.check_output(['sage','solve.sage',hex(n)[2:],hex(p)[2:]])
p,q = output.split() 
p = int(p,16)
q = int(q,16)
phi = (p-1)*(q-1)
e = 65537
d = pow(e,-1, phi)
# print(n,e,d,p,q)
# Generate private file for ssh
key = RSA.construct((n,e,d,p,q))
pem = key.export_key('PEM')
print(pem.decode())
open("private.pem","wb").write(pem)