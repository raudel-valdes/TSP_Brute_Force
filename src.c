#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

void createCityArray(FILE *fptr);
void printFileContent(FILE *fptr);

int main() {
  FILE *fptr;
  char filePath[25];

  printf("Please provide a file: \n");
  scanf("%s", filePath);

  fptr = fopen(filePath, "r");

  if (fptr != NULL) {
    //printFileContent(fptr);
    createCityArray(fptr);



  } else { 
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fclose(fptr);

  return 0;
}

void printFileContent(FILE * fptr) {
  char *c;
  char stringLine[1000];

  c = fgets(stringLine, sizeof(stringLine), fptr);
  while (c != NULL) 
  { 
    printf ("%s", c); 
    c = fgets(stringLine, sizeof(stringLine), fptr);
  }
}

void createCityArray(FILE *fptr) {
  int *cityArr;
  char *nextLine;
  char stringLine[1000];
  char str1[100];
  int citySize = 0;
  int lineNumb = 1;
  double xCord = 0;
  double yCord = 0;
  FILE *tmpFptr;

  rewind(fptr);

  nextLine = fgets(stringLine, sizeof(stringLine), fptr);


  while (nextLine != NULL) 
  {
    if ((strstr(nextLine, "DIMENSION: ")) != NULL) {
      printf("A match found on line: \n");
      tmpFptr = fopen ("temp.txt", "w+");
      fputs(stringLine, tmpFptr);
      rewind(tmpFptr);
      fscanf(tmpFptr, "%s %d", str1, &citySize);

      cityArr = (int *)malloc(citySize * sizeof(int));
    }

    // if (lineNumb >= 8) {
      // tmpFptr = fopen ("temp.txt", "w+");
      // fputs(stringLine, tmpFptr);

      rewind(tmpFptr);
      fscanf(tmpFptr, "%*d %lf %lf", &xCord, &yCord);
    // }  

    // printf ("%s", c); 
    nextLine = fgets(stringLine, sizeof(stringLine), fptr);
    lineNumb++;
    printf("Here is the demension: %d, %lf, %lf \n", citySize, xCord, yCord);
  }
}