/*
 *  g++ snukebird.cpp -std=c++17 -g -o snukebird
 */

#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cassert>
#include <unistd.h>
#include <vector>
#include <deque>
#include <utility>
using namespace std;

const int NUM_STAGE = 3;
const int NUM_SNUKE = 3;

enum block_type {
  EMPTY = 0,
  WALL,
  BARBED,
  FRUIT,
  SNUKE1_HEAD,
  SNUKE1_BODY,
  SNUKE2_HEAD,
  SNUKE2_BODY,
  SNUKE3_HEAD,
  SNUKE3_BODY,
};

using Coor = pair<int, int>;

const int dy[] = {-1, 0, 1, 0};
const int dx[] = {0, -1, 0, 1};

int H;
int W;
int num_snukes;
int num_remain;
int num_fruits;
int fruits_collected;
int goaly;
int goalx;
block_type field[100][100];
deque<Coor> snuke_pos[NUM_SNUKE];
bool reached_goal[NUM_SNUKE];
bool solved[NUM_STAGE+1];

void handler(int num) {
  puts("Time is up!");
  exit(0);
}

bool is_snuke_head(block_type b) {
  return b == SNUKE1_HEAD || b == SNUKE2_HEAD || b == SNUKE3_HEAD;
}

bool is_snuke_body(block_type b) {
  return b == SNUKE1_BODY || b == SNUKE2_BODY || b == SNUKE3_BODY;
}

bool is_snuke(block_type b) {
  return is_snuke_head(b) || is_snuke_body(b);
}

block_type get_body_for_head(block_type b) {
  assert(is_snuke_head(b));

  block_type ret = static_cast<block_type>(b+1);
  assert(is_snuke_body(ret));
  return ret;
}

int get_snuke_idx(block_type b) {
  if (b == SNUKE1_HEAD || b == SNUKE1_BODY) return 0;
  if (b == SNUKE2_HEAD || b == SNUKE2_BODY) return 1;
  if (b == SNUKE3_HEAD || b == SNUKE3_BODY) return 2;
  return -1;
}

void load_stage_information(int idx) {
  char fname[100];
  snprintf(fname, 50, "stage%02d.txt", idx);

  FILE *fp = fopen(fname, "rb");
  if (fp == NULL) {
    puts("[*] Something went wrong... Please contact the admin if you are encountring this repeatedly.");
    exit(-1);
  }

  int num_blocks;
  fscanf(fp, "%d%d%d%d%d%d", &H, &W, &num_snukes, &num_blocks, &goaly, &goalx);

  for (int i=0; i<num_snukes; i++) {
    snuke_pos[i].clear();
  }

  num_fruits = 0;
  for (int i=0; i<num_blocks; i++) {
    int y, x, bint;
    block_type b;
    fscanf(fp, "%d%d%d", &y, &x, &bint);
    b = block_type(bint);

    field[y][x] = b;

    if (b == FRUIT) num_fruits++;
    else if (is_snuke(b)) {
      int body_idx = -1;
      fscanf(fp, "%d", &body_idx);
      assert(body_idx >= 0);
      if (is_snuke_head(b)) assert(body_idx == 0);
      
      int idx = get_snuke_idx(b);
      if (snuke_pos[idx].size() <= body_idx) snuke_pos[idx].resize(body_idx+1);
      snuke_pos[idx][body_idx] = Coor(y, x);
    }
  }

  assert(field[goaly][goalx] == EMPTY);
}

void show_field(void) {
  for (int i=0; i<H; i++) {
    for (int j=0; j<W; j++) {
      block_type b = field[i][j];

      char c;
      const char *color;
      if (b == WALL) {
        c = '#';
        color = "";
      } else if (b == BARBED) {
        c = ';';
        color = "";
      } else if (b == SNUKE1_HEAD) {
        c = '1';
        color = "\e[31m";
      } else if (b == SNUKE1_BODY) {
        c = 'O';
        color = "\e[31m";
      } else if (b == SNUKE2_HEAD) {
        c = '2';
        color = "\e[34m";
      } else if (b == SNUKE2_BODY) {
        c = 'O';
        color = "\e[34m";
      } else if (b == SNUKE3_HEAD) {
        c = '3';
        color = "\e[32m";
      } else if (b == SNUKE3_BODY) {
        c = 'O';
        color = "\e[32m";
      } else if (b == FRUIT) {
        c = '$';
        color = "\e[33m";
      } else if (i == goaly && j == goalx) {
        color = "";
        if (fruits_collected == num_fruits) {
          c = '@';
        } else {
          c = '*';
        }
      } else {
        c = ' ';
        color = "";
      }

      printf("%s%c\e[0m", color, c);
    }

    puts("");
  }

  puts("");
}

