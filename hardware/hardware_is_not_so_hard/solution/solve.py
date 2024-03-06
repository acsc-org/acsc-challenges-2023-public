with open('spi.txt') as f:
    lines = f.readlines()

flag = [None] * 64
for i, l in enumerate(lines):
    desc, data = l.split(' : ')

    # CMD17: READ_SINGLE_BLOCK
    if data.startswith('51'):
        req = bytes.fromhex(data)
        rsp = bytes.fromhex(lines[i + 2].split(' : ')[1])

        addr = req[4]
        block = rsp[rsp.index(b'\xfe') + 1:-2]
        flag[addr] = block

with open('flag.bin', 'wb') as f:
    for block in flag:
        f.write(block)
