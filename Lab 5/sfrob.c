/*
 * Name: Jahan Kuruvilla Cherian
 * UID: 104436427
 * Email: jcherian@ucla.edu
 * Professor: Eggert
 * TA: Lauren Samy
 * File - sfrob.c - Program to take in frobnicated words and sort them
 * in unfrobnicated order without ever storing the unfrobnicated values
 * in memory.
 */

#include<stdio.h>
#include<stdlib.h>

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
  return frobcmp(a,b);
}

//Function to check for reading error
void readErr()
{
	if(ferror(stdin))
	{
	  fprintf(stderr, "Error while reading file!");
	  exit(1);
	}
}


int main(void)
{
  char* word; //Holds one word at a time (delimited by spaces)
  char** words; //Array to hold pointers to words
  word = (char*)malloc(sizeof(char));
  words = (char**)malloc(sizeof(char*));
  //curr and next act as current and next iterators to use for noting
  //EOF and auto adding spaces at the end of files
  char curr = getchar(); 
  readErr();
  char next = getchar();
  readErr();
  int letterIterator = 0;
  int wordsIterator = 0;
  while(curr != EOF && !ferror(stdin)) //Read file until EOF
  {

  	word[letterIterator] = curr; //Add letters to the word
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


	if(curr == ' ') //Hit the end of the word
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
	if(next == EOF && curr == ' ')
	{
	  break;
	}
	else if (curr == ' ' && next == ' ') //Ignore Extra Spaces
	{
	  while(curr == ' ')
	  {
	  	curr = getchar();
	  	readErr();
	  }
	  next = getchar();
	  readErr();
	  letterIterator++;
	  continue;
	}
	else if(next == EOF) //Add a space at the end if there isn't already one
	{
	  curr = ' ';
	  letterIterator++;
	  continue;
	}
	 //increment our letter counter and get the next character
	curr = next;
	next = getchar();
	readErr();
	letterIterator++;
	
  }


  //Sort the frobnicated words from our words list
  qsort(words, wordsIterator, sizeof(char*), cmp);

  //Output the words to STDOUT using putchar
  for(size_t i = 0; i < wordsIterator; i++)
  {
  	for(size_t j = 0; ;j++)
	{
		//EOF error checking
		if(putchar(words[i][j]) == EOF)
		{
		  fprintf(stderr, "Error while writing character!");
		  exit(1);
		}
		if(words[i][j] == ' ')
		{
		  break;
		}
	}
  }
  //De-allocate all the space taken up for the words
  for(size_t i = 0; i < wordsIterator; i++)
  {
  	free(words[i]);
  }
  free(words);

  exit(0);
}