void finish_stage(int stage_idx, bool clear) {
  if (clear) {
    puts("Well done!");
    solved[stage_idx] = true;
  } else {
    show_field();
    puts("Game Over...");
  }

  for (int i=0; i<H; i++) {
    for (int j=0; j<W; j++) {
      field[i][j] = EMPTY;
    }
  }
}

bool check_goal(int snuke_idx) {
  if (reached_goal[snuke_idx]) return false;

  if (fruits_collected == num_fruits && snuke_pos[snuke_idx][0] == Coor(goaly, goalx)) {
    for (auto [y, x] : snuke_pos[snuke_idx]) {
      field[y][x] = EMPTY;
    }
    snuke_pos[snuke_idx].clear();

    reached_goal[snuke_idx] = true;
    num_remain--;
  }

  return reached_goal[snuke_idx];
}

bool parallel_shift(int snuke_idx, char dir) {
  auto [cury, curx] = snuke_pos[snuke_idx][0];
  block_type head_val = field[cury][curx];
  block_type body_val = get_body_for_head(head_val);

  bool bumped = false;

  deque<Coor> nxt_pos;
  for (auto [y, x] : snuke_pos[snuke_idx]) {
    int ny = y + dy[dir];
    int nx = x + dx[dir];

    if (field[ny][nx] != EMPTY &&
        get_snuke_idx(field[ny][nx]) != snuke_idx) {

      bumped = true;
      break;
    }
    
    nxt_pos.push_back(Coor(ny, nx));
  }

  if (bumped) return false;

  for (auto [y, x] : snuke_pos[snuke_idx]) {
    field[y][x] = EMPTY;
  }

  for (auto [y, x] : nxt_pos) {
    field[y][x] = body_val;
  }

  auto [nheady, nheadx] = nxt_pos[0];
  field[nheady][nheadx] = head_val;

  snuke_pos[snuke_idx].swap(nxt_pos);

  return true;
}

int fall_down(int snuke_idx) {
  int dist_moved = 0;

  while (1) {
    bool moved = parallel_shift(snuke_idx, 2);
    if (!moved) break;
    dist_moved++;

    if (check_goal(snuke_idx)) break;

    bool gameover = true;
    for (auto [y, x] : snuke_pos[snuke_idx]) {
      if (0 <= y && y < H && 0 <= x && x < W) gameover = false;
    }

    if (gameover) return -1;
  }

  return dist_moved;
}

bool try_shift(int snuke_idx, char dir, vector<char>& tried) {
  if (tried[snuke_idx]) return false;
  tried[snuke_idx] = true;

  for (auto [y, x] : snuke_pos[snuke_idx]) {
    int ny = y + dy[dir];
    int nx = x + dx[dir];

    if (is_snuke(field[ny][nx])) {
      int bumped_snuke_idx = get_snuke_idx(field[ny][nx]);
      if (bumped_snuke_idx != snuke_idx) {
        if (try_shift(bumped_snuke_idx, dir, tried)) return false;
      }
    } else if (field[ny][nx] != EMPTY) {
      return false;
    }
  }

  bool res = parallel_shift(snuke_idx, dir);
  assert(res);
  return true;
}

void try_move(int snuke_idx, char dir) {
  int len_snuke = snuke_pos[snuke_idx].size();
  assert(len_snuke > 0);

  auto [cury, curx] = snuke_pos[snuke_idx][0];
  block_type head_val = field[cury][curx];
  block_type body_val = get_body_for_head(head_val);

  int nxty = cury + dy[dir];
  int nxtx = curx + dx[dir];

  if (is_snuke(field[nxty][nxtx]) && get_snuke_idx(field[nxty][nxtx]) == snuke_idx) return;

  bool should_restore_tail = false;
  auto [taily, tailx] = snuke_pos[snuke_idx].back();
  snuke_pos[snuke_idx].pop_back();
  field[taily][tailx] = EMPTY;

  if (is_snuke(field[nxty][nxtx])) {
    int bumped_snuke_idx = get_snuke_idx(field[nxty][nxtx]);
    vector<char> tried(num_snukes, 0);
    try_shift(bumped_snuke_idx, dir, tried);
  }

  if (field[nxty][nxtx] == FRUIT || field[nxty][nxtx] == EMPTY) {
    bool is_fruit = field[nxty][nxtx] == FRUIT;

    snuke_pos[snuke_idx].push_front(Coor(nxty, nxtx));
    field[nxty][nxtx] = head_val;
    if (len_snuke > 1) field[cury][curx] = body_val;

    if (is_fruit) {
      fruits_collected++;
      should_restore_tail = true;
    }
  } else {
    should_restore_tail = true;
  }

  if (should_restore_tail) {
    field[taily][tailx] = snuke_pos[snuke_idx].size() >= 1 ? body_val : head_val;
    snuke_pos[snuke_idx].push_back(Coor(taily, tailx));
  }
}

