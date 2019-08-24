#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

int createCityArray(FILE *fptr);
void printFileContent(FILE *fptr);

int main() {
  FILE *fptr;
  char filePath[25];
  int *cityArr;

  printf("Please provide a file: \n");
  scanf("%s", filePath);

  fptr = fopen(filePath, "r");

  if (fptr != NULL) {
    //printFileContent(fptr);
    cityArr = createCityArray(fptr);



  } else { 
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fclose(fptr);
  free(cityArr);

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

int createCityArray(FILE *fptr) {
  double *cityArr = 0;
  char *nextLine;
  char stringLine[1000];
  char str1[100];
  int citySize = 0;
  int lineNumb = 1;
  int cityNumb = 0;
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

      cityArr = (double *)malloc((2 * citySize) * sizeof(double));
    }

    if (lineNumb >= 8) {

      tmpFptr = fopen ("temp.txt", "w+");
      fputs(stringLine, tmpFptr);

      rewind(tmpFptr);
      fscanf(tmpFptr, "%*d %lf %lf", &xCord, &yCord);

      cityArr[cityNumb] = xCord;
      cityNumb++;
      cityArr[cityNumb] = yCord;
      cityNumb++;
    }  
    nextLine = fgets(stringLine, sizeof(stringLine), fptr);
    lineNumb++;
  }

  for(int i = 0; i < citySize * 2; i++) {
    printf("These are the values in the array: %lf \n", cityArr[i]);
  }

  return *cityArr;
}