/** matrices.c
    Program description: Reads a file to form N x M matrix (user specified size). From that matrix user can choose
        row and column index as a base to form anther 2x2 matrix. The program calculates that 2x2 matrix determinant
        and find its inverse. After each new matrix is created, the user has to specify its letter-only name.
    Date:   30/03/2019
    Author: Justas
**/

#include <stdio.h>      // input/output library
#include <string.h>     // used for input buffers

#define MATRIX_LIMIT 10 // defines maximum rows and columns of any matrix in the program

struct matrix
{
    char name;              // to hold the 1 character name of the matrix
    float mValues[10][10];  // to hold the values in the matrix, up to 10 rows x 10 columns
    int nrows;              // to hold the number of rows used in mValues
    int ncols;              // to hold the number of columns used in mValues
};

int matrixInput(struct matrix *mat); /* to input the size (number of rows and
columns) of the matrix, the name of the matrix, and read in the values from the file into the 2D
array that makes up the matrix */

void matrixDisplay(struct matrix mat); /* to display the name of the matrix and
the values in the 2D array that makes up the matrix*/

int matrixDeterminant(struct matrix m1, struct matrix *m2); /* to find the
determinant of m2, where m2 is a 2x2 subset matrix of m1; this function does not display
either m1 or m2 */

void matrixInverse(struct matrix m2, int determinant, struct matrix *m3); /* to find the inverse of the 2x2 matrix; this function does not display either m2 or
m3 */

void clearBuffer(); /* to clear input buffer; protects new inputs from assigning old buffer values */

int inputNumber(int lower_bound, int upper_bound); /* defensive integer input within specified range */

int main ()
{
    struct matrix myMatrix;         // hold the full matrix
    struct matrix my2x2Matrix;      // holds the 2x2 matrix
    struct matrix myInverseMatrix;  // holds inverse matrix
    int determinant;                // holds determinant value of 2x2 matrix
    char repeat;                    // hold condition for program loop


    // Main loop
    do
    {
        // Creates user defined matrix and displays it
        if (matrixInput(&myMatrix))
            break;
        matrixDisplay(myMatrix);

        // Makes 2x2 matrix from the first matrix and finds determinant and displays it
        determinant = matrixDeterminant(myMatrix, &my2x2Matrix);
        printf("The determinant is %d for\n", determinant);
        matrixDisplay(my2x2Matrix);

        // makes inverse matrix and displays it
        matrixInverse(my2x2Matrix, determinant, &myInverseMatrix);
        matrixDisplay(myInverseMatrix);


        // Asks user to repeat the calculations
        printf("\nEnter C if you want to repeat the calculations (or anything else if you don't)\n");
        scanf("%c", &repeat);
        clearBuffer();
        printf("\n\n");
    }
    while (repeat == 'c' || repeat == 'C'); // repeat condition is 'c' or 'C'.


    return 0;
}

/* to clear input buffer; protects new inputs from assigning old buffer values */
void clearBuffer()
{
    int buffer;
    while((buffer = getchar()) != '\n' );
}


/* Defensive integer input within specified range. Two inputs are lower and upper bounds.*/
int inputNumber(int lower_bound, int upper_bound)
{
    char buffer[50];    // string buffer
    int output;         // output integer
    int success;        // condition parameter for tracking integer inputs

    do
    {
        fgets(buffer, sizeof buffer, stdin);        // reads the line and stores into a buffer (up to 50 chars)
        success = sscanf(buffer, " %d", &output);   // reads the integer from the buffer; sets the state into success variable
        if (success != 1 || (output < lower_bound || output > upper_bound) )
        {                                           // is not integer or out of bounds
            printf("Wrong input! Only numbers in range %d-%d.\n", lower_bound, upper_bound);
        }
    }
    while (success != 1 || output < lower_bound || output > upper_bound);   // loops until integer within bounds


    return output;      // returns inbound integer
}


