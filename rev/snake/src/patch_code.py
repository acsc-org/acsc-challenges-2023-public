import struct
import re
import random
import lief

random.seed(832174)

cobf_placeholder = struct.pack("<Q", 0xea57ca34da91ba36)
pobf_placeholder = struct.pack("<Q", 0xfa57ca34da91ba36)

snake = open("snake", "rb").read()

# snake = re.sub(cobf_placeholder, b"\x90"*8, snake)
# snake = re.sub(pobf_placeholder + b"....", b"\x90"*12, snake)

p_ops_txt = open("p_ops.txt", "r").readlines()
p_ops = {}
for p_op_t in p_ops_txt:
    k, v = p_op_t.strip().split(":")
    ops = v.split(";")[:-1]
    p_ops[int(k)] = ops

binary = lief.parse("./snake")
text = binary.get_section(".text")
text_base = text.virtual_address
text_code = text.content.tobytes()

p_indices = []
idx = 0
while True:
    idx = snake.find(pobf_placeholder, idx)
    if idx == -1:
        break
    p_indices.append(idx)
    idx += 1

p_indices = p_indices[::-1]

for idx in p_indices:
    addr_key = struct.unpack("<I", snake[idx+8:idx+12])[0]
    key_idx = snake.find(b"\xba" + snake[idx+8:idx+12]) - 4
    key = struct.unpack("<I", snake[key_idx:key_idx+4])[0]
    assert(key >= 0x2000 and key < 0x3000)
    print(hex(addr_key), hex(key))
    ops = p_ops[key-0x2000]

    addr_in_text = text_base + text_code.find(pobf_placeholder + snake[idx+8:idx+12])
    xored_addr = addr_key ^ addr_in_text

    snake_l = list(snake)

    snake_l[key_idx+5:key_idx+9] = struct.pack("<I", xored_addr)

    for i, op in enumerate(ops):
        m, c = list(map(int, op.split(",")))

        if i < 12:
            code = 0x90
        else:
            code = snake[idx+i]

        if m == 0:
            code ^= c
        else:
            code = (code - c) % 0x100

        snake_l[idx+i] = code

    snake = bytes(snake_l)

c_ops_txt = open("c_ops.txt", "r").readlines()
c_ops = {}
for c_op_t in c_ops_txt:
    k, v = c_op_t.strip().split(":")
    ops = v.split(";")[:-1]
    c_ops[int(k)] = ops

idx = 0
while True:
    idx = snake.find(cobf_placeholder, idx)
    if idx == -1:
        break
    key = struct.unpack("<I", snake[idx-4:idx])[0]
    ops = c_ops[key-0x1000]

    snake_l = list(snake)

    snake_l[idx:idx+8] = [0x0f, 0x0b] + [random.randint(0, 0xff) for _ in range(6)]

    for i, op in enumerate(ops):
        m, c = list(map(int, op.split(",")))
        code = struct.unpack("<Q", snake[idx+8*(i+1):idx+8*(i+2)])[0]

        if m == 0:
            code ^= c
        else:
            code = (code - c) % (2**64)

        snake_l[idx+8*(i+1):idx+8*(i+2)] = struct.pack("<Q", code)

    snake = bytes(snake_l)

    idx += 1

open("snake", "wb").write(snake)