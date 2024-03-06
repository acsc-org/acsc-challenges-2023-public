import random
target = [141, 109, 174, 83, 181, 39, 15, 49, 226, 110, 18, 245, 75, 130, 20, 68, 232, 199, 32, 239, 33, 12, 61, 27, 22, 132, 145, 245, 137, 209, 27, 134, 138, 239, 173, 242, 218, 203, 88, 210, 176, 81, 42, 99, 147, 198, 231, 176, 17, 153, 149, 243, 53, 14, 209, 88, 132, 124, 85, 81, 183, 207, 167, 87]
ops = [i % 4 for i in range(64)]
random.shuffle(ops)

for i in range(64):
    y = target[i]
    ny = (256 - y) % 256
    print(f"""@njit('u1(u1,)', inline='never', debug=True)
def _cmp{i}(x):""")

    r = ops[i]
    if r == 0:
        print(
f"""    return x ^ {y}
""")
    elif r == 1:
        print(
f"""    return (x | {ny}) + {ny} - (~x & {ny})
""")
    elif r == 2:
        print(
f"""    return ((x | {y}) - (x & {y}))
""")
    elif r == 3:
        print(
f"""    return x - {y}
""")

for i in range(4):
    print(
f"""@njit('(u1[:],)', inline='never', debug=True)
def _cmp_part{i}(P):
    a = 0""")
    for j in range(i*16, (i+1)*16):
        print(f"    a |= _cmp{j}(P[{j}])")

    print("    return a")