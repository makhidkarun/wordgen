/* word.c - traveller word generation */

#include <stdio.h>
/* 2 levels of debug for following data input */
#define DEBUG 0
#define DEBUG2 0
/* Trace will allow user to follow which charts and die rolls were used */
#define TRACE 0
/* Define seperate "letters" with a . and syllables with a - */
#define DEFINE 0

char basic[6][6][5];
char alternate[6][6][5];

char ic[6][6][6][5];
char v[6][6][6][5];
char fc[6][6][6][5];

char next_table = 'B';
#define d6 	random(6)

main(argc,argv)
int argc;
char *argv[];
{
	int i;

	if (argc != 4) {
	  fprintf(stderr,
		"Usage: word [word_file] [num_words] [num_syllables]\n");
	  exit(-1); }
	read_table(argv[1]);
#if (DEBUG2)
	print_tables();
#endif
	for (i=0; i<atoi(argv[2]); i++) {
		do_word(random(atoi(argv[3])));
		printf("\n"); }
}

do_word(syl)
int syl;
{
	int i;

	if (syl == 0) syl = d6;

	for (i=0; i<syl; i++)
		if (next_table == 'B')
			do_syl(1);
		else
			do_syl(0);
	next_table = 'B';	/* set up for next word */
	printf(" ");
}

do_syl(isbasic)
int isbasic;
{
	char pattern[3];
	int i,j;

	i = d6-1; 
	j = d6-1;
	if (isbasic) {
#if (TRACE)
	printf("(Basic%d%d)",i+1,j+1);
#endif
		next_table = 'B';
		pattern[0] = basic[i][j][0];
		if ((pattern[1] = basic[i][j][1]) == '*')
			next_table = 'A';
		if ((pattern[2] = basic[i][j][2]) == '*')
			next_table = 'A';
		if (basic[i][j][3] == '*')
			next_table = 'A';
	} else {
#if (TRACE)
	printf("(Alter%d%d)",i+1,j+1);
#endif
		next_table = 'B';
		pattern[0] = alternate[i][j][0];
		if ((pattern[1] = alternate[i][j][1]) == '*')
			next_table = 'A';
		if ((pattern[2] = alternate[i][j][2]) == '*')
			next_table = 'A';
		if (alternate[i][j][3] == '*')
			next_table = 'A';
	}
	for (i=0; i<3; i++)
		do_element(pattern[i],i);
#if (DEFINE)
	printf("-");
#endif
}

do_element(ch,count)
char ch;
int count;
{
	if (ch == 'V')
		do_component(2); /* vowel */
	else if (ch == 'C')
		if (count == 0)
			do_component(1); /* initial consonant */
		else
			do_component(3); /* final consonant */

}

do_component(which)
int which;
{
	char output[5];
	int i,j,k,l;

	i = d6-1;
	j = d6-1;
	k = d6-1;

	switch (which) {
		case 1: /* initial consonant */
#if (TRACE)
	printf("(C%d%d%d)",i+1,j+1,k+1);
#endif
			for (l=0;l<4; l++)
				output[l] = ic[i][j][k][l];
			break;
		case 2: /* vowel */
#if (TRACE)
	printf("(V%d%d%d)",i+1,j+1,k+1);
#endif
			for (l=0;l<4; l++)
				output[l] = v[i][j][k][l];
			break;
		case 3: /* final consonant */
#if (TRACE)
	printf("(K%d%d%d)",i+1,j+1,k+1);
#endif
			for (l=0;l<4; l++)
				output[l] = fc[i][j][k][l];
			break;
	}
	for (i=4; i>0; i--)
		if (output[i] != ' ') {
			output[i+1] = NULL;
			break;
		}
	printf("%s",output);
#if (DEFINE)
	printf(".");
#endif
}

