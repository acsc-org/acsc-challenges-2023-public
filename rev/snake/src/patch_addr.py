import lief
import struct
from binascii import hexlify, unhexlify
import hashlib

binary = lief.parse("./snake")
text = binary.get_section(".text")
seg = next(text.segments)
seg.flags = lief.ELF.SEGMENT_FLAGS(7)   # rwx
binary.write("./snake")

snake = open("snake", "rb").read()
placeholder_start = struct.pack("<Q", 0xdeadbeefbeefdead)
placeholder_end = struct.pack("<Q", 0xcafebabebabecafe)

i = snake.count(placeholder_start)
assert(i == 1)
snake = snake.replace(placeholder_start, struct.pack("<Q", text.virtual_address))

i = snake.count(placeholder_end)
assert(i == 1)
snake = snake.replace(placeholder_end, struct.pack("<Q", text.virtual_address + text.size))

open("snake", "wb").write(snake)

open("snake_text", "wb").write(text.content.tobytes())

text_hash = hashlib.md5(text.content.tobytes()).digest()
print(hex(text.size))
print(hexlify(text_hash))

print("text start:", hex(text.virtual_address))
print("text end:", hex(text.virtual_address + text.size))
print("text size:", hex(text.size))