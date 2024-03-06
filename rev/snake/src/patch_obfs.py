import lief
import struct
from binascii import hexlify, unhexlify
import hashlib

binary = lief.parse("./snake")
text = binary.get_section(".text")

snake = open("snake", "rb").read()
placeholder = struct.pack("<Q", 0xfa57ca34da91ba36)

n_p_obfs = snake.count(placeholder)
print(n_p_obfs)
assert(i == 41)
snake = snake.replace(placeholder_start, struct.pack("<Q", text.virtual_address))

i = snake.count(placeholder_end)
assert(i == 1)
snake = snake.replace(placeholder_end, struct.pack("<Q", text.virtual_address + text.size))

open("snake", "wb").write(snake)

open("snake_text", "wb").write(text.content.tobytes())

text_hash = hashlib.md5(text.content.tobytes()).digest()
print(hex(text.size))
print(hexlify(text_hash))