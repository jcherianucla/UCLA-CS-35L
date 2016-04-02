#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
  //Check for correct number of args
  if(argc != 3)
  {
  	fprintf(stderr, "Incorrect number of arguments! Please input 2 arguments.");
  	exit(1);
  }
  char* from = argv[1];
  char* to = argv[2];
  //Make sure they are of the same length
  if(strlen(from) != strlen(to))
  {
  	fprintf(stderr, "Input from and to must be the same size!");
  	exit(1);
  }

  //Make sure FROM does not contain any duplicate bytes
  for(size_t i = 0; i < strlen(from); i++)
  {
  	for(size_t j = i+1; j < strlen(from); j++)
	{
	  if(from[j] == from[i])
	  {
	  	fprintf(stderr, "From should not contain duplicate bytes!");
	  	exit(1);
	  }
	}
  }

  char curr = getchar();
  int matchInFrom = 0;
  while(curr != EOF)
  {
  	for(size_t i = 0; i < strlen(from); i++)
	{
	  if(curr == from[i])
	  {
	  	putchar(to[i]);
	  	matchInFrom = 1;
	  	break;
	  }
	  else
	  {
	  	matchInFrom = 0;
	  }
	}
	if(!matchInFrom)
	{
	  putchar(curr);
	}
	curr = getchar();
  }

  return 0;
}
