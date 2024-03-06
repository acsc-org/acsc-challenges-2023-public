import random

random.seed(297834902)

snake_template = open(f"snake.c.template", "rb").read()
rng_template = open(f"rng.c.template", "rb").read()
md5_template = open(f"md5.c.template", "rb").read()
p_obf_placeholder = b'P_OBF("0x31337")'
c_obf_placeholder = b'C_OBF("0x31337")'
c_dobf_placeholder = b'C_DOBF(A, B, C)'

parent_obfs = {}
child_obfs = {}

def ensure_enough_obf_pairs():
    n_p_obfs = 0
    n_p_obfs += snake_template.count(p_obf_placeholder)
    n_p_obfs += md5_template.count(p_obf_placeholder)

    n_dobfs = snake_template.count(c_dobf_placeholder)
    n_dobfs += rng_template.count(c_dobf_placeholder)

    print("total n_p_obfs:", n_p_obfs)
    print("n_dobfs:", n_dobfs)
    assert(n_p_obfs == n_dobfs)

def patch_pobf(template):
    n_p_obfs = template.count(p_obf_placeholder)
    print("num p obfs:", n_p_obfs)

    for _ in range(n_p_obfs):
        key = random.randint(0, 0xff)
        while key in parent_obfs.keys():
            key = random.randint(0, 0xff)
        mode = random.randint(0, 2)     # +, -, ^
        width = random.randint(4, 8)
        const = random.randint(1, 0xff)
        addr_key = random.randint(0x111111, 0xffffff)

        parent_obfs[key] = (mode, width, const, addr_key)

        new_pobf = f'P_OBF("{addr_key:#x}")'.encode()
        template = template.replace(p_obf_placeholder, new_pobf, 1)

    return template

def patch_dobfs(templates):
    idx = 0
    pobfs = list(parent_obfs.items())
    res = []
    for template in templates:
        n_dobfs = template.count(c_dobf_placeholder)
        for _ in range(n_dobfs):
            key, v = pobfs[idx]
            idx += 1
            _, _, const, addr_key = v
            new_c_dobf = f'C_DOBF({key+0x2000:#x}, {addr_key:#x}, {const:#x})'.encode()
            template = template.replace(c_dobf_placeholder, new_c_dobf, 1)
        res.append(template)
    return res

def patch_cobf(template):
    n_c_obfs = template.count(c_obf_placeholder)
    print("num c obfs:", n_c_obfs)

    for _ in range(n_c_obfs):
        key = random.randint(0, 0x3f)
        while key in child_obfs.keys():
            key = random.randint(0, 0x3f)
        mode = random.randint(0, 2)     # +, -, ^
        width = random.randint(8, 16)
        const = random.randint(1, 0xff)

        child_obfs[key] = (mode, width, const)

        new_cobf = f'C_OBF({key+0x1000:#x})'.encode()
        template = template.replace(c_obf_placeholder, new_cobf, 1)

    return template

ensure_enough_obf_pairs()

snake_template = patch_pobf(snake_template)
md5_template = patch_pobf(md5_template)

snake_template = patch_cobf(snake_template)
rng_template = patch_cobf(rng_template)

snake_template, rng_template = patch_dobfs((snake_template, rng_template))

# generate tracer code to deob code
multi_tracer_code = ""

for i in range(0x100):
    multi_tracer_code += f"void multi_tracer_c{i}(struct user_regs_struct *regs);\n"
for i in range(0x100):
    multi_tracer_code += f"void multi_tracer_p{i}(struct user_regs_struct *regs);\n"

## for parent
keys_p = [i for i in range(0x100)]
random.shuffle(keys_p)

p_ops = {}

for i, k in enumerate(keys_p):
    p_ops[k] = []

    fn = f"void multi_tracer_p{k}(struct user_regs_struct *regs)\n" + "{\n"
    fn += f"if (regs->rax != {k+0x2000:#x})\n" + "{\n"
    if i != len(keys_p)-1:
        fn += f"multi_tracer_p{keys_p[i+1]}(regs);"
    fn += f"return;"
    fn += "\n}\n"

    # clear the ud2 on the child, only want to deob once
    fn += "uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);\n"
    fn += "code &= 0xffffffffffff0000;\n"
    fn += "code |= 0x9090;\n"
    fn += "ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);\n"

    # calculate address to patch
    addr_key = parent_obfs[k][3] if k in parent_obfs.keys() else random.randint(0x111111, 0xffffff);

    fn += f"char* ptr = (char*)({addr_key:#x} ^ regs->rbx);\n"

    c = random.randint(6, 16)
    for i in range(12+c):
        mode = random.randint(0, 1)
        const = random.randint(0x1, 0xff)
        ops = ["^", "+"]
        fn += f"ptr[{i}] {ops[mode]}= {const:#x};\n"

        p_ops[k].append((mode, const))

    fn += "\n}\n\n"

    multi_tracer_code += fn

## for child
keys_c = [i for i in range(0x100)]
random.shuffle(keys_c)

c_ops = {}

for i, k in enumerate(keys_c):
    c_ops[k] = []

    fn = f"void multi_tracer_c{k}(struct user_regs_struct *regs)\n" + "{\n"
    fn += f"if (regs->rax != {k+0x1000:#x})\n" + "{\n"
    if i != len(keys_c)-1:
        fn += f"multi_tracer_c{keys_c[i+1]}(regs);"
    else:
        fn += f"multi_tracer_p{keys_p[0]}(regs);"
    fn += f"return;"
    fn += "\n}\n"

    c = random.randint(1, 2)
    fn += "uint64_t code;\n"
    fn += "code = 0x9090909090909090;\n"
    fn += "ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);\n"
    fn += "code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);\n"
    for i in range(1, c+1):
        fn += f"code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+{i*8}, NULL);\n"
        mode = random.randint(0, 1)
        const = random.randint(0x1111111111111111, 0xffffffffffffffff)
        ops = ["^", "+"]
        fn += f"code {ops[mode]}= {const:#x};\n"
        fn += f"ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+{i*8}, code);\n"

        c_ops[k].append((mode, const))

    fn += "\n}\n\n"

    multi_tracer_code += fn

with open("p_ops.txt", "w") as out:
    for k, v in p_ops.items():
        out.write(f"{k}:")
        for mode, const in v:
            out.write(f"{mode},{const};")
        out.write("\n")

with open("c_ops.txt", "w") as out:
    for k, v in c_ops.items():
        out.write(f"{k}:")
        for mode, const in v:
            out.write(f"{mode},{const};")
        out.write("\n")


multi_tracer_code += f"void multi_tracer(struct user_regs_struct *regs)\n{{ multi_tracer_c{keys_c[0]}(regs); }}\n"
snake_template = snake_template.replace(b"void multi_tracer(struct user_regs_struct *regs) { }", multi_tracer_code.encode())

open("snake.c", "wb").write(snake_template)
open("rng.c", "wb").write(rng_template)
open("md5.c", "wb").write(md5_template)