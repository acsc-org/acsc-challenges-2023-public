#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int getnline(char *buf, int size);
static int getint(void);
static void edit(void);

struct Memo {
	size_t size;
	char* buf;
} mlist[10];

__attribute__((constructor))
static int init(){
	alarm(30);
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	return 0;
}

int main(void){
	for(;;){
		printf("\nMENU\n"
				"1. Edit\n"
				"2. List\n"
				"0. Exit\n"
				"> ");

		switch(getint()){
			case 0:
				goto end;
			case 1:
				edit();
				break;
			case 2:
				for(int i=0; i<sizeof(mlist)/sizeof(struct Memo); i++)
					if(mlist[i].size > 0 && mlist[i].buf)
						printf("[%d] %.*s\n", i, (int)mlist[i].size, mlist[i].buf);
				break;
		}
	}

end:
	puts("Bye.");
	return 0;
}

static void edit(void){
	unsigned idx, size;

	printf("Index: ");
	if((idx = getint()) >= sizeof(mlist)/sizeof(struct Memo)){
		puts("Out of list");
		return;
	}

	printf("Size: ");
	if((size = getint()) > 0x78){
		puts("Too big memo");
		return;
	}

	char *p = realloc(mlist[idx].buf, size);
	if(size > mlist[idx].size)
		mlist[idx].buf = p;
	mlist[idx].size = size;

	printf("Memo: ");
	getnline(mlist[idx].buf, size);

	puts("Done");
}

static int getnline(char *buf, int size){
	int len;

	if(size <= 0 || (len = read(STDIN_FILENO, buf, size-1)) <= 0)
		return -1;

	if(buf[len-1]=='\n')
		len--;
	buf[len] = '\0';

	return len;
}

static int getint(void){
	char buf[0x10] = {};

	getnline(buf, sizeof(buf));
	return atoi(buf);
}