read_table(f_name)
char *f_name;
{
	FILE *inp;
	char line[90];
	char test[20];
	int i, j;

	inp = fopen(f_name,"r");
	if (inp == NULL) {printf("Couldn't open file %s\n",f_name); exit(0);}

	do {
		fgetline(line, 89, inp);
		sscanf(line,"%s",test);
	} while (strcmp(test,"Basic") != 0);
	fgetline(line,89,inp);  /* discard */
	for (i=0;i<6;i++) {
		fgetline(line,89,inp);
		get_syll_line(i,line,inp);
	}
	do {
		fgetline(line, 89, inp);
		sscanf(line,"%s",test);
	} while (strcmp(test,"Initial") != 0);
	fgetline(line,89,inp);  /* discard */
	for (i=0; i<6; i++)
		for (j=0; j<6; j++) {
			fgetline(line,89,inp);
			get_elem_line(i,j,line,inp);
		}
#if (DEBUG)
	printf("Finished reading in file\n");
#endif
}

get_syll_line(i,line,inp)
int i;
char line[];
FILE *inp;
{
	int test;

	sscanf(line,"%d",&test);
	if (test != (i+1))	
		printf("Error in line %d >%s<\n",i,line);
	sscanf(line,"%*d %s %s %s %s %s %s %s %s %s %s %s %s",
		basic[i][0], basic[i][1], basic[i][2], 
		basic[i][3], basic[i][4], basic[i][5],
		alternate[i][0], alternate[i][1], alternate[i][2], 
		alternate[i][3], alternate[i][4], alternate[i][5]
		);
#if (DEBUG)
	printf("line is %s\n",line);
	printf("basic[%d][0] = %s\n",i,basic[i][0]);
#endif
}

get_elem_line(i,j,line,inp)
int i,j;
char line[];
FILE *inp;
{
	int test;

	sscanf(line,"%d",&test);
#if (DEBUG2)
	printf("line %d,%d, test is %d against %d\n",i,j,test,((i+1)*10+j+1));
#endif
	if (test != (i+1)*10+j+1) {
#if (DEBUG1)
		printf("Skipping line >%s<\n", line);
#endif
		fgetline(line,89,inp);
	}
	sscanf(line,"%d",&test);
	if (test != (i+1)*10+j+1)	
		printf("Error in line %d,%d >%s<\n",i,j,line);
	sscanf(line,"%*d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
		ic[i][j][0], ic[i][j][1], ic[i][j][2], 
		ic[i][j][3], ic[i][j][4], ic[i][j][5], 
		v[i][j][0], v[i][j][1], v[i][j][2], 
		v[i][j][3], v[i][j][4], v[i][j][5], 
		fc[i][j][0], fc[i][j][1], fc[i][j][2], 
		fc[i][j][3], fc[i][j][4], fc[i][j][5] 
		);
}

print_tables()
{
	int i,j;

	printf("\tBasic\t\t\t\t\tAlternate\n");
	printf("  1    2    3    4    5    6\t\t1    2    3    4    5    6\n");
	for (i=0;i<6;i++)
		printf("%d %4s %4s %4s %4s %4s %4s\t\t%4s %4s %4s %4s %4s %4s\n",i+1,
			basic[i][0], basic[i][1], basic[i][2], 
			basic[i][3], basic[i][4], basic[i][5],
			alternate[i][0], alternate[i][1], alternate[i][2], 
			alternate[i][3], alternate[i][4], alternate[i][5]
			);
	printf("(Only four choices on table V, CV, VC, and CVC, append * to use alternate table)\n\n");
	printf("\tInitial Consonant\t\tVowel\t\tFinal Consonant\n");
	printf("   1   2   3   4   5   6  \t1   2   3   4   5   6   1   2   3   4   5   6\n");
	for (i=0; i<6; i++) {
		for (j=0;j<6;j++)
			printf("%0d%0d %4s %4s %4s %4s %4s %4s\t%4s %4s %4s %4s %4s %4s %4s %4s %4s %4s %4s %4s\n",i+1,j+1,
				ic[i][j][0], ic[i][j][1], ic[i][j][2], 
				ic[i][j][3], ic[i][j][4], ic[i][j][5], 
				v[i][j][0], v[i][j][1], v[i][j][2], 
				v[i][j][3], v[i][j][4], v[i][j][5], 
				fc[i][j][0], fc[i][j][1], fc[i][j][2], 
				fc[i][j][3], fc[i][j][4], fc[i][j][5] 
				);
		printf("\n");
	}

}
