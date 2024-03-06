def egcd(a, b):
	if a == 0:
		return (b, 0, 1)
	else:
		g, y, x = egcd(b % a, a)
		return (g, x - (b // a) * y, y)

def modinv(a, m):
	g, x, y = egcd(a, m)
	if g != 1:
		raise Exception('modular inverse does not exist')
	else:
		return x % m

def gcd(a, b): 
	if a == 0: 
		return b 
	return gcd(b % a, a) 

w,q = [184, 332, 713, 1255, 2688, 5243, 10448], 20910
c = [8436, 22465, 30044, 22465, 51635, 10380, 11879, 50551, 35250, 51223, 14931, 25048, 7352, 50551, 37606, 39550]

for r in range(100,q):
	cond = []
	if gcd(r,q) == 1:
		r_inv = modinv(r,q)
		
		m = ['' for i in range(len(c))]
		for index,c_i in enumerate(c):
			c_inv = (c_i * r_inv) % q
			for w_i in w[::-1]:
				if w_i <= c_inv:
					c_inv -= w_i
					m[index] = '1'+m[index]
				else:
					m[index] = '0'+m[index]
			cond.append(c_inv == 0)
		if all(cond):
			print(''.join([chr(int(i,2)) for i in m]))
			break