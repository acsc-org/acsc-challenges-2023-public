import sys

EMPTY = 0
WALL = 1
BARBED = 2
FRUIT = 3
SNUKE1_HEAD = 4
SNUKE1_BODY = 5
SNUKE2_HEAD = 6
SNUKE2_BODY = 7
SNUKE3_HEAD = 8
SNUKE3_BODY = 9

def main():
  if len(sys.argv) < 2:
    print('Usage: {} stage_file'.format(sys.argv[0]))
    return

  with open(sys.argv[1], 'r') as f:
    s = f.read()

  snuke1 = "123456789"
  snuke2 = "qwertyuio"
  snuke3 = "asdfghjkl"

  ls = s.split('\n')[:-1]
  H = len(ls)
  W = len(ls[0])

  snukes = [snuke1, snuke2, snuke3]
  snuke_heads = [SNUKE1_HEAD, SNUKE2_HEAD, SNUKE3_HEAD]
  snuke_bodys = [SNUKE1_BODY, SNUKE2_BODY, SNUKE3_BODY]

  blocks = []
  available_snukes = {}
  for i, l in enumerate(ls):
    for j, c in enumerate(l[:W].ljust(W, ' ')):
      if c == ' ':
        continue

      if c == '@':
        goaly = i
        goalx = j

      for k in range(3):
        if c == snukes[k][0]:
          blocks.append((i, j, snuke_heads[k], 0))
          if not k in available_snukes:
            available_snukes[k] = []
          available_snukes[k].append(0)
        elif c in snukes[k]:
          blocks.append((i, j, snuke_bodys[k], snukes[k].find(c)))
          if not k in available_snukes:
            available_snukes[k] = []
          available_snukes[k].append(snukes[k].find(c))

      if c == '#':
        blocks.append((i, j, WALL))
      if c == ';':
        blocks.append((i, j, BARBED))
      if c == '$':
        blocks.append((i, j, FRUIT))

  assert available_snukes
  for k in available_snukes:
    available_snukes[k].sort()
    n = len(available_snukes[k])
    assert available_snukes[k] == list(range(n))

  print("{} {} {} {} {} {}".format(H, W, len(available_snukes), len(blocks), goaly, goalx))
  for block in blocks:
    print(" ".join(str(c) for c in block))

if __name__ == '__main__':
  main()
