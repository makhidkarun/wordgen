#include <stdio.h>
#include <string.h>

#define TRUE	1
#define FALSE	0

main(argc, argv)
int argc;
char *argv[];
{
  FILE *fd_in, *fd_out;
  char string[81], *ch;
  int i, j, len, flag;

  if (argc != 3) {
    fprintf(stderr, "Usage: padit <infile> <outfile>\n");
    exit(-1);
   }

  fd_in = fopen(argv[1], "r");
  if (fd_in == NULL) {
    fprintf(stderr, "Cannot open '%s' for input\n", argv[1]);
    exit(-1);
   }

  flag = TRUE;
  len = 0;
  while (flag) {
    if ((ch = fgets(string, 81, fd_in)) != NULL)
      flag = FALSE;
    else {
      i = strlen(string);
      if (i > len) len = i;
     }
   }
  fclose(fd_in);
  fprintf(stdout, "Max. word length = %d\n", len);

  fd_in = fopen(argv[1], "r");
  if (fd_in == NULL) {
    fprintf(stderr, "Cannot open '%s' for input\n", argv[1]);
    exit(-1);
   }
  fd_out = fopen(argv[2], "w");
  if (fd_out == NULL) {
    fprintf(stderr, "Cannot open '%s' for output\n", argv[2]);
    exit(-1);
   }

  flag = TRUE;
  while (flag) {
    if ((ch = fgets(string, 81, fd_in)) != NULL)
      flag = FALSE;
    else {
      i = strlen(string);
      fprintf(fd_out,"%s", string);
      for (j=i; j<len; j++)
        fprintf(fd_out, " ");
      fprintf(fd_out, "|\n");
     }
   }
  fclose(fd_in);
  fclose(fd_out);
}
