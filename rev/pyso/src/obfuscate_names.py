import re
import random

cfuncs = open("cfuncs", "rb").readlines()

code = open("libvalidator.py", "rb").read()
names = []

for match in re.finditer(b"def (_.*\().*\):", code):
    name = match.group(1)
    names.append(name)

random.shuffle(names)
random.shuffle(cfuncs)

for i, name in enumerate(names):
    code = code.replace(name, f"_{cfuncs[i].strip().decode()}_(".encode())

open("libvalidator.py", "wb").write(code)