/* to input the size (number of rows and
columns) of the matrix, the name of the matrix, and read in the values from the file into the 2D
array that makes up the matrix */
int matrixInput(struct matrix *mat)
{
    int i, j;   // Used for loops
    char name;  // holds the user entered name
    FILE *fin;  // Used to open a file


    // Open the file, check if it exists, kill if does not
    fin = fopen("matrix1.txt", "r");
    if(fin==NULL)
    {
        printf("File matrix1.txt does not exist.\n");
        mat->ncols = 0;
        mat->nrows = 0;
        return 1;
    }


    // Get user defined name
    printf("Please enter one letter character name for the matrix, e.g. A, B, etc.\n");
    do
    {
        scanf(" %c", &name);    // reads 1 character
        clearBuffer();          // clears any other inputs
        if (name < 'A' || name > 'Z' && name < 'a' || name > 'z')   // Letter check
            printf("Wrong input! Only letters are allowed.\n");
    }
    while (name < 'A' || name > 'Z' && name < 'a' || name > 'z');

    mat->name = name;           // stores name in struct variable


    // Get user defined number of rows
    printf("Enter number of rows of the matrix (in range 1-%d):\n\tNote: floating point number will be truncated.\n", MATRIX_LIMIT);
    mat->nrows = inputNumber(1, MATRIX_LIMIT);


    // Get user defined number of columns
    printf("Enter number of columns of the matrix (in range 1-%d):\n\tNote: floating point number will be truncated.\n", MATRIX_LIMIT);
    mat->ncols = inputNumber(1, MATRIX_LIMIT);

    // Get matrix from the file
    for (i=0; i<mat->nrows; i++)
    {
        for (j=0; j<mat->ncols; j++)
        {
            fscanf(fin, "%f", &mat->mValues[i][j]);
        }
    }


    fclose(fin);    // close the file
    return 0;
}

/* to display the name of the matrix and the values in the 2D array that makes up the matrix*/
void matrixDisplay(struct matrix mat)
{
    int i, j; // used for loops

    printf("Matrix %c:\n", mat.name);   // prints matrix name
    for (i=0; i<mat.nrows; i++)
    {
        printf("Row %d:\t", i);
        for (j=0; j<mat.ncols; j++)
        {
            printf("%6.2f ", mat.mValues[i][j]);    // prints matrix values; uses 6 space and 2 decimal place format
        }
        printf("\n");
    }
    printf("\n");
}


/* to find the determinant of m2, where m2 is a 2x2 subset matrix of m1; this function does
not display either m1 or m2 */
int matrixDeterminant(struct matrix m1, struct matrix *m2)
{
    int row, column;    // Holds user input of row and column
    int i, j;           // Used for loops
    int determinant=0;  // holds determinant value
    char name;          // holds the user entered name. Assigned space to be before 'A' in ACSII.

    printf("Finding determinant now...\n");

    // Get user defined name (only letters)
    printf("Please enter one letter character name for the matrix, e.g. A, B, etc.\n");
    do
    {
        scanf(" %c", &name);
        clearBuffer();
        if (name < 'A' || name > 'Z' && name < 'a' || name > 'z')
            printf("Wrong input! Only letters are allowed.\n");
    }
    while (name < 'A' || name > 'Z' && name < 'a' || name > 'z');

    m2->name = name;    // stores name in struct variable


    // Determinant cases
    // Case 1: 1x1 matrix
    if (m1.nrows == 1 && m1.ncols == 1)
    {
        m2->mValues[0][0] = m1.mValues[0][0];   // copies 1x1 matrix
        m2->nrows = 1;                          // sets row count to 1
        m2->ncols = 1;                          // sets column count to 1
        determinant = m2->mValues[0][0];        // finds determinant

        return determinant; // return special case 1x1 determinant
    }
    // Case 2: 2x2 matrix
    else if (m1.nrows == 2 && m1.ncols == 2)
    {
        m2->mValues[0][0] = m1.mValues[0][0];   // Copies 2x2 matrix
        m2->mValues[0][1] = m1.mValues[0][1];
        m2->mValues[1][0] = m1.mValues[1][0];
        m2->mValues[1][1] = m1.mValues[1][1];
    }
    // Case 3: 1xN matrix
    else if (m1.nrows == 1)
    {
        printf("\n\nSince determinant can only be calculated for a square matrix, the missing row is filled with 0 to form at least one 2x2 matrix.\n\n");

        // Get user defined column position
        printf("Enter column number where to start 2x2 matrix, number needs to be between 0 and %d:\n", m1.ncols-2);
        printf("\tNote: floating point number will be truncated.\n");
        column = inputNumber(0, m1.ncols-2);

        // copies row values and creates another row of 0 to form 2x2 matrix
        m2->mValues[0][0] = m1.mValues[0][column];
        m2->mValues[0][1] = m1.mValues[0][column+1];
        m2->mValues[1][0] = 0;
        m2->mValues[1][1] = 0;
    }
    // Case 4: 2xN matrix
    else if (m1.nrows == 2)
    {
        // Get user defined column position
        printf("Enter column number where to start 2x2 matrix, number needs to be between 0 and %d:\n", m1.ncols-2);
        printf("\tNote: floating point number will be truncated.\n");
        column = inputNumber(0, m1.ncols-2);

        // copies 2x2 matrix from specific place
        m2->mValues[0][0] = m1.mValues[0][column];
        m2->mValues[0][1] = m1.mValues[0][column+1];
        m2->mValues[1][0] = m1.mValues[1][column];
        m2->mValues[1][1] = m1.mValues[1][column+1];
    }
    // Case 5: Nx1 matrix
    else if (m1.ncols == 1)
    {
        printf("\n\nSince determinant can only be calculated for a square matrix, the missing column is filled with 0 to form 2x2 matrix.\n\n");

        // Get user defined row position
        printf("Enter row number where to start 2x2 matrix, number needs to be between 0 and %d:\n", m1.nrows-2);
        printf("\tNote: floating point number will be truncated.\n");
        row = inputNumber(0, m1.nrows-2);

        // copies column values and creates another column of 0 to form 2x2 matrix
        m2->mValues[0][0] = m1.mValues[row][0];
        m2->mValues[0][1] = 0;
        m2->mValues[1][0] = m1.mValues[row+1][0];
        m2->mValues[1][1] = 0;
    }
    // Case 6: Nx2 matrix
    else if (m1.ncols == 2)
    {
        // Get user defined row position
        printf("Enter row number where to start 2x2 matrix, number needs to be between 0 and %d:\n", m1.nrows-2);
        printf("\tNote: floating point number will be truncated.\n");
        row = inputNumber(0, m1.nrows-2);

        // copies 2x2 matrix from specific place
        m2->mValues[0][0] = m1.mValues[row][0];
        m2->mValues[0][1] = m1.mValues[row][1];
        m2->mValues[1][0] = m1.mValues[row+1][0];
        m2->mValues[1][1] = m1.mValues[row+1][1];
    }
    // Case 7: 3x3 and bigger matrices
    else
    {
        // Get user defined number of rows
        printf("Enter row number where to start 2x2 matrix, number needs to be between 0 and %d:\n", m1.nrows-2);
        printf("\tNote: floating point number will be truncated.\n");
        row = inputNumber(0, m1.nrows-2);


        // Get user defined number of columns
        printf("Enter column number where to start 2x2 matrix, number needs to be between 0 and %d:\n", m1.ncols-2);
        printf("\tNote: floating point number will be truncated.\n");
        column = inputNumber(0, m1.ncols-2);


        // Make a copy 2x2 matrix from specific place
        for (i=0; i<2; i++)
        {
            for (j=0; j<2; j++)
            {
                m2->mValues[i][j] = m1.mValues[i+row][j+column];
            }
        }
    } // End of cases

    // Matrix size set to 2x2
    m2->nrows = 2;
    m2->ncols = 2;

    // Calculating determinant
    determinant = m2->mValues[0][0] * m2->mValues[1][1] - m2->mValues[0][1] * m2->mValues[1][0];

    return determinant;     // returns determinant value
}