void play_stage(int stage_idx) {
  load_stage_information(stage_idx);

  printf("=========== Stage %d ===========\n", stage_idx);
  printf("Control and take %d snukebirds to the goal!\n\n", num_snukes);
  puts("Commands:");
  puts(" wN -- move up the N-th snukebird");
  puts(" aN -- move left the N-th snukebird");
  puts(" sN -- move down the N-th snukebird");
  puts(" dN -- move right the N-th snukebird");
  puts(" q -- quit the stage\n");

  fruits_collected = 0;
  num_remain = num_snukes;
  fill(reached_goal, reached_goal+num_snukes, false);

  while (num_remain > 0) {
    show_field();

    char cmd = ' ';
    scanf(" %c", &cmd);

    if (cmd == 'q') break;

    int snuke_idx = -1;
    scanf("%d", &snuke_idx);
    snuke_idx--;

    if (snuke_idx < 0 || num_snukes <= snuke_idx) {
      puts("Idiot");
      exit(0);
    }

    char dir;
    switch (cmd) {
    case 'w':
      dir = 0;
      break;
    case 'a':
      dir = 1;
      break;
    case 's':
      dir = 2;
      break;
    case 'd':
      dir = 3;
      break;
    default:
      puts("Idiot");
      exit(0);
    }

    if (reached_goal[snuke_idx]) continue;

    try_move(snuke_idx, dir);

    for (int i=0; i<num_snukes; i++) {
      check_goal(i);
    }

    while (1) {
      int num_stopped = 0;
      for (int i=0; i<num_snukes; i++) {
        if (reached_goal[i]) continue;

        int dist_moved = fall_down(i);
        if (dist_moved == -1) {
          finish_stage(stage_idx, false);
          return;
        }

        num_stopped += dist_moved == 0;
      }

      if (num_stopped == num_remain) break;
    }

    for (int i=0; i<num_snukes; i++) {
      if (reached_goal[i]) continue;

      int barbed_count = 0;
      for (auto [y, x] : snuke_pos[i]) {
        int ny = y + dy[2];
        int nx = x + dx[2];
  
        if (field[ny][nx] == BARBED || field[ny][nx] == EMPTY) barbed_count++;
      }
  
      if (barbed_count == snuke_pos[i].size()) {
        finish_stage(stage_idx, false);
        return;
      }
    }
  }

  finish_stage(stage_idx, num_remain == 0);
}

void show_banner() {
  puts("                                                                               ");
  puts(" _______  __    _  __   __  ___   _  _______  _______  ___   ______    ______  ");
  puts("|       ||  |  | ||  | |  ||   | | ||       ||  _    ||   | |    _ |  |      | ");
  puts("|  _____||   |_| ||  | |  ||   |_| ||    ___|| |_|   ||   | |   | ||  |  _    |");
  puts("| |_____ |       ||  |_|  ||      _||   |___ |       ||   | |   |_||_ | | |   |");
  puts("|_____  ||  _    ||       ||     |_ |    ___||  _   | |   | |    __  || |_|   |");
  puts(" _____| || | |   ||       ||    _  ||   |___ | |_|   ||   | |   |  | ||       |");
  puts("|_______||_|  |__||_______||___| |_||_______||_______||___| |___|  |_||______| ");
  puts("                                                                               ");
}

int main() {
  setbuf(stdout, NULL);

  if (signal(SIGALRM, handler) == SIG_ERR) {
    puts("[*] Something went wrong... Please contact the admin if you are encountring this repeatedly.");
    exit(-1);
  }

  show_banner();

  alarm(120);
  while (1) {
    int stage_idx;
    printf("Select stage: ");
    scanf("%d", &stage_idx);
    if (stage_idx < 1 || NUM_STAGE < stage_idx) {
      puts("Idiot");
      exit(0);
    }

    play_stage(stage_idx);

    int num_solved = 0;
    for (int i=1; i<=NUM_STAGE; i++) {
      num_solved += solved[i];
    }
    if (num_solved == NUM_STAGE) break;
  }

  puts("Congratulations!");

  FILE *fp = fopen("./flag.txt", "rb");
  if (!fp) {
    puts("[*] Something went wrong... Please contact the admin if you are encountring this repeatedly.");
    exit(-1);
  }
  
  char flag[100];
  fgets(flag, sizeof(flag), fp);
  printf("Here is the flag: %s", flag);
  fclose(fp);
}
