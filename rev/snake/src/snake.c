#include <time.h>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include "md5.h"
#include "rng.h"

/* PARENT */
unsigned int SEED;
pid_t CHILD_PID;
MD5Context ctx;

unsigned long TEXT_START_ADDR = 0xdeadbeefbeefdead;
unsigned long TEXT_END_ADDR = 0xcafebabebabecafe;

#define SERVER_HOST "snake.chal.ctf.acsc.asia"
#define SERVER_PORT "4444"
#define SA struct sockaddr

/* CHILD */
#define SEND_FOOD(x, y)                           \
 __asm__ __volatile__(  "" :: "b"(x), "c"(y)  )
#define SEND_SCORE()                           \
 __asm__ __volatile__(  "" :: "b"(score), "c"(maxX), "d"(maxY)  )
#define UD2(R)                           \
 __asm__ __volatile__(  ".byte 0x00,0x00\n" :: "a"(R)  )
#define C_OBF(R)                           \
 __asm__ __volatile__(  ".quad 0xea57ca34da91ba36\n" :: "a"(R)  )
#define C_DOBF(R, addr_key, noop)                           \
 __asm__ __volatile__(  ".byte 0x00,0x00\n" :: "a"(R), "b"(addr_key), "c"(noop)  )
#define P_OBF(K)                           \
 __asm__ __volatile__(  ".quad 0xfa57ca34da91ba36\n.long " K "\n" )
#define PAD()                           \
 __asm__ __volatile__(  ".byte 0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90"  )


#define DELAY 20000
#define TIMEOUT 10
#define STARTING_TAIL_LEN 3

typedef enum {
    LEFT, RIGHT, UP, DOWN
} direction_type;

typedef struct point {
    int x;
    int y;
} point;

int maxX = 0, maxY = 0;
int tail_len = STARTING_TAIL_LEN;
int score = 0;

direction_type current_dir = RIGHT;
point snake_parts[256] = {};
point food;

void create_food()
{
    C_OBF(0x100f);
    C_DOBF(0x20f0, 0x737aee, 0x47);

    food.x = genrand64_int64() % (maxX / 2);
    food.y = genrand64_int64() % maxY;
    PAD();
}

void init_curses()
{
    initscr();
    clear();
    getmaxyx(stdscr, maxY, maxX);
    C_DOBF(0x2013, 0x83c85a, 0xbc);
    if (maxX < 32 || maxY < 32)
    {
        mvprintw(0, 0, "Your screen is too small...");
        mvprintw(1, 0, "Expected width>=48, height>=48");
        mvprintw(2, 0, "Actual   width=%d, height=%d", maxX, maxY);
        refresh();
        exit(1);
    }

    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    timeout(TIMEOUT);
    curs_set(FALSE);
    refresh();
}

void cleanup_curses()
{
    echo();
    curs_set(TRUE);
    nocbreak();
    endwin();
}

void init_game()
{
    UD2(1);
    unsigned int seed;
    asm("movl %%eax, %0" : "=r"(seed) :);
    // mvprintw(0, 0, "seed: %d\n", seed);
    // refresh();
    // exit(0);
    C_OBF(0x1007);
    C_DOBF(0x201a, 0x870480, 0xdc);
    init_genrand64(seed);
    PAD();

    current_dir = RIGHT;
    tail_len = STARTING_TAIL_LEN;
    score = 0;

    C_OBF(0x102c);
    C_DOBF(0x20f7, 0x519234, 0xcc);
    int j = 0;
    PAD();
    for (int i = tail_len; i >= 0; --i)
    {
        point curr_point;
        curr_point.x = i * 2;
        curr_point.y = maxY / 2;
        snake_parts[j++] = curr_point;
    }

    C_OBF(0x1029);
    C_DOBF(0x20ac, 0xf225b3, 0x38);
    create_food();
    PAD();
}

void shift_snake(int next_x, int next_y)
{
    C_OBF(0x1018);
    C_DOBF(0x200d, 0xcd3967, 0xf0);
    PAD();
    for (int i = tail_len; i > 0; --i)
    {
        snake_parts[i] = snake_parts[i-1];
    }
    C_OBF(0x1000);
    C_DOBF(0x2081, 0xc98a4e, 0xde);
    snake_parts[0] = (point){.x=next_x, .y=next_y};
    PAD();
}

void extend_snake(int next_x, int next_y)
{
    C_OBF(0x1030);
    C_DOBF(0x2005, 0x9fbb01, 0xd4);
    if (tail_len == 255) return;
    tail_len++;
    PAD();
    for (int i = tail_len; i > 0; --i)
    {
        snake_parts[i] = snake_parts[i-1];
    }
    C_OBF(0x1036);
    C_DOBF(0x2071, 0x48ca6e, 0x80);
    snake_parts[0] = (point){.x=next_x, .y=next_y};
    PAD();
}

void draw_food()
{
    C_OBF(0x1014);
    C_DOBF(0x2008, 0x9c0d70, 0x96);
    mvprintw(food.y, food.x * 2, "x");
    PAD();
}

void draw_snake()
{
    C_OBF(0x102e);
    C_DOBF(0x201c, 0xe903a2, 0x63);
    mvprintw(snake_parts[0].y, snake_parts[0].x * 2, "* ");
    PAD();
    for (int i = 1; i <= tail_len; ++i)
        mvprintw(snake_parts[i].y, snake_parts[i].x * 2, "o ");
}

void draw_score()
{
    mvprintw(1, 2, "Score: %d", score);
    PAD();
}

void draw_game()
{
    clear();
    draw_food();
    draw_snake();
    refresh();
}

void end()
{
    cleanup_curses();
    puts("GG");
    printf("Score: %d\n\n", score);
    // puts("Submitting score to server...\n");
    fflush(stdout);
}

void game()
{
    init_curses();
    init_game();

    int ch;
    while(1) {
        UD2(2);
        ch = getch();

        C_OBF(0x103f);
        C_DOBF(0x207b, 0xeb900d, 0xc5);
        PAD();
        if (ch == 'q')
            break;

        C_OBF(0x1015);
        C_DOBF(0x2038, 0x42f5a7, 0xa4);
        PAD();
        if(( ch=='d' || ch == 'D' || ch=='l' || ch=='L' || ch == KEY_RIGHT) && current_dir != LEFT)
        {
            C_OBF(0x1010);
            current_dir = RIGHT;
            PAD();
        }
        else if (( ch=='a' || ch == 'A' ||  ch=='h' || ch=='H' || ch == KEY_LEFT) && current_dir != RIGHT)
        {
            C_OBF(0x1003);
            current_dir = LEFT;
            PAD();
        }
        else if(( ch=='s' || ch == 'S' || ch=='j' || ch=='J' || ch == KEY_DOWN) && current_dir != UP)
        {
            C_OBF(0x1002);
            current_dir = DOWN;
            PAD();
        }
        else if(( ch=='w' || ch == 'W' || ch=='k' || ch=='K' || ch == KEY_UP) && current_dir != DOWN)
        {
            C_OBF(0x1028);
            current_dir = UP;
            PAD();
        }

        C_OBF(0x1031);
        C_DOBF(0x2003, 0x92a234, 0x7b);
        PAD();
        PAD();
        C_OBF(0x101f);
        C_DOBF(0x20a4, 0xe44b43, 0xbf);
        PAD();
        PAD();
        C_OBF(0x103a);
        C_DOBF(0x205f, 0xec70a2, 0x37);
        PAD();
        PAD();
        C_OBF(0x1037);
        C_DOBF(0x20ca, 0xc09226, 0x77);
        PAD();
        PAD();

        C_OBF(0x103e);
        C_DOBF(0x20aa, 0x3f8935, 0xec);
        int next_x = snake_parts[0].x;
        PAD();

        C_OBF(0x103c);
        C_DOBF(0x2035, 0x85c92c, 0xc4);
        int next_y = snake_parts[0].y;
        PAD();

        if(current_dir == RIGHT)
        {
            C_OBF(0x102a);
            next_x = ((next_x + 1 + maxX/2) % (maxX / 2));
            PAD();
        }
        else if(current_dir == LEFT)
        {
            C_OBF(0x102d);
            next_x = ((next_x - 1 + maxX/2) % (maxX / 2));
            PAD();
        }
        else if(current_dir == UP)
        {
            C_OBF(0x100e);
            next_y = (next_y - 1 + maxY) % maxY;
            PAD();
        }
        else if(current_dir == DOWN)
        {
            C_OBF(0x1033);
            next_y = (next_y + 1 + maxY) % maxY;
            PAD();
        }

        C_OBF(0x1001);
        C_DOBF(0x20ae, 0x83d4a3, 0xf5);
        PAD();
        PAD();
        C_OBF(0x1034);
        C_DOBF(0x20c4, 0xad8587, 0xd9);
        PAD();
        PAD();
        C_OBF(0x1017);
        C_DOBF(0x20f8, 0xbb7002, 0xdc);
        int collide = 0;
        PAD();
        for (int i = 1; i <= tail_len; ++i)
            if (snake_parts[i].x == next_x && snake_parts[i].y == next_y)
                collide = 1;
        if (collide) break;

        C_OBF(0x100c);
        PAD();
        if (food.x == next_x && food.y == next_y)
        {
            SEND_FOOD(next_x, next_y);
            UD2(3);
            extend_snake(next_x, next_y);
            score++;
            create_food();
        }
        else {
            shift_snake(next_x, next_y);
        }

        draw_game();
    }

    end();
    SEND_SCORE();
    UD2(4);
}

/** PARENT */

void init_md5()
{
    P_OBF("0x737aee");
    md5Init(&ctx);
}

void update_hash_with_seed()
{
    // puts("TODO: update hash with SEED");
    P_OBF("0x83c85a");
    md5Update(&ctx, (uint8_t*)&SEED, sizeof(SEED));
}

void update_hash_with_food(point food)
{
    // puts("TODO: update hash with food");
    P_OBF("0x870480");
    md5Update(&ctx, (uint8_t*)&food, sizeof(point));
}

void print_bytes(void *p, size_t length){
    P_OBF("0x519234");
	uint8_t *pp = (uint8_t *)p;
	for(unsigned int i = 0; i < length; ++i){
		if(i && !(i % 16)){
			printf("\n");
		}
		printf("%02X ", pp[i]);
	}
	printf("\n");
}

void print_hash(uint8_t *p){
    P_OBF("0xf225b3");
	for(unsigned int i = 0; i < 16; ++i){
		printf("%02x", p[i]);
	}
	printf("\n");
}

void finalize_hash(uint64_t rip)
{
    P_OBF("0xcd3967");
    int text_size = TEXT_END_ADDR - TEXT_START_ADDR;
    uint8_t* text = (uint8_t*)malloc(text_size);
    PAD();
    PAD();
    PAD();
    PAD();
    P_OBF("0xc98a4e");
    PAD();
    PAD();
    PAD();
    PAD();
    P_OBF("0x9fbb01");
    PAD();
    PAD();
    PAD();
    PAD();
    for (unsigned int addr = TEXT_START_ADDR; addr < TEXT_END_ADDR; addr++)
    {
        int i = addr - TEXT_START_ADDR;
        uint8_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, addr, NULL) & 0xff;
        text[i] = code;
    }
    P_OBF("0x48ca6e");
    PAD();
    PAD();
    PAD();
    PAD();
    md5Update(&ctx, text, text_size);

    P_OBF("0x9c0d70");
    PAD();
    PAD();
    md5Finalize(&ctx);
    // print_hash(ctx.digest);
    // print_bytes(text, text_size);

    PAD();
    PAD();
    PAD();
    free(text);
}

void submit_score(int score, int maxX, int maxY)
{
    // puts("TODO: parent submit score to server");
    struct submission
    {
        unsigned int seed;
        int maxX, maxY;
        int score;
        uint8_t digest[16];
    } submission;
    submission.seed = SEED;
    submission.maxX = maxX;
    submission.maxY = maxY;
    submission.score = score;
    memcpy(submission.digest, ctx.digest, 16);

    P_OBF("0xe903a2");

    // getaddrinfo
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    P_OBF("0xeb900d");
    int rc = getaddrinfo(SERVER_HOST, SERVER_PORT, &hints, &res);
    if (rc != 0)
    {
        exit(0);
    }

    // socket stuff
    int sockfd, connfd;
    struct sockaddr_in servaddr;

    P_OBF("0x42f5a7");
    bzero(&servaddr, sizeof(servaddr));
    memcpy(&servaddr, res->ai_addr, sizeof(servaddr));
    freeaddrinfo(res);

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        // printf("socket creation failed...\n");
        exit(0);
    }
    // else
    //     printf("Socket successfully created..\n");

    // assign IP, PORT
    // servaddr.sin_family = AF_INET;
    // servaddr.sin_addr.s_addr = inet_addr(SERVER_HOST);
    // servaddr.sin_port = htons(SERVER_PORT);

    P_OBF("0x92a234");
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        // printf("connection with the server failed...\n");
        exit(0);
    }
    // else
    //     printf("connected to the server..\n");

    // send to server
    char response[512];
    // printf("submission size: %d\n", sizeof(submission));
    P_OBF("0xe44b43");
    write(sockfd, &submission, sizeof(submission));
    P_OBF("0xec70a2");
    bzero(response, sizeof(response));
    P_OBF("0xc09226");
    read(sockfd, response, sizeof(response));
    P_OBF("0x3f8935");
    puts(response);

    // close the socket
    close(sockfd);
    PAD();
}

void sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        kill(CHILD_PID, SIGINT);
        exit(0);
    }
}

void skip_ud2_and_continue(struct user_regs_struct *regs)
{
    regs->rip += 2;
    ptrace(PTRACE_SETREGS, CHILD_PID, NULL, regs);
    ptrace(PTRACE_CONT, CHILD_PID, NULL, NULL);
}

void multi_tracer_c0(struct user_regs_struct *regs);
void multi_tracer_c1(struct user_regs_struct *regs);
void multi_tracer_c2(struct user_regs_struct *regs);
void multi_tracer_c3(struct user_regs_struct *regs);
void multi_tracer_c4(struct user_regs_struct *regs);
void multi_tracer_c5(struct user_regs_struct *regs);
void multi_tracer_c6(struct user_regs_struct *regs);
void multi_tracer_c7(struct user_regs_struct *regs);
void multi_tracer_c8(struct user_regs_struct *regs);
void multi_tracer_c9(struct user_regs_struct *regs);
void multi_tracer_c10(struct user_regs_struct *regs);
void multi_tracer_c11(struct user_regs_struct *regs);
void multi_tracer_c12(struct user_regs_struct *regs);
void multi_tracer_c13(struct user_regs_struct *regs);
void multi_tracer_c14(struct user_regs_struct *regs);
void multi_tracer_c15(struct user_regs_struct *regs);
void multi_tracer_c16(struct user_regs_struct *regs);
void multi_tracer_c17(struct user_regs_struct *regs);
void multi_tracer_c18(struct user_regs_struct *regs);
void multi_tracer_c19(struct user_regs_struct *regs);
void multi_tracer_c20(struct user_regs_struct *regs);
void multi_tracer_c21(struct user_regs_struct *regs);
void multi_tracer_c22(struct user_regs_struct *regs);
void multi_tracer_c23(struct user_regs_struct *regs);
void multi_tracer_c24(struct user_regs_struct *regs);
void multi_tracer_c25(struct user_regs_struct *regs);
void multi_tracer_c26(struct user_regs_struct *regs);
void multi_tracer_c27(struct user_regs_struct *regs);
void multi_tracer_c28(struct user_regs_struct *regs);
void multi_tracer_c29(struct user_regs_struct *regs);
void multi_tracer_c30(struct user_regs_struct *regs);
void multi_tracer_c31(struct user_regs_struct *regs);
void multi_tracer_c32(struct user_regs_struct *regs);
void multi_tracer_c33(struct user_regs_struct *regs);
void multi_tracer_c34(struct user_regs_struct *regs);
void multi_tracer_c35(struct user_regs_struct *regs);
void multi_tracer_c36(struct user_regs_struct *regs);
void multi_tracer_c37(struct user_regs_struct *regs);
void multi_tracer_c38(struct user_regs_struct *regs);
void multi_tracer_c39(struct user_regs_struct *regs);
void multi_tracer_c40(struct user_regs_struct *regs);
void multi_tracer_c41(struct user_regs_struct *regs);
void multi_tracer_c42(struct user_regs_struct *regs);
void multi_tracer_c43(struct user_regs_struct *regs);
void multi_tracer_c44(struct user_regs_struct *regs);
void multi_tracer_c45(struct user_regs_struct *regs);
void multi_tracer_c46(struct user_regs_struct *regs);
void multi_tracer_c47(struct user_regs_struct *regs);
void multi_tracer_c48(struct user_regs_struct *regs);
void multi_tracer_c49(struct user_regs_struct *regs);
void multi_tracer_c50(struct user_regs_struct *regs);
void multi_tracer_c51(struct user_regs_struct *regs);
void multi_tracer_c52(struct user_regs_struct *regs);
void multi_tracer_c53(struct user_regs_struct *regs);
void multi_tracer_c54(struct user_regs_struct *regs);
void multi_tracer_c55(struct user_regs_struct *regs);
void multi_tracer_c56(struct user_regs_struct *regs);
void multi_tracer_c57(struct user_regs_struct *regs);
void multi_tracer_c58(struct user_regs_struct *regs);
void multi_tracer_c59(struct user_regs_struct *regs);
void multi_tracer_c60(struct user_regs_struct *regs);
void multi_tracer_c61(struct user_regs_struct *regs);
void multi_tracer_c62(struct user_regs_struct *regs);
void multi_tracer_c63(struct user_regs_struct *regs);
void multi_tracer_c64(struct user_regs_struct *regs);
void multi_tracer_c65(struct user_regs_struct *regs);
void multi_tracer_c66(struct user_regs_struct *regs);
void multi_tracer_c67(struct user_regs_struct *regs);
void multi_tracer_c68(struct user_regs_struct *regs);
void multi_tracer_c69(struct user_regs_struct *regs);
void multi_tracer_c70(struct user_regs_struct *regs);
void multi_tracer_c71(struct user_regs_struct *regs);
void multi_tracer_c72(struct user_regs_struct *regs);
void multi_tracer_c73(struct user_regs_struct *regs);
void multi_tracer_c74(struct user_regs_struct *regs);
void multi_tracer_c75(struct user_regs_struct *regs);
void multi_tracer_c76(struct user_regs_struct *regs);
void multi_tracer_c77(struct user_regs_struct *regs);
void multi_tracer_c78(struct user_regs_struct *regs);
void multi_tracer_c79(struct user_regs_struct *regs);
void multi_tracer_c80(struct user_regs_struct *regs);
void multi_tracer_c81(struct user_regs_struct *regs);
void multi_tracer_c82(struct user_regs_struct *regs);
void multi_tracer_c83(struct user_regs_struct *regs);
void multi_tracer_c84(struct user_regs_struct *regs);
void multi_tracer_c85(struct user_regs_struct *regs);
void multi_tracer_c86(struct user_regs_struct *regs);
void multi_tracer_c87(struct user_regs_struct *regs);
void multi_tracer_c88(struct user_regs_struct *regs);
void multi_tracer_c89(struct user_regs_struct *regs);
void multi_tracer_c90(struct user_regs_struct *regs);
void multi_tracer_c91(struct user_regs_struct *regs);
void multi_tracer_c92(struct user_regs_struct *regs);
void multi_tracer_c93(struct user_regs_struct *regs);
void multi_tracer_c94(struct user_regs_struct *regs);
void multi_tracer_c95(struct user_regs_struct *regs);
void multi_tracer_c96(struct user_regs_struct *regs);
void multi_tracer_c97(struct user_regs_struct *regs);
void multi_tracer_c98(struct user_regs_struct *regs);
void multi_tracer_c99(struct user_regs_struct *regs);
void multi_tracer_c100(struct user_regs_struct *regs);
void multi_tracer_c101(struct user_regs_struct *regs);
void multi_tracer_c102(struct user_regs_struct *regs);
void multi_tracer_c103(struct user_regs_struct *regs);
void multi_tracer_c104(struct user_regs_struct *regs);
void multi_tracer_c105(struct user_regs_struct *regs);
void multi_tracer_c106(struct user_regs_struct *regs);
void multi_tracer_c107(struct user_regs_struct *regs);
void multi_tracer_c108(struct user_regs_struct *regs);
void multi_tracer_c109(struct user_regs_struct *regs);
void multi_tracer_c110(struct user_regs_struct *regs);
void multi_tracer_c111(struct user_regs_struct *regs);
void multi_tracer_c112(struct user_regs_struct *regs);
void multi_tracer_c113(struct user_regs_struct *regs);
void multi_tracer_c114(struct user_regs_struct *regs);
void multi_tracer_c115(struct user_regs_struct *regs);
void multi_tracer_c116(struct user_regs_struct *regs);
void multi_tracer_c117(struct user_regs_struct *regs);
void multi_tracer_c118(struct user_regs_struct *regs);
void multi_tracer_c119(struct user_regs_struct *regs);
void multi_tracer_c120(struct user_regs_struct *regs);
void multi_tracer_c121(struct user_regs_struct *regs);
void multi_tracer_c122(struct user_regs_struct *regs);
void multi_tracer_c123(struct user_regs_struct *regs);
void multi_tracer_c124(struct user_regs_struct *regs);
void multi_tracer_c125(struct user_regs_struct *regs);
void multi_tracer_c126(struct user_regs_struct *regs);
void multi_tracer_c127(struct user_regs_struct *regs);
void multi_tracer_c128(struct user_regs_struct *regs);
void multi_tracer_c129(struct user_regs_struct *regs);
void multi_tracer_c130(struct user_regs_struct *regs);
void multi_tracer_c131(struct user_regs_struct *regs);
void multi_tracer_c132(struct user_regs_struct *regs);
void multi_tracer_c133(struct user_regs_struct *regs);
void multi_tracer_c134(struct user_regs_struct *regs);
void multi_tracer_c135(struct user_regs_struct *regs);
void multi_tracer_c136(struct user_regs_struct *regs);
void multi_tracer_c137(struct user_regs_struct *regs);
void multi_tracer_c138(struct user_regs_struct *regs);
void multi_tracer_c139(struct user_regs_struct *regs);
void multi_tracer_c140(struct user_regs_struct *regs);
void multi_tracer_c141(struct user_regs_struct *regs);
void multi_tracer_c142(struct user_regs_struct *regs);
void multi_tracer_c143(struct user_regs_struct *regs);
void multi_tracer_c144(struct user_regs_struct *regs);
void multi_tracer_c145(struct user_regs_struct *regs);
void multi_tracer_c146(struct user_regs_struct *regs);
void multi_tracer_c147(struct user_regs_struct *regs);
void multi_tracer_c148(struct user_regs_struct *regs);
void multi_tracer_c149(struct user_regs_struct *regs);
void multi_tracer_c150(struct user_regs_struct *regs);
void multi_tracer_c151(struct user_regs_struct *regs);
void multi_tracer_c152(struct user_regs_struct *regs);
void multi_tracer_c153(struct user_regs_struct *regs);
void multi_tracer_c154(struct user_regs_struct *regs);
void multi_tracer_c155(struct user_regs_struct *regs);
void multi_tracer_c156(struct user_regs_struct *regs);
void multi_tracer_c157(struct user_regs_struct *regs);
void multi_tracer_c158(struct user_regs_struct *regs);
void multi_tracer_c159(struct user_regs_struct *regs);
void multi_tracer_c160(struct user_regs_struct *regs);
void multi_tracer_c161(struct user_regs_struct *regs);
void multi_tracer_c162(struct user_regs_struct *regs);
void multi_tracer_c163(struct user_regs_struct *regs);
void multi_tracer_c164(struct user_regs_struct *regs);
void multi_tracer_c165(struct user_regs_struct *regs);
void multi_tracer_c166(struct user_regs_struct *regs);
void multi_tracer_c167(struct user_regs_struct *regs);
void multi_tracer_c168(struct user_regs_struct *regs);
void multi_tracer_c169(struct user_regs_struct *regs);
void multi_tracer_c170(struct user_regs_struct *regs);
void multi_tracer_c171(struct user_regs_struct *regs);
void multi_tracer_c172(struct user_regs_struct *regs);
void multi_tracer_c173(struct user_regs_struct *regs);
void multi_tracer_c174(struct user_regs_struct *regs);
void multi_tracer_c175(struct user_regs_struct *regs);
void multi_tracer_c176(struct user_regs_struct *regs);
void multi_tracer_c177(struct user_regs_struct *regs);
void multi_tracer_c178(struct user_regs_struct *regs);
void multi_tracer_c179(struct user_regs_struct *regs);
void multi_tracer_c180(struct user_regs_struct *regs);
void multi_tracer_c181(struct user_regs_struct *regs);
void multi_tracer_c182(struct user_regs_struct *regs);
void multi_tracer_c183(struct user_regs_struct *regs);
void multi_tracer_c184(struct user_regs_struct *regs);
void multi_tracer_c185(struct user_regs_struct *regs);
void multi_tracer_c186(struct user_regs_struct *regs);
void multi_tracer_c187(struct user_regs_struct *regs);
void multi_tracer_c188(struct user_regs_struct *regs);
void multi_tracer_c189(struct user_regs_struct *regs);
void multi_tracer_c190(struct user_regs_struct *regs);
void multi_tracer_c191(struct user_regs_struct *regs);
void multi_tracer_c192(struct user_regs_struct *regs);
void multi_tracer_c193(struct user_regs_struct *regs);
void multi_tracer_c194(struct user_regs_struct *regs);
void multi_tracer_c195(struct user_regs_struct *regs);
void multi_tracer_c196(struct user_regs_struct *regs);
void multi_tracer_c197(struct user_regs_struct *regs);
void multi_tracer_c198(struct user_regs_struct *regs);
void multi_tracer_c199(struct user_regs_struct *regs);
void multi_tracer_c200(struct user_regs_struct *regs);
void multi_tracer_c201(struct user_regs_struct *regs);
void multi_tracer_c202(struct user_regs_struct *regs);
void multi_tracer_c203(struct user_regs_struct *regs);
void multi_tracer_c204(struct user_regs_struct *regs);
void multi_tracer_c205(struct user_regs_struct *regs);
void multi_tracer_c206(struct user_regs_struct *regs);
void multi_tracer_c207(struct user_regs_struct *regs);
void multi_tracer_c208(struct user_regs_struct *regs);
void multi_tracer_c209(struct user_regs_struct *regs);
void multi_tracer_c210(struct user_regs_struct *regs);
void multi_tracer_c211(struct user_regs_struct *regs);
void multi_tracer_c212(struct user_regs_struct *regs);
void multi_tracer_c213(struct user_regs_struct *regs);
void multi_tracer_c214(struct user_regs_struct *regs);
void multi_tracer_c215(struct user_regs_struct *regs);
void multi_tracer_c216(struct user_regs_struct *regs);
void multi_tracer_c217(struct user_regs_struct *regs);
void multi_tracer_c218(struct user_regs_struct *regs);
void multi_tracer_c219(struct user_regs_struct *regs);
void multi_tracer_c220(struct user_regs_struct *regs);
void multi_tracer_c221(struct user_regs_struct *regs);
void multi_tracer_c222(struct user_regs_struct *regs);
void multi_tracer_c223(struct user_regs_struct *regs);
void multi_tracer_c224(struct user_regs_struct *regs);
void multi_tracer_c225(struct user_regs_struct *regs);
void multi_tracer_c226(struct user_regs_struct *regs);
void multi_tracer_c227(struct user_regs_struct *regs);
void multi_tracer_c228(struct user_regs_struct *regs);
void multi_tracer_c229(struct user_regs_struct *regs);
void multi_tracer_c230(struct user_regs_struct *regs);
void multi_tracer_c231(struct user_regs_struct *regs);
void multi_tracer_c232(struct user_regs_struct *regs);
void multi_tracer_c233(struct user_regs_struct *regs);
void multi_tracer_c234(struct user_regs_struct *regs);
void multi_tracer_c235(struct user_regs_struct *regs);
void multi_tracer_c236(struct user_regs_struct *regs);
void multi_tracer_c237(struct user_regs_struct *regs);
void multi_tracer_c238(struct user_regs_struct *regs);
void multi_tracer_c239(struct user_regs_struct *regs);
void multi_tracer_c240(struct user_regs_struct *regs);
void multi_tracer_c241(struct user_regs_struct *regs);
void multi_tracer_c242(struct user_regs_struct *regs);
void multi_tracer_c243(struct user_regs_struct *regs);
void multi_tracer_c244(struct user_regs_struct *regs);
void multi_tracer_c245(struct user_regs_struct *regs);
void multi_tracer_c246(struct user_regs_struct *regs);
void multi_tracer_c247(struct user_regs_struct *regs);
void multi_tracer_c248(struct user_regs_struct *regs);
void multi_tracer_c249(struct user_regs_struct *regs);
void multi_tracer_c250(struct user_regs_struct *regs);
void multi_tracer_c251(struct user_regs_struct *regs);
void multi_tracer_c252(struct user_regs_struct *regs);
void multi_tracer_c253(struct user_regs_struct *regs);
void multi_tracer_c254(struct user_regs_struct *regs);
void multi_tracer_c255(struct user_regs_struct *regs);
void multi_tracer_p0(struct user_regs_struct *regs);
void multi_tracer_p1(struct user_regs_struct *regs);
void multi_tracer_p2(struct user_regs_struct *regs);
void multi_tracer_p3(struct user_regs_struct *regs);
void multi_tracer_p4(struct user_regs_struct *regs);
void multi_tracer_p5(struct user_regs_struct *regs);
void multi_tracer_p6(struct user_regs_struct *regs);
void multi_tracer_p7(struct user_regs_struct *regs);
void multi_tracer_p8(struct user_regs_struct *regs);
void multi_tracer_p9(struct user_regs_struct *regs);
void multi_tracer_p10(struct user_regs_struct *regs);
void multi_tracer_p11(struct user_regs_struct *regs);
void multi_tracer_p12(struct user_regs_struct *regs);
void multi_tracer_p13(struct user_regs_struct *regs);
void multi_tracer_p14(struct user_regs_struct *regs);
void multi_tracer_p15(struct user_regs_struct *regs);
void multi_tracer_p16(struct user_regs_struct *regs);
void multi_tracer_p17(struct user_regs_struct *regs);
void multi_tracer_p18(struct user_regs_struct *regs);
void multi_tracer_p19(struct user_regs_struct *regs);
void multi_tracer_p20(struct user_regs_struct *regs);
void multi_tracer_p21(struct user_regs_struct *regs);
void multi_tracer_p22(struct user_regs_struct *regs);
void multi_tracer_p23(struct user_regs_struct *regs);
void multi_tracer_p24(struct user_regs_struct *regs);
void multi_tracer_p25(struct user_regs_struct *regs);
void multi_tracer_p26(struct user_regs_struct *regs);
void multi_tracer_p27(struct user_regs_struct *regs);
void multi_tracer_p28(struct user_regs_struct *regs);
void multi_tracer_p29(struct user_regs_struct *regs);
void multi_tracer_p30(struct user_regs_struct *regs);
void multi_tracer_p31(struct user_regs_struct *regs);
void multi_tracer_p32(struct user_regs_struct *regs);
void multi_tracer_p33(struct user_regs_struct *regs);
void multi_tracer_p34(struct user_regs_struct *regs);
void multi_tracer_p35(struct user_regs_struct *regs);
void multi_tracer_p36(struct user_regs_struct *regs);
void multi_tracer_p37(struct user_regs_struct *regs);
void multi_tracer_p38(struct user_regs_struct *regs);
void multi_tracer_p39(struct user_regs_struct *regs);
void multi_tracer_p40(struct user_regs_struct *regs);
void multi_tracer_p41(struct user_regs_struct *regs);
void multi_tracer_p42(struct user_regs_struct *regs);
void multi_tracer_p43(struct user_regs_struct *regs);
void multi_tracer_p44(struct user_regs_struct *regs);
void multi_tracer_p45(struct user_regs_struct *regs);
void multi_tracer_p46(struct user_regs_struct *regs);
void multi_tracer_p47(struct user_regs_struct *regs);
void multi_tracer_p48(struct user_regs_struct *regs);
void multi_tracer_p49(struct user_regs_struct *regs);
void multi_tracer_p50(struct user_regs_struct *regs);
void multi_tracer_p51(struct user_regs_struct *regs);
void multi_tracer_p52(struct user_regs_struct *regs);
void multi_tracer_p53(struct user_regs_struct *regs);
void multi_tracer_p54(struct user_regs_struct *regs);
void multi_tracer_p55(struct user_regs_struct *regs);
void multi_tracer_p56(struct user_regs_struct *regs);
void multi_tracer_p57(struct user_regs_struct *regs);
void multi_tracer_p58(struct user_regs_struct *regs);
void multi_tracer_p59(struct user_regs_struct *regs);
void multi_tracer_p60(struct user_regs_struct *regs);
void multi_tracer_p61(struct user_regs_struct *regs);
void multi_tracer_p62(struct user_regs_struct *regs);
void multi_tracer_p63(struct user_regs_struct *regs);
void multi_tracer_p64(struct user_regs_struct *regs);
void multi_tracer_p65(struct user_regs_struct *regs);
void multi_tracer_p66(struct user_regs_struct *regs);
void multi_tracer_p67(struct user_regs_struct *regs);
void multi_tracer_p68(struct user_regs_struct *regs);
void multi_tracer_p69(struct user_regs_struct *regs);
void multi_tracer_p70(struct user_regs_struct *regs);
void multi_tracer_p71(struct user_regs_struct *regs);
void multi_tracer_p72(struct user_regs_struct *regs);
void multi_tracer_p73(struct user_regs_struct *regs);
void multi_tracer_p74(struct user_regs_struct *regs);
void multi_tracer_p75(struct user_regs_struct *regs);
void multi_tracer_p76(struct user_regs_struct *regs);
void multi_tracer_p77(struct user_regs_struct *regs);
void multi_tracer_p78(struct user_regs_struct *regs);
void multi_tracer_p79(struct user_regs_struct *regs);
void multi_tracer_p80(struct user_regs_struct *regs);
void multi_tracer_p81(struct user_regs_struct *regs);
void multi_tracer_p82(struct user_regs_struct *regs);
void multi_tracer_p83(struct user_regs_struct *regs);
void multi_tracer_p84(struct user_regs_struct *regs);
void multi_tracer_p85(struct user_regs_struct *regs);
void multi_tracer_p86(struct user_regs_struct *regs);
void multi_tracer_p87(struct user_regs_struct *regs);
void multi_tracer_p88(struct user_regs_struct *regs);
void multi_tracer_p89(struct user_regs_struct *regs);
void multi_tracer_p90(struct user_regs_struct *regs);
void multi_tracer_p91(struct user_regs_struct *regs);
void multi_tracer_p92(struct user_regs_struct *regs);
void multi_tracer_p93(struct user_regs_struct *regs);
void multi_tracer_p94(struct user_regs_struct *regs);
void multi_tracer_p95(struct user_regs_struct *regs);
void multi_tracer_p96(struct user_regs_struct *regs);
void multi_tracer_p97(struct user_regs_struct *regs);
void multi_tracer_p98(struct user_regs_struct *regs);
void multi_tracer_p99(struct user_regs_struct *regs);
void multi_tracer_p100(struct user_regs_struct *regs);
void multi_tracer_p101(struct user_regs_struct *regs);
void multi_tracer_p102(struct user_regs_struct *regs);
void multi_tracer_p103(struct user_regs_struct *regs);
void multi_tracer_p104(struct user_regs_struct *regs);
void multi_tracer_p105(struct user_regs_struct *regs);
void multi_tracer_p106(struct user_regs_struct *regs);
void multi_tracer_p107(struct user_regs_struct *regs);
void multi_tracer_p108(struct user_regs_struct *regs);
void multi_tracer_p109(struct user_regs_struct *regs);
void multi_tracer_p110(struct user_regs_struct *regs);
void multi_tracer_p111(struct user_regs_struct *regs);
void multi_tracer_p112(struct user_regs_struct *regs);
void multi_tracer_p113(struct user_regs_struct *regs);
void multi_tracer_p114(struct user_regs_struct *regs);
void multi_tracer_p115(struct user_regs_struct *regs);
void multi_tracer_p116(struct user_regs_struct *regs);
void multi_tracer_p117(struct user_regs_struct *regs);
void multi_tracer_p118(struct user_regs_struct *regs);
void multi_tracer_p119(struct user_regs_struct *regs);
void multi_tracer_p120(struct user_regs_struct *regs);
void multi_tracer_p121(struct user_regs_struct *regs);
void multi_tracer_p122(struct user_regs_struct *regs);
void multi_tracer_p123(struct user_regs_struct *regs);
void multi_tracer_p124(struct user_regs_struct *regs);
void multi_tracer_p125(struct user_regs_struct *regs);
void multi_tracer_p126(struct user_regs_struct *regs);
void multi_tracer_p127(struct user_regs_struct *regs);
void multi_tracer_p128(struct user_regs_struct *regs);
void multi_tracer_p129(struct user_regs_struct *regs);
void multi_tracer_p130(struct user_regs_struct *regs);
void multi_tracer_p131(struct user_regs_struct *regs);
void multi_tracer_p132(struct user_regs_struct *regs);
void multi_tracer_p133(struct user_regs_struct *regs);
void multi_tracer_p134(struct user_regs_struct *regs);
void multi_tracer_p135(struct user_regs_struct *regs);
void multi_tracer_p136(struct user_regs_struct *regs);
void multi_tracer_p137(struct user_regs_struct *regs);
void multi_tracer_p138(struct user_regs_struct *regs);
void multi_tracer_p139(struct user_regs_struct *regs);
void multi_tracer_p140(struct user_regs_struct *regs);
void multi_tracer_p141(struct user_regs_struct *regs);
void multi_tracer_p142(struct user_regs_struct *regs);
void multi_tracer_p143(struct user_regs_struct *regs);
void multi_tracer_p144(struct user_regs_struct *regs);
void multi_tracer_p145(struct user_regs_struct *regs);
void multi_tracer_p146(struct user_regs_struct *regs);
void multi_tracer_p147(struct user_regs_struct *regs);
void multi_tracer_p148(struct user_regs_struct *regs);
void multi_tracer_p149(struct user_regs_struct *regs);
void multi_tracer_p150(struct user_regs_struct *regs);
void multi_tracer_p151(struct user_regs_struct *regs);
void multi_tracer_p152(struct user_regs_struct *regs);
void multi_tracer_p153(struct user_regs_struct *regs);
void multi_tracer_p154(struct user_regs_struct *regs);
void multi_tracer_p155(struct user_regs_struct *regs);
void multi_tracer_p156(struct user_regs_struct *regs);
void multi_tracer_p157(struct user_regs_struct *regs);
void multi_tracer_p158(struct user_regs_struct *regs);
void multi_tracer_p159(struct user_regs_struct *regs);
void multi_tracer_p160(struct user_regs_struct *regs);
void multi_tracer_p161(struct user_regs_struct *regs);
void multi_tracer_p162(struct user_regs_struct *regs);
void multi_tracer_p163(struct user_regs_struct *regs);
void multi_tracer_p164(struct user_regs_struct *regs);
void multi_tracer_p165(struct user_regs_struct *regs);
void multi_tracer_p166(struct user_regs_struct *regs);
void multi_tracer_p167(struct user_regs_struct *regs);
void multi_tracer_p168(struct user_regs_struct *regs);
void multi_tracer_p169(struct user_regs_struct *regs);
void multi_tracer_p170(struct user_regs_struct *regs);
void multi_tracer_p171(struct user_regs_struct *regs);
void multi_tracer_p172(struct user_regs_struct *regs);
void multi_tracer_p173(struct user_regs_struct *regs);
void multi_tracer_p174(struct user_regs_struct *regs);
void multi_tracer_p175(struct user_regs_struct *regs);
void multi_tracer_p176(struct user_regs_struct *regs);
void multi_tracer_p177(struct user_regs_struct *regs);
void multi_tracer_p178(struct user_regs_struct *regs);
void multi_tracer_p179(struct user_regs_struct *regs);
void multi_tracer_p180(struct user_regs_struct *regs);
void multi_tracer_p181(struct user_regs_struct *regs);
void multi_tracer_p182(struct user_regs_struct *regs);
void multi_tracer_p183(struct user_regs_struct *regs);
void multi_tracer_p184(struct user_regs_struct *regs);
void multi_tracer_p185(struct user_regs_struct *regs);
void multi_tracer_p186(struct user_regs_struct *regs);
void multi_tracer_p187(struct user_regs_struct *regs);
void multi_tracer_p188(struct user_regs_struct *regs);
void multi_tracer_p189(struct user_regs_struct *regs);
void multi_tracer_p190(struct user_regs_struct *regs);
void multi_tracer_p191(struct user_regs_struct *regs);
void multi_tracer_p192(struct user_regs_struct *regs);
void multi_tracer_p193(struct user_regs_struct *regs);
void multi_tracer_p194(struct user_regs_struct *regs);
void multi_tracer_p195(struct user_regs_struct *regs);
void multi_tracer_p196(struct user_regs_struct *regs);
void multi_tracer_p197(struct user_regs_struct *regs);
void multi_tracer_p198(struct user_regs_struct *regs);
void multi_tracer_p199(struct user_regs_struct *regs);
void multi_tracer_p200(struct user_regs_struct *regs);
void multi_tracer_p201(struct user_regs_struct *regs);
void multi_tracer_p202(struct user_regs_struct *regs);
void multi_tracer_p203(struct user_regs_struct *regs);
void multi_tracer_p204(struct user_regs_struct *regs);
void multi_tracer_p205(struct user_regs_struct *regs);
void multi_tracer_p206(struct user_regs_struct *regs);
void multi_tracer_p207(struct user_regs_struct *regs);
void multi_tracer_p208(struct user_regs_struct *regs);
void multi_tracer_p209(struct user_regs_struct *regs);
void multi_tracer_p210(struct user_regs_struct *regs);
void multi_tracer_p211(struct user_regs_struct *regs);
void multi_tracer_p212(struct user_regs_struct *regs);
void multi_tracer_p213(struct user_regs_struct *regs);
void multi_tracer_p214(struct user_regs_struct *regs);
void multi_tracer_p215(struct user_regs_struct *regs);
void multi_tracer_p216(struct user_regs_struct *regs);
void multi_tracer_p217(struct user_regs_struct *regs);
void multi_tracer_p218(struct user_regs_struct *regs);
void multi_tracer_p219(struct user_regs_struct *regs);
void multi_tracer_p220(struct user_regs_struct *regs);
void multi_tracer_p221(struct user_regs_struct *regs);
void multi_tracer_p222(struct user_regs_struct *regs);
void multi_tracer_p223(struct user_regs_struct *regs);
void multi_tracer_p224(struct user_regs_struct *regs);
void multi_tracer_p225(struct user_regs_struct *regs);
void multi_tracer_p226(struct user_regs_struct *regs);
void multi_tracer_p227(struct user_regs_struct *regs);
void multi_tracer_p228(struct user_regs_struct *regs);
void multi_tracer_p229(struct user_regs_struct *regs);
void multi_tracer_p230(struct user_regs_struct *regs);
void multi_tracer_p231(struct user_regs_struct *regs);
void multi_tracer_p232(struct user_regs_struct *regs);
void multi_tracer_p233(struct user_regs_struct *regs);
void multi_tracer_p234(struct user_regs_struct *regs);
void multi_tracer_p235(struct user_regs_struct *regs);
void multi_tracer_p236(struct user_regs_struct *regs);
void multi_tracer_p237(struct user_regs_struct *regs);
void multi_tracer_p238(struct user_regs_struct *regs);
void multi_tracer_p239(struct user_regs_struct *regs);
void multi_tracer_p240(struct user_regs_struct *regs);
void multi_tracer_p241(struct user_regs_struct *regs);
void multi_tracer_p242(struct user_regs_struct *regs);
void multi_tracer_p243(struct user_regs_struct *regs);
void multi_tracer_p244(struct user_regs_struct *regs);
void multi_tracer_p245(struct user_regs_struct *regs);
void multi_tracer_p246(struct user_regs_struct *regs);
void multi_tracer_p247(struct user_regs_struct *regs);
void multi_tracer_p248(struct user_regs_struct *regs);
void multi_tracer_p249(struct user_regs_struct *regs);
void multi_tracer_p250(struct user_regs_struct *regs);
void multi_tracer_p251(struct user_regs_struct *regs);
void multi_tracer_p252(struct user_regs_struct *regs);
void multi_tracer_p253(struct user_regs_struct *regs);
void multi_tracer_p254(struct user_regs_struct *regs);
void multi_tracer_p255(struct user_regs_struct *regs);
void multi_tracer_p224(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e0)
{
multi_tracer_p54(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xcb3c9b ^ regs->rbx);
ptr[0] ^= 0x23;
ptr[1] ^= 0x82;
ptr[2] ^= 0xce;
ptr[3] ^= 0x3b;
ptr[4] += 0x44;
ptr[5] ^= 0xe4;
ptr[6] += 0x4b;
ptr[7] ^= 0x6e;
ptr[8] += 0x30;
ptr[9] ^= 0x86;
ptr[10] ^= 0xec;
ptr[11] += 0x1d;
ptr[12] ^= 0x2c;
ptr[13] ^= 0xa1;
ptr[14] ^= 0x62;
ptr[15] += 0x89;
ptr[16] ^= 0xb8;
ptr[17] += 0x19;
ptr[18] += 0x1b;
ptr[19] ^= 0x6b;
ptr[20] ^= 0x6c;
ptr[21] ^= 0x6c;
ptr[22] += 0xeb;
ptr[23] += 0x82;

}

void multi_tracer_p54(struct user_regs_struct *regs)
{
if (regs->rax != 0x2036)
{
multi_tracer_p169(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xce275c ^ regs->rbx);
ptr[0] += 0x61;
ptr[1] ^= 0xdd;
ptr[2] ^= 0xcd;
ptr[3] ^= 0xf7;
ptr[4] += 0xd4;
ptr[5] ^= 0x1e;
ptr[6] ^= 0x2f;
ptr[7] ^= 0x67;
ptr[8] += 0xa0;
ptr[9] ^= 0xe8;
ptr[10] += 0x1e;
ptr[11] ^= 0x57;
ptr[12] ^= 0x3e;
ptr[13] += 0xda;
ptr[14] ^= 0xc0;
ptr[15] ^= 0xf8;
ptr[16] += 0xc7;
ptr[17] ^= 0x24;
ptr[18] ^= 0x12;
ptr[19] += 0xb5;
ptr[20] += 0x2;
ptr[21] += 0x23;
ptr[22] += 0x76;
ptr[23] ^= 0xd6;
ptr[24] += 0xc6;
ptr[25] ^= 0xde;
ptr[26] ^= 0xa9;
ptr[27] += 0x20;

}

void multi_tracer_p169(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a9)
{
multi_tracer_p215(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe1cb8b ^ regs->rbx);
ptr[0] += 0x2f;
ptr[1] ^= 0xd2;
ptr[2] += 0x3;
ptr[3] += 0x96;
ptr[4] += 0xd1;
ptr[5] ^= 0x1a;
ptr[6] += 0x29;
ptr[7] ^= 0x35;
ptr[8] ^= 0x74;
ptr[9] += 0x9c;
ptr[10] += 0x91;
ptr[11] += 0xc1;
ptr[12] ^= 0x87;
ptr[13] ^= 0x4d;
ptr[14] ^= 0xc0;
ptr[15] += 0x1a;
ptr[16] ^= 0xec;
ptr[17] ^= 0x16;

}

void multi_tracer_p215(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d7)
{
multi_tracer_p76(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x44b8ed ^ regs->rbx);
ptr[0] += 0xb7;
ptr[1] ^= 0x3e;
ptr[2] ^= 0x87;
ptr[3] ^= 0x33;
ptr[4] += 0xf;
ptr[5] += 0xbb;
ptr[6] ^= 0x66;
ptr[7] += 0x74;
ptr[8] ^= 0xea;
ptr[9] ^= 0x5d;
ptr[10] += 0xd5;
ptr[11] += 0x86;
ptr[12] ^= 0x5f;
ptr[13] += 0x6d;
ptr[14] ^= 0x2f;
ptr[15] ^= 0x47;
ptr[16] += 0xa;
ptr[17] ^= 0x30;
ptr[18] ^= 0x6;
ptr[19] ^= 0x5e;
ptr[20] += 0x3f;
ptr[21] += 0x96;
ptr[22] += 0x6b;
ptr[23] ^= 0x6f;
ptr[24] ^= 0x4c;

}

void multi_tracer_p76(struct user_regs_struct *regs)
{
if (regs->rax != 0x204c)
{
multi_tracer_p136(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x1facb9 ^ regs->rbx);
ptr[0] += 0x30;
ptr[1] ^= 0xf5;
ptr[2] ^= 0xe0;
ptr[3] += 0xd1;
ptr[4] += 0x79;
ptr[5] ^= 0x8d;
ptr[6] += 0x9f;
ptr[7] += 0x72;
ptr[8] += 0xe9;
ptr[9] += 0x88;
ptr[10] ^= 0x81;
ptr[11] += 0xb1;
ptr[12] += 0x4c;
ptr[13] += 0x4c;
ptr[14] += 0x1f;
ptr[15] ^= 0x48;
ptr[16] += 0xe3;
ptr[17] ^= 0xbd;
ptr[18] ^= 0xcb;
ptr[19] ^= 0x38;
ptr[20] += 0x14;
ptr[21] ^= 0xfd;
ptr[22] += 0x2b;
ptr[23] ^= 0x44;
ptr[24] += 0x6b;
ptr[25] += 0x89;

}

void multi_tracer_p136(struct user_regs_struct *regs)
{
if (regs->rax != 0x2088)
{
multi_tracer_p55(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x6c76f7 ^ regs->rbx);
ptr[0] ^= 0x4c;
ptr[1] += 0xdd;
ptr[2] += 0x4d;
ptr[3] ^= 0xc6;
ptr[4] += 0xf0;
ptr[5] ^= 0x97;
ptr[6] ^= 0x45;
ptr[7] ^= 0xbd;
ptr[8] ^= 0x3b;
ptr[9] += 0xd2;
ptr[10] ^= 0xb3;
ptr[11] += 0xe2;
ptr[12] ^= 0xb1;
ptr[13] += 0x17;
ptr[14] ^= 0x1f;
ptr[15] += 0x8a;
ptr[16] += 0xd1;
ptr[17] ^= 0x4f;
ptr[18] += 0xd4;
ptr[19] ^= 0xe1;
ptr[20] += 0x9;
ptr[21] ^= 0x10;
ptr[22] += 0x62;
ptr[23] += 0x8d;
ptr[24] += 0xb9;
ptr[25] ^= 0x83;
ptr[26] += 0xa7;

}

void multi_tracer_p55(struct user_regs_struct *regs)
{
if (regs->rax != 0x2037)
{
multi_tracer_p91(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbb75d8 ^ regs->rbx);
ptr[0] += 0xe5;
ptr[1] ^= 0x31;
ptr[2] += 0x71;
ptr[3] ^= 0xce;
ptr[4] += 0xdf;
ptr[5] += 0xda;
ptr[6] ^= 0x68;
ptr[7] += 0xe2;
ptr[8] ^= 0xe9;
ptr[9] ^= 0x57;
ptr[10] += 0x43;
ptr[11] ^= 0x5f;
ptr[12] ^= 0x72;
ptr[13] ^= 0xf1;
ptr[14] ^= 0x4;
ptr[15] += 0xdf;
ptr[16] += 0x50;
ptr[17] += 0x7b;
ptr[18] ^= 0xf7;
ptr[19] ^= 0x2e;
ptr[20] += 0x3e;

}

void multi_tracer_p91(struct user_regs_struct *regs)
{
if (regs->rax != 0x205b)
{
multi_tracer_p114(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xccf38f ^ regs->rbx);
ptr[0] += 0xeb;
ptr[1] ^= 0x1b;
ptr[2] += 0xf4;
ptr[3] += 0x3a;
ptr[4] ^= 0x8a;
ptr[5] ^= 0x28;
ptr[6] += 0xd9;
ptr[7] += 0x33;
ptr[8] += 0xcf;
ptr[9] += 0x42;
ptr[10] += 0xf8;
ptr[11] += 0xd7;
ptr[12] ^= 0xe0;
ptr[13] ^= 0xf6;
ptr[14] += 0x7a;
ptr[15] += 0x87;
ptr[16] ^= 0xfb;
ptr[17] += 0xf8;
ptr[18] += 0x91;
ptr[19] ^= 0xaa;
ptr[20] ^= 0x4;
ptr[21] ^= 0x18;
ptr[22] += 0xfd;
ptr[23] ^= 0xf0;
ptr[24] ^= 0x9f;
ptr[25] ^= 0xc4;

}

void multi_tracer_p114(struct user_regs_struct *regs)
{
if (regs->rax != 0x2072)
{
multi_tracer_p6(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xaaeb66 ^ regs->rbx);
ptr[0] += 0x7d;
ptr[1] ^= 0x77;
ptr[2] ^= 0x72;
ptr[3] ^= 0x39;
ptr[4] += 0x40;
ptr[5] ^= 0x19;
ptr[6] += 0x93;
ptr[7] += 0x1e;
ptr[8] += 0x7c;
ptr[9] ^= 0x2a;
ptr[10] += 0xf5;
ptr[11] ^= 0xc1;
ptr[12] ^= 0x99;
ptr[13] ^= 0x22;
ptr[14] += 0x6a;
ptr[15] += 0xf9;
ptr[16] += 0xc1;
ptr[17] ^= 0x60;
ptr[18] += 0x6b;
ptr[19] += 0xd6;
ptr[20] += 0x72;
ptr[21] ^= 0x3b;
ptr[22] ^= 0x1e;
ptr[23] ^= 0x33;
ptr[24] += 0x15;
ptr[25] += 0x7a;
ptr[26] += 0xd2;

}

void multi_tracer_p6(struct user_regs_struct *regs)
{
if (regs->rax != 0x2006)
{
multi_tracer_p78(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9466b1 ^ regs->rbx);
ptr[0] ^= 0x19;
ptr[1] ^= 0x2f;
ptr[2] += 0x86;
ptr[3] ^= 0x88;
ptr[4] += 0xc1;
ptr[5] += 0x91;
ptr[6] += 0x4e;
ptr[7] ^= 0x76;
ptr[8] += 0xe0;
ptr[9] ^= 0x52;
ptr[10] ^= 0x30;
ptr[11] += 0x44;
ptr[12] += 0x8;
ptr[13] ^= 0x3b;
ptr[14] ^= 0xcb;
ptr[15] += 0x8c;
ptr[16] ^= 0x23;
ptr[17] ^= 0x4c;
ptr[18] ^= 0x5b;
ptr[19] ^= 0x94;
ptr[20] += 0x82;
ptr[21] ^= 0x7c;
ptr[22] += 0xb4;
ptr[23] ^= 0x4;
ptr[24] += 0x6d;
ptr[25] ^= 0x98;
ptr[26] += 0xf3;

}

void multi_tracer_p78(struct user_regs_struct *regs)
{
if (regs->rax != 0x204e)
{
multi_tracer_p102(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf17760 ^ regs->rbx);
ptr[0] ^= 0xd3;
ptr[1] += 0xea;
ptr[2] ^= 0x6e;
ptr[3] += 0x43;
ptr[4] ^= 0xf0;
ptr[5] ^= 0xda;
ptr[6] ^= 0x2f;
ptr[7] ^= 0x4c;
ptr[8] ^= 0x9b;
ptr[9] += 0x67;
ptr[10] ^= 0xb2;
ptr[11] ^= 0x60;
ptr[12] ^= 0x17;
ptr[13] += 0x9;
ptr[14] += 0xa9;
ptr[15] ^= 0xa3;
ptr[16] ^= 0x88;
ptr[17] += 0x7c;

}

void multi_tracer_p102(struct user_regs_struct *regs)
{
if (regs->rax != 0x2066)
{
multi_tracer_p113(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7cd873 ^ regs->rbx);
ptr[0] += 0x64;
ptr[1] += 0x1;
ptr[2] += 0x95;
ptr[3] += 0x1a;
ptr[4] += 0xcd;
ptr[5] ^= 0x81;
ptr[6] ^= 0x77;
ptr[7] += 0x55;
ptr[8] ^= 0xf2;
ptr[9] += 0x4;
ptr[10] += 0x51;
ptr[11] += 0xc0;
ptr[12] ^= 0xb2;
ptr[13] ^= 0xe7;
ptr[14] += 0xba;
ptr[15] ^= 0x85;
ptr[16] += 0x5d;
ptr[17] ^= 0x7c;
ptr[18] += 0xb2;
ptr[19] ^= 0x72;
ptr[20] ^= 0x8d;
ptr[21] ^= 0xce;
ptr[22] += 0x53;
ptr[23] += 0x5e;
ptr[24] ^= 0x4b;
ptr[25] ^= 0x24;
ptr[26] ^= 0xc6;

}

void multi_tracer_p113(struct user_regs_struct *regs)
{
if (regs->rax != 0x2071)
{
multi_tracer_p9(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x48ca6e ^ regs->rbx);
ptr[0] += 0xf7;
ptr[1] ^= 0x57;
ptr[2] ^= 0xff;
ptr[3] += 0xc0;
ptr[4] ^= 0x4;
ptr[5] += 0xcc;
ptr[6] ^= 0xbf;
ptr[7] ^= 0xbb;
ptr[8] += 0x8b;
ptr[9] += 0x6f;
ptr[10] ^= 0x57;
ptr[11] ^= 0xf;
ptr[12] ^= 0x13;
ptr[13] += 0xff;
ptr[14] += 0xca;
ptr[15] += 0x49;
ptr[16] += 0xc9;
ptr[17] ^= 0x7;
ptr[18] ^= 0x9a;
ptr[19] ^= 0x13;
ptr[20] += 0x55;
ptr[21] ^= 0xa4;
ptr[22] += 0x4d;
ptr[23] += 0x62;
ptr[24] += 0x71;
ptr[25] += 0x1d;

}

void multi_tracer_p9(struct user_regs_struct *regs)
{
if (regs->rax != 0x2009)
{
multi_tracer_p199(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd8d764 ^ regs->rbx);
ptr[0] += 0x29;
ptr[1] += 0x6d;
ptr[2] += 0x52;
ptr[3] ^= 0xdb;
ptr[4] ^= 0x69;
ptr[5] ^= 0xef;
ptr[6] ^= 0x62;
ptr[7] ^= 0xf2;
ptr[8] += 0x9;
ptr[9] += 0x4f;
ptr[10] ^= 0x6f;
ptr[11] += 0x3e;
ptr[12] ^= 0x14;
ptr[13] += 0xa4;
ptr[14] ^= 0x61;
ptr[15] += 0xfe;
ptr[16] += 0xf0;
ptr[17] ^= 0x10;
ptr[18] ^= 0x86;
ptr[19] += 0x47;

}

void multi_tracer_p199(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c7)
{
multi_tracer_p122(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7be7b1 ^ regs->rbx);
ptr[0] += 0xdf;
ptr[1] += 0x5e;
ptr[2] ^= 0xa1;
ptr[3] += 0x2b;
ptr[4] ^= 0x96;
ptr[5] ^= 0x43;
ptr[6] += 0x4e;
ptr[7] += 0x76;
ptr[8] += 0xaf;
ptr[9] ^= 0x9c;
ptr[10] += 0xec;
ptr[11] += 0x76;
ptr[12] ^= 0xb5;
ptr[13] += 0x8f;
ptr[14] += 0xce;
ptr[15] += 0xe6;
ptr[16] += 0x14;
ptr[17] += 0xb4;
ptr[18] += 0xce;
ptr[19] += 0xef;
ptr[20] += 0xae;
ptr[21] ^= 0x7f;
ptr[22] += 0x5d;
ptr[23] += 0x7d;
ptr[24] += 0x33;

}

void multi_tracer_p122(struct user_regs_struct *regs)
{
if (regs->rax != 0x207a)
{
multi_tracer_p110(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x43a510 ^ regs->rbx);
ptr[0] ^= 0x7a;
ptr[1] ^= 0x43;
ptr[2] ^= 0x2c;
ptr[3] ^= 0x9d;
ptr[4] += 0x2;
ptr[5] ^= 0x7b;
ptr[6] ^= 0xbf;
ptr[7] += 0x32;
ptr[8] += 0x14;
ptr[9] ^= 0x8c;
ptr[10] ^= 0x9c;
ptr[11] += 0xcd;
ptr[12] ^= 0xf1;
ptr[13] ^= 0xdb;
ptr[14] += 0x7b;
ptr[15] ^= 0x94;
ptr[16] ^= 0xd7;
ptr[17] += 0x32;
ptr[18] += 0x22;
ptr[19] ^= 0x2c;
ptr[20] += 0x90;
ptr[21] += 0xa3;
ptr[22] ^= 0x5b;
ptr[23] += 0x49;
ptr[24] ^= 0x32;
ptr[25] += 0xb7;
ptr[26] ^= 0xe4;
ptr[27] ^= 0x3a;

}

void multi_tracer_p110(struct user_regs_struct *regs)
{
if (regs->rax != 0x206e)
{
multi_tracer_p93(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc5b5e8 ^ regs->rbx);
ptr[0] ^= 0x2e;
ptr[1] ^= 0x2d;
ptr[2] += 0xbf;
ptr[3] ^= 0x48;
ptr[4] ^= 0x4a;
ptr[5] ^= 0x86;
ptr[6] += 0x74;
ptr[7] += 0x14;
ptr[8] += 0x38;
ptr[9] += 0x3e;
ptr[10] += 0x50;
ptr[11] += 0xfe;
ptr[12] += 0x15;
ptr[13] += 0x58;
ptr[14] += 0x4e;
ptr[15] ^= 0x3;
ptr[16] ^= 0x6d;
ptr[17] ^= 0xe5;
ptr[18] ^= 0x9f;
ptr[19] += 0xc5;
ptr[20] += 0x67;
ptr[21] += 0x29;
ptr[22] ^= 0x69;
ptr[23] ^= 0x3;

}

void multi_tracer_p93(struct user_regs_struct *regs)
{
if (regs->rax != 0x205d)
{
multi_tracer_p98(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9192c5 ^ regs->rbx);
ptr[0] += 0x13;
ptr[1] ^= 0xf2;
ptr[2] ^= 0x58;
ptr[3] += 0x87;
ptr[4] += 0x9f;
ptr[5] += 0x12;
ptr[6] ^= 0x5b;
ptr[7] ^= 0x4f;
ptr[8] += 0x3f;
ptr[9] += 0xbe;
ptr[10] ^= 0x14;
ptr[11] += 0x4c;
ptr[12] += 0xd6;
ptr[13] += 0xe4;
ptr[14] += 0xfe;
ptr[15] += 0x4d;
ptr[16] += 0x51;
ptr[17] += 0x6d;
ptr[18] += 0xe7;
ptr[19] += 0x88;
ptr[20] += 0x1f;
ptr[21] ^= 0x62;
ptr[22] ^= 0xa8;
ptr[23] += 0xc6;
ptr[24] += 0x52;

}

void multi_tracer_p98(struct user_regs_struct *regs)
{
if (regs->rax != 0x2062)
{
multi_tracer_p15(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7f1888 ^ regs->rbx);
ptr[0] ^= 0xeb;
ptr[1] ^= 0xbd;
ptr[2] ^= 0x4c;
ptr[3] += 0xfb;
ptr[4] ^= 0x1a;
ptr[5] += 0xfe;
ptr[6] ^= 0xb8;
ptr[7] ^= 0x5b;
ptr[8] += 0x8;
ptr[9] ^= 0x6d;
ptr[10] ^= 0x82;
ptr[11] ^= 0xea;
ptr[12] ^= 0x89;
ptr[13] ^= 0x72;
ptr[14] ^= 0xb;
ptr[15] += 0x85;
ptr[16] += 0x27;
ptr[17] ^= 0xa8;
ptr[18] ^= 0x5a;
ptr[19] += 0x4a;

}

void multi_tracer_p15(struct user_regs_struct *regs)
{
if (regs->rax != 0x200f)
{
multi_tracer_p63(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd9515f ^ regs->rbx);
ptr[0] ^= 0xe7;
ptr[1] += 0x63;
ptr[2] += 0xed;
ptr[3] ^= 0x20;
ptr[4] ^= 0xec;
ptr[5] ^= 0xf1;
ptr[6] ^= 0x5e;
ptr[7] += 0x9;
ptr[8] += 0x76;
ptr[9] ^= 0xe2;
ptr[10] ^= 0xe6;
ptr[11] += 0x57;
ptr[12] += 0x87;
ptr[13] ^= 0xc4;
ptr[14] ^= 0xcc;
ptr[15] += 0xef;
ptr[16] ^= 0xc;
ptr[17] += 0x5f;
ptr[18] ^= 0xd0;
ptr[19] ^= 0xe2;
ptr[20] ^= 0xd6;
ptr[21] ^= 0xb9;
ptr[22] += 0x9e;
ptr[23] ^= 0x6e;
ptr[24] ^= 0xac;

}

void multi_tracer_p63(struct user_regs_struct *regs)
{
if (regs->rax != 0x203f)
{
multi_tracer_p111(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x926692 ^ regs->rbx);
ptr[0] ^= 0x3f;
ptr[1] += 0x88;
ptr[2] += 0xda;
ptr[3] ^= 0x47;
ptr[4] += 0xef;
ptr[5] += 0x8d;
ptr[6] ^= 0xcb;
ptr[7] += 0x97;
ptr[8] += 0xdc;
ptr[9] += 0x9a;
ptr[10] ^= 0x81;
ptr[11] += 0x33;
ptr[12] += 0x15;
ptr[13] ^= 0x38;
ptr[14] ^= 0x2d;
ptr[15] ^= 0x90;
ptr[16] ^= 0xf;
ptr[17] += 0x59;
ptr[18] += 0x58;
ptr[19] += 0xcd;
ptr[20] ^= 0xd6;
ptr[21] += 0x68;
ptr[22] ^= 0xc5;
ptr[23] ^= 0x9d;
ptr[24] += 0xb3;
ptr[25] ^= 0xce;
ptr[26] += 0x2b;
ptr[27] ^= 0x27;

}

void multi_tracer_p111(struct user_regs_struct *regs)
{
if (regs->rax != 0x206f)
{
multi_tracer_p134(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x23d146 ^ regs->rbx);
ptr[0] += 0x5f;
ptr[1] += 0x21;
ptr[2] += 0xa3;
ptr[3] += 0x7b;
ptr[4] ^= 0x64;
ptr[5] += 0xab;
ptr[6] ^= 0x9a;
ptr[7] ^= 0x87;
ptr[8] ^= 0x66;
ptr[9] ^= 0x6b;
ptr[10] += 0xca;
ptr[11] += 0x48;
ptr[12] += 0xc1;
ptr[13] ^= 0x1f;
ptr[14] ^= 0x84;
ptr[15] += 0x9e;
ptr[16] += 0xac;
ptr[17] ^= 0x13;
ptr[18] += 0x16;

}

void multi_tracer_p134(struct user_regs_struct *regs)
{
if (regs->rax != 0x2086)
{
multi_tracer_p156(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd0ca3f ^ regs->rbx);
ptr[0] ^= 0x47;
ptr[1] ^= 0x83;
ptr[2] ^= 0xcd;
ptr[3] ^= 0xc6;
ptr[4] += 0xd1;
ptr[5] ^= 0x94;
ptr[6] ^= 0xc4;
ptr[7] ^= 0xc7;
ptr[8] ^= 0x30;
ptr[9] += 0xe5;
ptr[10] += 0x4c;
ptr[11] += 0xe9;
ptr[12] ^= 0x35;
ptr[13] += 0x69;
ptr[14] ^= 0x86;
ptr[15] += 0xb5;
ptr[16] ^= 0x85;
ptr[17] += 0x8b;
ptr[18] ^= 0xa3;
ptr[19] += 0x2d;
ptr[20] ^= 0xad;
ptr[21] += 0x28;
ptr[22] ^= 0x27;
ptr[23] ^= 0xd4;
ptr[24] ^= 0x48;
ptr[25] ^= 0x19;
ptr[26] += 0x9;
ptr[27] += 0x3a;

}

void multi_tracer_p156(struct user_regs_struct *regs)
{
if (regs->rax != 0x209c)
{
multi_tracer_p25(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3bab38 ^ regs->rbx);
ptr[0] ^= 0x7;
ptr[1] ^= 0x6d;
ptr[2] += 0x7b;
ptr[3] += 0xf6;
ptr[4] ^= 0xa9;
ptr[5] += 0x42;
ptr[6] += 0x2a;
ptr[7] += 0x1c;
ptr[8] ^= 0x50;
ptr[9] += 0xb3;
ptr[10] += 0xa8;
ptr[11] += 0x72;
ptr[12] += 0x1d;
ptr[13] += 0xaa;
ptr[14] += 0x2b;
ptr[15] += 0x90;
ptr[16] ^= 0x44;
ptr[17] += 0xbd;
ptr[18] += 0x1a;

}

void multi_tracer_p25(struct user_regs_struct *regs)
{
if (regs->rax != 0x2019)
{
multi_tracer_p211(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf7768b ^ regs->rbx);
ptr[0] += 0x29;
ptr[1] ^= 0x15;
ptr[2] += 0xce;
ptr[3] += 0x18;
ptr[4] ^= 0x61;
ptr[5] += 0xa3;
ptr[6] ^= 0x2b;
ptr[7] += 0x2d;
ptr[8] += 0xaa;
ptr[9] ^= 0x53;
ptr[10] ^= 0x28;
ptr[11] ^= 0x81;
ptr[12] ^= 0x76;
ptr[13] ^= 0x19;
ptr[14] ^= 0x12;
ptr[15] ^= 0xef;
ptr[16] += 0x95;
ptr[17] ^= 0xf5;
ptr[18] += 0xa2;
ptr[19] ^= 0x8f;
ptr[20] ^= 0x61;
ptr[21] += 0xc6;
ptr[22] += 0x1b;
ptr[23] += 0xae;
ptr[24] += 0x4b;
ptr[25] ^= 0x81;
ptr[26] ^= 0xf1;

}

void multi_tracer_p211(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d3)
{
multi_tracer_p167(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5c9a5c ^ regs->rbx);
ptr[0] += 0x94;
ptr[1] ^= 0xe3;
ptr[2] ^= 0xdc;
ptr[3] ^= 0xb3;
ptr[4] += 0x6d;
ptr[5] ^= 0xe7;
ptr[6] += 0xa4;
ptr[7] ^= 0x18;
ptr[8] += 0xa6;
ptr[9] ^= 0xbc;
ptr[10] += 0xf3;
ptr[11] ^= 0xc8;
ptr[12] += 0xc6;
ptr[13] ^= 0x5b;
ptr[14] ^= 0xf1;
ptr[15] += 0xd6;
ptr[16] += 0x3d;
ptr[17] ^= 0xd7;
ptr[18] += 0x10;
ptr[19] ^= 0x8c;
ptr[20] ^= 0x3a;
ptr[21] ^= 0xa7;
ptr[22] ^= 0x76;

}

void multi_tracer_p167(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a7)
{
multi_tracer_p118(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x621704 ^ regs->rbx);
ptr[0] ^= 0xc3;
ptr[1] += 0xaa;
ptr[2] += 0x35;
ptr[3] += 0x70;
ptr[4] += 0xa8;
ptr[5] += 0x90;
ptr[6] += 0xa5;
ptr[7] += 0x1c;
ptr[8] ^= 0x93;
ptr[9] ^= 0x7c;
ptr[10] ^= 0x4a;
ptr[11] ^= 0xf4;
ptr[12] ^= 0x38;
ptr[13] += 0xe1;
ptr[14] ^= 0xa;
ptr[15] ^= 0xe2;
ptr[16] += 0x33;
ptr[17] += 0xdd;
ptr[18] ^= 0xa7;

}

void multi_tracer_p118(struct user_regs_struct *regs)
{
if (regs->rax != 0x2076)
{
multi_tracer_p220(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3495ab ^ regs->rbx);
ptr[0] ^= 0x6;
ptr[1] += 0x28;
ptr[2] ^= 0xd2;
ptr[3] ^= 0x12;
ptr[4] ^= 0x13;
ptr[5] += 0x27;
ptr[6] += 0xb7;
ptr[7] ^= 0x3a;
ptr[8] ^= 0x15;
ptr[9] ^= 0x1f;
ptr[10] += 0x6a;
ptr[11] ^= 0xa5;
ptr[12] += 0x96;
ptr[13] ^= 0xcb;
ptr[14] ^= 0xea;
ptr[15] += 0x57;
ptr[16] += 0xfb;
ptr[17] += 0x82;
ptr[18] += 0x24;
ptr[19] ^= 0x4e;
ptr[20] ^= 0x5;
ptr[21] += 0xb2;
ptr[22] ^= 0xb1;
ptr[23] += 0x31;
ptr[24] ^= 0x27;
ptr[25] ^= 0x69;

}

void multi_tracer_p220(struct user_regs_struct *regs)
{
if (regs->rax != 0x20dc)
{
multi_tracer_p232(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbba9dc ^ regs->rbx);
ptr[0] += 0x72;
ptr[1] += 0xfb;
ptr[2] += 0xe6;
ptr[3] += 0xe6;
ptr[4] ^= 0x5b;
ptr[5] += 0x8;
ptr[6] += 0x3e;
ptr[7] += 0x6b;
ptr[8] += 0x7e;
ptr[9] ^= 0x2b;
ptr[10] ^= 0x7e;
ptr[11] += 0xc3;
ptr[12] += 0x24;
ptr[13] ^= 0x9e;
ptr[14] ^= 0x90;
ptr[15] += 0xd3;
ptr[16] ^= 0xa8;
ptr[17] += 0xe1;
ptr[18] += 0x84;

}

void multi_tracer_p232(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e8)
{
multi_tracer_p178(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3d02ef ^ regs->rbx);
ptr[0] ^= 0x19;
ptr[1] ^= 0x36;
ptr[2] += 0x88;
ptr[3] += 0x72;
ptr[4] ^= 0xdc;
ptr[5] += 0xa5;
ptr[6] += 0x50;
ptr[7] ^= 0xe;
ptr[8] ^= 0xdc;
ptr[9] ^= 0x6b;
ptr[10] ^= 0xba;
ptr[11] += 0x83;
ptr[12] += 0x9f;
ptr[13] += 0x1a;
ptr[14] += 0x38;
ptr[15] += 0xd2;
ptr[16] ^= 0x1b;
ptr[17] += 0x38;
ptr[18] ^= 0x87;
ptr[19] += 0xc9;
ptr[20] += 0xd6;
ptr[21] ^= 0x13;

}

void multi_tracer_p178(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b2)
{
multi_tracer_p229(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3d57d3 ^ regs->rbx);
ptr[0] += 0x37;
ptr[1] ^= 0x26;
ptr[2] += 0x87;
ptr[3] += 0xe6;
ptr[4] += 0x5c;
ptr[5] ^= 0x4;
ptr[6] += 0xd2;
ptr[7] ^= 0xe3;
ptr[8] ^= 0x36;
ptr[9] += 0x17;
ptr[10] += 0xa8;
ptr[11] += 0xd4;
ptr[12] ^= 0x43;
ptr[13] ^= 0x3f;
ptr[14] += 0x6d;
ptr[15] += 0x50;
ptr[16] += 0xd5;
ptr[17] += 0xca;
ptr[18] += 0xf4;
ptr[19] += 0x59;
ptr[20] += 0x1;
ptr[21] ^= 0x2a;
ptr[22] += 0x44;
ptr[23] ^= 0xeb;
ptr[24] += 0x55;

}

void multi_tracer_p229(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e5)
{
multi_tracer_p239(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8adc3a ^ regs->rbx);
ptr[0] ^= 0x4;
ptr[1] ^= 0xff;
ptr[2] ^= 0xce;
ptr[3] ^= 0xd9;
ptr[4] += 0xb4;
ptr[5] += 0xb5;
ptr[6] += 0x28;
ptr[7] ^= 0x60;
ptr[8] += 0x57;
ptr[9] += 0x9d;
ptr[10] ^= 0x3d;
ptr[11] += 0x12;
ptr[12] ^= 0x74;
ptr[13] ^= 0xb0;
ptr[14] ^= 0xcd;
ptr[15] ^= 0xc7;
ptr[16] += 0x83;
ptr[17] += 0x3;
ptr[18] += 0xbe;
ptr[19] += 0x8e;
ptr[20] ^= 0xe3;
ptr[21] ^= 0x1c;
ptr[22] += 0xa7;
ptr[23] += 0x11;

}

void multi_tracer_p239(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ef)
{
multi_tracer_p205(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x12457d ^ regs->rbx);
ptr[0] += 0x19;
ptr[1] += 0x6e;
ptr[2] ^= 0x1;
ptr[3] ^= 0xae;
ptr[4] += 0x35;
ptr[5] += 0xeb;
ptr[6] ^= 0x80;
ptr[7] ^= 0x65;
ptr[8] += 0x92;
ptr[9] ^= 0xc8;
ptr[10] ^= 0x3d;
ptr[11] ^= 0x83;
ptr[12] += 0x21;
ptr[13] ^= 0x1;
ptr[14] ^= 0x64;
ptr[15] += 0x7e;
ptr[16] += 0x59;
ptr[17] += 0x11;
ptr[18] += 0x41;
ptr[19] ^= 0x7c;
ptr[20] ^= 0xbb;
ptr[21] += 0xf0;
ptr[22] += 0x14;

}

void multi_tracer_p205(struct user_regs_struct *regs)
{
if (regs->rax != 0x20cd)
{
multi_tracer_p61(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x47051d ^ regs->rbx);
ptr[0] += 0xbb;
ptr[1] += 0x61;
ptr[2] ^= 0x6c;
ptr[3] += 0x40;
ptr[4] += 0x49;
ptr[5] += 0x9c;
ptr[6] += 0x86;
ptr[7] ^= 0x30;
ptr[8] += 0x6d;
ptr[9] += 0x23;
ptr[10] += 0xa2;
ptr[11] ^= 0x19;
ptr[12] ^= 0x9c;
ptr[13] += 0x8e;
ptr[14] ^= 0xe;
ptr[15] += 0x84;
ptr[16] ^= 0xe5;
ptr[17] ^= 0x40;
ptr[18] += 0xce;
ptr[19] ^= 0x7c;
ptr[20] ^= 0xe6;
ptr[21] ^= 0xb1;
ptr[22] ^= 0xfb;
ptr[23] += 0x1a;
ptr[24] ^= 0xe2;
ptr[25] ^= 0x43;

}

void multi_tracer_p61(struct user_regs_struct *regs)
{
if (regs->rax != 0x203d)
{
multi_tracer_p37(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x28ab5b ^ regs->rbx);
ptr[0] ^= 0x7e;
ptr[1] += 0x9b;
ptr[2] += 0xfb;
ptr[3] += 0x71;
ptr[4] ^= 0x81;
ptr[5] ^= 0x53;
ptr[6] ^= 0x62;
ptr[7] ^= 0x70;
ptr[8] ^= 0x7;
ptr[9] ^= 0x7d;
ptr[10] ^= 0x1a;
ptr[11] ^= 0x4b;
ptr[12] ^= 0x26;
ptr[13] ^= 0xd;
ptr[14] ^= 0x66;
ptr[15] += 0x42;
ptr[16] += 0x5d;
ptr[17] ^= 0x2;
ptr[18] ^= 0xa1;
ptr[19] += 0x3a;
ptr[20] += 0x91;
ptr[21] += 0xd7;

}

void multi_tracer_p37(struct user_regs_struct *regs)
{
if (regs->rax != 0x2025)
{
multi_tracer_p41(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x16c726 ^ regs->rbx);
ptr[0] ^= 0x39;
ptr[1] += 0xb2;
ptr[2] ^= 0x2c;
ptr[3] += 0x3b;
ptr[4] ^= 0xe7;
ptr[5] ^= 0x6d;
ptr[6] ^= 0xbc;
ptr[7] += 0x9;
ptr[8] += 0xc5;
ptr[9] ^= 0x85;
ptr[10] += 0x94;
ptr[11] += 0xbd;
ptr[12] += 0x8f;
ptr[13] ^= 0x2b;
ptr[14] ^= 0x19;
ptr[15] ^= 0x72;
ptr[16] ^= 0xf7;
ptr[17] += 0xca;
ptr[18] ^= 0xa3;
ptr[19] += 0x2a;
ptr[20] ^= 0xa;
ptr[21] += 0xbc;
ptr[22] ^= 0x2a;
ptr[23] ^= 0xaf;
ptr[24] += 0xd4;

}

void multi_tracer_p41(struct user_regs_struct *regs)
{
if (regs->rax != 0x2029)
{
multi_tracer_p77(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xcd50d1 ^ regs->rbx);
ptr[0] += 0x97;
ptr[1] += 0x1a;
ptr[2] += 0x5c;
ptr[3] += 0xaa;
ptr[4] += 0x67;
ptr[5] += 0x91;
ptr[6] ^= 0xec;
ptr[7] += 0xd2;
ptr[8] += 0xe1;
ptr[9] += 0x55;
ptr[10] += 0xf7;
ptr[11] ^= 0x7b;
ptr[12] ^= 0x2b;
ptr[13] ^= 0x85;
ptr[14] ^= 0x39;
ptr[15] ^= 0x8;
ptr[16] += 0xd9;
ptr[17] ^= 0x87;
ptr[18] += 0x40;
ptr[19] += 0x6c;
ptr[20] ^= 0x9b;
ptr[21] += 0x4b;
ptr[22] ^= 0xf9;
ptr[23] ^= 0x12;
ptr[24] ^= 0xd4;
ptr[25] += 0x2a;
ptr[26] += 0xa4;
ptr[27] ^= 0x87;

}

void multi_tracer_p77(struct user_regs_struct *regs)
{
if (regs->rax != 0x204d)
{
multi_tracer_p5(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xdeb41d ^ regs->rbx);
ptr[0] += 0xd7;
ptr[1] ^= 0x5;
ptr[2] ^= 0x56;
ptr[3] ^= 0x20;
ptr[4] ^= 0x66;
ptr[5] ^= 0xe1;
ptr[6] += 0xb5;
ptr[7] ^= 0x6b;
ptr[8] += 0xea;
ptr[9] += 0x23;
ptr[10] += 0x3f;
ptr[11] += 0xac;
ptr[12] += 0xb0;
ptr[13] += 0x20;
ptr[14] += 0x79;
ptr[15] ^= 0xc8;
ptr[16] += 0x1;
ptr[17] ^= 0x23;
ptr[18] += 0x4e;
ptr[19] += 0x22;
ptr[20] ^= 0x17;
ptr[21] += 0x65;

}

void multi_tracer_p5(struct user_regs_struct *regs)
{
if (regs->rax != 0x2005)
{
multi_tracer_p153(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9fbb01 ^ regs->rbx);
ptr[0] ^= 0x50;
ptr[1] += 0xe6;
ptr[2] += 0x70;
ptr[3] ^= 0xc3;
ptr[4] += 0x52;
ptr[5] ^= 0xd4;
ptr[6] += 0xd;
ptr[7] += 0xd1;
ptr[8] ^= 0xb2;
ptr[9] ^= 0x53;
ptr[10] ^= 0x4a;
ptr[11] ^= 0x18;
ptr[12] ^= 0x3f;
ptr[13] += 0x1f;
ptr[14] ^= 0x3c;
ptr[15] += 0xd9;
ptr[16] ^= 0x1b;
ptr[17] ^= 0x11;
ptr[18] += 0x83;
ptr[19] += 0x2a;
ptr[20] ^= 0x38;
ptr[21] ^= 0xb6;
ptr[22] += 0x81;
ptr[23] += 0x1b;
ptr[24] += 0x72;
ptr[25] ^= 0x2b;
ptr[26] += 0xd9;
ptr[27] ^= 0x29;

}

void multi_tracer_p153(struct user_regs_struct *regs)
{
if (regs->rax != 0x2099)
{
multi_tracer_p73(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x520a8e ^ regs->rbx);
ptr[0] += 0x97;
ptr[1] += 0xcf;
ptr[2] ^= 0xff;
ptr[3] += 0xad;
ptr[4] ^= 0x14;
ptr[5] += 0x3a;
ptr[6] += 0x69;
ptr[7] ^= 0x47;
ptr[8] += 0x90;
ptr[9] += 0x98;
ptr[10] += 0xc0;
ptr[11] ^= 0x1;
ptr[12] ^= 0x53;
ptr[13] += 0x35;
ptr[14] ^= 0x85;
ptr[15] ^= 0x1a;
ptr[16] += 0xb4;
ptr[17] ^= 0xc3;
ptr[18] += 0x3;
ptr[19] ^= 0xe1;
ptr[20] += 0xe2;
ptr[21] ^= 0x73;
ptr[22] ^= 0xf1;
ptr[23] ^= 0x50;
ptr[24] += 0x58;
ptr[25] += 0xcb;
ptr[26] ^= 0x9c;
ptr[27] ^= 0xf9;

}

void multi_tracer_p73(struct user_regs_struct *regs)
{
if (regs->rax != 0x2049)
{
multi_tracer_p125(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3b226c ^ regs->rbx);
ptr[0] += 0x1c;
ptr[1] ^= 0xa9;
ptr[2] += 0xe;
ptr[3] += 0xce;
ptr[4] += 0x52;
ptr[5] ^= 0xa7;
ptr[6] ^= 0xb7;
ptr[7] += 0x51;
ptr[8] ^= 0xad;
ptr[9] += 0x2d;
ptr[10] += 0xeb;
ptr[11] ^= 0x84;
ptr[12] += 0xd3;
ptr[13] += 0xdb;
ptr[14] += 0xfe;
ptr[15] += 0x16;
ptr[16] += 0xcb;
ptr[17] ^= 0xee;
ptr[18] ^= 0xcb;

}

void multi_tracer_p125(struct user_regs_struct *regs)
{
if (regs->rax != 0x207d)
{
multi_tracer_p161(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xab6b49 ^ regs->rbx);
ptr[0] ^= 0xe3;
ptr[1] ^= 0xe4;
ptr[2] ^= 0x7a;
ptr[3] += 0xef;
ptr[4] ^= 0x4b;
ptr[5] += 0xf3;
ptr[6] ^= 0x7e;
ptr[7] += 0x3f;
ptr[8] += 0x83;
ptr[9] ^= 0xeb;
ptr[10] ^= 0x6a;
ptr[11] ^= 0xa6;
ptr[12] += 0x7;
ptr[13] += 0x7f;
ptr[14] ^= 0xa5;
ptr[15] ^= 0x5d;
ptr[16] += 0x4f;
ptr[17] += 0x5c;

}

void multi_tracer_p161(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a1)
{
multi_tracer_p59(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc70ecb ^ regs->rbx);
ptr[0] ^= 0x7f;
ptr[1] ^= 0xfd;
ptr[2] ^= 0x4a;
ptr[3] += 0x97;
ptr[4] += 0x3f;
ptr[5] += 0xa;
ptr[6] += 0xca;
ptr[7] += 0xb6;
ptr[8] += 0x8;
ptr[9] ^= 0xd6;
ptr[10] += 0x7b;
ptr[11] += 0x84;
ptr[12] ^= 0x2f;
ptr[13] ^= 0xce;
ptr[14] ^= 0xad;
ptr[15] += 0xd7;
ptr[16] += 0xc6;
ptr[17] += 0xb;
ptr[18] += 0xef;
ptr[19] ^= 0xf;
ptr[20] += 0xc1;
ptr[21] += 0xce;
ptr[22] += 0xd1;
ptr[23] ^= 0xcc;

}

void multi_tracer_p59(struct user_regs_struct *regs)
{
if (regs->rax != 0x203b)
{
multi_tracer_p141(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xad51f3 ^ regs->rbx);
ptr[0] ^= 0x1;
ptr[1] += 0xd1;
ptr[2] ^= 0x8d;
ptr[3] ^= 0xa;
ptr[4] ^= 0x89;
ptr[5] ^= 0x62;
ptr[6] += 0x2;
ptr[7] += 0xcc;
ptr[8] ^= 0x78;
ptr[9] += 0x3d;
ptr[10] += 0x49;
ptr[11] ^= 0x84;
ptr[12] += 0x24;
ptr[13] += 0xc5;
ptr[14] += 0xd;
ptr[15] += 0xb2;
ptr[16] ^= 0x45;
ptr[17] ^= 0xb0;
ptr[18] += 0xfd;
ptr[19] ^= 0x6b;
ptr[20] ^= 0x1f;
ptr[21] += 0x64;
ptr[22] ^= 0x55;

}

void multi_tracer_p141(struct user_regs_struct *regs)
{
if (regs->rax != 0x208d)
{
multi_tracer_p108(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x204f8f ^ regs->rbx);
ptr[0] ^= 0xcf;
ptr[1] += 0x49;
ptr[2] += 0x80;
ptr[3] += 0x62;
ptr[4] ^= 0xba;
ptr[5] += 0x48;
ptr[6] ^= 0xbf;
ptr[7] ^= 0x1c;
ptr[8] ^= 0x8b;
ptr[9] += 0x6d;
ptr[10] ^= 0x5e;
ptr[11] ^= 0xd9;
ptr[12] += 0x62;
ptr[13] ^= 0xb;
ptr[14] += 0xbe;
ptr[15] ^= 0xdf;
ptr[16] += 0x85;
ptr[17] += 0x68;
ptr[18] += 0x68;
ptr[19] ^= 0x2c;
ptr[20] += 0xf2;
ptr[21] += 0x1d;
ptr[22] += 0x7d;

}

void multi_tracer_p108(struct user_regs_struct *regs)
{
if (regs->rax != 0x206c)
{
multi_tracer_p206(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x21ea53 ^ regs->rbx);
ptr[0] ^= 0x36;
ptr[1] += 0x4;
ptr[2] ^= 0x92;
ptr[3] ^= 0x6f;
ptr[4] ^= 0xb6;
ptr[5] += 0x89;
ptr[6] ^= 0xd2;
ptr[7] ^= 0xf5;
ptr[8] ^= 0x85;
ptr[9] += 0x93;
ptr[10] ^= 0x43;
ptr[11] += 0xa;
ptr[12] += 0xf0;
ptr[13] += 0x86;
ptr[14] += 0x2d;
ptr[15] += 0x80;
ptr[16] += 0x2f;
ptr[17] += 0x11;

}

void multi_tracer_p206(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ce)
{
multi_tracer_p52(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5106fb ^ regs->rbx);
ptr[0] += 0x13;
ptr[1] += 0x4c;
ptr[2] ^= 0x11;
ptr[3] ^= 0xc7;
ptr[4] ^= 0x78;
ptr[5] += 0x68;
ptr[6] += 0xd8;
ptr[7] += 0xba;
ptr[8] += 0xe4;
ptr[9] += 0xe4;
ptr[10] += 0x35;
ptr[11] += 0x97;
ptr[12] ^= 0x27;
ptr[13] += 0xad;
ptr[14] ^= 0x79;
ptr[15] += 0xf1;
ptr[16] ^= 0xf9;
ptr[17] ^= 0xf5;
ptr[18] ^= 0x1f;
ptr[19] ^= 0xdf;
ptr[20] ^= 0x4b;
ptr[21] += 0x5;
ptr[22] += 0x2a;
ptr[23] ^= 0x5e;
ptr[24] ^= 0xb1;
ptr[25] += 0x79;
ptr[26] ^= 0xaf;
ptr[27] ^= 0x92;

}

void multi_tracer_p52(struct user_regs_struct *regs)
{
if (regs->rax != 0x2034)
{
multi_tracer_p230(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2483c5 ^ regs->rbx);
ptr[0] += 0x31;
ptr[1] += 0x31;
ptr[2] += 0x6d;
ptr[3] ^= 0x90;
ptr[4] += 0xd3;
ptr[5] ^= 0xfc;
ptr[6] ^= 0x12;
ptr[7] ^= 0xa7;
ptr[8] += 0xd9;
ptr[9] ^= 0xf9;
ptr[10] ^= 0xfb;
ptr[11] += 0x9b;
ptr[12] += 0x43;
ptr[13] += 0x7;
ptr[14] ^= 0xce;
ptr[15] ^= 0x95;
ptr[16] += 0x21;
ptr[17] += 0xb1;
ptr[18] ^= 0x90;
ptr[19] += 0x75;

}

void multi_tracer_p230(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e6)
{
multi_tracer_p154(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xa970cf ^ regs->rbx);
ptr[0] += 0x7e;
ptr[1] += 0xb5;
ptr[2] ^= 0x62;
ptr[3] += 0x2f;
ptr[4] += 0xcd;
ptr[5] ^= 0x7c;
ptr[6] += 0x8f;
ptr[7] += 0x6d;
ptr[8] ^= 0x6b;
ptr[9] += 0xba;
ptr[10] ^= 0x59;
ptr[11] ^= 0xe3;
ptr[12] += 0x56;
ptr[13] += 0xfb;
ptr[14] ^= 0x85;
ptr[15] ^= 0x44;
ptr[16] += 0x84;
ptr[17] ^= 0xd1;
ptr[18] += 0x1;
ptr[19] ^= 0xbc;
ptr[20] ^= 0xa9;

}

void multi_tracer_p154(struct user_regs_struct *regs)
{
if (regs->rax != 0x209a)
{
multi_tracer_p28(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc885b1 ^ regs->rbx);
ptr[0] ^= 0x8;
ptr[1] ^= 0x28;
ptr[2] ^= 0x3f;
ptr[3] += 0x62;
ptr[4] += 0x11;
ptr[5] += 0x69;
ptr[6] ^= 0x5c;
ptr[7] += 0xb1;
ptr[8] += 0x56;
ptr[9] += 0xe0;
ptr[10] += 0x72;
ptr[11] ^= 0xf5;
ptr[12] += 0x4f;
ptr[13] ^= 0xc;
ptr[14] += 0x4b;
ptr[15] ^= 0x38;
ptr[16] ^= 0x1a;
ptr[17] += 0x79;
ptr[18] ^= 0xde;
ptr[19] += 0x13;
ptr[20] ^= 0xbe;
ptr[21] += 0xaf;
ptr[22] ^= 0x86;
ptr[23] += 0x3f;

}

void multi_tracer_p28(struct user_regs_struct *regs)
{
if (regs->rax != 0x201c)
{
multi_tracer_p35(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe903a2 ^ regs->rbx);
ptr[0] ^= 0xe6;
ptr[1] += 0x3a;
ptr[2] ^= 0xdd;
ptr[3] ^= 0x9;
ptr[4] += 0x83;
ptr[5] += 0xb2;
ptr[6] += 0x2e;
ptr[7] ^= 0xa4;
ptr[8] ^= 0x4c;
ptr[9] ^= 0xe2;
ptr[10] += 0x11;
ptr[11] ^= 0x2c;
ptr[12] += 0x66;
ptr[13] ^= 0x7e;
ptr[14] += 0x23;
ptr[15] += 0x25;
ptr[16] ^= 0xbd;
ptr[17] ^= 0xaf;
ptr[18] += 0xe1;
ptr[19] ^= 0xee;
ptr[20] ^= 0x84;
ptr[21] ^= 0x23;
ptr[22] += 0xbf;
ptr[23] ^= 0x28;

}

void multi_tracer_p35(struct user_regs_struct *regs)
{
if (regs->rax != 0x2023)
{
multi_tracer_p86(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xfd9fa7 ^ regs->rbx);
ptr[0] += 0xc;
ptr[1] ^= 0x53;
ptr[2] ^= 0xa5;
ptr[3] += 0x41;
ptr[4] ^= 0xf4;
ptr[5] ^= 0x43;
ptr[6] ^= 0xc8;
ptr[7] ^= 0x62;
ptr[8] += 0x5c;
ptr[9] ^= 0xf;
ptr[10] ^= 0xd6;
ptr[11] += 0xf9;
ptr[12] ^= 0xb1;
ptr[13] ^= 0x54;
ptr[14] += 0x8c;
ptr[15] += 0xa9;
ptr[16] ^= 0xcf;
ptr[17] ^= 0xe;
ptr[18] ^= 0x6;
ptr[19] ^= 0xb;
ptr[20] ^= 0xb5;
ptr[21] ^= 0x5d;
ptr[22] += 0x3c;
ptr[23] += 0x3c;
ptr[24] ^= 0x4c;

}

void multi_tracer_p86(struct user_regs_struct *regs)
{
if (regs->rax != 0x2056)
{
multi_tracer_p146(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xa83201 ^ regs->rbx);
ptr[0] += 0xf8;
ptr[1] ^= 0xd9;
ptr[2] ^= 0xe5;
ptr[3] += 0x6c;
ptr[4] += 0xab;
ptr[5] += 0x79;
ptr[6] ^= 0x97;
ptr[7] ^= 0xc9;
ptr[8] += 0xaa;
ptr[9] += 0xbb;
ptr[10] += 0x27;
ptr[11] ^= 0xb7;
ptr[12] += 0x7c;
ptr[13] += 0xd0;
ptr[14] += 0xbc;
ptr[15] += 0x25;
ptr[16] ^= 0x47;
ptr[17] ^= 0x51;
ptr[18] ^= 0xd5;
ptr[19] += 0xd5;
ptr[20] ^= 0xa;
ptr[21] ^= 0x9d;
ptr[22] ^= 0xa;
ptr[23] += 0xce;
ptr[24] += 0xa7;
ptr[25] += 0x98;

}

void multi_tracer_p146(struct user_regs_struct *regs)
{
if (regs->rax != 0x2092)
{
multi_tracer_p123(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd96c01 ^ regs->rbx);
ptr[0] += 0xf2;
ptr[1] ^= 0xb4;
ptr[2] ^= 0x85;
ptr[3] += 0x92;
ptr[4] ^= 0xc9;
ptr[5] += 0x4a;
ptr[6] ^= 0x4c;
ptr[7] += 0x76;
ptr[8] ^= 0x4d;
ptr[9] += 0xd8;
ptr[10] += 0xab;
ptr[11] ^= 0x8c;
ptr[12] += 0x1d;
ptr[13] ^= 0xdb;
ptr[14] += 0x8c;
ptr[15] += 0xa6;
ptr[16] ^= 0xa2;
ptr[17] ^= 0x5a;
ptr[18] += 0xb6;
ptr[19] += 0xc3;
ptr[20] += 0xcc;
ptr[21] += 0x89;
ptr[22] += 0x40;
ptr[23] ^= 0xb9;
ptr[24] ^= 0x73;
ptr[25] ^= 0x11;
ptr[26] += 0x23;

}

void multi_tracer_p123(struct user_regs_struct *regs)
{
if (regs->rax != 0x207b)
{
multi_tracer_p142(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xeb900d ^ regs->rbx);
ptr[0] += 0x6b;
ptr[1] += 0x96;
ptr[2] += 0x7;
ptr[3] ^= 0xb0;
ptr[4] ^= 0xb6;
ptr[5] ^= 0xc7;
ptr[6] ^= 0xff;
ptr[7] ^= 0xcd;
ptr[8] ^= 0x47;
ptr[9] += 0x95;
ptr[10] += 0xec;
ptr[11] ^= 0x73;
ptr[12] ^= 0x83;
ptr[13] ^= 0xcd;
ptr[14] += 0x90;
ptr[15] ^= 0x86;
ptr[16] ^= 0x15;
ptr[17] ^= 0xbf;
ptr[18] += 0x65;
ptr[19] ^= 0x1b;
ptr[20] += 0x7f;
ptr[21] += 0x39;
ptr[22] ^= 0x72;

}

void multi_tracer_p142(struct user_regs_struct *regs)
{
if (regs->rax != 0x208e)
{
multi_tracer_p225(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x1ede90 ^ regs->rbx);
ptr[0] ^= 0x7c;
ptr[1] += 0xa5;
ptr[2] ^= 0xc2;
ptr[3] ^= 0x56;
ptr[4] ^= 0x7;
ptr[5] ^= 0x37;
ptr[6] += 0x79;
ptr[7] ^= 0x4f;
ptr[8] ^= 0x2;
ptr[9] += 0xdf;
ptr[10] += 0x9d;
ptr[11] ^= 0xbc;
ptr[12] += 0xed;
ptr[13] ^= 0xf1;
ptr[14] ^= 0x42;
ptr[15] += 0x16;
ptr[16] += 0x3d;
ptr[17] ^= 0x52;
ptr[18] += 0xbd;
ptr[19] += 0xfe;
ptr[20] += 0x9e;
ptr[21] += 0xcc;
ptr[22] += 0x52;
ptr[23] ^= 0x8d;
ptr[24] += 0x8c;
ptr[25] ^= 0x2f;

}

void multi_tracer_p225(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e1)
{
multi_tracer_p185(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc064df ^ regs->rbx);
ptr[0] += 0xc5;
ptr[1] ^= 0x13;
ptr[2] ^= 0x2c;
ptr[3] ^= 0x90;
ptr[4] ^= 0x88;
ptr[5] += 0xb;
ptr[6] ^= 0x8b;
ptr[7] ^= 0x8c;
ptr[8] += 0x31;
ptr[9] += 0x40;
ptr[10] += 0x39;
ptr[11] += 0xd6;
ptr[12] ^= 0x8c;
ptr[13] += 0x43;
ptr[14] ^= 0x89;
ptr[15] += 0x50;
ptr[16] ^= 0xa2;
ptr[17] ^= 0x9b;
ptr[18] ^= 0x1f;
ptr[19] += 0x9b;
ptr[20] ^= 0x8d;
ptr[21] += 0x8a;

}

void multi_tracer_p185(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b9)
{
multi_tracer_p121(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x638146 ^ regs->rbx);
ptr[0] ^= 0x8f;
ptr[1] += 0x6;
ptr[2] += 0x31;
ptr[3] ^= 0x8;
ptr[4] += 0xb8;
ptr[5] += 0x35;
ptr[6] += 0x5;
ptr[7] += 0xc0;
ptr[8] ^= 0x4c;
ptr[9] += 0x35;
ptr[10] += 0xa6;
ptr[11] += 0x56;
ptr[12] ^= 0xcc;
ptr[13] += 0x2d;
ptr[14] ^= 0x9b;
ptr[15] += 0x98;
ptr[16] += 0xb8;
ptr[17] += 0xac;

}

void multi_tracer_p121(struct user_regs_struct *regs)
{
if (regs->rax != 0x2079)
{
multi_tracer_p183(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x197665 ^ regs->rbx);
ptr[0] ^= 0x15;
ptr[1] ^= 0x83;
ptr[2] += 0x50;
ptr[3] += 0xd2;
ptr[4] += 0x32;
ptr[5] += 0xde;
ptr[6] ^= 0x1a;
ptr[7] += 0xf3;
ptr[8] += 0x1;
ptr[9] += 0xce;
ptr[10] ^= 0xdd;
ptr[11] ^= 0x38;
ptr[12] += 0xee;
ptr[13] += 0xdc;
ptr[14] += 0xa9;
ptr[15] ^= 0x6a;
ptr[16] += 0xb;
ptr[17] ^= 0xf;
ptr[18] += 0xb;

}

void multi_tracer_p183(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b7)
{
multi_tracer_p56(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x331f61 ^ regs->rbx);
ptr[0] ^= 0x8b;
ptr[1] += 0x84;
ptr[2] ^= 0xf3;
ptr[3] ^= 0x71;
ptr[4] ^= 0xf0;
ptr[5] += 0x83;
ptr[6] += 0xaa;
ptr[7] ^= 0xb5;
ptr[8] ^= 0x22;
ptr[9] += 0xa4;
ptr[10] ^= 0xee;
ptr[11] += 0xf4;
ptr[12] += 0x28;
ptr[13] ^= 0xd9;
ptr[14] += 0x20;
ptr[15] += 0x73;
ptr[16] ^= 0x1a;
ptr[17] ^= 0xab;
ptr[18] += 0xc;

}

void multi_tracer_p56(struct user_regs_struct *regs)
{
if (regs->rax != 0x2038)
{
multi_tracer_p140(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x42f5a7 ^ regs->rbx);
ptr[0] += 0x4b;
ptr[1] ^= 0x39;
ptr[2] += 0xec;
ptr[3] += 0xf1;
ptr[4] += 0x9c;
ptr[5] += 0x38;
ptr[6] += 0xae;
ptr[7] ^= 0xac;
ptr[8] += 0x87;
ptr[9] ^= 0xe4;
ptr[10] ^= 0xb;
ptr[11] ^= 0x6a;
ptr[12] ^= 0x5b;
ptr[13] ^= 0x5a;
ptr[14] += 0x2e;
ptr[15] ^= 0xd9;
ptr[16] += 0x75;
ptr[17] ^= 0x89;
ptr[18] ^= 0x99;
ptr[19] ^= 0xf0;
ptr[20] ^= 0x5;
ptr[21] += 0x4e;
ptr[22] ^= 0x79;
ptr[23] += 0xe3;
ptr[24] ^= 0x20;

}

void multi_tracer_p140(struct user_regs_struct *regs)
{
if (regs->rax != 0x208c)
{
multi_tracer_p8(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xa368ad ^ regs->rbx);
ptr[0] ^= 0xa3;
ptr[1] += 0xb1;
ptr[2] += 0xc6;
ptr[3] ^= 0xf8;
ptr[4] ^= 0x50;
ptr[5] += 0xee;
ptr[6] += 0x31;
ptr[7] += 0xc8;
ptr[8] += 0xb3;
ptr[9] ^= 0xca;
ptr[10] ^= 0x65;
ptr[11] += 0x2e;
ptr[12] += 0x6d;
ptr[13] += 0xe3;
ptr[14] ^= 0x5b;
ptr[15] ^= 0x8;
ptr[16] += 0xef;
ptr[17] += 0x30;
ptr[18] ^= 0x61;
ptr[19] ^= 0xc1;
ptr[20] += 0xd9;
ptr[21] ^= 0x84;
ptr[22] ^= 0x4c;
ptr[23] ^= 0x6a;
ptr[24] += 0xae;
ptr[25] += 0x23;

}

void multi_tracer_p8(struct user_regs_struct *regs)
{
if (regs->rax != 0x2008)
{
multi_tracer_p64(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9c0d70 ^ regs->rbx);
ptr[0] += 0x99;
ptr[1] += 0x9d;
ptr[2] ^= 0x3f;
ptr[3] += 0x77;
ptr[4] += 0xb8;
ptr[5] += 0xda;
ptr[6] ^= 0xbc;
ptr[7] ^= 0x87;
ptr[8] += 0x94;
ptr[9] ^= 0xdc;
ptr[10] ^= 0xae;
ptr[11] += 0x27;
ptr[12] ^= 0x6c;
ptr[13] += 0x72;
ptr[14] += 0xb6;
ptr[15] += 0xf0;
ptr[16] ^= 0xa1;
ptr[17] += 0xf3;
ptr[18] += 0xc9;
ptr[19] ^= 0x9d;
ptr[20] += 0x36;
ptr[21] += 0xfd;
ptr[22] ^= 0xf4;
ptr[23] += 0x98;

}

void multi_tracer_p64(struct user_regs_struct *regs)
{
if (regs->rax != 0x2040)
{
multi_tracer_p187(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x369174 ^ regs->rbx);
ptr[0] ^= 0xe4;
ptr[1] ^= 0x36;
ptr[2] ^= 0xd;
ptr[3] += 0x53;
ptr[4] += 0xd8;
ptr[5] += 0x70;
ptr[6] ^= 0x40;
ptr[7] ^= 0x74;
ptr[8] ^= 0x6f;
ptr[9] += 0x27;
ptr[10] ^= 0xa9;
ptr[11] ^= 0xaa;
ptr[12] ^= 0x1b;
ptr[13] ^= 0x8a;
ptr[14] += 0x6b;
ptr[15] += 0x79;
ptr[16] ^= 0x94;
ptr[17] += 0x63;
ptr[18] += 0x45;
ptr[19] += 0xea;
ptr[20] += 0x5;

}

void multi_tracer_p187(struct user_regs_struct *regs)
{
if (regs->rax != 0x20bb)
{
multi_tracer_p1(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8ed079 ^ regs->rbx);
ptr[0] += 0x29;
ptr[1] ^= 0xbd;
ptr[2] ^= 0xc2;
ptr[3] += 0x9a;
ptr[4] += 0x36;
ptr[5] ^= 0xa7;
ptr[6] ^= 0x8d;
ptr[7] ^= 0x19;
ptr[8] ^= 0xa0;
ptr[9] += 0xa7;
ptr[10] += 0x66;
ptr[11] ^= 0x18;
ptr[12] ^= 0xf3;
ptr[13] += 0x87;
ptr[14] ^= 0x49;
ptr[15] ^= 0x18;
ptr[16] += 0x94;
ptr[17] += 0xf4;

}

void multi_tracer_p1(struct user_regs_struct *regs)
{
if (regs->rax != 0x2001)
{
multi_tracer_p217(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb56f70 ^ regs->rbx);
ptr[0] ^= 0x23;
ptr[1] ^= 0x95;
ptr[2] ^= 0xff;
ptr[3] ^= 0x7b;
ptr[4] ^= 0x3b;
ptr[5] ^= 0xfe;
ptr[6] ^= 0x79;
ptr[7] += 0xae;
ptr[8] += 0x88;
ptr[9] ^= 0xd8;
ptr[10] += 0x90;
ptr[11] += 0xc0;
ptr[12] += 0xa9;
ptr[13] += 0xad;
ptr[14] ^= 0x16;
ptr[15] += 0x3;
ptr[16] ^= 0xfe;
ptr[17] += 0x6c;
ptr[18] ^= 0x29;
ptr[19] ^= 0x17;
ptr[20] ^= 0x82;
ptr[21] += 0x2f;
ptr[22] ^= 0x4;
ptr[23] ^= 0x92;
ptr[24] += 0x1;

}

void multi_tracer_p217(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d9)
{
multi_tracer_p212(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd57a96 ^ regs->rbx);
ptr[0] ^= 0xdf;
ptr[1] += 0xce;
ptr[2] += 0xce;
ptr[3] ^= 0xbc;
ptr[4] ^= 0x9c;
ptr[5] ^= 0x5e;
ptr[6] += 0xb1;
ptr[7] += 0x63;
ptr[8] += 0x2e;
ptr[9] ^= 0x32;
ptr[10] ^= 0x32;
ptr[11] ^= 0xa3;
ptr[12] += 0xd9;
ptr[13] += 0x90;
ptr[14] ^= 0xeb;
ptr[15] += 0xbc;
ptr[16] += 0x67;
ptr[17] ^= 0xe;
ptr[18] += 0x4a;
ptr[19] ^= 0xe6;
ptr[20] ^= 0x7f;

}

void multi_tracer_p212(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d4)
{
multi_tracer_p85(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xdb6940 ^ regs->rbx);
ptr[0] ^= 0x51;
ptr[1] ^= 0xce;
ptr[2] += 0x56;
ptr[3] ^= 0x15;
ptr[4] ^= 0x8d;
ptr[5] += 0x27;
ptr[6] += 0xd3;
ptr[7] += 0xc2;
ptr[8] += 0xad;
ptr[9] += 0x1a;
ptr[10] += 0xc2;
ptr[11] += 0x79;
ptr[12] += 0xa6;
ptr[13] ^= 0x5;
ptr[14] += 0x32;
ptr[15] += 0x1e;
ptr[16] ^= 0x1c;
ptr[17] ^= 0xa8;
ptr[18] ^= 0xe1;
ptr[19] += 0x78;
ptr[20] ^= 0xf6;
ptr[21] ^= 0x3b;
ptr[22] += 0x51;
ptr[23] += 0x7d;
ptr[24] ^= 0x2b;

}

void multi_tracer_p85(struct user_regs_struct *regs)
{
if (regs->rax != 0x2055)
{
multi_tracer_p191(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xa9a1e7 ^ regs->rbx);
ptr[0] ^= 0xbd;
ptr[1] += 0x21;
ptr[2] ^= 0x71;
ptr[3] ^= 0xce;
ptr[4] ^= 0x59;
ptr[5] += 0x4c;
ptr[6] += 0x43;
ptr[7] += 0x25;
ptr[8] += 0x4;
ptr[9] ^= 0x16;
ptr[10] ^= 0x83;
ptr[11] += 0x32;
ptr[12] ^= 0x65;
ptr[13] += 0xc0;
ptr[14] ^= 0x13;
ptr[15] ^= 0x7d;
ptr[16] += 0x7f;
ptr[17] += 0x11;
ptr[18] ^= 0xbc;
ptr[19] ^= 0xad;
ptr[20] += 0x66;
ptr[21] += 0x13;

}

void multi_tracer_p191(struct user_regs_struct *regs)
{
if (regs->rax != 0x20bf)
{
multi_tracer_p254(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8eeda5 ^ regs->rbx);
ptr[0] += 0xc4;
ptr[1] += 0x42;
ptr[2] += 0xc0;
ptr[3] += 0x47;
ptr[4] ^= 0x9;
ptr[5] += 0x62;
ptr[6] ^= 0x20;
ptr[7] += 0xe8;
ptr[8] += 0x9c;
ptr[9] += 0xc6;
ptr[10] ^= 0x4f;
ptr[11] ^= 0xc4;
ptr[12] ^= 0x4a;
ptr[13] += 0x77;
ptr[14] += 0xa;
ptr[15] += 0x6f;
ptr[16] ^= 0xd6;
ptr[17] += 0xd8;
ptr[18] += 0x9c;
ptr[19] ^= 0xdc;
ptr[20] += 0xf6;
ptr[21] ^= 0x34;

}

void multi_tracer_p254(struct user_regs_struct *regs)
{
if (regs->rax != 0x20fe)
{
multi_tracer_p137(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x29f1b0 ^ regs->rbx);
ptr[0] ^= 0x96;
ptr[1] ^= 0x5d;
ptr[2] += 0x56;
ptr[3] ^= 0xbc;
ptr[4] += 0xc1;
ptr[5] += 0x32;
ptr[6] ^= 0x41;
ptr[7] += 0x65;
ptr[8] ^= 0x1;
ptr[9] ^= 0x4e;
ptr[10] ^= 0x6b;
ptr[11] += 0xe;
ptr[12] ^= 0x5b;
ptr[13] ^= 0x9f;
ptr[14] ^= 0x62;
ptr[15] ^= 0x39;
ptr[16] ^= 0x4a;
ptr[17] += 0x4c;
ptr[18] += 0x2d;
ptr[19] += 0x80;
ptr[20] += 0x93;
ptr[21] ^= 0xc1;
ptr[22] ^= 0xda;
ptr[23] += 0xe8;
ptr[24] ^= 0x3a;
ptr[25] ^= 0x31;

}

void multi_tracer_p137(struct user_regs_struct *regs)
{
if (regs->rax != 0x2089)
{
multi_tracer_p26(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5933cd ^ regs->rbx);
ptr[0] ^= 0x88;
ptr[1] += 0x37;
ptr[2] += 0x9;
ptr[3] ^= 0x3c;
ptr[4] += 0x58;
ptr[5] += 0x67;
ptr[6] ^= 0x7d;
ptr[7] ^= 0x10;
ptr[8] ^= 0xcc;
ptr[9] ^= 0x92;
ptr[10] += 0x43;
ptr[11] ^= 0x8c;
ptr[12] += 0x3;
ptr[13] += 0xb0;
ptr[14] += 0x9a;
ptr[15] ^= 0x26;
ptr[16] += 0xe4;
ptr[17] ^= 0x90;
ptr[18] += 0x62;
ptr[19] ^= 0xc0;
ptr[20] += 0x39;
ptr[21] ^= 0x9d;
ptr[22] += 0x16;
ptr[23] ^= 0x9a;
ptr[24] ^= 0x9f;

}

void multi_tracer_p26(struct user_regs_struct *regs)
{
if (regs->rax != 0x201a)
{
multi_tracer_p10(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x870480 ^ regs->rbx);
ptr[0] += 0xd5;
ptr[1] ^= 0x2a;
ptr[2] += 0x3f;
ptr[3] ^= 0xba;
ptr[4] += 0x25;
ptr[5] += 0x76;
ptr[6] += 0x7a;
ptr[7] ^= 0x9e;
ptr[8] ^= 0x4e;
ptr[9] += 0xd8;
ptr[10] ^= 0xad;
ptr[11] ^= 0xe4;
ptr[12] += 0x7;
ptr[13] ^= 0x1a;
ptr[14] ^= 0x36;
ptr[15] ^= 0xee;
ptr[16] += 0x25;
ptr[17] ^= 0x80;
ptr[18] += 0x4b;
ptr[19] ^= 0x73;
ptr[20] ^= 0x1d;
ptr[21] ^= 0x20;
ptr[22] += 0xb;
ptr[23] ^= 0x60;
ptr[24] ^= 0xdf;
ptr[25] += 0x5d;
ptr[26] += 0x77;

}

void multi_tracer_p10(struct user_regs_struct *regs)
{
if (regs->rax != 0x200a)
{
multi_tracer_p190(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x429059 ^ regs->rbx);
ptr[0] ^= 0x58;
ptr[1] += 0xa;
ptr[2] ^= 0x2;
ptr[3] += 0xa;
ptr[4] += 0xa8;
ptr[5] ^= 0x63;
ptr[6] ^= 0xa9;
ptr[7] += 0x3b;
ptr[8] += 0x6b;
ptr[9] += 0x6c;
ptr[10] += 0x4d;
ptr[11] ^= 0x71;
ptr[12] ^= 0x85;
ptr[13] ^= 0x71;
ptr[14] ^= 0x4d;
ptr[15] += 0x62;
ptr[16] ^= 0x1f;
ptr[17] += 0xb6;
ptr[18] ^= 0x2f;
ptr[19] ^= 0x58;
ptr[20] ^= 0x3;
ptr[21] ^= 0xd;

}

void multi_tracer_p190(struct user_regs_struct *regs)
{
if (regs->rax != 0x20be)
{
multi_tracer_p242(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3aa4e3 ^ regs->rbx);
ptr[0] += 0x79;
ptr[1] += 0xfb;
ptr[2] += 0x11;
ptr[3] += 0x21;
ptr[4] ^= 0x9e;
ptr[5] ^= 0xba;
ptr[6] ^= 0xd6;
ptr[7] ^= 0xb3;
ptr[8] += 0x2c;
ptr[9] += 0xa1;
ptr[10] += 0xc8;
ptr[11] ^= 0xe5;
ptr[12] ^= 0xc9;
ptr[13] += 0x13;
ptr[14] += 0xb4;
ptr[15] ^= 0x9;
ptr[16] ^= 0x21;
ptr[17] += 0x16;
ptr[18] += 0x63;
ptr[19] += 0x24;
ptr[20] += 0x90;
ptr[21] ^= 0x4d;
ptr[22] += 0x50;
ptr[23] ^= 0xf0;
ptr[24] ^= 0x17;
ptr[25] += 0x6a;

}

void multi_tracer_p242(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f2)
{
multi_tracer_p166(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2d3e1f ^ regs->rbx);
ptr[0] ^= 0x3f;
ptr[1] ^= 0xb5;
ptr[2] += 0x17;
ptr[3] += 0xd2;
ptr[4] += 0xba;
ptr[5] += 0x2;
ptr[6] += 0x65;
ptr[7] ^= 0xfb;
ptr[8] ^= 0x9a;
ptr[9] += 0xdd;
ptr[10] += 0x5a;
ptr[11] += 0x56;
ptr[12] ^= 0x3a;
ptr[13] ^= 0xd4;
ptr[14] += 0x66;
ptr[15] += 0x73;
ptr[16] += 0xb7;
ptr[17] += 0xab;
ptr[18] += 0xb6;
ptr[19] ^= 0x96;
ptr[20] ^= 0xe2;
ptr[21] += 0x12;
ptr[22] += 0xed;
ptr[23] += 0xf0;

}

void multi_tracer_p166(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a6)
{
multi_tracer_p4(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x21774b ^ regs->rbx);
ptr[0] += 0x7a;
ptr[1] += 0xa3;
ptr[2] += 0x39;
ptr[3] ^= 0xaf;
ptr[4] ^= 0xcd;
ptr[5] ^= 0x46;
ptr[6] ^= 0x91;
ptr[7] += 0x45;
ptr[8] ^= 0x3c;
ptr[9] ^= 0x3d;
ptr[10] ^= 0x93;
ptr[11] += 0xa6;
ptr[12] ^= 0xf3;
ptr[13] ^= 0xf;
ptr[14] += 0xf5;
ptr[15] ^= 0x82;
ptr[16] += 0x26;
ptr[17] ^= 0xd7;
ptr[18] ^= 0x58;
ptr[19] ^= 0x73;
ptr[20] += 0x37;
ptr[21] ^= 0x1;

}

void multi_tracer_p4(struct user_regs_struct *regs)
{
if (regs->rax != 0x2004)
{
multi_tracer_p186(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x995b45 ^ regs->rbx);
ptr[0] ^= 0x56;
ptr[1] += 0xd4;
ptr[2] ^= 0xf3;
ptr[3] += 0x1d;
ptr[4] += 0x37;
ptr[5] += 0xa9;
ptr[6] ^= 0x39;
ptr[7] += 0x8e;
ptr[8] ^= 0x46;
ptr[9] ^= 0x79;
ptr[10] += 0x52;
ptr[11] ^= 0x87;
ptr[12] += 0x15;
ptr[13] += 0x14;
ptr[14] ^= 0x2d;
ptr[15] += 0xf8;
ptr[16] += 0xe6;
ptr[17] ^= 0x87;
ptr[18] ^= 0xb0;
ptr[19] += 0xe1;
ptr[20] += 0x26;
ptr[21] += 0x9b;
ptr[22] += 0xc7;
ptr[23] += 0x6b;
ptr[24] += 0x8c;
ptr[25] ^= 0xa9;
ptr[26] ^= 0x13;
ptr[27] ^= 0x51;

}

void multi_tracer_p186(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ba)
{
multi_tracer_p235(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf0e9e0 ^ regs->rbx);
ptr[0] += 0xab;
ptr[1] ^= 0x25;
ptr[2] ^= 0x7f;
ptr[3] ^= 0x13;
ptr[4] += 0x76;
ptr[5] += 0x98;
ptr[6] ^= 0x74;
ptr[7] += 0x6b;
ptr[8] += 0xb7;
ptr[9] += 0x43;
ptr[10] += 0x7e;
ptr[11] += 0x6;
ptr[12] ^= 0x67;
ptr[13] += 0x54;
ptr[14] ^= 0xe2;
ptr[15] += 0x27;
ptr[16] ^= 0x52;
ptr[17] ^= 0x34;

}

void multi_tracer_p235(struct user_regs_struct *regs)
{
if (regs->rax != 0x20eb)
{
multi_tracer_p14(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x65a83a ^ regs->rbx);
ptr[0] ^= 0x49;
ptr[1] ^= 0xc7;
ptr[2] ^= 0x57;
ptr[3] ^= 0x51;
ptr[4] ^= 0xa5;
ptr[5] ^= 0x18;
ptr[6] ^= 0xd3;
ptr[7] ^= 0x28;
ptr[8] += 0x9f;
ptr[9] += 0x90;
ptr[10] ^= 0xf7;
ptr[11] ^= 0x48;
ptr[12] += 0x95;
ptr[13] += 0xb1;
ptr[14] += 0x5a;
ptr[15] += 0x43;
ptr[16] ^= 0xf6;
ptr[17] ^= 0x51;

}

void multi_tracer_p14(struct user_regs_struct *regs)
{
if (regs->rax != 0x200e)
{
multi_tracer_p2(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x117702 ^ regs->rbx);
ptr[0] ^= 0x26;
ptr[1] += 0x69;
ptr[2] ^= 0xc8;
ptr[3] ^= 0x96;
ptr[4] ^= 0x22;
ptr[5] += 0x21;
ptr[6] ^= 0xc5;
ptr[7] += 0xe0;
ptr[8] ^= 0xdf;
ptr[9] += 0x45;
ptr[10] += 0x27;
ptr[11] += 0x96;
ptr[12] += 0xe4;
ptr[13] += 0x95;
ptr[14] ^= 0x2c;
ptr[15] ^= 0xf6;
ptr[16] += 0x1e;
ptr[17] ^= 0x47;
ptr[18] += 0x2d;

}

void multi_tracer_p2(struct user_regs_struct *regs)
{
if (regs->rax != 0x2002)
{
multi_tracer_p126(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd6b994 ^ regs->rbx);
ptr[0] ^= 0xf;
ptr[1] ^= 0x25;
ptr[2] ^= 0x39;
ptr[3] += 0xfb;
ptr[4] ^= 0x21;
ptr[5] ^= 0x6d;
ptr[6] ^= 0x3f;
ptr[7] += 0x4b;
ptr[8] ^= 0x9;
ptr[9] ^= 0xc5;
ptr[10] += 0x5e;
ptr[11] ^= 0x75;
ptr[12] += 0x13;
ptr[13] += 0xdd;
ptr[14] += 0xdd;
ptr[15] += 0x6;
ptr[16] ^= 0xbd;
ptr[17] += 0x75;
ptr[18] += 0x91;
ptr[19] ^= 0xf3;
ptr[20] += 0x76;
ptr[21] ^= 0x17;

}

void multi_tracer_p126(struct user_regs_struct *regs)
{
if (regs->rax != 0x207e)
{
multi_tracer_p149(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x485770 ^ regs->rbx);
ptr[0] += 0x7a;
ptr[1] ^= 0x3f;
ptr[2] += 0xbf;
ptr[3] ^= 0xe9;
ptr[4] += 0xe6;
ptr[5] += 0xac;
ptr[6] += 0xc6;
ptr[7] += 0xe7;
ptr[8] ^= 0x55;
ptr[9] ^= 0x38;
ptr[10] ^= 0xf8;
ptr[11] += 0x27;
ptr[12] ^= 0x6b;
ptr[13] ^= 0x7;
ptr[14] += 0xcc;
ptr[15] += 0x4d;
ptr[16] += 0xb5;
ptr[17] ^= 0xdb;
ptr[18] += 0x98;
ptr[19] += 0xd7;
ptr[20] += 0x8d;
ptr[21] ^= 0xa2;

}

void multi_tracer_p149(struct user_regs_struct *regs)
{
if (regs->rax != 0x2095)
{
multi_tracer_p213(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe45f45 ^ regs->rbx);
ptr[0] += 0xa3;
ptr[1] += 0xcc;
ptr[2] ^= 0xfe;
ptr[3] ^= 0xbe;
ptr[4] ^= 0x4d;
ptr[5] ^= 0x10;
ptr[6] += 0xaa;
ptr[7] += 0xfa;
ptr[8] += 0x20;
ptr[9] ^= 0x1e;
ptr[10] ^= 0xba;
ptr[11] += 0x1;
ptr[12] ^= 0x82;
ptr[13] ^= 0x2f;
ptr[14] ^= 0x2b;
ptr[15] ^= 0xf4;
ptr[16] ^= 0xee;
ptr[17] ^= 0x82;
ptr[18] ^= 0x6d;
ptr[19] += 0x66;
ptr[20] += 0x31;
ptr[21] ^= 0xe7;

}

void multi_tracer_p213(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d5)
{
multi_tracer_p180(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x33f69c ^ regs->rbx);
ptr[0] ^= 0xe7;
ptr[1] += 0x41;
ptr[2] += 0xda;
ptr[3] ^= 0x86;
ptr[4] ^= 0x81;
ptr[5] ^= 0x8;
ptr[6] += 0x1;
ptr[7] += 0x19;
ptr[8] += 0xa9;
ptr[9] ^= 0x9e;
ptr[10] ^= 0xbf;
ptr[11] += 0x5c;
ptr[12] += 0x85;
ptr[13] += 0xd5;
ptr[14] ^= 0xa8;
ptr[15] ^= 0xb3;
ptr[16] += 0xc;
ptr[17] ^= 0x43;
ptr[18] += 0x78;
ptr[19] ^= 0xc4;
ptr[20] ^= 0xb1;
ptr[21] ^= 0x90;
ptr[22] ^= 0x80;
ptr[23] ^= 0xf1;

}

void multi_tracer_p180(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b4)
{
multi_tracer_p23(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x370fe5 ^ regs->rbx);
ptr[0] ^= 0xcd;
ptr[1] += 0x2a;
ptr[2] += 0x79;
ptr[3] += 0xdf;
ptr[4] ^= 0xf;
ptr[5] ^= 0x89;
ptr[6] ^= 0xdb;
ptr[7] += 0x6c;
ptr[8] += 0x9e;
ptr[9] ^= 0xc9;
ptr[10] += 0xc;
ptr[11] ^= 0x8f;
ptr[12] ^= 0x5f;
ptr[13] ^= 0x56;
ptr[14] ^= 0x53;
ptr[15] ^= 0x2d;
ptr[16] ^= 0x93;
ptr[17] ^= 0xed;
ptr[18] ^= 0xcf;
ptr[19] += 0xb;
ptr[20] += 0x28;
ptr[21] += 0x98;
ptr[22] ^= 0x4d;
ptr[23] ^= 0x7b;
ptr[24] ^= 0x9f;
ptr[25] += 0x3b;
ptr[26] ^= 0xb9;

}

void multi_tracer_p23(struct user_regs_struct *regs)
{
if (regs->rax != 0x2017)
{
multi_tracer_p82(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x1afc44 ^ regs->rbx);
ptr[0] ^= 0xcb;
ptr[1] ^= 0x72;
ptr[2] += 0xd0;
ptr[3] += 0x3e;
ptr[4] += 0xc3;
ptr[5] += 0x78;
ptr[6] ^= 0xe0;
ptr[7] += 0x2f;
ptr[8] ^= 0x9f;
ptr[9] += 0xc6;
ptr[10] ^= 0xed;
ptr[11] ^= 0x8b;
ptr[12] += 0x80;
ptr[13] += 0x8;
ptr[14] += 0xec;
ptr[15] += 0x32;
ptr[16] ^= 0xaa;
ptr[17] ^= 0xf;
ptr[18] ^= 0x7b;
ptr[19] ^= 0xdb;
ptr[20] += 0x62;
ptr[21] += 0x8f;
ptr[22] += 0x1a;
ptr[23] += 0xa9;
ptr[24] += 0xaf;
ptr[25] += 0xb6;

}

void multi_tracer_p82(struct user_regs_struct *regs)
{
if (regs->rax != 0x2052)
{
multi_tracer_p152(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc50554 ^ regs->rbx);
ptr[0] ^= 0xae;
ptr[1] += 0x71;
ptr[2] ^= 0xbb;
ptr[3] += 0xb0;
ptr[4] ^= 0xf6;
ptr[5] += 0x84;
ptr[6] ^= 0x4a;
ptr[7] += 0x72;
ptr[8] += 0x78;
ptr[9] += 0xda;
ptr[10] ^= 0xb8;
ptr[11] ^= 0xe;
ptr[12] += 0x6a;
ptr[13] += 0x3b;
ptr[14] ^= 0x3c;
ptr[15] ^= 0x53;
ptr[16] += 0x78;
ptr[17] += 0x9d;
ptr[18] ^= 0x22;
ptr[19] ^= 0x74;

}

void multi_tracer_p152(struct user_regs_struct *regs)
{
if (regs->rax != 0x2098)
{
multi_tracer_p96(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc3fe8e ^ regs->rbx);
ptr[0] ^= 0x2;
ptr[1] ^= 0xff;
ptr[2] ^= 0x15;
ptr[3] ^= 0x40;
ptr[4] ^= 0x1e;
ptr[5] ^= 0x64;
ptr[6] ^= 0xea;
ptr[7] += 0x57;
ptr[8] += 0xe5;
ptr[9] += 0x4b;
ptr[10] ^= 0xed;
ptr[11] ^= 0xe;
ptr[12] ^= 0x5;
ptr[13] += 0x25;
ptr[14] ^= 0x2b;
ptr[15] += 0xf7;
ptr[16] += 0x8f;
ptr[17] ^= 0x1e;
ptr[18] += 0x87;
ptr[19] += 0xb6;

}

void multi_tracer_p96(struct user_regs_struct *regs)
{
if (regs->rax != 0x2060)
{
multi_tracer_p175(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9d14b5 ^ regs->rbx);
ptr[0] ^= 0x9e;
ptr[1] += 0x36;
ptr[2] ^= 0xd0;
ptr[3] ^= 0x38;
ptr[4] ^= 0x69;
ptr[5] += 0xb8;
ptr[6] ^= 0x66;
ptr[7] ^= 0xa6;
ptr[8] += 0xbc;
ptr[9] += 0xc3;
ptr[10] ^= 0x84;
ptr[11] ^= 0x7b;
ptr[12] ^= 0x69;
ptr[13] ^= 0xd1;
ptr[14] ^= 0xc5;
ptr[15] ^= 0x68;
ptr[16] += 0xca;
ptr[17] ^= 0x63;
ptr[18] += 0x5f;
ptr[19] += 0xb6;
ptr[20] += 0x89;
ptr[21] += 0x3e;
ptr[22] += 0xfe;
ptr[23] += 0x69;
ptr[24] += 0x65;
ptr[25] += 0xd2;
ptr[26] += 0x61;
ptr[27] += 0x8b;

}

void multi_tracer_p175(struct user_regs_struct *regs)
{
if (regs->rax != 0x20af)
{
multi_tracer_p46(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf8f44f ^ regs->rbx);
ptr[0] += 0xf6;
ptr[1] += 0x35;
ptr[2] ^= 0xb6;
ptr[3] += 0xc9;
ptr[4] ^= 0xcc;
ptr[5] ^= 0x2c;
ptr[6] ^= 0x85;
ptr[7] ^= 0x9e;
ptr[8] ^= 0xd7;
ptr[9] += 0xbe;
ptr[10] ^= 0xab;
ptr[11] ^= 0x43;
ptr[12] += 0xd0;
ptr[13] += 0x49;
ptr[14] += 0xfc;
ptr[15] ^= 0xdc;
ptr[16] += 0x5a;
ptr[17] += 0x34;
ptr[18] += 0x20;
ptr[19] ^= 0xaf;
ptr[20] ^= 0x7a;
ptr[21] ^= 0xb0;
ptr[22] += 0x8d;
ptr[23] += 0x8e;
ptr[24] += 0xe5;
ptr[25] ^= 0x35;

}

void multi_tracer_p46(struct user_regs_struct *regs)
{
if (regs->rax != 0x202e)
{
multi_tracer_p58(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x775166 ^ regs->rbx);
ptr[0] ^= 0x48;
ptr[1] ^= 0x15;
ptr[2] ^= 0xb2;
ptr[3] ^= 0x7f;
ptr[4] += 0x22;
ptr[5] += 0xb9;
ptr[6] ^= 0xae;
ptr[7] += 0x88;
ptr[8] += 0x76;
ptr[9] += 0xda;
ptr[10] ^= 0xd7;
ptr[11] += 0x4f;
ptr[12] ^= 0xef;
ptr[13] ^= 0x50;
ptr[14] ^= 0x41;
ptr[15] += 0x26;
ptr[16] ^= 0x7a;
ptr[17] += 0x90;
ptr[18] += 0x2f;
ptr[19] ^= 0xf9;
ptr[20] += 0xca;
ptr[21] ^= 0x98;
ptr[22] ^= 0x24;
ptr[23] += 0x5c;
ptr[24] += 0x76;
ptr[25] += 0xf5;
ptr[26] ^= 0x8a;

}

void multi_tracer_p58(struct user_regs_struct *regs)
{
if (regs->rax != 0x203a)
{
multi_tracer_p170(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5a6d8f ^ regs->rbx);
ptr[0] ^= 0xaf;
ptr[1] ^= 0x9d;
ptr[2] += 0x54;
ptr[3] ^= 0x5;
ptr[4] ^= 0x6a;
ptr[5] += 0xb0;
ptr[6] ^= 0x20;
ptr[7] += 0xe6;
ptr[8] += 0xa7;
ptr[9] += 0xd7;
ptr[10] ^= 0x49;
ptr[11] += 0xad;
ptr[12] ^= 0xfa;
ptr[13] += 0xdc;
ptr[14] += 0x29;
ptr[15] += 0xa8;
ptr[16] ^= 0x32;
ptr[17] += 0x79;
ptr[18] ^= 0x8e;
ptr[19] ^= 0x84;
ptr[20] += 0xc7;
ptr[21] += 0x80;
ptr[22] += 0x91;
ptr[23] ^= 0x56;
ptr[24] += 0x92;
ptr[25] += 0x6;
ptr[26] ^= 0x4c;
ptr[27] ^= 0x3a;

}

void multi_tracer_p170(struct user_regs_struct *regs)
{
if (regs->rax != 0x20aa)
{
multi_tracer_p60(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3f8935 ^ regs->rbx);
ptr[0] ^= 0x37;
ptr[1] ^= 0xb5;
ptr[2] ^= 0x4c;
ptr[3] += 0x8c;
ptr[4] += 0x67;
ptr[5] += 0x1c;
ptr[6] += 0x99;
ptr[7] += 0x62;
ptr[8] += 0x42;
ptr[9] ^= 0x5d;
ptr[10] ^= 0xe7;
ptr[11] += 0x35;
ptr[12] ^= 0xba;
ptr[13] ^= 0x75;
ptr[14] += 0x3d;
ptr[15] ^= 0x52;
ptr[16] += 0x55;
ptr[17] ^= 0xf9;
ptr[18] += 0x1c;

}

void multi_tracer_p60(struct user_regs_struct *regs)
{
if (regs->rax != 0x203c)
{
multi_tracer_p145(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd966ff ^ regs->rbx);
ptr[0] += 0xf9;
ptr[1] += 0x5a;
ptr[2] += 0x53;
ptr[3] += 0xc1;
ptr[4] ^= 0xe3;
ptr[5] += 0x16;
ptr[6] ^= 0x7;
ptr[7] ^= 0x58;
ptr[8] += 0x6f;
ptr[9] ^= 0xe2;
ptr[10] ^= 0xb0;
ptr[11] ^= 0xe2;
ptr[12] ^= 0x97;
ptr[13] += 0xe;
ptr[14] ^= 0x10;
ptr[15] ^= 0xfc;
ptr[16] ^= 0xf9;
ptr[17] ^= 0xef;
ptr[18] += 0x14;
ptr[19] += 0xd0;
ptr[20] += 0xec;
ptr[21] += 0xbf;
ptr[22] += 0xbf;

}

void multi_tracer_p145(struct user_regs_struct *regs)
{
if (regs->rax != 0x2091)
{
multi_tracer_p164(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x47fdae ^ regs->rbx);
ptr[0] ^= 0xec;
ptr[1] ^= 0xf5;
ptr[2] += 0xc1;
ptr[3] += 0x67;
ptr[4] += 0x64;
ptr[5] += 0x45;
ptr[6] += 0x93;
ptr[7] += 0x42;
ptr[8] ^= 0x3d;
ptr[9] ^= 0xf0;
ptr[10] ^= 0xde;
ptr[11] += 0x72;
ptr[12] ^= 0x28;
ptr[13] ^= 0x6d;
ptr[14] += 0x3c;
ptr[15] ^= 0xf5;
ptr[16] ^= 0xc6;
ptr[17] ^= 0x45;
ptr[18] ^= 0xcf;
ptr[19] ^= 0x8;
ptr[20] ^= 0x10;
ptr[21] += 0xcf;
ptr[22] ^= 0xd4;
ptr[23] += 0xfc;
ptr[24] ^= 0x29;
ptr[25] ^= 0x3c;
ptr[26] += 0xa1;
ptr[27] += 0x38;

}

void multi_tracer_p164(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a4)
{
multi_tracer_p176(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe44b43 ^ regs->rbx);
ptr[0] ^= 0x3b;
ptr[1] ^= 0x4f;
ptr[2] += 0x19;
ptr[3] ^= 0x4e;
ptr[4] += 0xd1;
ptr[5] += 0x6c;
ptr[6] ^= 0x77;
ptr[7] ^= 0xd0;
ptr[8] += 0xec;
ptr[9] ^= 0xc4;
ptr[10] ^= 0x8d;
ptr[11] ^= 0xdb;
ptr[12] += 0x8f;
ptr[13] += 0x7a;
ptr[14] ^= 0xb6;
ptr[15] += 0x7;
ptr[16] ^= 0xe5;
ptr[17] += 0xce;
ptr[18] ^= 0xae;
ptr[19] ^= 0x53;
ptr[20] ^= 0xaa;
ptr[21] ^= 0x57;
ptr[22] ^= 0x9d;
ptr[23] += 0x20;
ptr[24] += 0x6b;
ptr[25] += 0x7a;
ptr[26] += 0xbd;

}

void multi_tracer_p176(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b0)
{
multi_tracer_p157(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf3ae5e ^ regs->rbx);
ptr[0] += 0x41;
ptr[1] ^= 0x7;
ptr[2] += 0x18;
ptr[3] ^= 0x38;
ptr[4] ^= 0xe0;
ptr[5] += 0x8e;
ptr[6] += 0xb;
ptr[7] ^= 0xa0;
ptr[8] += 0xea;
ptr[9] ^= 0x18;
ptr[10] ^= 0xd4;
ptr[11] += 0xdb;
ptr[12] += 0x35;
ptr[13] += 0x74;
ptr[14] ^= 0x2b;
ptr[15] += 0xad;
ptr[16] ^= 0x82;
ptr[17] ^= 0x44;
ptr[18] += 0xb0;
ptr[19] ^= 0xd2;
ptr[20] ^= 0x66;
ptr[21] += 0xc8;
ptr[22] ^= 0x58;
ptr[23] += 0xd2;

}

void multi_tracer_p157(struct user_regs_struct *regs)
{
if (regs->rax != 0x209d)
{
multi_tracer_p72(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x95bf01 ^ regs->rbx);
ptr[0] ^= 0xb7;
ptr[1] += 0xc8;
ptr[2] ^= 0x1e;
ptr[3] ^= 0x2b;
ptr[4] ^= 0xe4;
ptr[5] ^= 0x2a;
ptr[6] += 0x97;
ptr[7] ^= 0x44;
ptr[8] += 0xc0;
ptr[9] ^= 0x4;
ptr[10] += 0xab;
ptr[11] += 0x23;
ptr[12] ^= 0xba;
ptr[13] ^= 0x2d;
ptr[14] ^= 0x7c;
ptr[15] ^= 0xfb;
ptr[16] += 0x8;
ptr[17] ^= 0x2d;
ptr[18] += 0x95;
ptr[19] ^= 0xa0;
ptr[20] += 0xe9;
ptr[21] += 0x13;

}

void multi_tracer_p72(struct user_regs_struct *regs)
{
if (regs->rax != 0x2048)
{
multi_tracer_p117(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x93e38b ^ regs->rbx);
ptr[0] ^= 0x9b;
ptr[1] ^= 0x6e;
ptr[2] += 0x62;
ptr[3] ^= 0x7;
ptr[4] ^= 0xa6;
ptr[5] ^= 0x3a;
ptr[6] += 0x39;
ptr[7] += 0xb;
ptr[8] ^= 0xf7;
ptr[9] += 0xe9;
ptr[10] += 0xd7;
ptr[11] ^= 0x7d;
ptr[12] ^= 0x74;
ptr[13] += 0xd9;
ptr[14] += 0x9d;
ptr[15] += 0xb1;
ptr[16] ^= 0x3a;
ptr[17] ^= 0x2c;
ptr[18] += 0x32;
ptr[19] ^= 0x46;
ptr[20] ^= 0xe;
ptr[21] += 0x58;
ptr[22] += 0x92;
ptr[23] += 0x51;
ptr[24] ^= 0x60;

}

void multi_tracer_p117(struct user_regs_struct *regs)
{
if (regs->rax != 0x2075)
{
multi_tracer_p188(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb35e14 ^ regs->rbx);
ptr[0] ^= 0xdb;
ptr[1] ^= 0xf;
ptr[2] += 0x98;
ptr[3] += 0x53;
ptr[4] ^= 0x21;
ptr[5] += 0x3;
ptr[6] += 0xa0;
ptr[7] += 0x5f;
ptr[8] ^= 0x65;
ptr[9] += 0xb4;
ptr[10] += 0xce;
ptr[11] ^= 0x30;
ptr[12] ^= 0xe0;
ptr[13] ^= 0xca;
ptr[14] ^= 0x5c;
ptr[15] += 0x3e;
ptr[16] ^= 0xd5;
ptr[17] += 0xa5;
ptr[18] += 0x8d;
ptr[19] += 0x17;
ptr[20] ^= 0x9e;
ptr[21] += 0xce;
ptr[22] ^= 0x3a;

}

void multi_tracer_p188(struct user_regs_struct *regs)
{
if (regs->rax != 0x20bc)
{
multi_tracer_p18(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x55bca9 ^ regs->rbx);
ptr[0] ^= 0x5;
ptr[1] ^= 0x85;
ptr[2] += 0x94;
ptr[3] ^= 0xfa;
ptr[4] ^= 0x4b;
ptr[5] ^= 0x89;
ptr[6] ^= 0x99;
ptr[7] += 0xf0;
ptr[8] += 0x89;
ptr[9] ^= 0x89;
ptr[10] += 0x9a;
ptr[11] += 0x58;
ptr[12] += 0xde;
ptr[13] += 0x9b;
ptr[14] += 0x5b;
ptr[15] ^= 0x89;
ptr[16] ^= 0x2;
ptr[17] += 0x1e;
ptr[18] ^= 0x83;
ptr[19] += 0x31;
ptr[20] ^= 0xb7;
ptr[21] ^= 0x8d;
ptr[22] ^= 0x5;
ptr[23] += 0xd7;
ptr[24] += 0x35;
ptr[25] += 0xcf;
ptr[26] += 0x7d;

}

void multi_tracer_p18(struct user_regs_struct *regs)
{
if (regs->rax != 0x2012)
{
multi_tracer_p20(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7b90ac ^ regs->rbx);
ptr[0] ^= 0x55;
ptr[1] += 0xac;
ptr[2] ^= 0x25;
ptr[3] ^= 0x78;
ptr[4] += 0xf9;
ptr[5] += 0xfa;
ptr[6] += 0x69;
ptr[7] += 0x68;
ptr[8] += 0xe6;
ptr[9] ^= 0x6b;
ptr[10] ^= 0x80;
ptr[11] ^= 0xb5;
ptr[12] += 0x43;
ptr[13] ^= 0xe7;
ptr[14] += 0x4b;
ptr[15] += 0x28;
ptr[16] += 0x90;
ptr[17] ^= 0xa4;
ptr[18] ^= 0xdc;
ptr[19] += 0x2c;
ptr[20] ^= 0x20;
ptr[21] ^= 0xcc;
ptr[22] += 0xa9;
ptr[23] ^= 0xec;

}

void multi_tracer_p20(struct user_regs_struct *regs)
{
if (regs->rax != 0x2014)
{
multi_tracer_p30(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5f699e ^ regs->rbx);
ptr[0] ^= 0x9c;
ptr[1] ^= 0x4a;
ptr[2] += 0x31;
ptr[3] += 0x68;
ptr[4] ^= 0x7b;
ptr[5] ^= 0x30;
ptr[6] += 0x96;
ptr[7] ^= 0xb4;
ptr[8] ^= 0x8b;
ptr[9] ^= 0xad;
ptr[10] += 0x3d;
ptr[11] ^= 0xb0;
ptr[12] += 0xce;
ptr[13] += 0xf3;
ptr[14] += 0xed;
ptr[15] ^= 0x34;
ptr[16] ^= 0xcc;
ptr[17] ^= 0x93;
ptr[18] ^= 0x50;
ptr[19] ^= 0x98;
ptr[20] ^= 0xa2;
ptr[21] += 0x4e;
ptr[22] ^= 0x7f;
ptr[23] += 0x82;

}

void multi_tracer_p30(struct user_regs_struct *regs)
{
if (regs->rax != 0x201e)
{
multi_tracer_p234(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb0e9e7 ^ regs->rbx);
ptr[0] ^= 0xd7;
ptr[1] += 0x65;
ptr[2] ^= 0xd4;
ptr[3] += 0xae;
ptr[4] ^= 0x39;
ptr[5] ^= 0xeb;
ptr[6] += 0x74;
ptr[7] ^= 0x9b;
ptr[8] ^= 0xc6;
ptr[9] ^= 0xc3;
ptr[10] += 0x1e;
ptr[11] ^= 0x1b;
ptr[12] += 0xfe;
ptr[13] ^= 0xfe;
ptr[14] += 0x7c;
ptr[15] += 0xee;
ptr[16] += 0x13;
ptr[17] += 0xa;
ptr[18] += 0x12;
ptr[19] += 0x6f;
ptr[20] ^= 0x7c;
ptr[21] += 0x48;
ptr[22] += 0x14;
ptr[23] += 0xa3;
ptr[24] += 0xaa;

}

void multi_tracer_p234(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ea)
{
multi_tracer_p237(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x114f25 ^ regs->rbx);
ptr[0] += 0x31;
ptr[1] ^= 0xed;
ptr[2] ^= 0x8b;
ptr[3] ^= 0xf;
ptr[4] ^= 0x7e;
ptr[5] ^= 0xb8;
ptr[6] ^= 0xaa;
ptr[7] += 0x6f;
ptr[8] += 0xc8;
ptr[9] ^= 0x6e;
ptr[10] ^= 0xf5;
ptr[11] += 0xba;
ptr[12] += 0xc5;
ptr[13] ^= 0x3c;
ptr[14] ^= 0x7c;
ptr[15] += 0xa8;
ptr[16] += 0x97;
ptr[17] += 0x4b;
ptr[18] ^= 0x9d;
ptr[19] += 0xfb;
ptr[20] += 0x6e;
ptr[21] += 0xa2;
ptr[22] += 0x3b;
ptr[23] ^= 0x28;

}

void multi_tracer_p237(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ed)
{
multi_tracer_p132(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x149c4c ^ regs->rbx);
ptr[0] += 0xac;
ptr[1] += 0x58;
ptr[2] += 0x6;
ptr[3] ^= 0x9d;
ptr[4] += 0xf3;
ptr[5] ^= 0x68;
ptr[6] += 0x41;
ptr[7] += 0xae;
ptr[8] += 0x58;
ptr[9] += 0xd8;
ptr[10] += 0x15;
ptr[11] += 0xb5;
ptr[12] += 0xbf;
ptr[13] += 0x94;
ptr[14] ^= 0xd6;
ptr[15] ^= 0x1e;
ptr[16] += 0x4d;
ptr[17] ^= 0x4d;
ptr[18] += 0xbd;
ptr[19] ^= 0x92;
ptr[20] ^= 0xdb;
ptr[21] += 0x17;
ptr[22] += 0x86;
ptr[23] ^= 0x78;

}

void multi_tracer_p132(struct user_regs_struct *regs)
{
if (regs->rax != 0x2084)
{
multi_tracer_p47(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xfab9c6 ^ regs->rbx);
ptr[0] += 0x42;
ptr[1] ^= 0xf;
ptr[2] += 0x5b;
ptr[3] ^= 0x27;
ptr[4] ^= 0x96;
ptr[5] += 0xf3;
ptr[6] += 0x4c;
ptr[7] += 0x29;
ptr[8] += 0x87;
ptr[9] += 0xa8;
ptr[10] ^= 0x43;
ptr[11] += 0x89;
ptr[12] += 0x82;
ptr[13] ^= 0xe5;
ptr[14] ^= 0x58;
ptr[15] += 0xa2;
ptr[16] ^= 0x50;
ptr[17] += 0x21;
ptr[18] += 0xc1;
ptr[19] += 0xd9;
ptr[20] ^= 0x36;

}

void multi_tracer_p47(struct user_regs_struct *regs)
{
if (regs->rax != 0x202f)
{
multi_tracer_p105(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe94981 ^ regs->rbx);
ptr[0] += 0xa4;
ptr[1] += 0xc;
ptr[2] += 0x89;
ptr[3] += 0x50;
ptr[4] += 0x1e;
ptr[5] ^= 0xb6;
ptr[6] += 0x91;
ptr[7] += 0xb6;
ptr[8] += 0x48;
ptr[9] += 0xfe;
ptr[10] += 0xcf;
ptr[11] += 0xbe;
ptr[12] += 0x3c;
ptr[13] += 0xbe;
ptr[14] += 0x9b;
ptr[15] += 0x6b;
ptr[16] += 0x28;
ptr[17] ^= 0xe4;
ptr[18] += 0xa2;
ptr[19] ^= 0x94;
ptr[20] += 0x9f;
ptr[21] ^= 0x65;

}

void multi_tracer_p105(struct user_regs_struct *regs)
{
if (regs->rax != 0x2069)
{
multi_tracer_p128(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2e1d05 ^ regs->rbx);
ptr[0] += 0xb2;
ptr[1] += 0xae;
ptr[2] ^= 0xad;
ptr[3] += 0xc1;
ptr[4] ^= 0x9c;
ptr[5] += 0x88;
ptr[6] += 0xfa;
ptr[7] ^= 0x8c;
ptr[8] ^= 0x53;
ptr[9] ^= 0xed;
ptr[10] += 0xab;
ptr[11] ^= 0xa0;
ptr[12] += 0x1b;
ptr[13] += 0xe5;
ptr[14] ^= 0x39;
ptr[15] ^= 0x5;
ptr[16] += 0xd9;
ptr[17] += 0xbb;
ptr[18] ^= 0x11;
ptr[19] ^= 0xf2;
ptr[20] += 0xb9;
ptr[21] += 0xf4;
ptr[22] += 0xf8;
ptr[23] ^= 0xf1;
ptr[24] += 0x57;
ptr[25] += 0x42;
ptr[26] += 0x33;

}

void multi_tracer_p128(struct user_regs_struct *regs)
{
if (regs->rax != 0x2080)
{
multi_tracer_p189(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc8ed8f ^ regs->rbx);
ptr[0] ^= 0x41;
ptr[1] += 0xd4;
ptr[2] += 0x5;
ptr[3] ^= 0xce;
ptr[4] += 0x7;
ptr[5] ^= 0x7e;
ptr[6] += 0xae;
ptr[7] ^= 0x5;
ptr[8] ^= 0x2b;
ptr[9] += 0x93;
ptr[10] += 0xe3;
ptr[11] ^= 0xf;
ptr[12] += 0x7a;
ptr[13] += 0xca;
ptr[14] += 0xa1;
ptr[15] += 0xd0;
ptr[16] += 0xde;
ptr[17] += 0xda;
ptr[18] ^= 0xe4;

}

void multi_tracer_p189(struct user_regs_struct *regs)
{
if (regs->rax != 0x20bd)
{
multi_tracer_p31(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7ce89f ^ regs->rbx);
ptr[0] ^= 0xdd;
ptr[1] ^= 0xd2;
ptr[2] += 0x12;
ptr[3] += 0xbf;
ptr[4] += 0x69;
ptr[5] ^= 0xe5;
ptr[6] += 0xaa;
ptr[7] += 0xff;
ptr[8] ^= 0x69;
ptr[9] ^= 0xf8;
ptr[10] ^= 0xb6;
ptr[11] += 0xd0;
ptr[12] ^= 0x47;
ptr[13] += 0x1d;
ptr[14] ^= 0x81;
ptr[15] ^= 0xb1;
ptr[16] ^= 0x58;
ptr[17] ^= 0xa4;
ptr[18] += 0x94;
ptr[19] += 0xd1;
ptr[20] += 0x76;
ptr[21] += 0xe3;

}

void multi_tracer_p31(struct user_regs_struct *regs)
{
if (regs->rax != 0x201f)
{
multi_tracer_p194(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd26b10 ^ regs->rbx);
ptr[0] ^= 0x99;
ptr[1] ^= 0x1b;
ptr[2] ^= 0x45;
ptr[3] += 0xb0;
ptr[4] ^= 0x6d;
ptr[5] += 0x5;
ptr[6] ^= 0x6e;
ptr[7] += 0x5c;
ptr[8] += 0x4e;
ptr[9] += 0xfc;
ptr[10] ^= 0xcf;
ptr[11] += 0x6a;
ptr[12] += 0x2e;
ptr[13] += 0xd7;
ptr[14] += 0x38;
ptr[15] ^= 0x56;
ptr[16] ^= 0x1b;
ptr[17] += 0xca;
ptr[18] ^= 0x4d;
ptr[19] += 0x59;
ptr[20] ^= 0xc6;
ptr[21] += 0x20;
ptr[22] += 0xe4;
ptr[23] += 0xd4;
ptr[24] ^= 0x22;
ptr[25] += 0x3b;

}

void multi_tracer_p194(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c2)
{
multi_tracer_p219(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x81f042 ^ regs->rbx);
ptr[0] ^= 0xd5;
ptr[1] += 0xbb;
ptr[2] += 0x22;
ptr[3] ^= 0xaf;
ptr[4] += 0xec;
ptr[5] += 0x3;
ptr[6] += 0xd9;
ptr[7] += 0xbc;
ptr[8] ^= 0x11;
ptr[9] ^= 0x5a;
ptr[10] += 0xb4;
ptr[11] ^= 0xea;
ptr[12] += 0xf4;
ptr[13] ^= 0x1a;
ptr[14] += 0xec;
ptr[15] += 0xab;
ptr[16] ^= 0x19;
ptr[17] += 0x9b;
ptr[18] += 0x55;
ptr[19] ^= 0x46;
ptr[20] += 0xc9;
ptr[21] ^= 0x51;
ptr[22] += 0xfc;

}

void multi_tracer_p219(struct user_regs_struct *regs)
{
if (regs->rax != 0x20db)
{
multi_tracer_p90(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xcc6fcc ^ regs->rbx);
ptr[0] ^= 0x43;
ptr[1] += 0xe2;
ptr[2] += 0x79;
ptr[3] ^= 0x8f;
ptr[4] ^= 0x91;
ptr[5] += 0xb8;
ptr[6] += 0xe3;
ptr[7] += 0x20;
ptr[8] += 0xee;
ptr[9] += 0x41;
ptr[10] ^= 0x74;
ptr[11] += 0x60;
ptr[12] += 0xcd;
ptr[13] += 0x6b;
ptr[14] ^= 0xaa;
ptr[15] ^= 0xe4;
ptr[16] += 0xea;
ptr[17] ^= 0x84;
ptr[18] ^= 0x18;
ptr[19] += 0xb3;

}

void multi_tracer_p90(struct user_regs_struct *regs)
{
if (regs->rax != 0x205a)
{
multi_tracer_p193(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xdf489a ^ regs->rbx);
ptr[0] += 0x7b;
ptr[1] ^= 0x4e;
ptr[2] += 0xa2;
ptr[3] += 0x78;
ptr[4] ^= 0xd9;
ptr[5] ^= 0xea;
ptr[6] ^= 0xc8;
ptr[7] += 0x27;
ptr[8] += 0x88;
ptr[9] ^= 0x30;
ptr[10] += 0x2e;
ptr[11] += 0x8e;
ptr[12] += 0x30;
ptr[13] += 0x7a;
ptr[14] += 0x64;
ptr[15] ^= 0x83;
ptr[16] += 0xa4;
ptr[17] ^= 0x36;
ptr[18] ^= 0xb4;
ptr[19] += 0xbd;
ptr[20] ^= 0x78;
ptr[21] ^= 0xb5;
ptr[22] += 0x40;

}

void multi_tracer_p193(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c1)
{
multi_tracer_p44(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x39c02a ^ regs->rbx);
ptr[0] ^= 0x13;
ptr[1] ^= 0xf2;
ptr[2] ^= 0xc2;
ptr[3] += 0xb9;
ptr[4] ^= 0xeb;
ptr[5] ^= 0x2a;
ptr[6] ^= 0xaa;
ptr[7] += 0x51;
ptr[8] += 0xe3;
ptr[9] += 0xaf;
ptr[10] ^= 0xde;
ptr[11] += 0x8b;
ptr[12] += 0xdc;
ptr[13] ^= 0x4d;
ptr[14] ^= 0x23;
ptr[15] ^= 0xad;
ptr[16] ^= 0x4c;
ptr[17] += 0x3b;
ptr[18] ^= 0xf5;
ptr[19] ^= 0xb4;
ptr[20] ^= 0xc6;
ptr[21] += 0x96;
ptr[22] += 0x77;
ptr[23] += 0x94;

}

void multi_tracer_p44(struct user_regs_struct *regs)
{
if (regs->rax != 0x202c)
{
multi_tracer_p221(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x25297a ^ regs->rbx);
ptr[0] ^= 0xc2;
ptr[1] ^= 0xec;
ptr[2] ^= 0x8f;
ptr[3] ^= 0xe2;
ptr[4] ^= 0xa4;
ptr[5] += 0x75;
ptr[6] ^= 0x84;
ptr[7] ^= 0x34;
ptr[8] += 0xfa;
ptr[9] += 0xd6;
ptr[10] += 0x99;
ptr[11] += 0x51;
ptr[12] += 0x2c;
ptr[13] ^= 0xd7;
ptr[14] ^= 0xe9;
ptr[15] += 0x9d;
ptr[16] += 0x3d;
ptr[17] += 0x36;
ptr[18] += 0x1b;
ptr[19] += 0x21;
ptr[20] += 0x6d;
ptr[21] += 0xb5;
ptr[22] += 0xf7;
ptr[23] ^= 0x11;
ptr[24] += 0xa9;

}

void multi_tracer_p221(struct user_regs_struct *regs)
{
if (regs->rax != 0x20dd)
{
multi_tracer_p68(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x74c4e5 ^ regs->rbx);
ptr[0] += 0x4e;
ptr[1] ^= 0xb4;
ptr[2] ^= 0x8e;
ptr[3] ^= 0x38;
ptr[4] += 0x41;
ptr[5] ^= 0x61;
ptr[6] ^= 0x51;
ptr[7] ^= 0xec;
ptr[8] ^= 0xed;
ptr[9] += 0x2;
ptr[10] ^= 0x14;
ptr[11] += 0x4f;
ptr[12] += 0x3f;
ptr[13] ^= 0xde;
ptr[14] ^= 0x52;
ptr[15] ^= 0x11;
ptr[16] += 0x22;
ptr[17] += 0x2c;

}

void multi_tracer_p68(struct user_regs_struct *regs)
{
if (regs->rax != 0x2044)
{
multi_tracer_p84(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xa480ce ^ regs->rbx);
ptr[0] += 0xc0;
ptr[1] ^= 0x80;
ptr[2] += 0x27;
ptr[3] ^= 0xf5;
ptr[4] ^= 0xc5;
ptr[5] += 0xd6;
ptr[6] ^= 0x70;
ptr[7] += 0xce;
ptr[8] ^= 0x17;
ptr[9] += 0xd2;
ptr[10] += 0x23;
ptr[11] += 0xa1;
ptr[12] += 0x50;
ptr[13] += 0x43;
ptr[14] += 0x59;
ptr[15] += 0xb3;
ptr[16] ^= 0xfb;
ptr[17] ^= 0xd8;
ptr[18] += 0x5a;
ptr[19] += 0xfb;
ptr[20] ^= 0x9;
ptr[21] += 0x2e;
ptr[22] ^= 0x3e;
ptr[23] += 0x5a;
ptr[24] ^= 0x20;
ptr[25] += 0x73;

}

void multi_tracer_p84(struct user_regs_struct *regs)
{
if (regs->rax != 0x2054)
{
multi_tracer_p226(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc41ab1 ^ regs->rbx);
ptr[0] ^= 0xf1;
ptr[1] += 0x44;
ptr[2] += 0xfa;
ptr[3] ^= 0x85;
ptr[4] += 0x68;
ptr[5] += 0x95;
ptr[6] += 0xdf;
ptr[7] += 0xc4;
ptr[8] ^= 0x27;
ptr[9] ^= 0x76;
ptr[10] ^= 0x33;
ptr[11] ^= 0xb7;
ptr[12] += 0xff;
ptr[13] ^= 0x1;
ptr[14] ^= 0x69;
ptr[15] += 0x86;
ptr[16] ^= 0x1c;
ptr[17] ^= 0xfb;
ptr[18] ^= 0x87;
ptr[19] += 0xea;
ptr[20] += 0xd1;
ptr[21] ^= 0x80;
ptr[22] ^= 0x3;
ptr[23] ^= 0x2f;
ptr[24] += 0x78;

}

void multi_tracer_p226(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e2)
{
multi_tracer_p248(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe7cf51 ^ regs->rbx);
ptr[0] ^= 0x70;
ptr[1] += 0xef;
ptr[2] ^= 0x8;
ptr[3] += 0xc4;
ptr[4] += 0x6e;
ptr[5] ^= 0x52;
ptr[6] += 0x2c;
ptr[7] += 0x84;
ptr[8] ^= 0xb9;
ptr[9] ^= 0xf3;
ptr[10] ^= 0xa8;
ptr[11] += 0x40;
ptr[12] ^= 0xcc;
ptr[13] ^= 0x25;
ptr[14] += 0xf3;
ptr[15] += 0x57;
ptr[16] += 0xd1;
ptr[17] += 0xad;
ptr[18] ^= 0x80;
ptr[19] ^= 0x6e;
ptr[20] += 0x94;
ptr[21] ^= 0xb;
ptr[22] ^= 0xdb;

}

void multi_tracer_p248(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f8)
{
multi_tracer_p147(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbb7002 ^ regs->rbx);
ptr[0] ^= 0xe7;
ptr[1] ^= 0x42;
ptr[2] += 0xb5;
ptr[3] += 0x84;
ptr[4] += 0x9;
ptr[5] += 0x66;
ptr[6] += 0x1d;
ptr[7] ^= 0xd3;
ptr[8] += 0x1f;
ptr[9] += 0x87;
ptr[10] += 0xab;
ptr[11] += 0x7;
ptr[12] ^= 0xfa;
ptr[13] += 0xfa;
ptr[14] += 0xcc;
ptr[15] ^= 0xe8;
ptr[16] += 0x4e;
ptr[17] ^= 0x6c;
ptr[18] += 0xf0;
ptr[19] ^= 0xe;
ptr[20] += 0xb5;
ptr[21] += 0xae;
ptr[22] ^= 0x32;
ptr[23] += 0xe2;

}

void multi_tracer_p147(struct user_regs_struct *regs)
{
if (regs->rax != 0x2093)
{
multi_tracer_p159(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x190111 ^ regs->rbx);
ptr[0] += 0xeb;
ptr[1] += 0x26;
ptr[2] ^= 0xc7;
ptr[3] += 0x6;
ptr[4] ^= 0x2;
ptr[5] ^= 0x2e;
ptr[6] += 0x83;
ptr[7] ^= 0x6e;
ptr[8] += 0x27;
ptr[9] ^= 0xfa;
ptr[10] ^= 0x29;
ptr[11] += 0x5f;
ptr[12] ^= 0x33;
ptr[13] ^= 0x42;
ptr[14] ^= 0x8e;
ptr[15] ^= 0x29;
ptr[16] += 0x64;
ptr[17] += 0x90;
ptr[18] ^= 0x67;
ptr[19] ^= 0x7b;
ptr[20] += 0x93;
ptr[21] += 0x5c;
ptr[22] ^= 0xb7;
ptr[23] += 0xf7;
ptr[24] ^= 0xab;
ptr[25] += 0x9a;

}

void multi_tracer_p159(struct user_regs_struct *regs)
{
if (regs->rax != 0x209f)
{
multi_tracer_p218(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf6bea2 ^ regs->rbx);
ptr[0] ^= 0x2d;
ptr[1] ^= 0xd0;
ptr[2] ^= 0x7d;
ptr[3] ^= 0x28;
ptr[4] ^= 0xec;
ptr[5] += 0x23;
ptr[6] ^= 0x5b;
ptr[7] += 0x2;
ptr[8] += 0xef;
ptr[9] ^= 0x5c;
ptr[10] ^= 0xc;
ptr[11] += 0xbf;
ptr[12] ^= 0x97;
ptr[13] ^= 0x55;
ptr[14] ^= 0x96;
ptr[15] += 0x37;
ptr[16] ^= 0xe7;
ptr[17] += 0x94;
ptr[18] ^= 0x92;
ptr[19] ^= 0xf3;
ptr[20] ^= 0x46;
ptr[21] ^= 0xa2;
ptr[22] += 0x6c;

}

void multi_tracer_p218(struct user_regs_struct *regs)
{
if (regs->rax != 0x20da)
{
multi_tracer_p181(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x475c4a ^ regs->rbx);
ptr[0] ^= 0x38;
ptr[1] += 0xf;
ptr[2] += 0x95;
ptr[3] += 0x30;
ptr[4] ^= 0x4f;
ptr[5] ^= 0xc;
ptr[6] ^= 0x2c;
ptr[7] += 0x11;
ptr[8] ^= 0xa4;
ptr[9] ^= 0xdb;
ptr[10] ^= 0x9d;
ptr[11] += 0xc8;
ptr[12] ^= 0x55;
ptr[13] += 0x65;
ptr[14] += 0x42;
ptr[15] ^= 0x6;
ptr[16] += 0x32;
ptr[17] ^= 0x61;
ptr[18] += 0xff;
ptr[19] += 0xae;
ptr[20] += 0x8a;
ptr[21] += 0xb4;

}

void multi_tracer_p181(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b5)
{
multi_tracer_p208(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x40bd1b ^ regs->rbx);
ptr[0] ^= 0x9c;
ptr[1] ^= 0x79;
ptr[2] ^= 0x1a;
ptr[3] += 0x66;
ptr[4] += 0x28;
ptr[5] ^= 0xe8;
ptr[6] ^= 0xc3;
ptr[7] ^= 0x36;
ptr[8] ^= 0x5b;
ptr[9] ^= 0xeb;
ptr[10] ^= 0x6b;
ptr[11] ^= 0x80;
ptr[12] += 0x28;
ptr[13] += 0x52;
ptr[14] += 0x22;
ptr[15] += 0x5;
ptr[16] ^= 0x92;
ptr[17] += 0x86;

}

void multi_tracer_p208(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d0)
{
multi_tracer_p207(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x996b60 ^ regs->rbx);
ptr[0] ^= 0x4c;
ptr[1] ^= 0x1;
ptr[2] ^= 0xab;
ptr[3] ^= 0x7f;
ptr[4] ^= 0xa6;
ptr[5] += 0x42;
ptr[6] += 0xd1;
ptr[7] ^= 0x76;
ptr[8] += 0xb1;
ptr[9] += 0x60;
ptr[10] ^= 0x3e;
ptr[11] += 0x1d;
ptr[12] ^= 0xbd;
ptr[13] += 0x43;
ptr[14] += 0xd;
ptr[15] ^= 0xb4;
ptr[16] += 0x2a;
ptr[17] ^= 0x24;
ptr[18] ^= 0xf9;
ptr[19] ^= 0x77;

}

void multi_tracer_p207(struct user_regs_struct *regs)
{
if (regs->rax != 0x20cf)
{
multi_tracer_p249(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd0b018 ^ regs->rbx);
ptr[0] += 0x27;
ptr[1] ^= 0xb7;
ptr[2] += 0xb5;
ptr[3] ^= 0x4f;
ptr[4] ^= 0x2;
ptr[5] ^= 0x83;
ptr[6] += 0x89;
ptr[7] += 0x55;
ptr[8] += 0xc9;
ptr[9] ^= 0xa1;
ptr[10] += 0x7;
ptr[11] ^= 0x83;
ptr[12] += 0x3c;
ptr[13] += 0xf2;
ptr[14] ^= 0xd9;
ptr[15] += 0x90;
ptr[16] ^= 0xda;
ptr[17] += 0x41;
ptr[18] ^= 0x78;

}

void multi_tracer_p249(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f9)
{
multi_tracer_p231(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8a0486 ^ regs->rbx);
ptr[0] += 0x7e;
ptr[1] ^= 0x4;
ptr[2] += 0x37;
ptr[3] += 0xd1;
ptr[4] ^= 0x1c;
ptr[5] += 0x5;
ptr[6] ^= 0x72;
ptr[7] ^= 0xd0;
ptr[8] += 0xea;
ptr[9] ^= 0xce;
ptr[10] += 0x9d;
ptr[11] += 0xd5;
ptr[12] ^= 0xc3;
ptr[13] ^= 0xee;
ptr[14] ^= 0x38;
ptr[15] += 0xb;
ptr[16] ^= 0xfb;
ptr[17] ^= 0x73;
ptr[18] += 0xe9;
ptr[19] += 0x6f;
ptr[20] += 0x3b;
ptr[21] += 0x94;
ptr[22] ^= 0x72;
ptr[23] += 0x4b;
ptr[24] ^= 0xf9;
ptr[25] ^= 0x5;
ptr[26] += 0xd0;
ptr[27] += 0x19;

}

void multi_tracer_p231(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e7)
{
multi_tracer_p53(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf405e7 ^ regs->rbx);
ptr[0] += 0xc8;
ptr[1] ^= 0x5c;
ptr[2] ^= 0x4b;
ptr[3] ^= 0x7e;
ptr[4] ^= 0xcc;
ptr[5] += 0x79;
ptr[6] ^= 0xba;
ptr[7] ^= 0xc8;
ptr[8] ^= 0x2d;
ptr[9] ^= 0x80;
ptr[10] ^= 0xf7;
ptr[11] += 0x66;
ptr[12] += 0xa1;
ptr[13] += 0xf9;
ptr[14] ^= 0xe0;
ptr[15] += 0xce;
ptr[16] ^= 0xb0;
ptr[17] += 0x86;
ptr[18] += 0x93;
ptr[19] ^= 0x61;
ptr[20] += 0x8b;
ptr[21] += 0x8e;
ptr[22] += 0xb4;
ptr[23] += 0x9b;
ptr[24] += 0xc5;
ptr[25] ^= 0x22;
ptr[26] ^= 0x57;

}

void multi_tracer_p53(struct user_regs_struct *regs)
{
if (regs->rax != 0x2035)
{
multi_tracer_p33(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x85c92c ^ regs->rbx);
ptr[0] += 0x87;
ptr[1] ^= 0xb1;
ptr[2] ^= 0xb;
ptr[3] += 0x4e;
ptr[4] += 0xc;
ptr[5] ^= 0x47;
ptr[6] ^= 0xb9;
ptr[7] ^= 0x5d;
ptr[8] ^= 0x2a;
ptr[9] += 0x92;
ptr[10] ^= 0x6e;
ptr[11] += 0x96;
ptr[12] ^= 0x3c;
ptr[13] ^= 0x42;
ptr[14] += 0xbb;
ptr[15] ^= 0x83;
ptr[16] ^= 0x8d;
ptr[17] += 0xeb;
ptr[18] ^= 0xc0;

}

void multi_tracer_p33(struct user_regs_struct *regs)
{
if (regs->rax != 0x2021)
{
multi_tracer_p245(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5a38b4 ^ regs->rbx);
ptr[0] ^= 0xe5;
ptr[1] ^= 0xfc;
ptr[2] ^= 0x68;
ptr[3] ^= 0x79;
ptr[4] ^= 0x8b;
ptr[5] += 0x42;
ptr[6] ^= 0xde;
ptr[7] ^= 0x92;
ptr[8] += 0x51;
ptr[9] ^= 0x1b;
ptr[10] ^= 0x5d;
ptr[11] += 0x77;
ptr[12] += 0xf8;
ptr[13] += 0x7a;
ptr[14] ^= 0x2c;
ptr[15] += 0x65;
ptr[16] += 0xe7;
ptr[17] += 0x19;
ptr[18] += 0x5b;
ptr[19] += 0x48;
ptr[20] += 0xf5;
ptr[21] += 0xd8;
ptr[22] += 0xeb;
ptr[23] += 0x2;

}

void multi_tracer_p245(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f5)
{
multi_tracer_p163(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2092d6 ^ regs->rbx);
ptr[0] ^= 0x93;
ptr[1] += 0xd3;
ptr[2] += 0x2;
ptr[3] ^= 0x14;
ptr[4] += 0xf2;
ptr[5] ^= 0x41;
ptr[6] += 0x7e;
ptr[7] += 0xbb;
ptr[8] += 0x8c;
ptr[9] += 0x58;
ptr[10] ^= 0x1e;
ptr[11] ^= 0x5c;
ptr[12] ^= 0xbd;
ptr[13] ^= 0x86;
ptr[14] += 0x19;
ptr[15] += 0x31;
ptr[16] += 0xde;
ptr[17] ^= 0x8a;
ptr[18] ^= 0x78;
ptr[19] ^= 0x3e;
ptr[20] ^= 0xbe;
ptr[21] ^= 0x73;

}

void multi_tracer_p163(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a3)
{
multi_tracer_p228(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xec480e ^ regs->rbx);
ptr[0] += 0x32;
ptr[1] ^= 0x68;
ptr[2] ^= 0x3f;
ptr[3] ^= 0x11;
ptr[4] += 0x85;
ptr[5] ^= 0xe2;
ptr[6] += 0x5b;
ptr[7] += 0x1a;
ptr[8] ^= 0x38;
ptr[9] += 0x24;
ptr[10] += 0x19;
ptr[11] += 0xf4;
ptr[12] ^= 0x5e;
ptr[13] ^= 0x80;
ptr[14] += 0x9c;
ptr[15] += 0xaa;
ptr[16] ^= 0xd5;
ptr[17] += 0x5d;
ptr[18] += 0xa9;
ptr[19] += 0xc4;

}

void multi_tracer_p228(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e4)
{
multi_tracer_p210(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xac0974 ^ regs->rbx);
ptr[0] += 0xfe;
ptr[1] ^= 0xe1;
ptr[2] ^= 0x57;
ptr[3] += 0x89;
ptr[4] += 0x2d;
ptr[5] += 0xfc;
ptr[6] ^= 0x9d;
ptr[7] ^= 0x9;
ptr[8] ^= 0x65;
ptr[9] += 0xef;
ptr[10] ^= 0xf1;
ptr[11] ^= 0xdb;
ptr[12] += 0x3b;
ptr[13] ^= 0xb0;
ptr[14] ^= 0xce;
ptr[15] ^= 0x48;
ptr[16] ^= 0x6f;
ptr[17] += 0x3b;
ptr[18] += 0xb9;
ptr[19] ^= 0x6a;
ptr[20] ^= 0x65;
ptr[21] ^= 0x17;
ptr[22] += 0xaf;
ptr[23] += 0xe6;

}

void multi_tracer_p210(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d2)
{
multi_tracer_p241(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd1d2dd ^ regs->rbx);
ptr[0] ^= 0x5d;
ptr[1] += 0x87;
ptr[2] += 0x15;
ptr[3] += 0x4c;
ptr[4] += 0x95;
ptr[5] += 0xf8;
ptr[6] += 0xbc;
ptr[7] ^= 0x58;
ptr[8] += 0x12;
ptr[9] ^= 0xd1;
ptr[10] ^= 0x65;
ptr[11] ^= 0x7d;
ptr[12] ^= 0x63;
ptr[13] += 0x8e;
ptr[14] += 0x3e;
ptr[15] += 0x5e;
ptr[16] += 0xc4;
ptr[17] += 0x26;
ptr[18] += 0x44;
ptr[19] ^= 0x4f;
ptr[20] += 0x51;
ptr[21] ^= 0x37;
ptr[22] += 0xe7;
ptr[23] ^= 0x40;
ptr[24] ^= 0x3;
ptr[25] ^= 0x80;
ptr[26] ^= 0x13;

}

void multi_tracer_p241(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f1)
{
multi_tracer_p66(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xab05e5 ^ regs->rbx);
ptr[0] ^= 0xe8;
ptr[1] ^= 0x47;
ptr[2] += 0x2b;
ptr[3] ^= 0x5e;
ptr[4] ^= 0x90;
ptr[5] += 0x77;
ptr[6] ^= 0x30;
ptr[7] += 0x2d;
ptr[8] += 0x12;
ptr[9] ^= 0x32;
ptr[10] += 0x99;
ptr[11] += 0xf9;
ptr[12] ^= 0x62;
ptr[13] += 0x6;
ptr[14] ^= 0x59;
ptr[15] ^= 0xd6;
ptr[16] += 0x35;
ptr[17] += 0x16;
ptr[18] += 0xa5;

}

void multi_tracer_p66(struct user_regs_struct *regs)
{
if (regs->rax != 0x2042)
{
multi_tracer_p97(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9fd693 ^ regs->rbx);
ptr[0] += 0xd0;
ptr[1] ^= 0x11;
ptr[2] += 0x8d;
ptr[3] += 0x67;
ptr[4] ^= 0xf6;
ptr[5] += 0xf;
ptr[6] ^= 0x1e;
ptr[7] ^= 0x2d;
ptr[8] += 0x96;
ptr[9] ^= 0xa2;
ptr[10] ^= 0x53;
ptr[11] ^= 0x7a;
ptr[12] += 0x3a;
ptr[13] ^= 0xc0;
ptr[14] += 0x4d;
ptr[15] += 0x70;
ptr[16] += 0x46;
ptr[17] ^= 0x30;
ptr[18] += 0x2f;
ptr[19] += 0x5a;
ptr[20] += 0xd9;
ptr[21] ^= 0xc1;
ptr[22] ^= 0xd0;
ptr[23] ^= 0x25;
ptr[24] ^= 0x69;

}

void multi_tracer_p97(struct user_regs_struct *regs)
{
if (regs->rax != 0x2061)
{
multi_tracer_p39(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x52b288 ^ regs->rbx);
ptr[0] ^= 0x5f;
ptr[1] ^= 0xc;
ptr[2] += 0x36;
ptr[3] ^= 0x43;
ptr[4] ^= 0x6f;
ptr[5] ^= 0x7c;
ptr[6] ^= 0x93;
ptr[7] += 0xea;
ptr[8] ^= 0x61;
ptr[9] ^= 0x94;
ptr[10] ^= 0x3f;
ptr[11] ^= 0x13;
ptr[12] ^= 0x6c;
ptr[13] += 0xc2;
ptr[14] += 0x5f;
ptr[15] += 0xd1;
ptr[16] += 0xe8;
ptr[17] += 0x15;
ptr[18] ^= 0xd3;
ptr[19] += 0xd7;
ptr[20] += 0x45;
ptr[21] += 0x57;
ptr[22] += 0x93;

}

void multi_tracer_p39(struct user_regs_struct *regs)
{
if (regs->rax != 0x2027)
{
multi_tracer_p16(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc7b28d ^ regs->rbx);
ptr[0] += 0xae;
ptr[1] ^= 0xef;
ptr[2] += 0x21;
ptr[3] ^= 0x8f;
ptr[4] += 0x18;
ptr[5] ^= 0x91;
ptr[6] += 0x10;
ptr[7] += 0xce;
ptr[8] += 0xc9;
ptr[9] += 0x50;
ptr[10] += 0x80;
ptr[11] ^= 0xe9;
ptr[12] += 0xf1;
ptr[13] += 0x9b;
ptr[14] ^= 0x46;
ptr[15] += 0x51;
ptr[16] += 0xef;
ptr[17] ^= 0x7b;
ptr[18] += 0x12;
ptr[19] ^= 0xed;
ptr[20] += 0x6b;
ptr[21] += 0x71;
ptr[22] ^= 0x21;
ptr[23] += 0x63;
ptr[24] += 0x82;
ptr[25] ^= 0xd7;
ptr[26] ^= 0x98;

}

void multi_tracer_p16(struct user_regs_struct *regs)
{
if (regs->rax != 0x2010)
{
multi_tracer_p148(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x548729 ^ regs->rbx);
ptr[0] ^= 0xf3;
ptr[1] += 0x7a;
ptr[2] ^= 0x3c;
ptr[3] ^= 0x8e;
ptr[4] ^= 0xb7;
ptr[5] += 0xb8;
ptr[6] += 0x58;
ptr[7] += 0x73;
ptr[8] ^= 0xf4;
ptr[9] += 0x62;
ptr[10] += 0x83;
ptr[11] += 0xe3;
ptr[12] ^= 0xdc;
ptr[13] += 0x95;
ptr[14] ^= 0xb;
ptr[15] += 0x64;
ptr[16] += 0x60;
ptr[17] ^= 0xd0;
ptr[18] ^= 0xfd;
ptr[19] += 0x54;
ptr[20] ^= 0xb5;
ptr[21] ^= 0xb3;

}

void multi_tracer_p148(struct user_regs_struct *regs)
{
if (regs->rax != 0x2094)
{
multi_tracer_p45(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9f407a ^ regs->rbx);
ptr[0] += 0x26;
ptr[1] ^= 0x4f;
ptr[2] += 0xcb;
ptr[3] ^= 0xcd;
ptr[4] ^= 0x36;
ptr[5] ^= 0xf8;
ptr[6] ^= 0xd2;
ptr[7] ^= 0xc8;
ptr[8] ^= 0x1c;
ptr[9] += 0x6e;
ptr[10] ^= 0x9a;
ptr[11] ^= 0x1;
ptr[12] += 0x86;
ptr[13] += 0x26;
ptr[14] ^= 0x7c;
ptr[15] += 0x36;
ptr[16] ^= 0x99;
ptr[17] ^= 0x18;
ptr[18] ^= 0xf2;
ptr[19] ^= 0x70;
ptr[20] += 0x32;
ptr[21] ^= 0x2b;
ptr[22] += 0xc1;
ptr[23] += 0xdf;
ptr[24] += 0x12;
ptr[25] += 0x80;
ptr[26] += 0x3;
ptr[27] ^= 0xee;

}

void multi_tracer_p45(struct user_regs_struct *regs)
{
if (regs->rax != 0x202d)
{
multi_tracer_p0(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2493d6 ^ regs->rbx);
ptr[0] += 0x71;
ptr[1] ^= 0x57;
ptr[2] += 0x68;
ptr[3] ^= 0xde;
ptr[4] += 0xc2;
ptr[5] ^= 0xfa;
ptr[6] += 0xf5;
ptr[7] ^= 0x17;
ptr[8] += 0x4f;
ptr[9] ^= 0x6b;
ptr[10] ^= 0x3d;
ptr[11] += 0xda;
ptr[12] ^= 0xc0;
ptr[13] += 0x57;
ptr[14] ^= 0x47;
ptr[15] += 0x8;
ptr[16] ^= 0x26;
ptr[17] += 0x9b;
ptr[18] ^= 0x9f;
ptr[19] += 0x3;
ptr[20] += 0xde;
ptr[21] ^= 0xc5;
ptr[22] += 0x89;
ptr[23] += 0x61;
ptr[24] += 0x4f;

}

void multi_tracer_p0(struct user_regs_struct *regs)
{
if (regs->rax != 0x2000)
{
multi_tracer_p223(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2f6860 ^ regs->rbx);
ptr[0] ^= 0xfb;
ptr[1] ^= 0xb;
ptr[2] += 0x12;
ptr[3] ^= 0x7;
ptr[4] ^= 0xad;
ptr[5] += 0xa1;
ptr[6] += 0x53;
ptr[7] += 0xb0;
ptr[8] += 0x77;
ptr[9] ^= 0x20;
ptr[10] ^= 0x99;
ptr[11] ^= 0x5a;
ptr[12] ^= 0xf4;
ptr[13] += 0xb3;
ptr[14] += 0xe7;
ptr[15] += 0x26;
ptr[16] ^= 0x64;
ptr[17] += 0x7e;
ptr[18] ^= 0x42;

}

void multi_tracer_p223(struct user_regs_struct *regs)
{
if (regs->rax != 0x20df)
{
multi_tracer_p22(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x917e1e ^ regs->rbx);
ptr[0] += 0x4e;
ptr[1] += 0xff;
ptr[2] += 0xc8;
ptr[3] += 0xb2;
ptr[4] ^= 0x7e;
ptr[5] += 0x57;
ptr[6] += 0x27;
ptr[7] ^= 0x83;
ptr[8] ^= 0xf;
ptr[9] += 0x9c;
ptr[10] ^= 0x76;
ptr[11] += 0x76;
ptr[12] ^= 0xe4;
ptr[13] ^= 0xef;
ptr[14] += 0x8;
ptr[15] += 0x54;
ptr[16] ^= 0x43;
ptr[17] ^= 0x72;
ptr[18] ^= 0x57;
ptr[19] += 0x9b;

}

void multi_tracer_p22(struct user_regs_struct *regs)
{
if (regs->rax != 0x2016)
{
multi_tracer_p69(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xec6ab1 ^ regs->rbx);
ptr[0] += 0xa3;
ptr[1] += 0x83;
ptr[2] += 0xde;
ptr[3] ^= 0x5a;
ptr[4] ^= 0xec;
ptr[5] ^= 0x6e;
ptr[6] += 0x27;
ptr[7] += 0x4a;
ptr[8] ^= 0x85;
ptr[9] += 0x51;
ptr[10] += 0xe2;
ptr[11] ^= 0x10;
ptr[12] ^= 0x87;
ptr[13] += 0xf4;
ptr[14] ^= 0xc9;
ptr[15] ^= 0xce;
ptr[16] ^= 0xbe;
ptr[17] ^= 0xa8;
ptr[18] += 0xf9;
ptr[19] ^= 0x5c;

}

void multi_tracer_p69(struct user_regs_struct *regs)
{
if (regs->rax != 0x2045)
{
multi_tracer_p29(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe46e31 ^ regs->rbx);
ptr[0] += 0x1d;
ptr[1] += 0x9e;
ptr[2] += 0xe9;
ptr[3] ^= 0xc1;
ptr[4] += 0xb3;
ptr[5] ^= 0xab;
ptr[6] += 0x47;
ptr[7] ^= 0xa7;
ptr[8] ^= 0x7f;
ptr[9] += 0x70;
ptr[10] ^= 0x70;
ptr[11] += 0x20;
ptr[12] += 0xe1;
ptr[13] ^= 0x2c;
ptr[14] += 0x1d;
ptr[15] ^= 0xca;
ptr[16] ^= 0xbf;
ptr[17] ^= 0x67;
ptr[18] ^= 0xfd;
ptr[19] += 0x95;
ptr[20] += 0x5;
ptr[21] += 0x5;
ptr[22] ^= 0x67;
ptr[23] += 0x2b;
ptr[24] ^= 0xb9;

}

void multi_tracer_p29(struct user_regs_struct *regs)
{
if (regs->rax != 0x201d)
{
multi_tracer_p115(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xeca3a5 ^ regs->rbx);
ptr[0] ^= 0xf5;
ptr[1] += 0x2a;
ptr[2] ^= 0xf7;
ptr[3] += 0xd6;
ptr[4] += 0xe6;
ptr[5] += 0x4b;
ptr[6] += 0xe8;
ptr[7] += 0x66;
ptr[8] += 0x89;
ptr[9] ^= 0xc6;
ptr[10] += 0x7c;
ptr[11] += 0x1d;
ptr[12] ^= 0xf2;
ptr[13] += 0x9b;
ptr[14] += 0x40;
ptr[15] ^= 0x65;
ptr[16] ^= 0xde;
ptr[17] += 0x1c;
ptr[18] += 0xf1;
ptr[19] ^= 0xd7;
ptr[20] += 0x65;
ptr[21] ^= 0x8f;
ptr[22] += 0x4c;
ptr[23] += 0xd;
ptr[24] += 0x23;

}

void multi_tracer_p115(struct user_regs_struct *regs)
{
if (regs->rax != 0x2073)
{
multi_tracer_p172(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xdd2019 ^ regs->rbx);
ptr[0] += 0x58;
ptr[1] ^= 0x8c;
ptr[2] += 0xed;
ptr[3] ^= 0x77;
ptr[4] += 0x3;
ptr[5] += 0xb7;
ptr[6] += 0xb;
ptr[7] += 0xc7;
ptr[8] += 0xea;
ptr[9] += 0x8d;
ptr[10] ^= 0xc0;
ptr[11] += 0x53;
ptr[12] += 0xe9;
ptr[13] ^= 0x6e;
ptr[14] ^= 0xb0;
ptr[15] ^= 0x2f;
ptr[16] += 0x95;
ptr[17] ^= 0xc0;
ptr[18] ^= 0xbe;
ptr[19] += 0x16;
ptr[20] += 0xac;
ptr[21] += 0x7b;
ptr[22] ^= 0x14;
ptr[23] ^= 0xcb;
ptr[24] ^= 0x65;
ptr[25] += 0xf7;

}

void multi_tracer_p172(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ac)
{
multi_tracer_p38(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf225b3 ^ regs->rbx);
ptr[0] += 0x27;
ptr[1] ^= 0x30;
ptr[2] ^= 0x23;
ptr[3] += 0x71;
ptr[4] ^= 0x50;
ptr[5] += 0xd2;
ptr[6] ^= 0x3a;
ptr[7] += 0x32;
ptr[8] ^= 0xfa;
ptr[9] += 0xe0;
ptr[10] ^= 0xd8;
ptr[11] ^= 0x21;
ptr[12] ^= 0xb0;
ptr[13] ^= 0x3f;
ptr[14] ^= 0x9;
ptr[15] += 0xc1;
ptr[16] += 0xe7;
ptr[17] += 0x66;
ptr[18] ^= 0xcf;
ptr[19] += 0x9a;
ptr[20] ^= 0xbb;

}

void multi_tracer_p38(struct user_regs_struct *regs)
{
if (regs->rax != 0x2026)
{
multi_tracer_p182(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xcfaf1c ^ regs->rbx);
ptr[0] ^= 0x43;
ptr[1] += 0xf0;
ptr[2] += 0x1;
ptr[3] ^= 0x84;
ptr[4] ^= 0x66;
ptr[5] += 0x23;
ptr[6] ^= 0x2e;
ptr[7] ^= 0xab;
ptr[8] += 0xb3;
ptr[9] ^= 0x19;
ptr[10] += 0x54;
ptr[11] += 0x6e;
ptr[12] += 0xbb;
ptr[13] ^= 0x72;
ptr[14] += 0x16;
ptr[15] += 0xcb;
ptr[16] += 0x44;
ptr[17] ^= 0xc6;
ptr[18] ^= 0xc3;
ptr[19] += 0x71;
ptr[20] ^= 0x6c;
ptr[21] ^= 0x80;

}

void multi_tracer_p182(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b6)
{
multi_tracer_p253(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x91eab2 ^ regs->rbx);
ptr[0] += 0x37;
ptr[1] ^= 0xb5;
ptr[2] ^= 0x95;
ptr[3] += 0x7b;
ptr[4] ^= 0x1b;
ptr[5] ^= 0x5c;
ptr[6] ^= 0xe1;
ptr[7] ^= 0x31;
ptr[8] ^= 0x6d;
ptr[9] ^= 0xf0;
ptr[10] ^= 0x47;
ptr[11] += 0x2e;
ptr[12] += 0xe5;
ptr[13] += 0xd5;
ptr[14] ^= 0xa8;
ptr[15] ^= 0x34;
ptr[16] += 0x8;
ptr[17] ^= 0x4a;
ptr[18] += 0xf4;
ptr[19] += 0x76;
ptr[20] += 0xa5;
ptr[21] ^= 0xba;
ptr[22] += 0xb;
ptr[23] += 0x28;
ptr[24] ^= 0xdb;

}

void multi_tracer_p253(struct user_regs_struct *regs)
{
if (regs->rax != 0x20fd)
{
multi_tracer_p129(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x6ba2e0 ^ regs->rbx);
ptr[0] += 0x7;
ptr[1] ^= 0x5;
ptr[2] += 0x86;
ptr[3] += 0xc8;
ptr[4] ^= 0xfb;
ptr[5] += 0xb7;
ptr[6] ^= 0xce;
ptr[7] += 0xd3;
ptr[8] ^= 0x27;
ptr[9] ^= 0x65;
ptr[10] ^= 0x22;
ptr[11] ^= 0x65;
ptr[12] += 0x8b;
ptr[13] += 0xaa;
ptr[14] ^= 0x6d;
ptr[15] += 0xf8;
ptr[16] ^= 0x66;
ptr[17] ^= 0x9c;
ptr[18] ^= 0xcb;
ptr[19] += 0x27;
ptr[20] += 0x83;
ptr[21] ^= 0x28;
ptr[22] += 0x51;
ptr[23] ^= 0xe;
ptr[24] += 0xb4;
ptr[25] ^= 0xfb;
ptr[26] += 0x79;
ptr[27] ^= 0xe;

}

void multi_tracer_p129(struct user_regs_struct *regs)
{
if (regs->rax != 0x2081)
{
multi_tracer_p100(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc98a4e ^ regs->rbx);
ptr[0] += 0x9d;
ptr[1] ^= 0x2f;
ptr[2] += 0x30;
ptr[3] ^= 0x97;
ptr[4] += 0x99;
ptr[5] ^= 0x4a;
ptr[6] += 0x26;
ptr[7] += 0x1f;
ptr[8] += 0xad;
ptr[9] += 0x44;
ptr[10] += 0x9a;
ptr[11] += 0x8a;
ptr[12] += 0x76;
ptr[13] += 0x54;
ptr[14] ^= 0x42;
ptr[15] ^= 0x23;
ptr[16] += 0x4a;
ptr[17] ^= 0xcd;
ptr[18] ^= 0xf9;
ptr[19] ^= 0x66;
ptr[20] ^= 0x8c;
ptr[21] += 0xd8;
ptr[22] += 0x72;
ptr[23] += 0x5c;
ptr[24] += 0xac;

}

void multi_tracer_p100(struct user_regs_struct *regs)
{
if (regs->rax != 0x2064)
{
multi_tracer_p209(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xd7b8a7 ^ regs->rbx);
ptr[0] ^= 0xe6;
ptr[1] ^= 0x69;
ptr[2] ^= 0x42;
ptr[3] += 0xb;
ptr[4] ^= 0xc2;
ptr[5] ^= 0x19;
ptr[6] ^= 0xec;
ptr[7] ^= 0x27;
ptr[8] += 0xa3;
ptr[9] += 0xf;
ptr[10] ^= 0xfe;
ptr[11] ^= 0x79;
ptr[12] += 0x1d;
ptr[13] ^= 0xae;
ptr[14] ^= 0x63;
ptr[15] += 0xe8;
ptr[16] += 0x3;
ptr[17] += 0x81;
ptr[18] += 0x3a;
ptr[19] += 0xa4;
ptr[20] ^= 0x80;
ptr[21] += 0x42;

}

void multi_tracer_p209(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d1)
{
multi_tracer_p48(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x407606 ^ regs->rbx);
ptr[0] += 0xfb;
ptr[1] ^= 0x35;
ptr[2] += 0x6b;
ptr[3] ^= 0x5f;
ptr[4] ^= 0x75;
ptr[5] += 0xcf;
ptr[6] += 0x99;
ptr[7] ^= 0xa8;
ptr[8] += 0xd7;
ptr[9] += 0x8a;
ptr[10] ^= 0x9b;
ptr[11] += 0x46;
ptr[12] += 0xfc;
ptr[13] += 0x8f;
ptr[14] += 0x2b;
ptr[15] ^= 0x76;
ptr[16] += 0xfc;
ptr[17] ^= 0xdc;
ptr[18] ^= 0x9f;
ptr[19] += 0x66;
ptr[20] ^= 0x8e;
ptr[21] ^= 0xed;
ptr[22] ^= 0xf9;
ptr[23] ^= 0x9f;
ptr[24] ^= 0xf7;

}

void multi_tracer_p48(struct user_regs_struct *regs)
{
if (regs->rax != 0x2030)
{
multi_tracer_p222(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x589790 ^ regs->rbx);
ptr[0] += 0xf;
ptr[1] ^= 0xde;
ptr[2] ^= 0x51;
ptr[3] ^= 0xf;
ptr[4] ^= 0x1d;
ptr[5] ^= 0xb;
ptr[6] ^= 0x3e;
ptr[7] ^= 0x97;
ptr[8] += 0x9e;
ptr[9] ^= 0xa6;
ptr[10] += 0x9c;
ptr[11] += 0x62;
ptr[12] += 0x1c;
ptr[13] += 0x53;
ptr[14] ^= 0x6b;
ptr[15] += 0x3e;
ptr[16] ^= 0x1d;
ptr[17] += 0x1f;
ptr[18] += 0x6c;
ptr[19] += 0xa1;
ptr[20] += 0x4d;
ptr[21] ^= 0x8e;
ptr[22] ^= 0xc2;
ptr[23] ^= 0xa7;
ptr[24] += 0xa1;
ptr[25] ^= 0xa;
ptr[26] ^= 0x5d;
ptr[27] ^= 0x2;

}

void multi_tracer_p222(struct user_regs_struct *regs)
{
if (regs->rax != 0x20de)
{
multi_tracer_p71(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8d4485 ^ regs->rbx);
ptr[0] ^= 0xf0;
ptr[1] += 0x62;
ptr[2] ^= 0xc4;
ptr[3] += 0x66;
ptr[4] += 0x96;
ptr[5] ^= 0xc6;
ptr[6] += 0x96;
ptr[7] ^= 0xbb;
ptr[8] += 0x2a;
ptr[9] ^= 0xa3;
ptr[10] ^= 0x51;
ptr[11] ^= 0xd7;
ptr[12] ^= 0xa9;
ptr[13] ^= 0xa3;
ptr[14] += 0xc1;
ptr[15] += 0xb;
ptr[16] += 0x5e;
ptr[17] += 0x25;

}

void multi_tracer_p71(struct user_regs_struct *regs)
{
if (regs->rax != 0x2047)
{
multi_tracer_p106(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc81143 ^ regs->rbx);
ptr[0] ^= 0xd3;
ptr[1] += 0x20;
ptr[2] ^= 0x23;
ptr[3] += 0xd6;
ptr[4] ^= 0xe0;
ptr[5] += 0x59;
ptr[6] ^= 0x9c;
ptr[7] ^= 0xae;
ptr[8] += 0x56;
ptr[9] ^= 0x8;
ptr[10] += 0xc4;
ptr[11] += 0x47;
ptr[12] ^= 0x24;
ptr[13] += 0xf3;
ptr[14] += 0x82;
ptr[15] += 0xa8;
ptr[16] += 0x25;
ptr[17] += 0x65;
ptr[18] ^= 0x12;
ptr[19] ^= 0xbf;
ptr[20] += 0x6c;
ptr[21] ^= 0x78;
ptr[22] ^= 0xe7;
ptr[23] += 0x3;
ptr[24] ^= 0x14;
ptr[25] += 0x8e;

}

void multi_tracer_p106(struct user_regs_struct *regs)
{
if (regs->rax != 0x206a)
{
multi_tracer_p12(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x51dc1b ^ regs->rbx);
ptr[0] += 0x51;
ptr[1] += 0x6e;
ptr[2] += 0xd0;
ptr[3] += 0xf9;
ptr[4] += 0x5e;
ptr[5] ^= 0x62;
ptr[6] += 0x8e;
ptr[7] += 0x9f;
ptr[8] += 0xea;
ptr[9] ^= 0x2;
ptr[10] ^= 0x14;
ptr[11] += 0xf9;
ptr[12] += 0x43;
ptr[13] += 0xfb;
ptr[14] += 0x56;
ptr[15] ^= 0x64;
ptr[16] ^= 0x56;
ptr[17] ^= 0x3d;
ptr[18] ^= 0x70;
ptr[19] ^= 0x1f;
ptr[20] += 0x25;
ptr[21] ^= 0xdd;
ptr[22] ^= 0x51;
ptr[23] ^= 0xa4;
ptr[24] += 0x23;
ptr[25] += 0xa6;

}

void multi_tracer_p12(struct user_regs_struct *regs)
{
if (regs->rax != 0x200c)
{
multi_tracer_p238(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5b8be4 ^ regs->rbx);
ptr[0] ^= 0xb5;
ptr[1] ^= 0x78;
ptr[2] ^= 0x3f;
ptr[3] += 0xfa;
ptr[4] += 0x3d;
ptr[5] += 0x4c;
ptr[6] += 0xe;
ptr[7] += 0xbc;
ptr[8] ^= 0xe1;
ptr[9] += 0xbd;
ptr[10] ^= 0xac;
ptr[11] ^= 0x49;
ptr[12] ^= 0xcb;
ptr[13] ^= 0xd6;
ptr[14] ^= 0xed;
ptr[15] += 0xec;
ptr[16] += 0x44;
ptr[17] ^= 0x78;
ptr[18] += 0xcd;
ptr[19] += 0xe3;
ptr[20] ^= 0x75;

}

void multi_tracer_p238(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ee)
{
multi_tracer_p42(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xae7e52 ^ regs->rbx);
ptr[0] ^= 0xbb;
ptr[1] += 0x6f;
ptr[2] ^= 0x54;
ptr[3] ^= 0x84;
ptr[4] += 0xce;
ptr[5] ^= 0x42;
ptr[6] ^= 0xd3;
ptr[7] ^= 0x47;
ptr[8] += 0xfa;
ptr[9] += 0xc9;
ptr[10] ^= 0x9c;
ptr[11] += 0x87;
ptr[12] ^= 0x64;
ptr[13] ^= 0x8b;
ptr[14] ^= 0x6;
ptr[15] += 0x3c;
ptr[16] ^= 0x8d;
ptr[17] ^= 0xe9;
ptr[18] += 0xcc;
ptr[19] += 0x26;

}

void multi_tracer_p42(struct user_regs_struct *regs)
{
if (regs->rax != 0x202a)
{
multi_tracer_p171(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x744af0 ^ regs->rbx);
ptr[0] ^= 0xd;
ptr[1] += 0x56;
ptr[2] += 0x1e;
ptr[3] ^= 0xb;
ptr[4] ^= 0xb;
ptr[5] ^= 0x87;
ptr[6] ^= 0xf2;
ptr[7] ^= 0x7e;
ptr[8] ^= 0x51;
ptr[9] ^= 0xb8;
ptr[10] ^= 0xc7;
ptr[11] += 0x6b;
ptr[12] ^= 0xfa;
ptr[13] += 0x82;
ptr[14] ^= 0x26;
ptr[15] ^= 0x69;
ptr[16] += 0xae;
ptr[17] ^= 0x2;
ptr[18] ^= 0x69;
ptr[19] += 0xab;
ptr[20] ^= 0xf5;
ptr[21] ^= 0xe9;
ptr[22] += 0x6;
ptr[23] ^= 0x73;
ptr[24] += 0x38;
ptr[25] ^= 0x68;
ptr[26] += 0x49;
ptr[27] ^= 0x16;

}

void multi_tracer_p171(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ab)
{
multi_tracer_p43(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7c24d1 ^ regs->rbx);
ptr[0] += 0xc0;
ptr[1] ^= 0x97;
ptr[2] += 0xee;
ptr[3] ^= 0x75;
ptr[4] ^= 0x4d;
ptr[5] += 0xc7;
ptr[6] ^= 0x98;
ptr[7] += 0xff;
ptr[8] += 0x74;
ptr[9] ^= 0x9c;
ptr[10] += 0xc7;
ptr[11] ^= 0x49;
ptr[12] += 0x76;
ptr[13] += 0x38;
ptr[14] += 0x95;
ptr[15] += 0xb3;
ptr[16] += 0xe4;
ptr[17] += 0x85;
ptr[18] += 0x46;
ptr[19] += 0x39;
ptr[20] ^= 0xf4;
ptr[21] += 0xb6;

}

void multi_tracer_p43(struct user_regs_struct *regs)
{
if (regs->rax != 0x202b)
{
multi_tracer_p27(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x986309 ^ regs->rbx);
ptr[0] += 0x34;
ptr[1] ^= 0xf8;
ptr[2] += 0x2f;
ptr[3] += 0xbb;
ptr[4] += 0xc5;
ptr[5] += 0x7f;
ptr[6] += 0xed;
ptr[7] ^= 0x1a;
ptr[8] ^= 0xde;
ptr[9] ^= 0x7d;
ptr[10] += 0xbd;
ptr[11] ^= 0x67;
ptr[12] ^= 0x97;
ptr[13] += 0x78;
ptr[14] ^= 0xcc;
ptr[15] += 0x12;
ptr[16] += 0x58;
ptr[17] += 0x5;
ptr[18] ^= 0x33;
ptr[19] ^= 0x45;
ptr[20] ^= 0xf4;
ptr[21] ^= 0x21;
ptr[22] += 0x1d;
ptr[23] ^= 0xeb;
ptr[24] += 0xd;
ptr[25] += 0x63;
ptr[26] += 0xa1;

}

void multi_tracer_p27(struct user_regs_struct *regs)
{
if (regs->rax != 0x201b)
{
multi_tracer_p195(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x241ff5 ^ regs->rbx);
ptr[0] += 0x63;
ptr[1] ^= 0x1a;
ptr[2] ^= 0x65;
ptr[3] ^= 0x64;
ptr[4] ^= 0xa3;
ptr[5] += 0x71;
ptr[6] += 0x50;
ptr[7] ^= 0xc7;
ptr[8] ^= 0xd5;
ptr[9] ^= 0xf8;
ptr[10] ^= 0x4e;
ptr[11] += 0xac;
ptr[12] += 0x32;
ptr[13] += 0x72;
ptr[14] ^= 0xdc;
ptr[15] ^= 0xa0;
ptr[16] += 0x73;
ptr[17] ^= 0x38;
ptr[18] += 0xef;
ptr[19] ^= 0x6a;
ptr[20] ^= 0x79;
ptr[21] ^= 0xb7;
ptr[22] += 0xc9;

}

void multi_tracer_p195(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c3)
{
multi_tracer_p107(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x760479 ^ regs->rbx);
ptr[0] ^= 0x62;
ptr[1] += 0x37;
ptr[2] ^= 0x24;
ptr[3] ^= 0x28;
ptr[4] += 0xfc;
ptr[5] ^= 0xdf;
ptr[6] ^= 0x17;
ptr[7] += 0xc4;
ptr[8] ^= 0x7a;
ptr[9] ^= 0xbf;
ptr[10] += 0x11;
ptr[11] ^= 0x89;
ptr[12] += 0x96;
ptr[13] += 0xfe;
ptr[14] += 0x79;
ptr[15] ^= 0x44;
ptr[16] ^= 0x4a;
ptr[17] += 0x4b;
ptr[18] += 0x3;
ptr[19] ^= 0x82;
ptr[20] += 0x42;
ptr[21] ^= 0x8a;
ptr[22] ^= 0xbc;
ptr[23] += 0xb6;
ptr[24] ^= 0x56;
ptr[25] ^= 0xeb;

}

void multi_tracer_p107(struct user_regs_struct *regs)
{
if (regs->rax != 0x206b)
{
multi_tracer_p203(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9aa30d ^ regs->rbx);
ptr[0] ^= 0x31;
ptr[1] += 0x9b;
ptr[2] += 0x37;
ptr[3] ^= 0x57;
ptr[4] ^= 0x57;
ptr[5] += 0x25;
ptr[6] ^= 0x7c;
ptr[7] += 0x45;
ptr[8] ^= 0x17;
ptr[9] += 0xe9;
ptr[10] += 0x9;
ptr[11] += 0xe7;
ptr[12] ^= 0x45;
ptr[13] += 0x2e;
ptr[14] ^= 0x8c;
ptr[15] ^= 0xf5;
ptr[16] ^= 0x8c;
ptr[17] ^= 0xf3;
ptr[18] ^= 0xe1;
ptr[19] ^= 0xe8;
ptr[20] ^= 0x36;
ptr[21] ^= 0xc6;
ptr[22] ^= 0x26;
ptr[23] ^= 0xac;
ptr[24] ^= 0xf5;
ptr[25] ^= 0x5a;

}

void multi_tracer_p203(struct user_regs_struct *regs)
{
if (regs->rax != 0x20cb)
{
multi_tracer_p94(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xcaf8cc ^ regs->rbx);
ptr[0] += 0x5a;
ptr[1] += 0xff;
ptr[2] ^= 0x1d;
ptr[3] ^= 0xb2;
ptr[4] ^= 0x1d;
ptr[5] += 0x3a;
ptr[6] += 0x29;
ptr[7] += 0x39;
ptr[8] ^= 0x2c;
ptr[9] ^= 0x84;
ptr[10] ^= 0x1f;
ptr[11] += 0x1f;
ptr[12] ^= 0xd5;
ptr[13] += 0xbf;
ptr[14] ^= 0x2c;
ptr[15] ^= 0xf;
ptr[16] += 0x99;
ptr[17] += 0x80;
ptr[18] += 0xf9;

}

void multi_tracer_p94(struct user_regs_struct *regs)
{
if (regs->rax != 0x205e)
{
multi_tracer_p243(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x67362d ^ regs->rbx);
ptr[0] ^= 0x74;
ptr[1] ^= 0x46;
ptr[2] += 0x27;
ptr[3] += 0xd6;
ptr[4] += 0xda;
ptr[5] += 0xd0;
ptr[6] += 0xab;
ptr[7] ^= 0x8b;
ptr[8] += 0xc8;
ptr[9] ^= 0x45;
ptr[10] += 0x6e;
ptr[11] ^= 0x1c;
ptr[12] ^= 0x5c;
ptr[13] ^= 0x4c;
ptr[14] += 0xd9;
ptr[15] ^= 0x35;
ptr[16] ^= 0x97;
ptr[17] += 0x65;
ptr[18] += 0xdc;
ptr[19] += 0x67;
ptr[20] ^= 0xc1;

}

void multi_tracer_p243(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f3)
{
multi_tracer_p179(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xfa1f65 ^ regs->rbx);
ptr[0] += 0x61;
ptr[1] ^= 0xfc;
ptr[2] ^= 0x45;
ptr[3] ^= 0x25;
ptr[4] += 0xca;
ptr[5] ^= 0x29;
ptr[6] ^= 0x83;
ptr[7] ^= 0x29;
ptr[8] ^= 0x91;
ptr[9] ^= 0x7d;
ptr[10] ^= 0x6f;
ptr[11] ^= 0xdc;
ptr[12] ^= 0x3f;
ptr[13] ^= 0x2e;
ptr[14] ^= 0x18;
ptr[15] ^= 0x5;
ptr[16] ^= 0x4;
ptr[17] ^= 0xa7;
ptr[18] += 0x7;
ptr[19] ^= 0xec;
ptr[20] += 0xba;
ptr[21] ^= 0x10;
ptr[22] ^= 0x48;
ptr[23] ^= 0xb0;
ptr[24] ^= 0x97;
ptr[25] ^= 0xaf;
ptr[26] ^= 0xc1;

}

void multi_tracer_p179(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b3)
{
multi_tracer_p83(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe236a0 ^ regs->rbx);
ptr[0] ^= 0x4d;
ptr[1] += 0x65;
ptr[2] += 0xd0;
ptr[3] += 0x35;
ptr[4] += 0x83;
ptr[5] ^= 0x87;
ptr[6] ^= 0xd2;
ptr[7] += 0xf;
ptr[8] ^= 0x36;
ptr[9] += 0x1;
ptr[10] ^= 0x5a;
ptr[11] ^= 0x45;
ptr[12] += 0x71;
ptr[13] += 0xc1;
ptr[14] += 0x6b;
ptr[15] ^= 0x5e;
ptr[16] += 0xc1;
ptr[17] += 0xf2;
ptr[18] ^= 0x8e;
ptr[19] ^= 0xc7;
ptr[20] ^= 0xba;
ptr[21] += 0xbe;

}

void multi_tracer_p83(struct user_regs_struct *regs)
{
if (regs->rax != 0x2053)
{
multi_tracer_p204(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbdeede ^ regs->rbx);
ptr[0] += 0x14;
ptr[1] += 0x5c;
ptr[2] ^= 0x21;
ptr[3] += 0x6d;
ptr[4] ^= 0x8;
ptr[5] += 0x19;
ptr[6] += 0xf;
ptr[7] += 0x6e;
ptr[8] += 0xc2;
ptr[9] ^= 0xea;
ptr[10] += 0x28;
ptr[11] += 0x20;
ptr[12] ^= 0xe1;
ptr[13] ^= 0x5b;
ptr[14] ^= 0x21;
ptr[15] += 0x8f;
ptr[16] ^= 0xc6;
ptr[17] += 0xa;

}

void multi_tracer_p204(struct user_regs_struct *regs)
{
if (regs->rax != 0x20cc)
{
multi_tracer_p214(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x1a7a67 ^ regs->rbx);
ptr[0] += 0xa2;
ptr[1] += 0x3d;
ptr[2] += 0x2;
ptr[3] += 0xd1;
ptr[4] += 0x93;
ptr[5] ^= 0x73;
ptr[6] += 0xc7;
ptr[7] += 0x2a;
ptr[8] += 0x16;
ptr[9] += 0x2a;
ptr[10] += 0x3;
ptr[11] += 0x1d;
ptr[12] ^= 0x21;
ptr[13] += 0xd3;
ptr[14] += 0x88;
ptr[15] ^= 0x99;
ptr[16] += 0x58;
ptr[17] += 0x9f;
ptr[18] ^= 0x75;
ptr[19] ^= 0x9a;

}

void multi_tracer_p214(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d6)
{
multi_tracer_p7(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3069ef ^ regs->rbx);
ptr[0] ^= 0x76;
ptr[1] += 0x3d;
ptr[2] ^= 0xdd;
ptr[3] ^= 0xd5;
ptr[4] += 0x60;
ptr[5] ^= 0xec;
ptr[6] += 0x97;
ptr[7] += 0x16;
ptr[8] += 0x27;
ptr[9] += 0x43;
ptr[10] ^= 0x2b;
ptr[11] ^= 0x77;
ptr[12] ^= 0x7b;
ptr[13] ^= 0x26;
ptr[14] ^= 0xc0;
ptr[15] ^= 0x38;
ptr[16] ^= 0x28;
ptr[17] += 0xcb;
ptr[18] ^= 0xe8;
ptr[19] ^= 0x67;
ptr[20] ^= 0x4f;
ptr[21] ^= 0x7f;
ptr[22] ^= 0xc9;
ptr[23] ^= 0xe8;
ptr[24] += 0xe;

}

void multi_tracer_p7(struct user_regs_struct *regs)
{
if (regs->rax != 0x2007)
{
multi_tracer_p198(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9b145c ^ regs->rbx);
ptr[0] ^= 0xc3;
ptr[1] ^= 0xba;
ptr[2] ^= 0xd7;
ptr[3] += 0xbd;
ptr[4] ^= 0xbb;
ptr[5] += 0x1a;
ptr[6] += 0xed;
ptr[7] += 0x4c;
ptr[8] += 0xd1;
ptr[9] ^= 0xdb;
ptr[10] += 0x11;
ptr[11] += 0x40;
ptr[12] ^= 0xdc;
ptr[13] ^= 0xb9;
ptr[14] ^= 0x17;
ptr[15] += 0x91;
ptr[16] ^= 0x72;
ptr[17] += 0xa;
ptr[18] ^= 0x8d;
ptr[19] ^= 0x23;
ptr[20] ^= 0xd4;
ptr[21] ^= 0xe0;
ptr[22] ^= 0x13;
ptr[23] += 0x3c;
ptr[24] ^= 0xee;
ptr[25] += 0xf5;
ptr[26] ^= 0xd5;

}

void multi_tracer_p198(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c6)
{
multi_tracer_p112(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9aca75 ^ regs->rbx);
ptr[0] ^= 0xb5;
ptr[1] += 0x74;
ptr[2] ^= 0x5c;
ptr[3] ^= 0xf2;
ptr[4] += 0x6b;
ptr[5] += 0x85;
ptr[6] ^= 0xdc;
ptr[7] += 0xe3;
ptr[8] += 0xec;
ptr[9] ^= 0x69;
ptr[10] ^= 0xb2;
ptr[11] += 0xc0;
ptr[12] ^= 0xed;
ptr[13] += 0x9d;
ptr[14] += 0x8a;
ptr[15] += 0xee;
ptr[16] += 0x73;
ptr[17] += 0xe6;

}

void multi_tracer_p112(struct user_regs_struct *regs)
{
if (regs->rax != 0x2070)
{
multi_tracer_p135(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x955a0c ^ regs->rbx);
ptr[0] += 0x87;
ptr[1] ^= 0x4;
ptr[2] += 0x92;
ptr[3] ^= 0x2d;
ptr[4] ^= 0xe9;
ptr[5] += 0xc0;
ptr[6] += 0xab;
ptr[7] += 0xb7;
ptr[8] ^= 0x95;
ptr[9] ^= 0xef;
ptr[10] += 0xc2;
ptr[11] ^= 0xad;
ptr[12] += 0xab;
ptr[13] += 0xeb;
ptr[14] ^= 0x8a;
ptr[15] ^= 0x6;
ptr[16] += 0x21;
ptr[17] ^= 0xb4;
ptr[18] ^= 0xb4;
ptr[19] += 0xd2;
ptr[20] ^= 0x42;
ptr[21] += 0x41;
ptr[22] ^= 0x44;

}

void multi_tracer_p135(struct user_regs_struct *regs)
{
if (regs->rax != 0x2087)
{
multi_tracer_p32(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbc7ab1 ^ regs->rbx);
ptr[0] ^= 0xc1;
ptr[1] += 0xd;
ptr[2] += 0x2d;
ptr[3] += 0xcb;
ptr[4] += 0xec;
ptr[5] ^= 0x7c;
ptr[6] ^= 0x52;
ptr[7] += 0xb;
ptr[8] += 0xc0;
ptr[9] ^= 0x88;
ptr[10] += 0x82;
ptr[11] += 0x43;
ptr[12] += 0xe0;
ptr[13] ^= 0x34;
ptr[14] += 0x40;
ptr[15] ^= 0x29;
ptr[16] += 0x89;
ptr[17] += 0x8a;

}

void multi_tracer_p32(struct user_regs_struct *regs)
{
if (regs->rax != 0x2020)
{
multi_tracer_p133(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5cc39e ^ regs->rbx);
ptr[0] += 0x85;
ptr[1] ^= 0xea;
ptr[2] ^= 0xcc;
ptr[3] += 0x30;
ptr[4] += 0x15;
ptr[5] += 0x4a;
ptr[6] ^= 0x5a;
ptr[7] += 0x3e;
ptr[8] += 0xf4;
ptr[9] ^= 0xb1;
ptr[10] ^= 0x64;
ptr[11] += 0x91;
ptr[12] += 0x2b;
ptr[13] += 0x85;
ptr[14] += 0xb9;
ptr[15] ^= 0xcd;
ptr[16] ^= 0x6b;
ptr[17] += 0xec;
ptr[18] += 0xad;
ptr[19] += 0xbd;
ptr[20] += 0x30;
ptr[21] ^= 0xf6;
ptr[22] += 0xef;
ptr[23] += 0xcd;

}

void multi_tracer_p133(struct user_regs_struct *regs)
{
if (regs->rax != 0x2085)
{
multi_tracer_p173(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xac6899 ^ regs->rbx);
ptr[0] += 0xac;
ptr[1] ^= 0xd3;
ptr[2] ^= 0x7;
ptr[3] += 0xbc;
ptr[4] += 0x4a;
ptr[5] ^= 0xc5;
ptr[6] ^= 0x98;
ptr[7] ^= 0x6b;
ptr[8] += 0xe8;
ptr[9] ^= 0x37;
ptr[10] ^= 0x35;
ptr[11] ^= 0x55;
ptr[12] ^= 0x52;
ptr[13] += 0x2d;
ptr[14] += 0x32;
ptr[15] ^= 0xd8;
ptr[16] ^= 0x5f;
ptr[17] ^= 0xa8;
ptr[18] += 0xa8;
ptr[19] += 0xeb;
ptr[20] += 0xa4;
ptr[21] += 0x9b;
ptr[22] += 0x47;

}

void multi_tracer_p173(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ad)
{
multi_tracer_p131(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9d818d ^ regs->rbx);
ptr[0] ^= 0x62;
ptr[1] ^= 0x98;
ptr[2] ^= 0xba;
ptr[3] ^= 0xae;
ptr[4] ^= 0x38;
ptr[5] ^= 0xa6;
ptr[6] ^= 0x5b;
ptr[7] ^= 0x24;
ptr[8] ^= 0x5a;
ptr[9] ^= 0xa6;
ptr[10] += 0xf0;
ptr[11] ^= 0xa4;
ptr[12] ^= 0x73;
ptr[13] ^= 0xab;
ptr[14] ^= 0xaa;
ptr[15] ^= 0x7;
ptr[16] ^= 0xe7;
ptr[17] ^= 0x8f;
ptr[18] ^= 0xeb;
ptr[19] += 0x90;

}

void multi_tracer_p131(struct user_regs_struct *regs)
{
if (regs->rax != 0x2083)
{
multi_tracer_p130(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x542996 ^ regs->rbx);
ptr[0] ^= 0xa2;
ptr[1] += 0x16;
ptr[2] ^= 0xb7;
ptr[3] ^= 0xde;
ptr[4] += 0xa;
ptr[5] ^= 0x6c;
ptr[6] ^= 0xbe;
ptr[7] += 0xea;
ptr[8] += 0x57;
ptr[9] ^= 0xf5;
ptr[10] += 0x15;
ptr[11] += 0xf5;
ptr[12] += 0x4f;
ptr[13] ^= 0x41;
ptr[14] += 0xaf;
ptr[15] ^= 0x3a;
ptr[16] += 0xf1;
ptr[17] ^= 0x8f;
ptr[18] ^= 0x11;
ptr[19] ^= 0xfc;

}

void multi_tracer_p130(struct user_regs_struct *regs)
{
if (regs->rax != 0x2082)
{
multi_tracer_p21(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbd7357 ^ regs->rbx);
ptr[0] ^= 0x92;
ptr[1] += 0xea;
ptr[2] ^= 0x48;
ptr[3] += 0x44;
ptr[4] += 0x5f;
ptr[5] += 0x34;
ptr[6] += 0xe4;
ptr[7] ^= 0x2d;
ptr[8] += 0x86;
ptr[9] += 0xc4;
ptr[10] += 0xc5;
ptr[11] ^= 0xda;
ptr[12] ^= 0xf;
ptr[13] ^= 0xc6;
ptr[14] ^= 0xfd;
ptr[15] += 0xaa;
ptr[16] += 0xc7;
ptr[17] ^= 0xd0;
ptr[18] += 0x4a;
ptr[19] += 0xf3;
ptr[20] += 0xc1;
ptr[21] += 0xde;
ptr[22] ^= 0x6f;
ptr[23] += 0x2;
ptr[24] ^= 0xb6;
ptr[25] += 0xf7;

}

void multi_tracer_p21(struct user_regs_struct *regs)
{
if (regs->rax != 0x2015)
{
multi_tracer_p162(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf45686 ^ regs->rbx);
ptr[0] += 0x3;
ptr[1] += 0x1c;
ptr[2] ^= 0x15;
ptr[3] += 0xf1;
ptr[4] += 0x3d;
ptr[5] ^= 0x98;
ptr[6] += 0x35;
ptr[7] ^= 0x3b;
ptr[8] += 0x3;
ptr[9] += 0x59;
ptr[10] += 0x3a;
ptr[11] ^= 0x3d;
ptr[12] += 0x81;
ptr[13] += 0xe1;
ptr[14] += 0x47;
ptr[15] += 0x70;
ptr[16] += 0x4a;
ptr[17] += 0x2e;
ptr[18] += 0x90;
ptr[19] += 0x22;
ptr[20] ^= 0xd3;
ptr[21] ^= 0x52;
ptr[22] ^= 0x41;
ptr[23] += 0x7c;

}

void multi_tracer_p162(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a2)
{
multi_tracer_p65(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x68eb02 ^ regs->rbx);
ptr[0] ^= 0xe;
ptr[1] ^= 0x45;
ptr[2] ^= 0x85;
ptr[3] += 0xc9;
ptr[4] += 0x94;
ptr[5] += 0x1c;
ptr[6] ^= 0x9;
ptr[7] ^= 0x98;
ptr[8] ^= 0x26;
ptr[9] += 0xff;
ptr[10] += 0x83;
ptr[11] ^= 0x9c;
ptr[12] ^= 0xea;
ptr[13] ^= 0x6c;
ptr[14] += 0x79;
ptr[15] += 0x2f;
ptr[16] += 0x59;
ptr[17] ^= 0x3a;
ptr[18] ^= 0xeb;
ptr[19] ^= 0x7f;
ptr[20] ^= 0xe7;
ptr[21] ^= 0x85;
ptr[22] += 0xff;

}

void multi_tracer_p65(struct user_regs_struct *regs)
{
if (regs->rax != 0x2041)
{
multi_tracer_p250(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x988c6c ^ regs->rbx);
ptr[0] += 0x3d;
ptr[1] += 0xcb;
ptr[2] ^= 0x63;
ptr[3] += 0x17;
ptr[4] ^= 0x5f;
ptr[5] += 0x62;
ptr[6] += 0x59;
ptr[7] += 0x1b;
ptr[8] += 0x12;
ptr[9] += 0x1;
ptr[10] ^= 0x9c;
ptr[11] += 0xc9;
ptr[12] += 0x5e;
ptr[13] ^= 0x7a;
ptr[14] ^= 0xf;
ptr[15] ^= 0x78;
ptr[16] ^= 0x96;
ptr[17] ^= 0x2e;
ptr[18] ^= 0xa7;
ptr[19] ^= 0x2a;

}

void multi_tracer_p250(struct user_regs_struct *regs)
{
if (regs->rax != 0x20fa)
{
multi_tracer_p17(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe229bf ^ regs->rbx);
ptr[0] += 0x7e;
ptr[1] += 0x36;
ptr[2] ^= 0xdb;
ptr[3] += 0x1f;
ptr[4] ^= 0xd;
ptr[5] += 0x4d;
ptr[6] += 0xee;
ptr[7] += 0xcb;
ptr[8] ^= 0x46;
ptr[9] ^= 0x98;
ptr[10] += 0x1c;
ptr[11] += 0x79;
ptr[12] ^= 0x9d;
ptr[13] ^= 0x9d;
ptr[14] ^= 0x63;
ptr[15] += 0x9a;
ptr[16] += 0xa;
ptr[17] += 0xf5;
ptr[18] += 0x25;
ptr[19] ^= 0x64;
ptr[20] ^= 0x7e;
ptr[21] += 0x95;
ptr[22] += 0xda;
ptr[23] += 0x30;
ptr[24] ^= 0x29;
ptr[25] += 0x2d;
ptr[26] += 0x16;

}

void multi_tracer_p17(struct user_regs_struct *regs)
{
if (regs->rax != 0x2011)
{
multi_tracer_p127(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9aef92 ^ regs->rbx);
ptr[0] += 0xf;
ptr[1] += 0x22;
ptr[2] ^= 0x4d;
ptr[3] ^= 0x92;
ptr[4] ^= 0xbb;
ptr[5] += 0xdc;
ptr[6] ^= 0x71;
ptr[7] += 0x6b;
ptr[8] += 0xbb;
ptr[9] ^= 0x9e;
ptr[10] ^= 0x3a;
ptr[11] += 0xb0;
ptr[12] += 0x63;
ptr[13] ^= 0x92;
ptr[14] += 0x49;
ptr[15] += 0x52;
ptr[16] ^= 0x10;
ptr[17] += 0xcd;
ptr[18] ^= 0x65;
ptr[19] += 0x46;
ptr[20] ^= 0x16;

}

void multi_tracer_p127(struct user_regs_struct *regs)
{
if (regs->rax != 0x207f)
{
multi_tracer_p240(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x455d75 ^ regs->rbx);
ptr[0] ^= 0x4a;
ptr[1] += 0xa7;
ptr[2] ^= 0x61;
ptr[3] ^= 0x4e;
ptr[4] ^= 0x6e;
ptr[5] ^= 0xde;
ptr[6] += 0x59;
ptr[7] += 0x17;
ptr[8] += 0x84;
ptr[9] ^= 0x12;
ptr[10] ^= 0x4c;
ptr[11] ^= 0x93;
ptr[12] ^= 0x18;
ptr[13] += 0x20;
ptr[14] ^= 0x9c;
ptr[15] += 0xdb;
ptr[16] ^= 0x8f;
ptr[17] ^= 0xb7;
ptr[18] ^= 0x6f;
ptr[19] ^= 0x61;
ptr[20] ^= 0x26;
ptr[21] ^= 0x8c;
ptr[22] += 0x7d;
ptr[23] += 0xea;
ptr[24] += 0xfa;

}

void multi_tracer_p240(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f0)
{
multi_tracer_p236(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x737aee ^ regs->rbx);
ptr[0] ^= 0x28;
ptr[1] += 0xfc;
ptr[2] ^= 0xfb;
ptr[3] ^= 0xf5;
ptr[4] += 0x3a;
ptr[5] ^= 0xe1;
ptr[6] += 0x1d;
ptr[7] ^= 0xfe;
ptr[8] ^= 0xf7;
ptr[9] ^= 0x4;
ptr[10] ^= 0x37;
ptr[11] ^= 0x91;
ptr[12] += 0xb3;
ptr[13] += 0xce;
ptr[14] ^= 0xb0;
ptr[15] += 0xc7;
ptr[16] += 0xc8;
ptr[17] += 0x84;
ptr[18] ^= 0x48;
ptr[19] ^= 0x9e;
ptr[20] += 0x47;
ptr[21] += 0x36;
ptr[22] ^= 0xc6;
ptr[23] += 0x37;

}

void multi_tracer_p236(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ec)
{
multi_tracer_p120(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2a5198 ^ regs->rbx);
ptr[0] += 0x4f;
ptr[1] += 0x43;
ptr[2] ^= 0xa2;
ptr[3] += 0x46;
ptr[4] ^= 0x66;
ptr[5] += 0xfe;
ptr[6] ^= 0xdc;
ptr[7] ^= 0xf8;
ptr[8] ^= 0x78;
ptr[9] ^= 0x6b;
ptr[10] += 0xf6;
ptr[11] += 0xc4;
ptr[12] ^= 0xad;
ptr[13] ^= 0x60;
ptr[14] += 0xf9;
ptr[15] += 0x3e;
ptr[16] ^= 0xf0;
ptr[17] ^= 0x1c;
ptr[18] ^= 0x90;
ptr[19] ^= 0x36;
ptr[20] += 0xc;
ptr[21] ^= 0xd6;
ptr[22] ^= 0xb4;
ptr[23] ^= 0x98;
ptr[24] += 0x18;

}

void multi_tracer_p120(struct user_regs_struct *regs)
{
if (regs->rax != 0x2078)
{
multi_tracer_p109(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x7cfcde ^ regs->rbx);
ptr[0] += 0x62;
ptr[1] ^= 0x96;
ptr[2] ^= 0xec;
ptr[3] += 0x41;
ptr[4] ^= 0xe8;
ptr[5] += 0x4e;
ptr[6] += 0x6e;
ptr[7] += 0xd8;
ptr[8] += 0x15;
ptr[9] ^= 0xf1;
ptr[10] += 0x9b;
ptr[11] += 0xd8;
ptr[12] += 0x86;
ptr[13] += 0x2f;
ptr[14] ^= 0x44;
ptr[15] += 0x7d;
ptr[16] += 0x8;
ptr[17] += 0x3a;
ptr[18] ^= 0x5d;
ptr[19] += 0x87;
ptr[20] += 0x37;
ptr[21] += 0x86;
ptr[22] ^= 0xc2;
ptr[23] += 0xeb;
ptr[24] += 0xaa;
ptr[25] += 0xf3;
ptr[26] ^= 0x28;

}

void multi_tracer_p109(struct user_regs_struct *regs)
{
if (regs->rax != 0x206d)
{
multi_tracer_p62(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xe75ec3 ^ regs->rbx);
ptr[0] ^= 0x46;
ptr[1] ^= 0x1;
ptr[2] += 0x8d;
ptr[3] ^= 0x20;
ptr[4] += 0x7c;
ptr[5] += 0x93;
ptr[6] ^= 0x2f;
ptr[7] += 0xe6;
ptr[8] ^= 0xaa;
ptr[9] ^= 0xfd;
ptr[10] += 0xd7;
ptr[11] ^= 0x41;
ptr[12] ^= 0x4f;
ptr[13] ^= 0xd7;
ptr[14] += 0x11;
ptr[15] ^= 0x55;
ptr[16] ^= 0xe9;
ptr[17] += 0xec;
ptr[18] ^= 0x35;
ptr[19] += 0xf5;
ptr[20] ^= 0x5;
ptr[21] += 0xcd;

}

void multi_tracer_p62(struct user_regs_struct *regs)
{
if (regs->rax != 0x203e)
{
multi_tracer_p24(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8ce027 ^ regs->rbx);
ptr[0] += 0xd2;
ptr[1] ^= 0x22;
ptr[2] ^= 0x68;
ptr[3] += 0x6d;
ptr[4] ^= 0x6c;
ptr[5] ^= 0xaf;
ptr[6] += 0xd7;
ptr[7] += 0xa3;
ptr[8] += 0xe7;
ptr[9] += 0x21;
ptr[10] += 0x35;
ptr[11] += 0x48;
ptr[12] ^= 0xbf;
ptr[13] ^= 0x6a;
ptr[14] ^= 0x19;
ptr[15] ^= 0xf4;
ptr[16] ^= 0xa8;
ptr[17] ^= 0xb1;
ptr[18] += 0xc8;
ptr[19] += 0x2f;
ptr[20] += 0xea;
ptr[21] += 0x7a;
ptr[22] ^= 0x59;
ptr[23] ^= 0x40;
ptr[24] += 0x4b;
ptr[25] += 0xef;

}

void multi_tracer_p24(struct user_regs_struct *regs)
{
if (regs->rax != 0x2018)
{
multi_tracer_p119(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb51ca9 ^ regs->rbx);
ptr[0] += 0x4d;
ptr[1] += 0x48;
ptr[2] += 0x88;
ptr[3] += 0xfc;
ptr[4] += 0xd8;
ptr[5] ^= 0x23;
ptr[6] += 0xd8;
ptr[7] += 0xeb;
ptr[8] += 0xb9;
ptr[9] ^= 0xb4;
ptr[10] += 0xfc;
ptr[11] ^= 0x8;
ptr[12] += 0xd3;
ptr[13] ^= 0x2c;
ptr[14] ^= 0x4e;
ptr[15] += 0xed;
ptr[16] += 0x87;
ptr[17] += 0xda;
ptr[18] += 0x6f;
ptr[19] ^= 0xdc;
ptr[20] += 0xa9;
ptr[21] ^= 0x13;
ptr[22] += 0x13;
ptr[23] ^= 0x1f;
ptr[24] ^= 0x68;
ptr[25] ^= 0x59;

}

void multi_tracer_p119(struct user_regs_struct *regs)
{
if (regs->rax != 0x2077)
{
multi_tracer_p143(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xfe1fe4 ^ regs->rbx);
ptr[0] += 0xcc;
ptr[1] ^= 0xfa;
ptr[2] ^= 0x83;
ptr[3] += 0x5f;
ptr[4] += 0xf3;
ptr[5] += 0x7c;
ptr[6] ^= 0x79;
ptr[7] ^= 0xf2;
ptr[8] += 0xda;
ptr[9] ^= 0x6d;
ptr[10] += 0x1c;
ptr[11] ^= 0x36;
ptr[12] ^= 0x68;
ptr[13] ^= 0xce;
ptr[14] ^= 0xfc;
ptr[15] ^= 0x78;
ptr[16] ^= 0xdb;
ptr[17] += 0x33;
ptr[18] += 0xf9;
ptr[19] += 0x3e;

}

void multi_tracer_p143(struct user_regs_struct *regs)
{
if (regs->rax != 0x208f)
{
multi_tracer_p150(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x34ab5a ^ regs->rbx);
ptr[0] ^= 0x9e;
ptr[1] += 0x75;
ptr[2] ^= 0x26;
ptr[3] += 0x15;
ptr[4] += 0xbc;
ptr[5] += 0x56;
ptr[6] += 0x4e;
ptr[7] ^= 0x10;
ptr[8] += 0x84;
ptr[9] ^= 0xf5;
ptr[10] += 0x5c;
ptr[11] += 0xdd;
ptr[12] ^= 0xff;
ptr[13] ^= 0x3;
ptr[14] += 0x69;
ptr[15] += 0x8c;
ptr[16] ^= 0xc1;
ptr[17] ^= 0x55;
ptr[18] ^= 0x4f;
ptr[19] += 0x4c;
ptr[20] += 0xa4;

}

void multi_tracer_p150(struct user_regs_struct *regs)
{
if (regs->rax != 0x2096)
{
multi_tracer_p88(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb683be ^ regs->rbx);
ptr[0] += 0x63;
ptr[1] += 0x40;
ptr[2] ^= 0xfe;
ptr[3] += 0x97;
ptr[4] += 0x77;
ptr[5] ^= 0x14;
ptr[6] += 0x4d;
ptr[7] ^= 0xaa;
ptr[8] ^= 0xf5;
ptr[9] += 0xf3;
ptr[10] += 0x3;
ptr[11] += 0xee;
ptr[12] ^= 0xbc;
ptr[13] ^= 0xa;
ptr[14] += 0x4;
ptr[15] += 0x6d;
ptr[16] += 0xd6;
ptr[17] ^= 0xfd;
ptr[18] ^= 0x19;
ptr[19] += 0xcf;
ptr[20] += 0x8e;
ptr[21] ^= 0x1c;
ptr[22] ^= 0x4f;
ptr[23] += 0xaf;

}

void multi_tracer_p88(struct user_regs_struct *regs)
{
if (regs->rax != 0x2058)
{
multi_tracer_p144(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3ed287 ^ regs->rbx);
ptr[0] += 0xe8;
ptr[1] += 0x50;
ptr[2] ^= 0x43;
ptr[3] ^= 0xff;
ptr[4] ^= 0xa8;
ptr[5] += 0xb7;
ptr[6] += 0x79;
ptr[7] ^= 0x48;
ptr[8] += 0xcf;
ptr[9] ^= 0xa3;
ptr[10] += 0x66;
ptr[11] ^= 0x34;
ptr[12] ^= 0x3d;
ptr[13] += 0x48;
ptr[14] ^= 0x9;
ptr[15] += 0x17;
ptr[16] ^= 0x76;
ptr[17] ^= 0x5;
ptr[18] ^= 0x3a;
ptr[19] += 0xf0;
ptr[20] ^= 0xd;

}

void multi_tracer_p144(struct user_regs_struct *regs)
{
if (regs->rax != 0x2090)
{
multi_tracer_p124(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xef22db ^ regs->rbx);
ptr[0] ^= 0x96;
ptr[1] += 0x7a;
ptr[2] ^= 0x1c;
ptr[3] += 0x8d;
ptr[4] ^= 0x78;
ptr[5] += 0x4e;
ptr[6] += 0xb9;
ptr[7] ^= 0x2d;
ptr[8] ^= 0x81;
ptr[9] ^= 0xcd;
ptr[10] += 0xdc;
ptr[11] += 0x84;
ptr[12] += 0x30;
ptr[13] ^= 0x6e;
ptr[14] ^= 0x2a;
ptr[15] += 0xe6;
ptr[16] += 0x9b;
ptr[17] += 0x49;
ptr[18] += 0x1c;
ptr[19] += 0x7f;
ptr[20] += 0xbd;
ptr[21] += 0x5a;
ptr[22] += 0x20;
ptr[23] += 0x64;

}

void multi_tracer_p124(struct user_regs_struct *regs)
{
if (regs->rax != 0x207c)
{
multi_tracer_p51(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc263e4 ^ regs->rbx);
ptr[0] ^= 0xe4;
ptr[1] += 0x12;
ptr[2] ^= 0x27;
ptr[3] += 0xce;
ptr[4] += 0xff;
ptr[5] += 0xdf;
ptr[6] ^= 0x3a;
ptr[7] ^= 0x9b;
ptr[8] ^= 0x23;
ptr[9] += 0x44;
ptr[10] += 0xff;
ptr[11] += 0x6d;
ptr[12] ^= 0xb1;
ptr[13] ^= 0x52;
ptr[14] ^= 0x45;
ptr[15] ^= 0xf9;
ptr[16] ^= 0xd4;
ptr[17] += 0xbb;
ptr[18] += 0xe1;

}

void multi_tracer_p51(struct user_regs_struct *regs)
{
if (regs->rax != 0x2033)
{
multi_tracer_p87(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x1e17aa ^ regs->rbx);
ptr[0] += 0xf4;
ptr[1] += 0xc5;
ptr[2] ^= 0x14;
ptr[3] += 0x89;
ptr[4] ^= 0xc;
ptr[5] += 0x67;
ptr[6] ^= 0x18;
ptr[7] += 0x77;
ptr[8] += 0x6d;
ptr[9] += 0x3a;
ptr[10] += 0x31;
ptr[11] ^= 0x57;
ptr[12] += 0x15;
ptr[13] ^= 0xa5;
ptr[14] ^= 0xeb;
ptr[15] += 0xbe;
ptr[16] += 0x21;
ptr[17] ^= 0xea;
ptr[18] += 0xc;

}

void multi_tracer_p87(struct user_regs_struct *regs)
{
if (regs->rax != 0x2057)
{
multi_tracer_p99(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x32ba6d ^ regs->rbx);
ptr[0] += 0xa4;
ptr[1] ^= 0x3a;
ptr[2] ^= 0x5a;
ptr[3] ^= 0x9c;
ptr[4] ^= 0xb4;
ptr[5] ^= 0x25;
ptr[6] += 0x2c;
ptr[7] += 0x13;
ptr[8] += 0x6;
ptr[9] ^= 0x81;
ptr[10] += 0xd5;
ptr[11] += 0x3e;
ptr[12] += 0x98;
ptr[13] += 0xb3;
ptr[14] ^= 0xe;
ptr[15] += 0xb0;
ptr[16] ^= 0xb6;
ptr[17] += 0xee;
ptr[18] += 0xb9;
ptr[19] ^= 0x78;
ptr[20] += 0xb3;
ptr[21] += 0x8e;
ptr[22] += 0xf4;
ptr[23] ^= 0x22;
ptr[24] += 0x79;
ptr[25] += 0xbe;
ptr[26] += 0xdd;
ptr[27] += 0x7e;

}

void multi_tracer_p99(struct user_regs_struct *regs)
{
if (regs->rax != 0x2063)
{
multi_tracer_p158(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x49b50c ^ regs->rbx);
ptr[0] ^= 0x95;
ptr[1] += 0xa3;
ptr[2] += 0x58;
ptr[3] ^= 0x83;
ptr[4] += 0x9c;
ptr[5] += 0xb;
ptr[6] ^= 0xcd;
ptr[7] ^= 0xeb;
ptr[8] ^= 0x13;
ptr[9] += 0x16;
ptr[10] ^= 0x81;
ptr[11] += 0xde;
ptr[12] += 0xfd;
ptr[13] ^= 0x25;
ptr[14] ^= 0x3c;
ptr[15] += 0x6;
ptr[16] ^= 0x4;
ptr[17] ^= 0xa4;
ptr[18] ^= 0xea;
ptr[19] ^= 0xc0;
ptr[20] += 0xc6;
ptr[21] ^= 0x3e;
ptr[22] += 0x14;
ptr[23] ^= 0x41;
ptr[24] += 0x44;
ptr[25] ^= 0xc3;

}

void multi_tracer_p158(struct user_regs_struct *regs)
{
if (regs->rax != 0x209e)
{
multi_tracer_p3(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xfe32cc ^ regs->rbx);
ptr[0] ^= 0xb;
ptr[1] ^= 0x34;
ptr[2] += 0xc3;
ptr[3] += 0x38;
ptr[4] ^= 0x55;
ptr[5] += 0xe7;
ptr[6] += 0xd6;
ptr[7] += 0xb8;
ptr[8] ^= 0x5b;
ptr[9] += 0x6e;
ptr[10] += 0x5d;
ptr[11] ^= 0x46;
ptr[12] += 0xcc;
ptr[13] += 0xf8;
ptr[14] ^= 0xba;
ptr[15] += 0xc;
ptr[16] += 0xc6;
ptr[17] ^= 0x3b;
ptr[18] ^= 0x61;
ptr[19] += 0xe6;
ptr[20] ^= 0x7e;
ptr[21] += 0x9c;
ptr[22] += 0x88;
ptr[23] ^= 0x79;
ptr[24] ^= 0x4d;
ptr[25] ^= 0x6d;

}

void multi_tracer_p3(struct user_regs_struct *regs)
{
if (regs->rax != 0x2003)
{
multi_tracer_p92(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x92a234 ^ regs->rbx);
ptr[0] ^= 0x12;
ptr[1] ^= 0xf2;
ptr[2] += 0x91;
ptr[3] ^= 0x4f;
ptr[4] += 0xc;
ptr[5] ^= 0x45;
ptr[6] += 0x5f;
ptr[7] += 0x9f;
ptr[8] += 0x33;
ptr[9] += 0x79;
ptr[10] += 0x3c;
ptr[11] += 0x23;
ptr[12] += 0x8f;
ptr[13] ^= 0xeb;
ptr[14] ^= 0xca;
ptr[15] ^= 0x2a;
ptr[16] ^= 0x9d;
ptr[17] ^= 0x1e;
ptr[18] += 0x82;
ptr[19] += 0xcc;
ptr[20] += 0x46;
ptr[21] += 0x1c;
ptr[22] ^= 0x61;
ptr[23] ^= 0x51;

}

void multi_tracer_p92(struct user_regs_struct *regs)
{
if (regs->rax != 0x205c)
{
multi_tracer_p74(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x562b3d ^ regs->rbx);
ptr[0] ^= 0x36;
ptr[1] += 0x6f;
ptr[2] += 0x99;
ptr[3] += 0x53;
ptr[4] ^= 0x1c;
ptr[5] ^= 0x75;
ptr[6] += 0x9a;
ptr[7] += 0x75;
ptr[8] += 0x78;
ptr[9] += 0x21;
ptr[10] ^= 0x35;
ptr[11] += 0xc7;
ptr[12] ^= 0x3e;
ptr[13] += 0x2;
ptr[14] ^= 0xa7;
ptr[15] += 0x8b;
ptr[16] ^= 0x1b;
ptr[17] ^= 0x8;
ptr[18] ^= 0xd6;
ptr[19] += 0xb5;
ptr[20] += 0xc9;
ptr[21] ^= 0x42;
ptr[22] ^= 0xe1;
ptr[23] += 0x5;

}

void multi_tracer_p74(struct user_regs_struct *regs)
{
if (regs->rax != 0x204a)
{
multi_tracer_p139(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x5979c8 ^ regs->rbx);
ptr[0] += 0xcb;
ptr[1] += 0x25;
ptr[2] ^= 0xd5;
ptr[3] += 0x31;
ptr[4] ^= 0xac;
ptr[5] += 0xb6;
ptr[6] ^= 0x82;
ptr[7] += 0xaf;
ptr[8] ^= 0x7e;
ptr[9] += 0xfe;
ptr[10] += 0x69;
ptr[11] ^= 0xa1;
ptr[12] += 0x52;
ptr[13] ^= 0x42;
ptr[14] += 0x42;
ptr[15] ^= 0xac;
ptr[16] += 0xd3;
ptr[17] += 0xf;
ptr[18] ^= 0xe0;
ptr[19] += 0x1c;

}

void multi_tracer_p139(struct user_regs_struct *regs)
{
if (regs->rax != 0x208b)
{
multi_tracer_p165(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf93a33 ^ regs->rbx);
ptr[0] += 0x82;
ptr[1] += 0x99;
ptr[2] += 0x90;
ptr[3] += 0xf4;
ptr[4] += 0x1b;
ptr[5] ^= 0x33;
ptr[6] += 0xb5;
ptr[7] ^= 0xca;
ptr[8] += 0x7e;
ptr[9] += 0x23;
ptr[10] += 0x8a;
ptr[11] += 0x8f;
ptr[12] += 0xad;
ptr[13] ^= 0x85;
ptr[14] ^= 0x68;
ptr[15] += 0xdc;
ptr[16] ^= 0x7;
ptr[17] ^= 0x75;

}

void multi_tracer_p165(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a5)
{
multi_tracer_p79(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x17252e ^ regs->rbx);
ptr[0] ^= 0xee;
ptr[1] += 0x5d;
ptr[2] += 0x6f;
ptr[3] ^= 0x88;
ptr[4] += 0x1a;
ptr[5] += 0xe3;
ptr[6] ^= 0x7;
ptr[7] ^= 0xf0;
ptr[8] += 0xf9;
ptr[9] += 0xad;
ptr[10] ^= 0x6d;
ptr[11] += 0x5;
ptr[12] += 0x21;
ptr[13] ^= 0x38;
ptr[14] ^= 0xd3;
ptr[15] ^= 0xaf;
ptr[16] ^= 0xca;
ptr[17] ^= 0xa;
ptr[18] ^= 0xf2;
ptr[19] ^= 0xba;
ptr[20] += 0x6b;
ptr[21] ^= 0x4;
ptr[22] ^= 0x4d;

}

void multi_tracer_p79(struct user_regs_struct *regs)
{
if (regs->rax != 0x204f)
{
multi_tracer_p174(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xedc094 ^ regs->rbx);
ptr[0] += 0xbd;
ptr[1] += 0x51;
ptr[2] ^= 0x61;
ptr[3] ^= 0x30;
ptr[4] += 0xa2;
ptr[5] ^= 0x42;
ptr[6] ^= 0x9d;
ptr[7] ^= 0x7f;
ptr[8] ^= 0xc0;
ptr[9] += 0xac;
ptr[10] += 0x80;
ptr[11] += 0xdf;
ptr[12] += 0x63;
ptr[13] ^= 0x5e;
ptr[14] += 0x40;
ptr[15] ^= 0xbd;
ptr[16] += 0x5d;
ptr[17] ^= 0xec;
ptr[18] += 0x74;
ptr[19] += 0x80;

}

void multi_tracer_p174(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ae)
{
multi_tracer_p49(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x83d4a3 ^ regs->rbx);
ptr[0] ^= 0x35;
ptr[1] ^= 0x9b;
ptr[2] += 0x4;
ptr[3] ^= 0xd3;
ptr[4] += 0x18;
ptr[5] ^= 0x83;
ptr[6] ^= 0x53;
ptr[7] += 0x61;
ptr[8] ^= 0x69;
ptr[9] += 0x91;
ptr[10] += 0x90;
ptr[11] += 0x16;
ptr[12] += 0x20;
ptr[13] ^= 0xb1;
ptr[14] += 0xc4;
ptr[15] += 0xbd;
ptr[16] += 0xed;
ptr[17] += 0x58;

}

void multi_tracer_p49(struct user_regs_struct *regs)
{
if (regs->rax != 0x2031)
{
multi_tracer_p160(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x651b26 ^ regs->rbx);
ptr[0] ^= 0xfd;
ptr[1] += 0x6a;
ptr[2] ^= 0x5c;
ptr[3] += 0xda;
ptr[4] += 0x5c;
ptr[5] += 0x5b;
ptr[6] += 0x2d;
ptr[7] ^= 0xb1;
ptr[8] += 0x87;
ptr[9] += 0xa6;
ptr[10] ^= 0x8e;
ptr[11] += 0x65;
ptr[12] += 0x99;
ptr[13] += 0x60;
ptr[14] += 0xca;
ptr[15] += 0xd6;
ptr[16] += 0xb1;
ptr[17] += 0xd4;
ptr[18] ^= 0xf5;
ptr[19] ^= 0xd0;

}

void multi_tracer_p160(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a0)
{
multi_tracer_p227(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x42b9dc ^ regs->rbx);
ptr[0] ^= 0xa5;
ptr[1] += 0xf5;
ptr[2] ^= 0x1f;
ptr[3] += 0xbc;
ptr[4] += 0xca;
ptr[5] ^= 0x38;
ptr[6] ^= 0x40;
ptr[7] ^= 0x8f;
ptr[8] += 0x4f;
ptr[9] ^= 0xe;
ptr[10] ^= 0x57;
ptr[11] ^= 0xa6;
ptr[12] ^= 0x3f;
ptr[13] ^= 0x51;
ptr[14] ^= 0x8f;
ptr[15] ^= 0x44;
ptr[16] += 0x1d;
ptr[17] += 0x89;
ptr[18] += 0x99;
ptr[19] += 0xae;
ptr[20] ^= 0x3e;
ptr[21] += 0xe2;
ptr[22] += 0xaf;
ptr[23] ^= 0xa9;

}

void multi_tracer_p227(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e3)
{
multi_tracer_p95(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x609eb4 ^ regs->rbx);
ptr[0] += 0x72;
ptr[1] += 0xca;
ptr[2] += 0x6c;
ptr[3] += 0x7e;
ptr[4] += 0x46;
ptr[5] += 0x9;
ptr[6] += 0x49;
ptr[7] ^= 0x4a;
ptr[8] ^= 0x4;
ptr[9] += 0x2e;
ptr[10] += 0xe0;
ptr[11] += 0x4c;
ptr[12] ^= 0x9;
ptr[13] ^= 0x7c;
ptr[14] += 0x36;
ptr[15] ^= 0xb1;
ptr[16] += 0x3d;
ptr[17] += 0xb4;
ptr[18] ^= 0xfd;
ptr[19] ^= 0xa0;
ptr[20] += 0xb5;

}

void multi_tracer_p95(struct user_regs_struct *regs)
{
if (regs->rax != 0x205f)
{
multi_tracer_p116(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xec70a2 ^ regs->rbx);
ptr[0] += 0x20;
ptr[1] ^= 0x1a;
ptr[2] += 0xbf;
ptr[3] += 0x3e;
ptr[4] ^= 0x1c;
ptr[5] ^= 0x7;
ptr[6] ^= 0x9b;
ptr[7] += 0x7c;
ptr[8] ^= 0x30;
ptr[9] ^= 0x10;
ptr[10] ^= 0x88;
ptr[11] += 0x17;
ptr[12] += 0x91;
ptr[13] += 0xf5;
ptr[14] += 0x69;
ptr[15] ^= 0x17;
ptr[16] += 0x77;
ptr[17] ^= 0x53;
ptr[18] += 0xd3;
ptr[19] += 0xd9;
ptr[20] += 0x7c;
ptr[21] ^= 0xd9;
ptr[22] ^= 0x50;
ptr[23] ^= 0xad;

}

void multi_tracer_p116(struct user_regs_struct *regs)
{
if (regs->rax != 0x2074)
{
multi_tracer_p177(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8938d6 ^ regs->rbx);
ptr[0] ^= 0x97;
ptr[1] ^= 0xe1;
ptr[2] ^= 0x46;
ptr[3] ^= 0xab;
ptr[4] ^= 0xa0;
ptr[5] ^= 0x61;
ptr[6] ^= 0x70;
ptr[7] += 0x85;
ptr[8] ^= 0x1c;
ptr[9] += 0xf5;
ptr[10] ^= 0x8b;
ptr[11] += 0xb9;
ptr[12] ^= 0xb2;
ptr[13] ^= 0xc4;
ptr[14] += 0x42;
ptr[15] ^= 0x2b;
ptr[16] += 0xe7;
ptr[17] ^= 0x6a;
ptr[18] ^= 0xa7;
ptr[19] += 0xa9;
ptr[20] ^= 0x2a;
ptr[21] += 0x1;
ptr[22] ^= 0x3e;

}

void multi_tracer_p177(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b1)
{
multi_tracer_p202(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x299024 ^ regs->rbx);
ptr[0] ^= 0x26;
ptr[1] += 0x28;
ptr[2] += 0x4b;
ptr[3] ^= 0x9b;
ptr[4] ^= 0x35;
ptr[5] ^= 0xad;
ptr[6] += 0x45;
ptr[7] += 0x6e;
ptr[8] ^= 0x33;
ptr[9] ^= 0x1;
ptr[10] += 0xd0;
ptr[11] ^= 0xf1;
ptr[12] ^= 0x91;
ptr[13] += 0xc2;
ptr[14] += 0x6f;
ptr[15] ^= 0xc8;
ptr[16] ^= 0xa9;
ptr[17] ^= 0xf7;
ptr[18] ^= 0xa0;
ptr[19] += 0x41;
ptr[20] += 0x2a;

}

void multi_tracer_p202(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ca)
{
multi_tracer_p11(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc09226 ^ regs->rbx);
ptr[0] += 0x4c;
ptr[1] += 0x23;
ptr[2] ^= 0x89;
ptr[3] ^= 0x4d;
ptr[4] ^= 0x2d;
ptr[5] ^= 0x34;
ptr[6] ^= 0x80;
ptr[7] += 0x86;
ptr[8] += 0x6e;
ptr[9] ^= 0x65;
ptr[10] += 0xd4;
ptr[11] ^= 0xf0;
ptr[12] ^= 0x70;
ptr[13] ^= 0xb4;
ptr[14] += 0xd7;
ptr[15] += 0xc;
ptr[16] ^= 0x9c;
ptr[17] += 0x67;
ptr[18] ^= 0xcd;
ptr[19] += 0x94;
ptr[20] ^= 0x49;
ptr[21] ^= 0x53;
ptr[22] += 0x7f;
ptr[23] ^= 0x9a;
ptr[24] ^= 0x26;
ptr[25] += 0x5;
ptr[26] += 0x1b;

}

void multi_tracer_p11(struct user_regs_struct *regs)
{
if (regs->rax != 0x200b)
{
multi_tracer_p13(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x6d149f ^ regs->rbx);
ptr[0] ^= 0x6b;
ptr[1] ^= 0xa;
ptr[2] ^= 0x19;
ptr[3] ^= 0x3a;
ptr[4] += 0xc;
ptr[5] ^= 0xa2;
ptr[6] += 0xb0;
ptr[7] ^= 0x5a;
ptr[8] += 0x5e;
ptr[9] ^= 0x21;
ptr[10] += 0xae;
ptr[11] ^= 0x28;
ptr[12] ^= 0x1d;
ptr[13] += 0x94;
ptr[14] += 0xc3;
ptr[15] += 0xb8;
ptr[16] ^= 0x26;
ptr[17] ^= 0xb4;

}

void multi_tracer_p13(struct user_regs_struct *regs)
{
if (regs->rax != 0x200d)
{
multi_tracer_p67(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xcd3967 ^ regs->rbx);
ptr[0] += 0x22;
ptr[1] += 0xd4;
ptr[2] += 0xd8;
ptr[3] ^= 0x2c;
ptr[4] ^= 0xd;
ptr[5] += 0x94;
ptr[6] += 0x28;
ptr[7] ^= 0x87;
ptr[8] ^= 0x7c;
ptr[9] += 0x7b;
ptr[10] += 0x99;
ptr[11] ^= 0x96;
ptr[12] ^= 0x50;
ptr[13] += 0x7f;
ptr[14] += 0xe8;
ptr[15] ^= 0xf4;
ptr[16] ^= 0x4e;
ptr[17] += 0x8a;
ptr[18] ^= 0x56;
ptr[19] += 0x71;
ptr[20] += 0x61;
ptr[21] += 0x5d;
ptr[22] ^= 0x14;

}

void multi_tracer_p67(struct user_regs_struct *regs)
{
if (regs->rax != 0x2043)
{
multi_tracer_p151(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xbfb3e8 ^ regs->rbx);
ptr[0] += 0x63;
ptr[1] += 0xdd;
ptr[2] += 0xdf;
ptr[3] += 0xbe;
ptr[4] += 0x69;
ptr[5] += 0xe4;
ptr[6] ^= 0xa7;
ptr[7] ^= 0x2f;
ptr[8] += 0xe3;
ptr[9] += 0xdc;
ptr[10] ^= 0xaa;
ptr[11] ^= 0xc;
ptr[12] ^= 0x54;
ptr[13] ^= 0x4;
ptr[14] += 0xfa;
ptr[15] += 0x7;
ptr[16] ^= 0x5c;
ptr[17] += 0xbb;

}

void multi_tracer_p151(struct user_regs_struct *regs)
{
if (regs->rax != 0x2097)
{
multi_tracer_p101(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2b4513 ^ regs->rbx);
ptr[0] ^= 0xf5;
ptr[1] += 0xa4;
ptr[2] += 0xb6;
ptr[3] += 0xc2;
ptr[4] += 0x8c;
ptr[5] += 0x9e;
ptr[6] += 0x3;
ptr[7] += 0x4c;
ptr[8] += 0xc2;
ptr[9] ^= 0x29;
ptr[10] ^= 0xe5;
ptr[11] ^= 0x58;
ptr[12] += 0x2e;
ptr[13] ^= 0x69;
ptr[14] ^= 0xaf;
ptr[15] ^= 0xbc;
ptr[16] += 0x71;
ptr[17] += 0x83;
ptr[18] += 0xcb;
ptr[19] ^= 0x6c;
ptr[20] += 0xae;
ptr[21] += 0xad;
ptr[22] ^= 0xbc;
ptr[23] += 0x30;
ptr[24] ^= 0xda;
ptr[25] += 0x3;
ptr[26] += 0x5d;

}

void multi_tracer_p101(struct user_regs_struct *regs)
{
if (regs->rax != 0x2065)
{
multi_tracer_p89(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xfd0b1b ^ regs->rbx);
ptr[0] ^= 0xae;
ptr[1] += 0xc;
ptr[2] ^= 0xd5;
ptr[3] += 0x2c;
ptr[4] ^= 0x51;
ptr[5] += 0x3a;
ptr[6] ^= 0x29;
ptr[7] += 0xc3;
ptr[8] += 0x7d;
ptr[9] ^= 0x77;
ptr[10] += 0x4;
ptr[11] ^= 0x5c;
ptr[12] ^= 0x95;
ptr[13] ^= 0x44;
ptr[14] ^= 0x14;
ptr[15] ^= 0xba;
ptr[16] ^= 0xd1;
ptr[17] += 0xbf;
ptr[18] += 0x2c;
ptr[19] ^= 0xc;
ptr[20] ^= 0x3;
ptr[21] ^= 0x8d;
ptr[22] ^= 0xf0;

}

void multi_tracer_p89(struct user_regs_struct *regs)
{
if (regs->rax != 0x2059)
{
multi_tracer_p138(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x470587 ^ regs->rbx);
ptr[0] ^= 0xbc;
ptr[1] += 0x2a;
ptr[2] ^= 0x82;
ptr[3] += 0xcd;
ptr[4] += 0xe0;
ptr[5] ^= 0x16;
ptr[6] ^= 0x21;
ptr[7] ^= 0xd9;
ptr[8] += 0x1a;
ptr[9] += 0x2c;
ptr[10] += 0x6d;
ptr[11] += 0x75;
ptr[12] += 0xde;
ptr[13] += 0x8b;
ptr[14] ^= 0x17;
ptr[15] ^= 0x5c;
ptr[16] += 0xdf;
ptr[17] += 0x53;
ptr[18] ^= 0x3a;
ptr[19] += 0x66;
ptr[20] ^= 0x57;
ptr[21] ^= 0x8c;

}

void multi_tracer_p138(struct user_regs_struct *regs)
{
if (regs->rax != 0x208a)
{
multi_tracer_p197(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x224b99 ^ regs->rbx);
ptr[0] ^= 0x46;
ptr[1] ^= 0x8b;
ptr[2] ^= 0x96;
ptr[3] ^= 0xa2;
ptr[4] ^= 0x67;
ptr[5] += 0xac;
ptr[6] += 0xc6;
ptr[7] ^= 0xc8;
ptr[8] += 0x68;
ptr[9] += 0x2e;
ptr[10] ^= 0x91;
ptr[11] ^= 0xa1;
ptr[12] ^= 0x71;
ptr[13] += 0x47;
ptr[14] ^= 0x9d;
ptr[15] ^= 0xd4;
ptr[16] ^= 0x1c;
ptr[17] += 0x81;
ptr[18] ^= 0x4f;
ptr[19] ^= 0x3d;

}

void multi_tracer_p197(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c5)
{
multi_tracer_p233(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x738fcd ^ regs->rbx);
ptr[0] += 0x2e;
ptr[1] ^= 0x8a;
ptr[2] += 0x49;
ptr[3] ^= 0x70;
ptr[4] += 0xfd;
ptr[5] += 0x29;
ptr[6] += 0x64;
ptr[7] += 0x99;
ptr[8] += 0x14;
ptr[9] += 0x72;
ptr[10] ^= 0x4a;
ptr[11] += 0xf6;
ptr[12] += 0xa4;
ptr[13] ^= 0xa2;
ptr[14] ^= 0x62;
ptr[15] += 0x64;
ptr[16] ^= 0xe2;
ptr[17] += 0x20;
ptr[18] += 0x63;
ptr[19] ^= 0x92;

}

void multi_tracer_p233(struct user_regs_struct *regs)
{
if (regs->rax != 0x20e9)
{
multi_tracer_p19(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8c01fa ^ regs->rbx);
ptr[0] += 0xab;
ptr[1] += 0x8c;
ptr[2] ^= 0x60;
ptr[3] += 0x46;
ptr[4] += 0x87;
ptr[5] ^= 0xd3;
ptr[6] ^= 0xde;
ptr[7] += 0x84;
ptr[8] += 0x7b;
ptr[9] += 0x18;
ptr[10] += 0x8c;
ptr[11] ^= 0xb0;
ptr[12] += 0xad;
ptr[13] ^= 0x56;
ptr[14] ^= 0x11;
ptr[15] ^= 0x54;
ptr[16] += 0xd8;
ptr[17] ^= 0xd3;
ptr[18] ^= 0x70;
ptr[19] ^= 0xea;
ptr[20] += 0x6f;
ptr[21] += 0x8;

}

void multi_tracer_p19(struct user_regs_struct *regs)
{
if (regs->rax != 0x2013)
{
multi_tracer_p251(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x83c85a ^ regs->rbx);
ptr[0] += 0xc;
ptr[1] += 0xaf;
ptr[2] ^= 0xa6;
ptr[3] ^= 0x48;
ptr[4] += 0x7b;
ptr[5] += 0x28;
ptr[6] += 0xdd;
ptr[7] ^= 0x79;
ptr[8] ^= 0xde;
ptr[9] += 0x89;
ptr[10] += 0xaf;
ptr[11] += 0x45;
ptr[12] ^= 0x52;
ptr[13] ^= 0xb5;
ptr[14] += 0xe0;
ptr[15] += 0xf1;
ptr[16] += 0xdf;
ptr[17] += 0x9;

}

void multi_tracer_p251(struct user_regs_struct *regs)
{
if (regs->rax != 0x20fb)
{
multi_tracer_p247(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xce851c ^ regs->rbx);
ptr[0] += 0x3a;
ptr[1] ^= 0x79;
ptr[2] += 0xdb;
ptr[3] ^= 0xad;
ptr[4] ^= 0xcc;
ptr[5] += 0xd6;
ptr[6] += 0xd2;
ptr[7] += 0xc3;
ptr[8] += 0x39;
ptr[9] ^= 0xac;
ptr[10] += 0x28;
ptr[11] += 0x3a;
ptr[12] += 0x4f;
ptr[13] += 0x2b;
ptr[14] ^= 0xb7;
ptr[15] += 0x53;
ptr[16] += 0xd9;
ptr[17] ^= 0x31;
ptr[18] += 0xec;
ptr[19] ^= 0x58;
ptr[20] += 0x9e;
ptr[21] ^= 0xf3;
ptr[22] ^= 0xb;
ptr[23] ^= 0x90;
ptr[24] ^= 0xa1;
ptr[25] ^= 0x11;
ptr[26] ^= 0xeb;

}

void multi_tracer_p247(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f7)
{
multi_tracer_p36(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x519234 ^ regs->rbx);
ptr[0] ^= 0x8f;
ptr[1] += 0x2f;
ptr[2] ^= 0xe0;
ptr[3] ^= 0xce;
ptr[4] += 0xb0;
ptr[5] += 0x54;
ptr[6] += 0x4c;
ptr[7] ^= 0x57;
ptr[8] ^= 0xf7;
ptr[9] += 0x75;
ptr[10] += 0x3e;
ptr[11] ^= 0x6e;
ptr[12] += 0x33;
ptr[13] ^= 0x71;
ptr[14] ^= 0xfd;
ptr[15] += 0x1;
ptr[16] ^= 0xf1;
ptr[17] ^= 0xee;

}

void multi_tracer_p36(struct user_regs_struct *regs)
{
if (regs->rax != 0x2024)
{
multi_tracer_p103(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x113a14 ^ regs->rbx);
ptr[0] ^= 0xb4;
ptr[1] ^= 0x3;
ptr[2] ^= 0xd7;
ptr[3] ^= 0xb2;
ptr[4] ^= 0xba;
ptr[5] ^= 0x32;
ptr[6] += 0x2f;
ptr[7] += 0xd2;
ptr[8] ^= 0xf4;
ptr[9] += 0x87;
ptr[10] += 0xcd;
ptr[11] ^= 0xff;
ptr[12] ^= 0xae;
ptr[13] ^= 0xed;
ptr[14] += 0x33;
ptr[15] += 0xc3;
ptr[16] += 0xd;
ptr[17] += 0xc;
ptr[18] += 0x27;
ptr[19] ^= 0x52;
ptr[20] += 0x2;
ptr[21] ^= 0xc2;
ptr[22] += 0x25;

}

void multi_tracer_p103(struct user_regs_struct *regs)
{
if (regs->rax != 0x2067)
{
multi_tracer_p192(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x3362af ^ regs->rbx);
ptr[0] += 0x31;
ptr[1] += 0x7b;
ptr[2] += 0xdb;
ptr[3] ^= 0xb1;
ptr[4] += 0x3e;
ptr[5] ^= 0xbb;
ptr[6] ^= 0x9;
ptr[7] ^= 0x3b;
ptr[8] += 0xac;
ptr[9] += 0xa0;
ptr[10] ^= 0xe3;
ptr[11] ^= 0x66;
ptr[12] += 0xc2;
ptr[13] ^= 0x5;
ptr[14] ^= 0x17;
ptr[15] += 0x2e;
ptr[16] ^= 0xb5;
ptr[17] += 0x5d;
ptr[18] ^= 0xf4;
ptr[19] += 0xa1;
ptr[20] ^= 0x61;
ptr[21] ^= 0x84;
ptr[22] ^= 0xcb;
ptr[23] += 0x85;
ptr[24] += 0xb6;
ptr[25] += 0x97;
ptr[26] += 0xc6;

}

void multi_tracer_p192(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c0)
{
multi_tracer_p50(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xdff578 ^ regs->rbx);
ptr[0] ^= 0xe9;
ptr[1] += 0x9a;
ptr[2] += 0x5f;
ptr[3] += 0xf7;
ptr[4] += 0x6e;
ptr[5] ^= 0x58;
ptr[6] ^= 0x62;
ptr[7] ^= 0x1e;
ptr[8] ^= 0x3;
ptr[9] += 0x5;
ptr[10] += 0x9a;
ptr[11] += 0xb3;
ptr[12] += 0xff;
ptr[13] += 0xb;
ptr[14] += 0x7a;
ptr[15] ^= 0xb8;
ptr[16] ^= 0xb9;
ptr[17] += 0x13;
ptr[18] ^= 0x1c;
ptr[19] ^= 0xe1;
ptr[20] += 0xb0;
ptr[21] ^= 0x17;
ptr[22] += 0x2e;
ptr[23] += 0x17;
ptr[24] ^= 0xea;
ptr[25] += 0x8;

}

void multi_tracer_p50(struct user_regs_struct *regs)
{
if (regs->rax != 0x2032)
{
multi_tracer_p81(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb696ee ^ regs->rbx);
ptr[0] ^= 0x23;
ptr[1] += 0x57;
ptr[2] ^= 0xe1;
ptr[3] ^= 0x17;
ptr[4] += 0x49;
ptr[5] += 0x73;
ptr[6] += 0x9b;
ptr[7] += 0xb;
ptr[8] ^= 0xee;
ptr[9] ^= 0xfe;
ptr[10] += 0xcf;
ptr[11] ^= 0x1f;
ptr[12] += 0xca;
ptr[13] += 0x44;
ptr[14] ^= 0xbb;
ptr[15] ^= 0xb0;
ptr[16] += 0x64;
ptr[17] ^= 0x27;
ptr[18] ^= 0xd;
ptr[19] += 0xd7;
ptr[20] += 0xfa;
ptr[21] += 0xd0;
ptr[22] ^= 0xde;
ptr[23] ^= 0x64;

}

void multi_tracer_p81(struct user_regs_struct *regs)
{
if (regs->rax != 0x2051)
{
multi_tracer_p80(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x52fad0 ^ regs->rbx);
ptr[0] += 0xb6;
ptr[1] ^= 0x64;
ptr[2] += 0x77;
ptr[3] ^= 0x87;
ptr[4] += 0x99;
ptr[5] ^= 0xa4;
ptr[6] ^= 0xe8;
ptr[7] += 0xcc;
ptr[8] ^= 0xdb;
ptr[9] ^= 0xe1;
ptr[10] += 0x33;
ptr[11] += 0x4;
ptr[12] ^= 0x94;
ptr[13] ^= 0x93;
ptr[14] += 0xbb;
ptr[15] += 0x9d;
ptr[16] += 0xb3;
ptr[17] += 0x52;
ptr[18] ^= 0x6b;

}

void multi_tracer_p80(struct user_regs_struct *regs)
{
if (regs->rax != 0x2050)
{
multi_tracer_p75(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xba4402 ^ regs->rbx);
ptr[0] ^= 0x73;
ptr[1] ^= 0x49;
ptr[2] += 0x8e;
ptr[3] ^= 0xd3;
ptr[4] += 0xd9;
ptr[5] ^= 0x7c;
ptr[6] ^= 0x1d;
ptr[7] ^= 0xdd;
ptr[8] ^= 0x47;
ptr[9] ^= 0x78;
ptr[10] += 0xb9;
ptr[11] += 0x26;
ptr[12] ^= 0xd2;
ptr[13] += 0xe;
ptr[14] ^= 0x49;
ptr[15] += 0x27;
ptr[16] += 0xb9;
ptr[17] ^= 0x85;

}

void multi_tracer_p75(struct user_regs_struct *regs)
{
if (regs->rax != 0x204b)
{
multi_tracer_p196(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x9966d8 ^ regs->rbx);
ptr[0] ^= 0x1c;
ptr[1] ^= 0x8;
ptr[2] += 0x94;
ptr[3] ^= 0x7c;
ptr[4] ^= 0x1a;
ptr[5] ^= 0x93;
ptr[6] += 0xb9;
ptr[7] ^= 0xba;
ptr[8] ^= 0xb3;
ptr[9] += 0x9f;
ptr[10] ^= 0x4b;
ptr[11] ^= 0x73;
ptr[12] += 0x3;
ptr[13] += 0x40;
ptr[14] += 0xb9;
ptr[15] += 0xfe;
ptr[16] += 0x17;
ptr[17] += 0x3b;
ptr[18] ^= 0xac;
ptr[19] ^= 0x87;
ptr[20] ^= 0xa2;

}

void multi_tracer_p196(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c4)
{
multi_tracer_p70(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xad8587 ^ regs->rbx);
ptr[0] ^= 0xa;
ptr[1] += 0x51;
ptr[2] += 0xc8;
ptr[3] ^= 0x83;
ptr[4] += 0xb3;
ptr[5] ^= 0x6e;
ptr[6] ^= 0xf0;
ptr[7] ^= 0x2e;
ptr[8] += 0x93;
ptr[9] ^= 0xef;
ptr[10] += 0x88;
ptr[11] += 0xf0;
ptr[12] ^= 0x96;
ptr[13] ^= 0xd8;
ptr[14] += 0xc1;
ptr[15] += 0xb0;
ptr[16] ^= 0x60;
ptr[17] += 0x65;

}

void multi_tracer_p70(struct user_regs_struct *regs)
{
if (regs->rax != 0x2046)
{
multi_tracer_p255(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x242aa3 ^ regs->rbx);
ptr[0] ^= 0x69;
ptr[1] += 0x59;
ptr[2] += 0x58;
ptr[3] ^= 0x70;
ptr[4] += 0xba;
ptr[5] += 0x62;
ptr[6] ^= 0x7c;
ptr[7] += 0x9;
ptr[8] += 0x1a;
ptr[9] += 0x40;
ptr[10] ^= 0x42;
ptr[11] ^= 0x63;
ptr[12] += 0x77;
ptr[13] ^= 0x14;
ptr[14] ^= 0xf;
ptr[15] += 0x76;
ptr[16] ^= 0xb8;
ptr[17] ^= 0x45;
ptr[18] ^= 0x42;
ptr[19] += 0xba;
ptr[20] += 0x2c;
ptr[21] += 0x3b;
ptr[22] ^= 0x28;
ptr[23] += 0x22;
ptr[24] += 0xb0;

}

void multi_tracer_p255(struct user_regs_struct *regs)
{
if (regs->rax != 0x20ff)
{
multi_tracer_p155(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x47c7b5 ^ regs->rbx);
ptr[0] ^= 0x18;
ptr[1] ^= 0xc9;
ptr[2] ^= 0xf3;
ptr[3] += 0x11;
ptr[4] ^= 0x89;
ptr[5] ^= 0x95;
ptr[6] += 0xfe;
ptr[7] += 0x1c;
ptr[8] += 0xd8;
ptr[9] += 0x7f;
ptr[10] += 0x82;
ptr[11] += 0x63;
ptr[12] ^= 0xf5;
ptr[13] += 0xcd;
ptr[14] ^= 0x9b;
ptr[15] ^= 0x76;
ptr[16] += 0x8b;
ptr[17] ^= 0x1c;
ptr[18] += 0x8e;
ptr[19] += 0x6f;

}

void multi_tracer_p155(struct user_regs_struct *regs)
{
if (regs->rax != 0x209b)
{
multi_tracer_p184(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x1c114e ^ regs->rbx);
ptr[0] += 0xad;
ptr[1] ^= 0xf0;
ptr[2] ^= 0x8b;
ptr[3] ^= 0xdd;
ptr[4] ^= 0x27;
ptr[5] ^= 0xc7;
ptr[6] += 0x8f;
ptr[7] ^= 0xa8;
ptr[8] ^= 0xbb;
ptr[9] += 0x1b;
ptr[10] += 0x7f;
ptr[11] += 0xf0;
ptr[12] ^= 0xb7;
ptr[13] += 0x92;
ptr[14] ^= 0x90;
ptr[15] ^= 0x41;
ptr[16] ^= 0xcc;
ptr[17] += 0x4;
ptr[18] ^= 0xb2;
ptr[19] ^= 0x25;
ptr[20] += 0x47;

}

void multi_tracer_p184(struct user_regs_struct *regs)
{
if (regs->rax != 0x20b8)
{
multi_tracer_p104(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc63e1f ^ regs->rbx);
ptr[0] ^= 0x5f;
ptr[1] ^= 0xef;
ptr[2] += 0x82;
ptr[3] += 0xcc;
ptr[4] ^= 0x3a;
ptr[5] ^= 0x37;
ptr[6] ^= 0x2;
ptr[7] += 0xc3;
ptr[8] += 0xa9;
ptr[9] += 0x20;
ptr[10] += 0xe7;
ptr[11] ^= 0xfb;
ptr[12] += 0x1d;
ptr[13] += 0x92;
ptr[14] ^= 0xc8;
ptr[15] ^= 0xa6;
ptr[16] ^= 0x11;
ptr[17] ^= 0x54;
ptr[18] += 0x5f;
ptr[19] ^= 0x17;
ptr[20] += 0xf5;
ptr[21] ^= 0x88;
ptr[22] += 0xc2;
ptr[23] += 0xff;
ptr[24] ^= 0xad;
ptr[25] += 0x71;
ptr[26] += 0x8;

}

void multi_tracer_p104(struct user_regs_struct *regs)
{
if (regs->rax != 0x2068)
{
multi_tracer_p246(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xc29293 ^ regs->rbx);
ptr[0] += 0x76;
ptr[1] += 0x32;
ptr[2] += 0xb6;
ptr[3] ^= 0xc4;
ptr[4] ^= 0x80;
ptr[5] += 0x2d;
ptr[6] += 0xb3;
ptr[7] ^= 0x2a;
ptr[8] ^= 0x3b;
ptr[9] += 0x3b;
ptr[10] += 0x4f;
ptr[11] ^= 0x42;
ptr[12] += 0x41;
ptr[13] ^= 0xd0;
ptr[14] += 0x62;
ptr[15] += 0x16;
ptr[16] ^= 0xf2;
ptr[17] += 0xa6;
ptr[18] ^= 0x89;
ptr[19] += 0x60;

}

void multi_tracer_p246(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f6)
{
multi_tracer_p40(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xb86c2c ^ regs->rbx);
ptr[0] ^= 0x20;
ptr[1] ^= 0x71;
ptr[2] += 0x26;
ptr[3] ^= 0xfd;
ptr[4] ^= 0x61;
ptr[5] += 0xfb;
ptr[6] ^= 0xb7;
ptr[7] += 0xcd;
ptr[8] += 0x10;
ptr[9] ^= 0xd9;
ptr[10] += 0xa9;
ptr[11] += 0x9;
ptr[12] ^= 0x82;
ptr[13] += 0xc3;
ptr[14] += 0x72;
ptr[15] ^= 0x13;
ptr[16] += 0xa4;
ptr[17] += 0xf3;
ptr[18] ^= 0x53;
ptr[19] += 0x91;
ptr[20] += 0x77;
ptr[21] ^= 0x36;

}

void multi_tracer_p40(struct user_regs_struct *regs)
{
if (regs->rax != 0x2028)
{
multi_tracer_p252(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x2fd6fa ^ regs->rbx);
ptr[0] += 0x3f;
ptr[1] ^= 0x7c;
ptr[2] += 0x89;
ptr[3] += 0x9d;
ptr[4] ^= 0xb9;
ptr[5] += 0x33;
ptr[6] ^= 0x7a;
ptr[7] += 0x30;
ptr[8] += 0xa;
ptr[9] ^= 0x55;
ptr[10] ^= 0xc1;
ptr[11] ^= 0x19;
ptr[12] ^= 0x8d;
ptr[13] ^= 0x5a;
ptr[14] += 0x33;
ptr[15] ^= 0x68;
ptr[16] ^= 0x84;
ptr[17] += 0x63;
ptr[18] += 0x33;
ptr[19] += 0x76;
ptr[20] += 0xcf;
ptr[21] += 0xd0;
ptr[22] ^= 0xa2;
ptr[23] += 0x3a;
ptr[24] ^= 0x46;

}

void multi_tracer_p252(struct user_regs_struct *regs)
{
if (regs->rax != 0x20fc)
{
multi_tracer_p168(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x311135 ^ regs->rbx);
ptr[0] ^= 0x18;
ptr[1] ^= 0x31;
ptr[2] ^= 0x1;
ptr[3] ^= 0x4c;
ptr[4] ^= 0x9e;
ptr[5] ^= 0x6f;
ptr[6] ^= 0x1c;
ptr[7] += 0x31;
ptr[8] ^= 0x94;
ptr[9] ^= 0xd6;
ptr[10] += 0xf1;
ptr[11] += 0x48;
ptr[12] ^= 0xaa;
ptr[13] ^= 0x6c;
ptr[14] += 0x7b;
ptr[15] += 0x45;
ptr[16] += 0xb0;
ptr[17] += 0xe;
ptr[18] += 0x78;
ptr[19] += 0xb9;
ptr[20] += 0x74;
ptr[21] += 0x49;
ptr[22] ^= 0xcf;
ptr[23] += 0x3;
ptr[24] += 0x35;
ptr[25] += 0xba;

}

void multi_tracer_p168(struct user_regs_struct *regs)
{
if (regs->rax != 0x20a8)
{
multi_tracer_p57(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x6ca471 ^ regs->rbx);
ptr[0] ^= 0xac;
ptr[1] += 0x76;
ptr[2] ^= 0xa8;
ptr[3] += 0x3;
ptr[4] += 0x4b;
ptr[5] ^= 0x8e;
ptr[6] ^= 0xf1;
ptr[7] ^= 0x14;
ptr[8] ^= 0xdb;
ptr[9] += 0x7;
ptr[10] ^= 0x31;
ptr[11] += 0xda;
ptr[12] ^= 0x41;
ptr[13] += 0xdc;
ptr[14] ^= 0x88;
ptr[15] += 0x73;
ptr[16] ^= 0x69;
ptr[17] += 0xf;
ptr[18] ^= 0x65;
ptr[19] ^= 0x1;
ptr[20] ^= 0x69;
ptr[21] += 0xf8;

}

void multi_tracer_p57(struct user_regs_struct *regs)
{
if (regs->rax != 0x2039)
{
multi_tracer_p34(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x86e361 ^ regs->rbx);
ptr[0] += 0xee;
ptr[1] ^= 0xeb;
ptr[2] ^= 0x89;
ptr[3] ^= 0x12;
ptr[4] ^= 0x5f;
ptr[5] += 0xa9;
ptr[6] ^= 0xf5;
ptr[7] += 0x16;
ptr[8] += 0x24;
ptr[9] += 0x4;
ptr[10] ^= 0x7c;
ptr[11] += 0xd;
ptr[12] ^= 0x23;
ptr[13] += 0x40;
ptr[14] += 0xd8;
ptr[15] += 0xc5;
ptr[16] ^= 0x6f;
ptr[17] += 0xe4;
ptr[18] ^= 0x17;
ptr[19] ^= 0xdc;

}

void multi_tracer_p34(struct user_regs_struct *regs)
{
if (regs->rax != 0x2022)
{
multi_tracer_p216(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xa26830 ^ regs->rbx);
ptr[0] ^= 0x74;
ptr[1] ^= 0x3c;
ptr[2] ^= 0x6f;
ptr[3] += 0xd3;
ptr[4] += 0x4;
ptr[5] += 0xe5;
ptr[6] ^= 0x58;
ptr[7] += 0x8e;
ptr[8] ^= 0xe0;
ptr[9] += 0x65;
ptr[10] ^= 0xb0;
ptr[11] += 0xe6;
ptr[12] ^= 0xe0;
ptr[13] ^= 0x16;
ptr[14] += 0x36;
ptr[15] += 0xa8;
ptr[16] += 0x75;
ptr[17] += 0x49;
ptr[18] += 0xf1;
ptr[19] ^= 0xcf;
ptr[20] ^= 0x15;
ptr[21] += 0x8c;
ptr[22] += 0x31;
ptr[23] += 0xa4;
ptr[24] += 0x7c;

}

void multi_tracer_p216(struct user_regs_struct *regs)
{
if (regs->rax != 0x20d8)
{
multi_tracer_p201(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0x8d74de ^ regs->rbx);
ptr[0] ^= 0xe;
ptr[1] ^= 0x3e;
ptr[2] ^= 0xa5;
ptr[3] ^= 0x9a;
ptr[4] ^= 0x3a;
ptr[5] += 0xe4;
ptr[6] += 0xc8;
ptr[7] += 0xb;
ptr[8] += 0x7;
ptr[9] += 0x3f;
ptr[10] += 0xa;
ptr[11] ^= 0x62;
ptr[12] += 0x1a;
ptr[13] += 0x9e;
ptr[14] ^= 0x90;
ptr[15] += 0x52;
ptr[16] += 0xbe;
ptr[17] += 0xe5;
ptr[18] ^= 0xb0;
ptr[19] += 0x5b;
ptr[20] += 0x83;
ptr[21] += 0x2f;

}

void multi_tracer_p201(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c9)
{
multi_tracer_p244(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xf7623a ^ regs->rbx);
ptr[0] ^= 0x7d;
ptr[1] += 0xeb;
ptr[2] += 0xaa;
ptr[3] ^= 0xe;
ptr[4] += 0x32;
ptr[5] += 0x92;
ptr[6] += 0x22;
ptr[7] ^= 0x2f;
ptr[8] += 0x38;
ptr[9] ^= 0x56;
ptr[10] += 0x41;
ptr[11] += 0xf4;
ptr[12] += 0x8a;
ptr[13] += 0xe2;
ptr[14] += 0xa2;
ptr[15] ^= 0x31;
ptr[16] += 0xff;
ptr[17] ^= 0x7c;
ptr[18] ^= 0xa7;

}

void multi_tracer_p244(struct user_regs_struct *regs)
{
if (regs->rax != 0x20f4)
{
multi_tracer_p200(regs);return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xab13b8 ^ regs->rbx);
ptr[0] += 0x8d;
ptr[1] ^= 0x8c;
ptr[2] ^= 0xfc;
ptr[3] ^= 0x6;
ptr[4] += 0x1;
ptr[5] ^= 0x75;
ptr[6] ^= 0xc1;
ptr[7] += 0x7e;
ptr[8] ^= 0x5d;
ptr[9] += 0xe8;
ptr[10] ^= 0xc1;
ptr[11] += 0x31;
ptr[12] += 0xe8;
ptr[13] += 0xef;
ptr[14] += 0x57;
ptr[15] ^= 0x46;
ptr[16] ^= 0x25;
ptr[17] += 0x7b;
ptr[18] += 0x8;
ptr[19] += 0x67;
ptr[20] ^= 0x49;
ptr[21] += 0xc1;
ptr[22] += 0x4a;
ptr[23] += 0xb4;
ptr[24] += 0x3e;

}

void multi_tracer_p200(struct user_regs_struct *regs)
{
if (regs->rax != 0x20c8)
{
return;
}
uint64_t code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code &= 0xffffffffffff0000;
code |= 0x9090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
char* ptr = (char*)(0xaf7fc8 ^ regs->rbx);
ptr[0] ^= 0x76;
ptr[1] += 0x5f;
ptr[2] ^= 0xe7;
ptr[3] += 0xe7;
ptr[4] ^= 0xe1;
ptr[5] ^= 0x33;
ptr[6] ^= 0x6d;
ptr[7] ^= 0x96;
ptr[8] += 0x52;
ptr[9] += 0x1a;
ptr[10] ^= 0x12;
ptr[11] += 0x28;
ptr[12] ^= 0x8d;
ptr[13] ^= 0xea;
ptr[14] ^= 0x4b;
ptr[15] += 0x4f;
ptr[16] += 0x54;
ptr[17] += 0x33;
ptr[18] ^= 0xe2;
ptr[19] ^= 0xee;
ptr[20] ^= 0x12;
ptr[21] ^= 0xe9;
ptr[22] += 0x27;
ptr[23] ^= 0xbc;
ptr[24] += 0x16;
ptr[25] += 0xea;
ptr[26] += 0xd7;

}

void multi_tracer_c186(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ba)
{
multi_tracer_c241(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xedcf5b85b5eb5815;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xa5b88aa52f0980e3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c241(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f1)
{
multi_tracer_c121(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xa8653dd7e7a8b93e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c121(struct user_regs_struct *regs)
{
if (regs->rax != 0x1079)
{
multi_tracer_c216(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x21e44f7fcdec840f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x26c61935fba36105;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c216(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d8)
{
multi_tracer_c208(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x155b434ef424c82a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c208(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d0)
{
multi_tracer_c74(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x1e0d61f3b31a38d1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c74(struct user_regs_struct *regs)
{
if (regs->rax != 0x104a)
{
multi_tracer_c151(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x45e218a47fb51a7a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xefcb08d5eeb3532f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c151(struct user_regs_struct *regs)
{
if (regs->rax != 0x1097)
{
multi_tracer_c150(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x11b47eeb7c0202b4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c150(struct user_regs_struct *regs)
{
if (regs->rax != 0x1096)
{
multi_tracer_c164(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x79c5f0c7dea1b271;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c164(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a4)
{
multi_tracer_c129(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9e875823c53bd399;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c129(struct user_regs_struct *regs)
{
if (regs->rax != 0x1081)
{
multi_tracer_c199(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x85976e06c8875e50;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xa8ba839a8bb0e869;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c199(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c7)
{
multi_tracer_c160(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x3f3ff9fd2d9286ce;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c160(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a0)
{
multi_tracer_c183(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3c5daa07361ca600;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x7575555623f6c28d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c183(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b7)
{
multi_tracer_c157(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc8835552ca6dd1bc;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x523a4b3cc385fe3f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c157(struct user_regs_struct *regs)
{
if (regs->rax != 0x109d)
{
multi_tracer_c109(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xafac3cc7d81a12c1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x4c12f8f869c1f237;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c109(struct user_regs_struct *regs)
{
if (regs->rax != 0x106d)
{
multi_tracer_c229(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x5d14783fe512f239;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c229(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e5)
{
multi_tracer_c20(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xd9db6acf3d0ab2a8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x5b29464bf128af62;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c20(struct user_regs_struct *regs)
{
if (regs->rax != 0x1014)
{
multi_tracer_c10(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xdff194157dc5ada0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c10(struct user_regs_struct *regs)
{
if (regs->rax != 0x100a)
{
multi_tracer_c105(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x1402b1adc14597b3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c105(struct user_regs_struct *regs)
{
if (regs->rax != 0x1069)
{
multi_tracer_c195(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xfaecd88c73653c3f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x772c3a61c499a916;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c195(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c3)
{
multi_tracer_c103(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xd97e84d582ee77d5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x8a02c3679350f6b2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c103(struct user_regs_struct *regs)
{
if (regs->rax != 0x1067)
{
multi_tracer_c209(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x68e3f434e028d0d2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x23313cd06d09186b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c209(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d1)
{
multi_tracer_c240(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf734a0c7ffa2e563;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x1ce82b7e1d28d2fd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c240(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f0)
{
multi_tracer_c33(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xcb8d3a1096ad0300;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x16af89bda84f044c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c33(struct user_regs_struct *regs)
{
if (regs->rax != 0x1021)
{
multi_tracer_c47(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xf9fa436f17c58f40;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x5c1d2333229ae331;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c47(struct user_regs_struct *regs)
{
if (regs->rax != 0x102f)
{
multi_tracer_c75(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf2ca761b2d3198fc;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c75(struct user_regs_struct *regs)
{
if (regs->rax != 0x104b)
{
multi_tracer_c39(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x310d2fcf1686bc17;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c39(struct user_regs_struct *regs)
{
if (regs->rax != 0x1027)
{
multi_tracer_c215(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xd9bbc9e1074c8e87;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c215(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d7)
{
multi_tracer_c21(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x605d708e07b7eca1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c21(struct user_regs_struct *regs)
{
if (regs->rax != 0x1015)
{
multi_tracer_c8(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x664901628f4da1bc;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xca409e018c2c411d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c8(struct user_regs_struct *regs)
{
if (regs->rax != 0x1008)
{
multi_tracer_c191(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc1ecca1da1932f68;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x3814fea6b07ccd75;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c191(struct user_regs_struct *regs)
{
if (regs->rax != 0x10bf)
{
multi_tracer_c179(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x62473864baa2b57e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c179(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b3)
{
multi_tracer_c188(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x98f39d76613176ee;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c188(struct user_regs_struct *regs)
{
if (regs->rax != 0x10bc)
{
multi_tracer_c176(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x50aea9c62584a3da;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xab6d8d372c31e3b5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c176(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b0)
{
multi_tracer_c65(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x2b558dbdc023a2ae;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x6f23fc0452f3883b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c65(struct user_regs_struct *regs)
{
if (regs->rax != 0x1041)
{
multi_tracer_c67(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x61cb72d912717835;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c67(struct user_regs_struct *regs)
{
if (regs->rax != 0x1043)
{
multi_tracer_c24(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xca4fd5790a78f33d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x5fbac7087b8e8141;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c24(struct user_regs_struct *regs)
{
if (regs->rax != 0x1018)
{
multi_tracer_c171(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x2a0ca3c4d299cebb;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c171(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ab)
{
multi_tracer_c69(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xcd30d8d1e91788d6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xf7a057fe598016a3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c69(struct user_regs_struct *regs)
{
if (regs->rax != 0x1045)
{
multi_tracer_c146(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x45bc6fdaac433cc1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c146(struct user_regs_struct *regs)
{
if (regs->rax != 0x1092)
{
multi_tracer_c237(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x357fd99ba69ce404;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c237(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ed)
{
multi_tracer_c97(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x8d7de3bbef4bf556;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c97(struct user_regs_struct *regs)
{
if (regs->rax != 0x1061)
{
multi_tracer_c251(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x1df5de21b9cc2137;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c251(struct user_regs_struct *regs)
{
if (regs->rax != 0x10fb)
{
multi_tracer_c206(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x7fb78d8d591534ee;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xdee13f913a720b2d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c206(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ce)
{
multi_tracer_c19(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7df1bddaac894cf5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x48ae4af17dfe7d87;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c19(struct user_regs_struct *regs)
{
if (regs->rax != 0x1013)
{
multi_tracer_c42(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc7ec0bf47f1718ec;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x9e6fd6503644021e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c42(struct user_regs_struct *regs)
{
if (regs->rax != 0x102a)
{
multi_tracer_c153(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xb30a94364bfe8eec;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x71220e1aea39db07;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c153(struct user_regs_struct *regs)
{
if (regs->rax != 0x1099)
{
multi_tracer_c248(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x6d5631670dcfad7f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x75dd88bf4d4b4a2f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c248(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f8)
{
multi_tracer_c233(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x8672deb275c1a841;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x295acc96abbbb860;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c233(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e9)
{
multi_tracer_c117(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc694d4ca3b203ec0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xaf5a6bd86d2c1bde;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c117(struct user_regs_struct *regs)
{
if (regs->rax != 0x1075)
{
multi_tracer_c54(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x47d753527f364744;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xe258a2d9d4039ed3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c54(struct user_regs_struct *regs)
{
if (regs->rax != 0x1036)
{
multi_tracer_c84(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x23801ca477a6d5bf;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c84(struct user_regs_struct *regs)
{
if (regs->rax != 0x1054)
{
multi_tracer_c173(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc394ffe509803c78;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c173(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ad)
{
multi_tracer_c178(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xbd7509a55cf6752c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c178(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b2)
{
multi_tracer_c106(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xcbac79f057d3b8b3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c106(struct user_regs_struct *regs)
{
if (regs->rax != 0x106a)
{
multi_tracer_c200(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x2072418dc0448fde;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c200(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c8)
{
multi_tracer_c172(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x670d5e0ebf182bb0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c172(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ac)
{
multi_tracer_c55(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x19e25bc122f03ad9;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c55(struct user_regs_struct *regs)
{
if (regs->rax != 0x1037)
{
multi_tracer_c64(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xe761b15adceda08a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xa47d9f5ec7f90a88;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c64(struct user_regs_struct *regs)
{
if (regs->rax != 0x1040)
{
multi_tracer_c222(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x12d96da55e6b11aa;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xf088dfc6ca53ab77;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c222(struct user_regs_struct *regs)
{
if (regs->rax != 0x10de)
{
multi_tracer_c220(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x5896bd9a56b0fb73;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c220(struct user_regs_struct *regs)
{
if (regs->rax != 0x10dc)
{
multi_tracer_c98(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xdcd20e43d6929b9e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c98(struct user_regs_struct *regs)
{
if (regs->rax != 0x1062)
{
multi_tracer_c197(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xbe889877aedc0f32;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c197(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c5)
{
multi_tracer_c158(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xf89ce9a7755e589f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c158(struct user_regs_struct *regs)
{
if (regs->rax != 0x109e)
{
multi_tracer_c225(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x8cddf1fbd5df4cba;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x70da2de7e5a198e3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c225(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e1)
{
multi_tracer_c118(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x978e58a8d18e089d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xfb2535054a2debb7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c118(struct user_regs_struct *regs)
{
if (regs->rax != 0x1076)
{
multi_tracer_c51(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xac429d028ce4a84d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c51(struct user_regs_struct *regs)
{
if (regs->rax != 0x1033)
{
multi_tracer_c182(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc88d204d717f5c27;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x48b6ae0bd99687ea;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c182(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b6)
{
multi_tracer_c37(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xdfba710c8e8c10b8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c37(struct user_regs_struct *regs)
{
if (regs->rax != 0x1025)
{
multi_tracer_c87(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7dd83d64b7c8c0f6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xcd2f8880f83558dd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c87(struct user_regs_struct *regs)
{
if (regs->rax != 0x1057)
{
multi_tracer_c114(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xcedbda16de1bc1d5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xcdb26bc1d0ec3fe1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c114(struct user_regs_struct *regs)
{
if (regs->rax != 0x1072)
{
multi_tracer_c81(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x77c594b7c7eac842;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xa1a4f64fa0da484a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c81(struct user_regs_struct *regs)
{
if (regs->rax != 0x1051)
{
multi_tracer_c44(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc558f911a7c887b4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xa9f4ac2652a360d8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c44(struct user_regs_struct *regs)
{
if (regs->rax != 0x102c)
{
multi_tracer_c79(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xdd6c47696411dbd7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c79(struct user_regs_struct *regs)
{
if (regs->rax != 0x104f)
{
multi_tracer_c80(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x44fddf4cff3504fd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xd2fff271d5a33105;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c80(struct user_regs_struct *regs)
{
if (regs->rax != 0x1050)
{
multi_tracer_c12(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xb9e561898ecb9f2d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x1c7023954a1adabf;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c12(struct user_regs_struct *regs)
{
if (regs->rax != 0x100c)
{
multi_tracer_c31(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x94073b8b5d89a89b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c31(struct user_regs_struct *regs)
{
if (regs->rax != 0x101f)
{
multi_tracer_c252(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf381ee21676fb970;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xafae3f952d8ce39e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c252(struct user_regs_struct *regs)
{
if (regs->rax != 0x10fc)
{
multi_tracer_c125(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x65c6aba5d21b6a24;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x23595eda3a12954b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c125(struct user_regs_struct *regs)
{
if (regs->rax != 0x107d)
{
multi_tracer_c15(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x5af14f5d2bbbc18a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c15(struct user_regs_struct *regs)
{
if (regs->rax != 0x100f)
{
multi_tracer_c116(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xda72aeeba8cb555f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c116(struct user_regs_struct *regs)
{
if (regs->rax != 0x1074)
{
multi_tracer_c187(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x6f56fe92d2ac665e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c187(struct user_regs_struct *regs)
{
if (regs->rax != 0x10bb)
{
multi_tracer_c45(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x6c6833e0f8855cfb;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c45(struct user_regs_struct *regs)
{
if (regs->rax != 0x102d)
{
multi_tracer_c180(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x5eebacd9cfa72111;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x666480520337afd3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c180(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b4)
{
multi_tracer_c1(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x8880f4b36316d5b3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c1(struct user_regs_struct *regs)
{
if (regs->rax != 0x1001)
{
multi_tracer_c192(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9312388c6f67fa62;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x7d8e3af783993435;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c192(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c0)
{
multi_tracer_c57(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x2fb91c786d79fafa;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c57(struct user_regs_struct *regs)
{
if (regs->rax != 0x1039)
{
multi_tracer_c22(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x57def28db85c9e81;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x2e3785b2741c62e7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c22(struct user_regs_struct *regs)
{
if (regs->rax != 0x1016)
{
multi_tracer_c90(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x676caedb8e9e6350;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xaa967be6bc544299;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c90(struct user_regs_struct *regs)
{
if (regs->rax != 0x105a)
{
multi_tracer_c102(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc3c420043b9a5f19;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x139cdfb16476bc91;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c102(struct user_regs_struct *regs)
{
if (regs->rax != 0x1066)
{
multi_tracer_c127(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x6c11eb09f4f83405;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x522db63e563017be;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c127(struct user_regs_struct *regs)
{
if (regs->rax != 0x107f)
{
multi_tracer_c4(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc8ed572aa2967401;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x29cc165f2edcd0a4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c4(struct user_regs_struct *regs)
{
if (regs->rax != 0x1004)
{
multi_tracer_c85(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x81d2405a1a6846ed;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c85(struct user_regs_struct *regs)
{
if (regs->rax != 0x1055)
{
multi_tracer_c144(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x1aa05855f3fdd816;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c144(struct user_regs_struct *regs)
{
if (regs->rax != 0x1090)
{
multi_tracer_c181(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xbb8b84e1578c07a0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xf91e3b8244051f3c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c181(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b5)
{
multi_tracer_c77(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x25c264cfc40173df;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c77(struct user_regs_struct *regs)
{
if (regs->rax != 0x104d)
{
multi_tracer_c123(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xdf4db1ae74f59153;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x70a6e67a30bfd885;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c123(struct user_regs_struct *regs)
{
if (regs->rax != 0x107b)
{
multi_tracer_c73(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x82a1da15ac5b21d2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x5427df5eab39a07c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c73(struct user_regs_struct *regs)
{
if (regs->rax != 0x1049)
{
multi_tracer_c108(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xd840f783a1950d39;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x47ee1dc99bc05e7d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c108(struct user_regs_struct *regs)
{
if (regs->rax != 0x106c)
{
multi_tracer_c23(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x66e76e16bee9afb0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x792f9881d95f52c2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c23(struct user_regs_struct *regs)
{
if (regs->rax != 0x1017)
{
multi_tracer_c61(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xa037b524824a246c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c61(struct user_regs_struct *regs)
{
if (regs->rax != 0x103d)
{
multi_tracer_c128(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3351fdc0aed3a535;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x819977d364fedbc8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c128(struct user_regs_struct *regs)
{
if (regs->rax != 0x1080)
{
multi_tracer_c221(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xf0d6ab7b3a491a07;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c221(struct user_regs_struct *regs)
{
if (regs->rax != 0x10dd)
{
multi_tracer_c113(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xbb7a36ef8f40f9b6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c113(struct user_regs_struct *regs)
{
if (regs->rax != 0x1071)
{
multi_tracer_c204(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x5fccbe148ae2214d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c204(struct user_regs_struct *regs)
{
if (regs->rax != 0x10cc)
{
multi_tracer_c196(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xd2421d69b2f7c7f4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c196(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c4)
{
multi_tracer_c242(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x2b7e4c7a0955ecf1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xbf0481158a0df664;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c242(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f2)
{
multi_tracer_c6(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3b04f950d8aed904;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c6(struct user_regs_struct *regs)
{
if (regs->rax != 0x1006)
{
multi_tracer_c212(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x51665d2ff1b81101;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x25823c5f09783fc4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c212(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d4)
{
multi_tracer_c148(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x14859035884e1f49;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c148(struct user_regs_struct *regs)
{
if (regs->rax != 0x1094)
{
multi_tracer_c71(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xdef72ff7117546ed;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x79f2860860a3907d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c71(struct user_regs_struct *regs)
{
if (regs->rax != 0x1047)
{
multi_tracer_c149(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xed910a828e0dfa54;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c149(struct user_regs_struct *regs)
{
if (regs->rax != 0x1095)
{
multi_tracer_c110(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x316eb2be57673f90;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xb41b7b8974962bc4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c110(struct user_regs_struct *regs)
{
if (regs->rax != 0x106e)
{
multi_tracer_c115(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x2208b4b97c279b4e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xb159a3cc7a39b8e0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c115(struct user_regs_struct *regs)
{
if (regs->rax != 0x1073)
{
multi_tracer_c156(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xd100e8407e812340;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c156(struct user_regs_struct *regs)
{
if (regs->rax != 0x109c)
{
multi_tracer_c86(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc8b192e086a99cf8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c86(struct user_regs_struct *regs)
{
if (regs->rax != 0x1056)
{
multi_tracer_c132(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x4be432d478705781;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x98a1b1df36e77e35;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c132(struct user_regs_struct *regs)
{
if (regs->rax != 0x1084)
{
multi_tracer_c175(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x564abe489b6a8ba8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c175(struct user_regs_struct *regs)
{
if (regs->rax != 0x10af)
{
multi_tracer_c133(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x5b31623d45f8e7cf;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c133(struct user_regs_struct *regs)
{
if (regs->rax != 0x1085)
{
multi_tracer_c193(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x8e5836a8e5bdd928;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x1331398ea3687062;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c193(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c1)
{
multi_tracer_c0(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc62935b6582ede41;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x6551888c55a92c8d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c0(struct user_regs_struct *regs)
{
if (regs->rax != 0x1000)
{
multi_tracer_c3(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x4cb89e1aa16b8b33;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c3(struct user_regs_struct *regs)
{
if (regs->rax != 0x1003)
{
multi_tracer_c205(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x60a329e566e0615b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c205(struct user_regs_struct *regs)
{
if (regs->rax != 0x10cd)
{
multi_tracer_c154(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x423f625623dd4f82;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c154(struct user_regs_struct *regs)
{
if (regs->rax != 0x109a)
{
multi_tracer_c28(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xe4dd5ac2b4531f73;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x95bbef6f4da70d13;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c28(struct user_regs_struct *regs)
{
if (regs->rax != 0x101c)
{
multi_tracer_c246(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x6064d95ee30b7b4f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xe8d2bb6d7e0f6837;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c246(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f6)
{
multi_tracer_c30(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xe13f78c5e9dcc06d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xcace2fd136f19296;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c30(struct user_regs_struct *regs)
{
if (regs->rax != 0x101e)
{
multi_tracer_c185(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x9a583bb8e62a06ab;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c185(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b9)
{
multi_tracer_c88(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xb3455b9a57c1a098;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c88(struct user_regs_struct *regs)
{
if (regs->rax != 0x1058)
{
multi_tracer_c49(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x510799009f485597;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xeeed5cc9324a213d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c49(struct user_regs_struct *regs)
{
if (regs->rax != 0x1031)
{
multi_tracer_c72(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xf1cd4ff31d999a6d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xc697e6dc87d70b71;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c72(struct user_regs_struct *regs)
{
if (regs->rax != 0x1048)
{
multi_tracer_c35(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7120f56e929f8540;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c35(struct user_regs_struct *regs)
{
if (regs->rax != 0x1023)
{
multi_tracer_c5(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x3fcddeb4dcff7839;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x6690489e7aef8f6d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c5(struct user_regs_struct *regs)
{
if (regs->rax != 0x1005)
{
multi_tracer_c70(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x926bfd7c5953949f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xd6b0a9b81e1498b2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c70(struct user_regs_struct *regs)
{
if (regs->rax != 0x1046)
{
multi_tracer_c140(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x68c60054a760f76a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x8cd6d52d8b344d05;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c140(struct user_regs_struct *regs)
{
if (regs->rax != 0x108c)
{
multi_tracer_c60(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x795e2a26499238f1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xf93f0bf21f26c227;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c60(struct user_regs_struct *regs)
{
if (regs->rax != 0x103c)
{
multi_tracer_c52(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc3de9cfff51aa7f4;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x9e181215aee809f3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c52(struct user_regs_struct *regs)
{
if (regs->rax != 0x1034)
{
multi_tracer_c92(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xaa64bbde4ce76817;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x725402e4962dca49;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c92(struct user_regs_struct *regs)
{
if (regs->rax != 0x105c)
{
multi_tracer_c165(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xe918649789dcbef0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c165(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a5)
{
multi_tracer_c232(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xf295fa41b70af247;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x384a74957e6def51;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c232(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e8)
{
multi_tracer_c40(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x756e83d133026ea5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x165869d41c73bb0b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c40(struct user_regs_struct *regs)
{
if (regs->rax != 0x1028)
{
multi_tracer_c62(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x2d9e2a625ec3869f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c62(struct user_regs_struct *regs)
{
if (regs->rax != 0x103e)
{
multi_tracer_c202(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xbd627bc643b930ca;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c202(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ca)
{
multi_tracer_c130(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x82f7d7b227203ae3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x7fa67c21ed85e719;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c130(struct user_regs_struct *regs)
{
if (regs->rax != 0x1082)
{
multi_tracer_c177(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf28cb24b0aa1e6f8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c177(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b1)
{
multi_tracer_c247(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x231c47928805fa80;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xc647493c416abaea;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c247(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f7)
{
multi_tracer_c27(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xed1705bc31d6aa4b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xe913406cd7d8b65f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c27(struct user_regs_struct *regs)
{
if (regs->rax != 0x101b)
{
multi_tracer_c253(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7603be608e15c43e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c253(struct user_regs_struct *regs)
{
if (regs->rax != 0x10fd)
{
multi_tracer_c231(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x1c8ba3685d8c9479;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c231(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e7)
{
multi_tracer_c166(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x80a0ff0053866b9e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x531203b5635a57fc;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c166(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a6)
{
multi_tracer_c112(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xb9da55cf75868eb8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c112(struct user_regs_struct *regs)
{
if (regs->rax != 0x1070)
{
multi_tracer_c249(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x1cb40b577fe33ab6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c249(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f9)
{
multi_tracer_c161(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3e1eb78059a50fa7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xf201a50068d21d4d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c161(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a1)
{
multi_tracer_c46(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xefc0d34517dbf007;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x986381bc1af156aa;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c46(struct user_regs_struct *regs)
{
if (regs->rax != 0x102e)
{
multi_tracer_c141(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x1432dc9545f629ab;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x14ea346721997550;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c141(struct user_regs_struct *regs)
{
if (regs->rax != 0x108d)
{
multi_tracer_c137(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9cbed05756c44d5a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c137(struct user_regs_struct *regs)
{
if (regs->rax != 0x1089)
{
multi_tracer_c32(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xcb6c56406d99c30d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c32(struct user_regs_struct *regs)
{
if (regs->rax != 0x1020)
{
multi_tracer_c83(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x30df03f3c84aba32;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c83(struct user_regs_struct *regs)
{
if (regs->rax != 0x1053)
{
multi_tracer_c136(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7a2b8f8cfe536240;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c136(struct user_regs_struct *regs)
{
if (regs->rax != 0x1088)
{
multi_tracer_c189(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xec721fd2895ac491;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c189(struct user_regs_struct *regs)
{
if (regs->rax != 0x10bd)
{
multi_tracer_c230(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x49c00c944b38f536;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c230(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e6)
{
multi_tracer_c34(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x62c5249d880e330e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c34(struct user_regs_struct *regs)
{
if (regs->rax != 0x1022)
{
multi_tracer_c104(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x74b35c2ac1f02c80;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c104(struct user_regs_struct *regs)
{
if (regs->rax != 0x1068)
{
multi_tracer_c91(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x90fa4fa3373fc4cd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c91(struct user_regs_struct *regs)
{
if (regs->rax != 0x105b)
{
multi_tracer_c170(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x94fbc9aa7a42e532;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x9394edbc0c1376ba;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c170(struct user_regs_struct *regs)
{
if (regs->rax != 0x10aa)
{
multi_tracer_c13(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x2b0d298675b1a285;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x9d4fa354f1b7e637;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c13(struct user_regs_struct *regs)
{
if (regs->rax != 0x100d)
{
multi_tracer_c245(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xdd2c10bd597e2691;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x8167e7ad8c085110;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c245(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f5)
{
multi_tracer_c167(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x5a521908c539e502;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c167(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a7)
{
multi_tracer_c82(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x599746da35e5fef9;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c82(struct user_regs_struct *regs)
{
if (regs->rax != 0x1052)
{
multi_tracer_c99(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x3679349d7c824711;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c99(struct user_regs_struct *regs)
{
if (regs->rax != 0x1063)
{
multi_tracer_c76(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xb4f3f719c8b0fe2a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c76(struct user_regs_struct *regs)
{
if (regs->rax != 0x104c)
{
multi_tracer_c9(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x8e7fdeb51484c67f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c9(struct user_regs_struct *regs)
{
if (regs->rax != 0x1009)
{
multi_tracer_c184(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x9ee9fba09187a066;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c184(struct user_regs_struct *regs)
{
if (regs->rax != 0x10b8)
{
multi_tracer_c210(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xbaab8c0b5a333150;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xe0a39db0bf428674;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c210(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d2)
{
multi_tracer_c218(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf9dd3eb0997ddeb5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xa9a9774d353cacc1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c218(struct user_regs_struct *regs)
{
if (regs->rax != 0x10da)
{
multi_tracer_c29(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc37e592b85b0b7b7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c29(struct user_regs_struct *regs)
{
if (regs->rax != 0x101d)
{
multi_tracer_c142(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x24a01b07cce61f38;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xe301c838c25a08f8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c142(struct user_regs_struct *regs)
{
if (regs->rax != 0x108e)
{
multi_tracer_c152(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf5c77add0b36f06d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c152(struct user_regs_struct *regs)
{
if (regs->rax != 0x1098)
{
multi_tracer_c134(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x934333c44fe5530e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c134(struct user_regs_struct *regs)
{
if (regs->rax != 0x1086)
{
multi_tracer_c59(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9a29fe6279e0ffe0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x7452caa884b76c6d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c59(struct user_regs_struct *regs)
{
if (regs->rax != 0x103b)
{
multi_tracer_c228(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xb75f81740f81eb9b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xf8b6bf5e7ff0feed;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c228(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e4)
{
multi_tracer_c58(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x817c3d31b6616b5d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c58(struct user_regs_struct *regs)
{
if (regs->rax != 0x103a)
{
multi_tracer_c243(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xdc767fb69d8581a9;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c243(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f3)
{
multi_tracer_c68(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x74d24edab90037cd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x2dfe883d2c641880;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c68(struct user_regs_struct *regs)
{
if (regs->rax != 0x1044)
{
multi_tracer_c107(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x1e8be5cfd7f3b3ba;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x40a82828bf1bc9d5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c107(struct user_regs_struct *regs)
{
if (regs->rax != 0x106b)
{
multi_tracer_c111(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc2d556f73d4b1db2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c111(struct user_regs_struct *regs)
{
if (regs->rax != 0x106f)
{
multi_tracer_c120(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xe76404c630852e65;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c120(struct user_regs_struct *regs)
{
if (regs->rax != 0x1078)
{
multi_tracer_c227(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9620ed414c72f292;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c227(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e3)
{
multi_tracer_c48(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xefb6e3a991f8b956;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xc6c2b0a17e5357ad;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c48(struct user_regs_struct *regs)
{
if (regs->rax != 0x1030)
{
multi_tracer_c190(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x36543b9619dff5ea;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x3cf3f925c5b5bd1b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c190(struct user_regs_struct *regs)
{
if (regs->rax != 0x10be)
{
multi_tracer_c226(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x788959e77d1f1478;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c226(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e2)
{
multi_tracer_c217(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x70fe074748ed93ae;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c217(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d9)
{
multi_tracer_c100(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x808e424ece485492;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c100(struct user_regs_struct *regs)
{
if (regs->rax != 0x1064)
{
multi_tracer_c194(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x89730a88a4572308;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c194(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c2)
{
multi_tracer_c174(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x87a64ca884244ceb;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x4ab3a116e5551fdd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c174(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ae)
{
multi_tracer_c126(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3eab41412a9d2d2d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c126(struct user_regs_struct *regs)
{
if (regs->rax != 0x107e)
{
multi_tracer_c38(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3a72c08a5e17bd92;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c38(struct user_regs_struct *regs)
{
if (regs->rax != 0x1026)
{
multi_tracer_c155(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xcdd4fca44efb80fd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c155(struct user_regs_struct *regs)
{
if (regs->rax != 0x109b)
{
multi_tracer_c94(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x5cb984417bdc3367;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x98578c31751cabec;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c94(struct user_regs_struct *regs)
{
if (regs->rax != 0x105e)
{
multi_tracer_c198(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x5df73d5533adae83;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c198(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c6)
{
multi_tracer_c163(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x6cf7e17ebda7fbe8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xa3e3601c5b7c09fd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c163(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a3)
{
multi_tracer_c53(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x70a09eacb0256da3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x580525056c8af9fe;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c53(struct user_regs_struct *regs)
{
if (regs->rax != 0x1035)
{
multi_tracer_c11(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xb649d0b7cdd1d475;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c11(struct user_regs_struct *regs)
{
if (regs->rax != 0x100b)
{
multi_tracer_c250(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x623110856457d824;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xdecaa3236c91d65f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c250(struct user_regs_struct *regs)
{
if (regs->rax != 0x10fa)
{
multi_tracer_c255(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7d70898fafa58196;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c255(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ff)
{
multi_tracer_c17(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xa883d8820cf10b7f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c17(struct user_regs_struct *regs)
{
if (regs->rax != 0x1011)
{
multi_tracer_c214(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x8232df8339b248f5;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x511fb5e24bd4ec17;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c214(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d6)
{
multi_tracer_c41(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xa892187d9c1ebef7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xce946dd954f0459a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c41(struct user_regs_struct *regs)
{
if (regs->rax != 0x1029)
{
multi_tracer_c211(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xcb338bb305a50b06;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x646d26a71ac7d680;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c211(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d3)
{
multi_tracer_c96(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x5113d7bcd22ce064;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c96(struct user_regs_struct *regs)
{
if (regs->rax != 0x1060)
{
multi_tracer_c207(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x268b029bb21e164e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c207(struct user_regs_struct *regs)
{
if (regs->rax != 0x10cf)
{
multi_tracer_c168(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xaee46233704ac431;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c168(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a8)
{
multi_tracer_c101(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xba1c3e3fa2224f27;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c101(struct user_regs_struct *regs)
{
if (regs->rax != 0x1065)
{
multi_tracer_c56(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9a3baf8bf47b6d46;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xa6245939009974dd;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c56(struct user_regs_struct *regs)
{
if (regs->rax != 0x1038)
{
multi_tracer_c139(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x3dd506010707ec2d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c139(struct user_regs_struct *regs)
{
if (regs->rax != 0x108b)
{
multi_tracer_c122(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xf9bc349710b893e2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x735d02721051674c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c122(struct user_regs_struct *regs)
{
if (regs->rax != 0x107a)
{
multi_tracer_c25(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x680d1624087716ac;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xace4ef8ee518c673;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c25(struct user_regs_struct *regs)
{
if (regs->rax != 0x1019)
{
multi_tracer_c7(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x35091fc6c0b40994;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c7(struct user_regs_struct *regs)
{
if (regs->rax != 0x1007)
{
multi_tracer_c43(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x1f6595fbf7aacfa0;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c43(struct user_regs_struct *regs)
{
if (regs->rax != 0x102b)
{
multi_tracer_c131(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf248de9bbca8f2da;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c131(struct user_regs_struct *regs)
{
if (regs->rax != 0x1083)
{
multi_tracer_c93(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xab85e374fd86b60a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x7c7e0c1380ec9e36;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c93(struct user_regs_struct *regs)
{
if (regs->rax != 0x105d)
{
multi_tracer_c244(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x9601a83fe1e70341;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xcc3e3688f6d2181e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c244(struct user_regs_struct *regs)
{
if (regs->rax != 0x10f4)
{
multi_tracer_c36(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x9009fdce6efe3e04;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x95b678f36a7a26b2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c36(struct user_regs_struct *regs)
{
if (regs->rax != 0x1024)
{
multi_tracer_c201(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x646bf723d4b2125c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c201(struct user_regs_struct *regs)
{
if (regs->rax != 0x10c9)
{
multi_tracer_c18(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xd6a07bd3398014bb;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x6c4a05b4c74be7c2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c18(struct user_regs_struct *regs)
{
if (regs->rax != 0x1012)
{
multi_tracer_c78(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x4de1bc593d6dade3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c78(struct user_regs_struct *regs)
{
if (regs->rax != 0x104e)
{
multi_tracer_c203(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x3a49f5ce16bda97b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c203(struct user_regs_struct *regs)
{
if (regs->rax != 0x10cb)
{
multi_tracer_c95(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xa57e8cd0733875a8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c95(struct user_regs_struct *regs)
{
if (regs->rax != 0x105f)
{
multi_tracer_c169(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xcf898d1d6bf32da7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xbe8150553267b936;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c169(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a9)
{
multi_tracer_c26(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xebf8e159b30b6484;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c26(struct user_regs_struct *regs)
{
if (regs->rax != 0x101a)
{
multi_tracer_c213(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x56fc8944bfa433d7;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c213(struct user_regs_struct *regs)
{
if (regs->rax != 0x10d5)
{
multi_tracer_c119(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x93ab6179edca483a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xd7c444290b875f07;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c119(struct user_regs_struct *regs)
{
if (regs->rax != 0x1077)
{
multi_tracer_c2(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xfad7e7e6ca131dc6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x7c8d0e976588a073;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c2(struct user_regs_struct *regs)
{
if (regs->rax != 0x1002)
{
multi_tracer_c14(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xa78fa40944bb3194;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c14(struct user_regs_struct *regs)
{
if (regs->rax != 0x100e)
{
multi_tracer_c16(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xfe94b46c4a906970;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x2d0b55de83a71c52;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c16(struct user_regs_struct *regs)
{
if (regs->rax != 0x1010)
{
multi_tracer_c63(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xe8f631ab3b27cd53;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x3bed09ced801b4a8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c63(struct user_regs_struct *regs)
{
if (regs->rax != 0x103f)
{
multi_tracer_c162(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x64e474976ec8c72d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xa8efba93d055041f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c162(struct user_regs_struct *regs)
{
if (regs->rax != 0x10a2)
{
multi_tracer_c89(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xcd7ea1265cbe4eca;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0xcfb3d3e99e641a86;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c89(struct user_regs_struct *regs)
{
if (regs->rax != 0x1059)
{
multi_tracer_c236(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xaa55fc9c44895aa6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x93f401639f0b4e28;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c236(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ec)
{
multi_tracer_c138(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xea56f2a26a05cc76;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xe1969ee81e0c1481;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c138(struct user_regs_struct *regs)
{
if (regs->rax != 0x108a)
{
multi_tracer_c147(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xe135ac779e4baf18;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x790b9c35af2cb7f8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c147(struct user_regs_struct *regs)
{
if (regs->rax != 0x1093)
{
multi_tracer_c124(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xc86188859710bc48;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x3d66feae6c44d227;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c124(struct user_regs_struct *regs)
{
if (regs->rax != 0x107c)
{
multi_tracer_c66(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x24f49d004b8d466c;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x78d8aaac7dae4d57;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c66(struct user_regs_struct *regs)
{
if (regs->rax != 0x1042)
{
multi_tracer_c223(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xf14d2ea95563defe;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c223(struct user_regs_struct *regs)
{
if (regs->rax != 0x10df)
{
multi_tracer_c238(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xed9f52868a834db8;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c238(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ee)
{
multi_tracer_c219(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x9140bf959c42680b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c219(struct user_regs_struct *regs)
{
if (regs->rax != 0x10db)
{
multi_tracer_c145(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x614f18b0f2c70e7f;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xe35c9359f29eabc1;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c145(struct user_regs_struct *regs)
{
if (regs->rax != 0x1091)
{
multi_tracer_c159(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xc22eac4b7e4db24e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code += 0x1c01e4fffea685f6;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c159(struct user_regs_struct *regs)
{
if (regs->rax != 0x109f)
{
multi_tracer_c239(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x7233a0c227ba9c15;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x3a16eca61c75870d;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c239(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ef)
{
multi_tracer_c234(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x67a0700263aee656;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c234(struct user_regs_struct *regs)
{
if (regs->rax != 0x10ea)
{
multi_tracer_c135(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x78179d249666bfc2;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c135(struct user_regs_struct *regs)
{
if (regs->rax != 0x1087)
{
multi_tracer_c224(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0x46269a4c5bdb7c31;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x29057d96c5b6ef7b;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c224(struct user_regs_struct *regs)
{
if (regs->rax != 0x10e0)
{
multi_tracer_c235(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code ^= 0xd3954f4c6375f78e;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c235(struct user_regs_struct *regs)
{
if (regs->rax != 0x10eb)
{
multi_tracer_c50(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x55360ff11cabc0be;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x65d86d585e736136;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c50(struct user_regs_struct *regs)
{
if (regs->rax != 0x1032)
{
multi_tracer_c254(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xaa75e0b3de679c59;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0xd7fd57b39a3c3128;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer_c254(struct user_regs_struct *regs)
{
if (regs->rax != 0x10fe)
{
multi_tracer_c143(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0x61c6965982ce9dae;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);

}

void multi_tracer_c143(struct user_regs_struct *regs)
{
if (regs->rax != 0x108f)
{
multi_tracer_p224(regs);return;
}
uint64_t code;
code = 0x9090909090909090;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip, NULL);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+8, NULL);
code += 0xebde6d63d5d7add3;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+8, code);
code = ptrace(PTRACE_PEEKTEXT, CHILD_PID, regs->rip+16, NULL);
code ^= 0x1d6ff019ae58a23a;
ptrace(PTRACE_POKETEXT, CHILD_PID, regs->rip+16, code);

}

void multi_tracer(struct user_regs_struct *regs)
{ multi_tracer_c186(regs); }


void tracer()
{
    struct user_regs_struct regs;
    int wait_status;
    wait(&wait_status);
    int already_init_md5 = 0;

    while (WIFSTOPPED(wait_status))
    {
        if (WIFEXITED(wait_status)) exit(0);
        // puts("UD2 reached by child");
        ptrace(PTRACE_GETREGS, CHILD_PID, NULL, &regs);
        uint64_t rax = regs.rax;
        // printf("rax: %d\n", rax);

        switch(rax)
        {
        // do srand
        case 1:
            SEED = time(NULL);
            regs.rax = SEED;
            break;
        case 2:
            usleep(DELAY);
            break;
        case 3:
            point food;
            food.x = regs.rbx;
            food.y = regs.rcx;

            if (!already_init_md5)
            {
                init_md5();
                update_hash_with_seed();
                already_init_md5 = 1;
            }

            update_hash_with_food(food);
            break;

        case 4:
            int score = regs.rbx;
            int maxX = regs.rcx;
            int maxY = regs.rdx;
            if (score == 0)
            {
                puts("At least try to get 1 point?...");
            }
            else
            {
                finalize_hash(regs.rip);
                submit_score(score, maxX, maxY);
            }
            break;

        default:
            if (regs.rax >= 0x1000 && regs.rax < 0x3000)
                multi_tracer(&regs);
            break;
        }

        skip_ud2_and_continue(&regs);
        wait(&wait_status);
    }
}

int main()
{
    pid_t pid = fork();

    if (pid < 0)
        return 1;

    // child process
    if (pid == 0)
    {
        if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
            return 1;
        game();
    }

    // parent process
    else
    {
        CHILD_PID = pid;
        signal(SIGINT, sig_handler);
        tracer();
    }

    return 0;
}