/* to find the inverse of the 2x2 matrix; this function does not display either m2 or m3 */
void matrixInverse(struct matrix m2, int determinant, struct matrix *m3)
{
    char name;  // holds user entered name

    printf("Finding inverse now...\n");

    // Get user defined name
    printf("Please enter one letter character name for the matrix, e.g. A, B, etc.\n");
    do
    {
        scanf(" %c", &name);
        clearBuffer();
        if (name < 'A' || name > 'Z' && name < 'a' || name > 'z')
            printf("Wrong input! Only letters are allowed.\n");
    }
    while (name < 'A' || name > 'Z' && name < 'a' || name > 'z');

    m3->name = name;        // store name into sruct variable


    // Case 1: determinant = 0
    if (determinant == 0)
    {
        m3->nrows = 0;  // sets row and column counts to 0
        m3->ncols = 0;
        printf("Inverse is inpossible.\n");
    }
    // Case 2: 1x1 matrix
    else if (m2.nrows == 1 && m2.ncols == 1)
    {
        m3->mValues[0][0] = 1 / m2.mValues[0][0];   // finds inverse of 1x1 (reciprical)

        m3->nrows = 1;  // sets row and column counts to 1
        m3->ncols = 1;
    }
    // Case 3: 2x2 matrix
    else
    {
        // finds inverse of 2x2 matrix
        m3->mValues[0][0] = m2.mValues[1][1] / determinant;
        m3->mValues[0][1] = -m2.mValues[0][1] / determinant;
        m3->mValues[1][0] = -m2.mValues[1][0] / determinant;
        m3->mValues[1][1] = m2.mValues[0][0] / determinant;

        m3->nrows = 2;  // sets row and column counts to 2
        m3->ncols = 2;
    }
}

