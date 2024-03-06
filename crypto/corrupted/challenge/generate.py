from Crypto.PublicKey import RSA
from Crypto.Util.number import *
from Crypto.Cipher import PKCS1_v1_5
import base64
import random

# Generate private key
pem = ''
while True:
	p = getStrongPrime(1024)
	q = getStrongPrime(1024)
	n = p*q
	e = 0x10001
	phi = (p-1)*(q-1)
	d = inverse(e, phi)
	key = RSA.construct((n,e,d,p,q))
	pem = key.export_key('PEM')
	text = pem[32:-30]
	text = text[715:-512]
	text = text[:64] + text[65:65+64] + text[65+65:65+65+64] + text[65*3:(65*3)+64] + text[65*4:(65*4)+64] + text[65*5:(65*5)+64]
	a = text[12:172+12]
	b = text[12+176:-24]
	# Make sure the length is 1678 and primes doesn't ends with 02
	if len(pem) != 1678 or base64.b64decode(a).hex()[-2:] == "02":
		continue
	else:
		break
print(a)
print(b)
print(base64.b64decode(a).hex())
print(base64.b64decode(b).hex())
print(hex(n))
print(hex(p))
print(hex(q))
# Debug
# HEAD = 32
# TAIL = 30
# print(pem.decode()[HEAD:-TAIL])
# for i in pem.decode()[HEAD:-TAIL].split('\n'):
# 	print(base64.b64decode(i).hex())


# print(pem[32+715+12:-30-512-24])
newPem = bytearray(pem)
i = 0
while i < 357:
	newPem[-30-512-24-i] = ord('?')
	# Remove half bits of Q	
	if i == 40:
		i+=1
	elif i == 105:
		i+=1
	elif i == 105+65:
		i+=1

	# Remove half bits of P
	if i == 105+65+5:
		i+=3
	if i == 105+65+64:
		i+=1
	if i == 105+65+64+65:
		i+=1
	i+=2

# Random corrupt the key except N and p,q
l = list(range(len(pem)))
random.shuffle(l)
removeIndex = l[:(len(pem)//2)]
for r in removeIndex:
	if r not in range(32+715+12+85,1112) and r not in range(0,(65*5)+75):
		newPem[r] = ord('?')

open("corrupted.pem",'wb').write(newPem)
open("original.pem",'wb').write(pem)
public = key.publickey()
public_pem = public.export_key('OpenSSH')
# SSH public key for server authorized keys
open("public.pem","wb").write(public_pem)