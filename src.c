/*
Raudel Valdes
University of Louisville
CECS-525: Artificial Intelligence
Project #1
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
//Dont forget to free all of your mallocs!!!!

double * createCityArray(FILE *);
int calcCombinations();
int calcFactorial(int n);
struct path * calcPathDist(struct path *);
void swapCities(int *, int *);
void permuteCities(int *, struct path *, int);
double ** createDistMatrix(double *);
double calcDistance(int, int, double *);
void printFileContent(FILE *);
void printArrayContent(double *);
void printMatrixContent(double **);
void printPathStruct(struct path *);


int citySize = 0;
int permNumb = 0;

struct path {
  int *path;
  double totalDist;
  int pathNumb;
} path;

int main() {
  FILE *fptr;
  //char filePath[25];
  double *cityArr;
  double **distMatrix;
  struct path *salesmanInfo;
  int *cityList;

  // printf("Please provide a file: \n");
  //scanf("%s", filePath);

  fptr = fopen("Random4.tsp", "r");

  if (fptr != NULL) {
    //printFileContent(fptr);
    cityArr = createCityArray(fptr);
    //printArrayContent(cityArr);
    distMatrix = createDistMatrix(cityArr);
    //printMatrixContent(distMatrix);

  
    cityList = (int *)malloc(citySize * sizeof(int));
    salesmanInfo = (struct path *)malloc(calcFactorial(citySize) * sizeof(struct path));

    for (int i = 0; i <= calcFactorial(citySize); i++)
      salesmanInfo[i].path = (int *)malloc((citySize + 1) * sizeof(int));

    for (int i = 0; i <= citySize; i++)
      cityList[i] = i;

    permuteCities(cityList, salesmanInfo, 0);
    printPathStruct(salesmanInfo);

  } else { 
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fclose(fptr);
  free(salesmanInfo);
  free(cityList);

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

int calcCombinations() {
  int numerator = calcFactorial(citySize);
  int denominator = (calcFactorial(citySize) * calcFactorial(citySize - 2));
  double answer = numerator / denominator;

  return answer;
}

int calcFactorial(int n) {
  int answer = 1;
  for (int i = 2; i <= n; i++) answer *= i;
  return answer;
}

void swapCities(int *x, int *y) { 
  int temp; 
  temp = *x; 
  *x = *y; 
  *y = temp;
} 

void permuteCities(int *cityList, struct path * salesmanInfo, int c) { 
  int i = 0;
  int r = citySize - 1;

  if (c == r) {
    salesmanInfo[permNumb].pathNumb = permNumb;

    for (int j = 0; j < citySize; j++)
      salesmanInfo[permNumb].path[j] = cityList[j];

    salesmanInfo[permNumb].path[citySize] = salesmanInfo[permNumb].path[0];

    if (permNumb > 0) {
      for(int j =0; j < citySize; j++) {
      printf("%d -> ", salesmanInfo[permNumb].path[j]); 
      }
    }
    printf("\n");
    permNumb++;
  }
  else
  { 
    for (i = c; i <= r; i++) {     
      swapCities((cityList + c), (cityList + i)); 
      permuteCities(cityList, salesmanInfo,c + 1);
      swapCities((cityList + c),(cityList+i)); 
    } 
  } 

  return;
}

struct path  *calcPathDist(struct path *salesmanInfo) {

  return salesmanInfo;
}

double ** createDistMatrix(double *cityArr) {
  int rows = citySize;
  int columns = citySize;

  double **distMatrix = (double **)malloc(rows * sizeof(double *)); 
  for (int i = 0; i < rows; i++) 
    distMatrix[i] = (double *)malloc(columns * sizeof(double)); 

  // Note that distMatrix[i][j] is same as *(*(distMatrix+i)+j)  
  for (int i = 0; i <  rows; i++) 
    for (int j = 0; j < columns; j++) 
      distMatrix[i][j] = calcDistance(i,j, cityArr);

  return distMatrix;
}

double calcDistance(int cityOne, int cityTwo, double *cityArr) {
  double deltaX = 0;
  double deltaY = 0;

  if (cityOne == cityTwo) {
    return 0.0;
  }

  deltaX = cityArr[cityOne * 2] - cityArr[cityTwo * 2];
  deltaX *= deltaX;

  deltaY = cityArr[2 * cityOne + 1] - cityArr[2 * cityTwo + 1];
  deltaY *= deltaY;

  return sqrt(deltaX + deltaY);
}

void printFileContent(FILE *fptr) {
  char *c;
  char stringLine[1000];

  c = fgets(stringLine, sizeof(stringLine), fptr);
  while (c != NULL) 
  { 
    printf("%s", c); 
    c = fgets(stringLine, sizeof(stringLine), fptr);
  }
}

void printArrayContent(double *cityArr) {
  int cityArrSize = citySize * 2;
  int j = 1;

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

void printMatrixContent(double **distMatrix) {
  int rows = citySize;
  int columns = citySize;

  for (int i = 0; i < rows; i++) 
    for (int j = 0; j < columns; j++) 
        printf("From City %d to City %d: %lf \n", i, j, distMatrix[i][j]); 
}

void printPathStruct(struct path *salesmanInfo) {
  int numbPaths = calcFactorial(citySize);

  for (int i = 0; i < numbPaths; i++) {
    printf("************** START %d ******************* \n", i);
    printf("City Path: ");
    for (int j = 0; j < citySize; j++) {
      printf("%d -> ", salesmanInfo[i].path[j]);
    }
    printf("%d \n", salesmanInfo[i].path[citySize]);
    printf("path #: %d \n", salesmanInfo[i].pathNumb);
    printf(" ************** END %d ******************* \n", i);
  }
}

