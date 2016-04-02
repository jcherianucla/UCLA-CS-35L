/*
 * Name: Jahan Kuruvilla Cherian
 * UID: 104436427
 * Email: jcherian@ucla.edu
 * Professor: Eggert
 * TA: Lauren Samy
 * File - sfrobu.c - Program to take in frobnicated words and sort them
 * in unfrobnicated order without ever storing the unfrobnicated values
 * in memory. The method is different for regular file where we dont require
 * any realloc, but for irregular files, we use realloc.
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

int comparison = 0;
//Function to compare unfrobnicated words
int frobcmp(char const *a, char const *b)
{
  for(;; a++, b++) //Loop to look at entire words
  {
	if(*a == ' ' && *b == ' ') { return 0; }
	else if (*a == ' ' || ((*a^42) < (*b^42))) { return -1; }
	else if (*b == ' ' || ((*a^42) > (*b^42))) { return 1; }
  }
}

//Function to use within qsort without having to cast
int cmp(const void* in1, const void* in2)
{
  //We cast to pointers to pointers since thats what our
  //words array holds
  const char* a = *(const char**)in1;
  const char* b = *(const char**)in2;
  comparison++;
  return frobcmp(a,b);
}

//Function to check for reading error
void readErr(ssize_t state)
{
  if(state < 0)
  {
	fprintf(stderr, "Error while reading file!");
	exit(1);
  }
}


int main(void)
{
  struct stat fileStat;
  if(fstat(0,&fileStat) < 0) //Error with fstat
  {
	fprintf(stderr, "Error with fstat!"); //Change error to something better
	exit(1);
  }
  char **words;
  char *fileArray;
  size_t wordsIterator = 0;
  if(S_ISREG(fileStat.st_mode))
  {
	fileArray = (char*)malloc(fileStat.st_size * sizeof(char));
	int pointerCounter = 0;
	ssize_t state = read(0, fileArray, fileStat.st_size);
	int count = 1;
	if(state > 0) //No error reading
	{
	  for(size_t i = 0; i < fileStat.st_size; i+=count)
	  {
		count = 1;
		if(i == fileStat.st_size - 1) //Always set the end to a space
		{
		  fileArray[i] = ' ';
		}
		if(fileArray[i] == ' ')
		{
		  for(size_t j = i; fileArray[j] != ' '; j++)
		  {
			count++;
		  }
		  pointerCounter++;
		}
	  }
	}
	words = (char**)malloc(pointerCounter * sizeof(char*));
	int flag = 0;
	for(size_t i = 0; i < fileStat.st_size; i++)
	{
	  if(flag == 0 && fileArray[i] != ' ')
	  {
		words[wordsIterator] = &fileArray[i];
		wordsIterator++;
		flag = 1;
	  }
	  else if(flag == 1 && fileArray[i] == ' ')
	  {
		flag = 0;
	  }
	}
  }
  else
  {
	words = (char**)malloc(sizeof(char*));
  }

  char* word; //Holds one word at a time (delimited by spaces)
  word = (char*)malloc(sizeof(char));
  //curr and next act as current and next iterators to use for noting
  //EOF and auto adding spaces at the end of files
  char curr[1];
  ssize_t currState = read(0, curr, 1);
  readErr(currState);
  char next[1];
  ssize_t nextState = read(0, next, 1);
  readErr(nextState);
  int letterIterator = 0;
  while(currState > 0)
  {
	word[letterIterator] = curr[0]; //Add letters to the word
	//Constantly reallocate space for growing words
	char* temp = realloc(word, (letterIterator+2)*sizeof(char));
	if(temp != NULL)
	{
	  //Make the word equal to the reallocated space
	  word = temp;
	}
	else //Allocation error, print error and exit
	{
	  free(word);
	  fprintf(stderr, "Error Allocation Memory!");
	  exit(1);
	}

	if(curr[0] == ' ') //Hit the end of the word
	{
	  words[wordsIterator] = word; //Add word to words list
	  //Constantly reallocate space for growing wordslist
	  char** anotherOne = realloc(words, (wordsIterator+2)*sizeof(char*));
	  if(anotherOne != NULL)
	  {
		//Make words equal to reallocated space
		words = anotherOne;
		wordsIterator++;
		//Set word back to empty by pointing it to other space
		word = NULL;
		word = (char*)malloc(sizeof(char));
		letterIterator = -1; //-1 to bring back to 0 when summed later
	  }
	  else //Allocation error, print error and exit
	  {
		free(words);
		fprintf(stderr, "Error Allocation Memory!");
		exit(1);
	  }
	}
	if(nextState == 0 && curr[0] == ' ')
	{
	  break;
	}
	else if (curr[0] == ' ' && next[0] == ' ') //Ignore Extra Spaces
	{
	  while(curr[0] == ' ')
	  {
		currState = read(0,curr,1);
		readErr(currState);
	  }
	  nextState = read(0,next,1);
	  readErr(nextState);
	  letterIterator++;
	  continue;
	}
	else if(nextState == 0) //Add a space at the end if there isn't already one
	{
	  curr[0] = ' ';
	  letterIterator++;
	  continue;
	}
	//increment our letter counter and get the next character
	curr[0] = next[0];
	nextState = read(0, next, 1);
	readErr(nextState);
	letterIterator++;
  }
  //Sort the frobnicated words from our words list
  qsort(words, wordsIterator, sizeof(char*), cmp);
  //Output the words to STDOUT using write
  for(size_t i = 0; i < wordsIterator; i++)
  {
	long wordSize = 0;
	for(size_t j = 0; ;j++)
	{
	  wordSize++;
	  if(words[i][j] == ' ')
	  {
		break;
	  }
	}
	if(write(1,words[i], wordSize) == 0)
	{
	  fprintf(stderr, "Error while writing!");
	  exit(1);
	}
  }
  fprintf(stderr, "Comparisons: %i\n", comparison);
  free(words);
  exit(0);
}
