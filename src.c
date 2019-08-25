#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

double * createCityArray(FILE *);
double createDistMatrix(int *);
void printFileContent(FILE *);
void printArrayContent(double *);

int citySize = 0;

int main() {
  FILE *fptr;
  char filePath[25];
  double *cityArr;

  printf("Please provide a file: \n");
  scanf("%s", filePath);

  fptr = fopen(filePath, "r");

  if (fptr != NULL) {
    //printFileContent(fptr);
    cityArr = createCityArray(fptr);
    printArrayContent(cityArr);

  } else { 
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fclose(fptr);
  free(cityArr);

  return 0;
}

double * createCityArray(FILE *fptr) {
  double *cityArr = 0;
  char *nextLine;
  char stringLine[1000];
  char str1[100];
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

  return cityArr;
}

double createDistMatrix(int *cityArr) {
  return 0.0;
}

void printFileContent(FILE *fptr) {
  char *c;
  char stringLine[1000];

  c = fgets(stringLine, sizeof(stringLine), fptr);
  while (c != NULL) 
  { 
    printf ("%s", c); 
    c = fgets(stringLine, sizeof(stringLine), fptr);
  }
}

void printArrayContent(double *cityArr) {
  int cityArrSize = citySize * 2;
  int j = 1;

  printf("These are the city cordinates in the array: \n");

  for(int i = 0; i < cityArrSize; i++) {
    if (i % 2 == 0 && i != 0) {
      printf("City %d: X = %lf Y = %lf \n", j, cityArr[i-2], cityArr[i-1]);
      j++;
    }

    if (i == (cityArrSize - 1)) {
      printf("City %d: X = %lf Y = %lf \n", j, cityArr[i-1], cityArr[i]);
    }
  }
}