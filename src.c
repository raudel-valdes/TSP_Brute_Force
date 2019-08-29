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


//Function Prototypes
double * createCityArray(FILE *);
int calcFactorial(int n);
struct path * calcPathDist(struct path *, double **, int);
void swapCities(int *, int *);
void permuteCities(int *, struct path *, double **, int);
double ** createDistMatrix(double *);
double calcDistance(int, int, double *);
void printFileContent(FILE *);
void printArrayContent(double *);
void printMatrixContent(double **);
void printPathStruct(struct path *);
void findOptimalPath(struct path *);

//Total number of cities in a file
//gets used in various files and set 
//within the func. createCityArray()
int citySize = 0;

//The number of permutations that have been
//created already. It is used to index
//the array of struct inside of permuteCities()
int permNumb = 0;


//This struct is used to keep every unique 
//path data together
struct path {
  int *path;
  double totalDist;
  int pathNumb;
} path;


int main() {
  //Initializing variables 
  FILE *fptr;
  char filePath[25];
  double *cityArr;
  double **distMatrix;
  struct path *salesmanInfo;
  int *cityList;

  //Asks the user to enter a file that they want to use
  //one file at a time
  printf("Please provide a file: \n");
  scanf("%s", filePath);

  fptr = fopen(filePath, "r");

  //checks to see if the file was able to open
  if (fptr != NULL) {
    //printFileContent(fptr);
    cityArr = createCityArray(fptr);
    //printArrayContent(cityArr);
    distMatrix = createDistMatrix(cityArr);
    //printMatrixContent(distMatrix);

    //creates dynamic memory to create an array with all the cities
    cityList = (int *)malloc(citySize * sizeof(int));

    //creates a dynamic array of structs equal to the number of permutations 
    salesmanInfo = (struct path *)malloc(calcFactorial(citySize) * sizeof(struct path));

    //allocates memory for the struct attribute path in order to act as an array
    for (int i = 0; i <= calcFactorial(citySize); i++)
      salesmanInfo[i].path = (int *)malloc((citySize + 1) * sizeof(int));

    //stores values inside of an array in order
    //to match the cities that exist in the file
    for (int i = 0; i <= citySize; i++)
      cityList[i] = i;

    permuteCities(cityList, salesmanInfo, distMatrix, 0);
    //printPathStruct(salesmanInfo);
    findOptimalPath(salesmanInfo);

  } else { 
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  //Makes sure to deallocated dynamic memory
  //and close the file used to read in the cities.
  fclose(fptr);
  free(salesmanInfo);
  free(cityList);

  return 0;
}

//returns a double pointer and accepts a file pointer
//this function takes care of reading all of the 
//important file datga and then passes it back to
//the main function to use the data for other 
//caculations 
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

  //moves the file pointer to the begginning of the file
  rewind(fptr);

  //reads the file line by line
  nextLine = fgets(stringLine, sizeof(stringLine), fptr);

  while (nextLine != NULL) 
  {
    //saves the total number of cities in the file
    if ((strstr(nextLine, "DIMENSION: ")) != NULL) {
      tmpFptr = fopen ("temp.txt", "w+");
      fputs(stringLine, tmpFptr);
      rewind(tmpFptr);
      fscanf(tmpFptr, "%s %d", str1, &citySize);

      cityArr = (double *)malloc((2 * citySize) * sizeof(double));
    }

    //We assume that every .tsp file is the same and the first 
    //city information will start on line 8
    if (lineNumb >= 8) {

      tmpFptr = fopen ("temp.txt", "w+");
      fputs(stringLine, tmpFptr);

      //reads in the x and y cordinates of each city as floats
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

// this function calculates the factorial of any integer passed to it
//and return the inter answer
int calcFactorial(int n) {
  int answer = 1;
  for (int i = 2; i <= n; i++) answer *= i;
  return answer;
}

//this function swaps an arrays values using pointers
void swapCities(int *x, int *y) { 
  int temp; 
  temp = *x; 
  *x = *y; 
  *y = temp;
} 

//A recurssive function that uses backtracking algorithm to calculate
//the permutation of an array of cities. This function calls swapCities() in
//order to switch the values of cities within the array. It also calls calcPathDist()
//which calculates the total distance for a specific path and saves it to an array of structs
void permuteCities(int *cityList, struct path * salesmanInfo, double ** distMatrix,int c) { 
  int i = 0;
  int r = citySize - 1;

  //the base case of the recurssive function
  if (c == r) {

    for (int j = 0; j < citySize; j++)
      salesmanInfo[permNumb].path[j] = cityList[j];

    salesmanInfo[permNumb].pathNumb = permNumb;
    salesmanInfo[permNumb].path[citySize] = salesmanInfo[permNumb].path[0];

    //calculating the total distance of the path
    calcPathDist(salesmanInfo, distMatrix,permNumb);

    permNumb++;
  }
  else
  { 
    for (i = c; i <= r; i++) {   
      //swapping values in array  
      swapCities((cityList + c), (cityList + i)); 
      //the recursive part of this function
      permuteCities(cityList, salesmanInfo, distMatrix, c + 1);
      //swapping values in array  
      swapCities((cityList + c),(cityList+i)); 
    } 
  } 

  return;
}

//for some reason I cant get rid of this function or change it
//to return to void because then I get a lot of errors.
//maybe it is because it is the only function that returns
//salesmanInfo. ******** Saves the total distance of each path
//in the array of struct 
struct path  *calcPathDist(struct path *salesmanInfo, double **distMatrix, int permNumb) {
  double distTrav = 0.0;
  int r = 0;
  int c = 0;

  for (int i = 0; i < citySize; i++) {
    r = salesmanInfo[permNumb].path[i];
    c = salesmanInfo[permNumb].path[i+1];

    distTrav += distMatrix[r][c];
  }

  salesmanInfo[permNumb].totalDist = distTrav;

  return salesmanInfo;
}

//fills up the matrix with the calculated distances between two cities
double ** createDistMatrix(double *cityArr) {
  int rows = citySize;
  int columns = citySize;

  double **distMatrix = (double **)malloc(rows * sizeof(double *)); 
  for (int i = 0; i < rows; i++) 
    distMatrix[i] = (double *)malloc(columns * sizeof(double)); 
  
  for (int i = 0; i <  rows; i++) 
    for (int j = 0; j < columns; j++) 
      distMatrix[i][j] = calcDistance(i,j, cityArr);

  return distMatrix;
}

//calculates the distance between two given cities and returns it to be saved
//inside an array of structs. It uses the distance function.
double calcDistance(int cityOne, int cityTwo, double *cityArr) {
  double deltaX = 0;
  double deltaY = 0;

  //if the two cities are the same one then dont calculate because distance = 0.0
  if (cityOne == cityTwo) {
    return 0.0;
  }

  //accesses an array that has the X cords of both cities
  deltaX = cityArr[cityOne * 2] - cityArr[cityTwo * 2];
  deltaX *= deltaX;

  //accesses an array that has the Y cords of both cities
  deltaY = cityArr[2 * cityOne + 1] - cityArr[2 * cityTwo + 1];
  deltaY *= deltaY;

  //returns the calculated distance between the two cities
  return sqrt(deltaX + deltaY);
}

//this function loops through the array of structs in order to find which path dist
// is the smallest and then loops through all of the paths to get all paths that have that
//optiomal path distance in order to print them out to the user.
void findOptimalPath(struct path * salesmanInfo) {
  double optimalDist = 0.0;
  int numbBestPaths = 0;
  struct path *bestPaths = (struct path *)malloc(calcFactorial(citySize) * sizeof(struct path));

  for (int i = 0; i < calcFactorial(citySize); i++)
    bestPaths[i].path = (int *)malloc((citySize + 1) * sizeof(int));

  //this has to be les than (<) look more into it.
  //I have a feeling it has to be (<=)
  for (int i = 0; i < calcFactorial(citySize); i++) {
    if (i ==0 )
      optimalDist = salesmanInfo[i].totalDist;

    if (optimalDist > salesmanInfo[i].totalDist) {
      optimalDist = salesmanInfo[i].totalDist;
    }
  }

  for(int i = 0; i < calcFactorial(citySize); i++ ) {
    if (optimalDist == salesmanInfo[i].totalDist) {
      for (int j = 0; j < citySize; j++) {
        bestPaths[numbBestPaths].path[j] = salesmanInfo[i].path[j];
      }
      numbBestPaths++;
    }
  }

  printf("\n Optimal Dist: %lf \n", optimalDist);
  printf("Paths with Optimal Dist: %d \n", numbBestPaths);
  for (int i = 0; i < numbBestPaths; i++) {
    printf("\n ************** START %d ******************* \n", i);
    for (int j = 0; j < citySize; j++) {
      printf("%d -> ", bestPaths[i].path[j]+1);
    }
    printf("%d \n", bestPaths[i].path[0]+1);
    printf(" ************** END %d ******************* \n \n", i);
  }
}

//prints to the console all of the content inside of the file that has
//all of the cities information
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

//prints all of the content within the cityArray which basically all
// of the cities that exist in the file. I could make this function more
//reusable by calculating the  number of for loops from the index size of the
//array rather than just one specific size
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

//Prints the matrix information in case we want to see all of the
//distances between cities
void printMatrixContent(double **distMatrix) {
  int rows = citySize;
  int columns = citySize;

  for (int i = 0; i < rows; i++) 
    for (int j = 0; j < columns; j++) 
        printf("From City %d to City %d: %lf \n", i, j, distMatrix[i][j]); 
}

//iterates through the array structs and prints the path for each struct
void printPathStruct(struct path *salesmanInfo) {
  int numbPaths = calcFactorial(citySize);

  for (int i = 0; i < numbPaths; i++) {
    printf("\n ************** START %d ******************* \n", i);
    printf("path #: %d \n", salesmanInfo[i].pathNumb);
    printf("City Path: ");
    for (int j = 0; j < citySize; j++) {
      printf("%d -> ", salesmanInfo[i].path[j]+1);
    }
    printf("%d \n", salesmanInfo[i].path[0]+1);
    printf("Dist Traveled: %lf \n", salesmanInfo[i].totalDist);
    printf(" ************** END %d ******************* \n \n", i);
  }
}


