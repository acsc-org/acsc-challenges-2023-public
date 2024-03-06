import libvalidator
import numpy as np

KEY = np.array(list(b'\xf7\x9b\xc3\xcc\x81\x8d\xb9\xd1\x1a\xddK\xc2/\x85\xb8U'), dtype='uint8')
IV = np.array(list(b'\x84\x9a\xa6\xf6\x84s\x86\xf4\xb9\xda\xe9\xc6\xca@:2'), dtype='uint8')

flag = list(input("Enter flag: ").encode())
flag = np.array(flag, dtype=np.uint8)

if libvalidator.fast_len(flag) != 24:
    print("Wrong length!")

X = np.array(list(b'\xef\x1e\xe6\xd9jh\xaa\xea\xa6\xfcq\x0ec\x94\xd7\xc4\xcc\x7f\xe7\xa1\xf3\x9f\xa6z'), dtype='uint8')

flag = libvalidator.fast_xor(flag, X)
flag = libvalidator.fast_rev(flag)
flag = libvalidator.aes_encrypt(flag, KEY, IV)

E = np.array(list(b'\x81\xdd\xfc\xfd\xa9\xc1LE\xdfy\xfd0\x06f\xa1}\xd2\x8c%<U\xbaA\x97'), dtype='uint8')
if libvalidator.fast_cmp(flag, E):
    print("Correct!")
else:
    print("Try harder!")