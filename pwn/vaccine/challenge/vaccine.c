#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
	FILE* f = fopen("RNA.txt","r");
	char rna[100];
	char input[100];
	fgets(rna,100,f);

	printf("Give me vaccine: ");
	fflush(stdout);
	scanf("%s",input);

	for (int i = 0; i < strlen(input); ++i)
	{
		if(input[i] != 'A' && input[i] != 'C' && input[i] != 'G' && input[i] != 'T'){
			puts("Only DNA codes allowed!");
			exit(0);
		}
	}

	if(strcmp(rna,input) == 0){
		puts("Congrats! You give the correct vaccine!");
		FILE* f = fopen("secret.txt","r");
		char flag[100];
		fgets(flag,100,f);
		printf("Here is your reward: %s\n",flag);
	}else{
		puts("Oops.. Try again later");
		exit(0);
	}
	return 0;
}