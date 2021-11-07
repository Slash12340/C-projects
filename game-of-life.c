#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_DATA_LENGTH 255
#define MAX_ROWS 50
#define MAX_COLUMNS 78

static void read_file(FILE *fp, int *p_line, int rows, int columns, int init_rows[MAX_ROWS], int init_columns[MAX_COLUMNS])
{
    char input[MAX_DATA_LENGTH][MAX_DATA_LENGTH];           /* Array to hold all of the input data */
    char temp[MAX_DATA_LENGTH+1];                           /* Array to temporarily hold the current read line */
    char temp_rows[MAX_ROWS][MAX_ROWS];                     /* Array to temporarily hold the initial values for rows in string form */
    char temp_columns[MAX_COLUMNS][MAX_COLUMNS];            /* Array to temporarily hold the initial values for columns */
    int i;                                                  /* Indices for the for loops */ 
    int no_lines = *p_line;                                 /* Sets temporary int no_lines to the pointer pointing to the number of lines */

    /* ---- Read the file ---- */

    /* Loop through each line of file and assign it to temp array */
    while (fgets(temp, MAX_DATA_LENGTH, fp) != NULL)
    {
        /* If the line has a trailing '\n', remove it */
        if (temp[strlen(temp) - 1] == '\n')
        {
            temp[strlen(temp) - 1] = '\0';
        }

        /* If current line is empty, dont assign data to input[] */
        if (temp[0] != 0)
        {
            /* Copy the temp array into the inpyut array for further use */
            strcpy(input[no_lines], temp);
            no_lines++;
        }
    }

    /* Loop through the input data - skipping the first line - and split
     * up the initial rows and columns into 2 different arrays
     */
    for (i = 1; i < no_lines; i++)
    {
        /* Format the input data into rows and columns (strings) */
        sscanf(input[i], "%s %s", temp_rows[i], temp_columns[i]);

        /* Convert the formated rows and columns into integers */
        init_rows[i] = atoi(temp_rows[i]);
        init_columns[i] = atoi(temp_columns[i]);

        /* Check if any of the rows/columns is out of bounds from input file */
        if (init_rows[i] > rows-1 || init_rows[i] < 0 || init_columns[i] > columns-1 || init_columns[i] < 0)
        {
            printf( "Error: Incorrect input\n");
            exit(EXIT_FAILURE);
        }

        /* Check if the stated number of live cells in the input file matches the 
         * number of coordinates given
         */
        if (atoi(input[0]) != no_lines - 1)
        {
            printf("input = %.1f | lines = %i\n", atof(input[0]), no_lines);
            printf("Error: Incorrect input\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Sets the pointer to the the number of lines */
    *p_line = no_lines;
}







/* Create board:
 * Creates a board based on the file input data
 */
static void create_board(int rows, int columns, int *p_line, int board[rows+2][columns+2], int init_rows[MAX_ROWS], int init_columns[MAX_COLUMNS], int init_board[rows+2][columns+2])
{
    int i, j;                       /* Indices for the for loops */   
    int no_lines = *p_line;         /* Sets temporary int no_lines to the pointer pointing to the number of lines */

    /* ---- Create board ----*/

    /* Empty space */
    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < columns; i++)
        {
            board[j+1][i+1] = -1;
        }
    }

    /* Boundaries */
    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < rows; i++)
        {
            /* Loops through all rows and places a '|' at the begining and end of it */
            board[i+1][j*(columns+1)] = '|';

            /* The same can be achived with:
                * board[i+1][0] = '|';
                * board[i+1][coloumns+1] = '|';
                */
        }
    }
    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < columns; i++)
        {
            /* Loops through all columns and places a '-' at the begining and end of it */
            board[j*(rows+1)][i+1] = '-';
        }
    }
    
    /* Puts '*' at each corner of the board */
    board[0][0] = '*';
    board[0][columns+1] = '*';
    board[rows+1][0] = '*';
    board[rows+1][columns+1] = '*';

    /* Place initial live cells */
    for (i = 1; i < no_lines; i++)
    {
        board[init_rows[i] + 1][init_columns[i] + 1] = 0;
    }

    /* Copy the current/active board into the constant initial board */
    for (j = 0; j < rows + 2; j++)
    {
        for (i = 0; i < columns + 2; i++)
        {
            init_board[j][i] = board[j][i];
        }
    }

    /* Sets the pointer to the the number of lines */
    *p_line = no_lines;
}







/* Prints board:
 * Loops through the the 2D board array and prints out
 * each row (separated by a \n) and column
 */
static void print_board(int rows, int columns, int board[rows+2][columns+2])
{
    int i, j;                                   /* Indices for the for loops */
    int board_to_print[rows+2][columns+2];       /* Array to hold a copy the board to print */

    for (j = 0; j < rows + 2; j++)
    {
        for (i = 0; i < columns + 2; i++)
        {
            board_to_print[j][i] = board[j][i];
        }
    }

    for (j = 1; j < rows+1; j++)
    {
        for (i = 1; i < columns+1; i++)
        {
            if (board_to_print[j][i] > 9)
            {
                board_to_print[j][i] = 'x';
            }
            else if (board_to_print[j][i] > -1)
            {
                board_to_print[j][i] = board_to_print[j][i]+'0';
            }
            if (board_to_print[j][i] == -1)
            {
                board_to_print[j][i] = ' ';
            }
        }
    }

    /* Prints the whole board */
    for (j = 0; j < rows+2; j++)
    {
        for (i = 0; i < columns+2; i++)
        {
            /* Convert the int board layout to char (temporarily) to print out */
            printf("%c", board_to_print[j][i]);
        }
        putchar('\n');
    }
}










/* Updates the board based on game rules and initial settings */
static void update_board(int rows, int columns, int board[rows+2][columns+2])
{
    int i, j, k;                                        /* Indices for the for loops */
    int no_neighbours = 0;                              /* Number of neighbours a cell has */
    int copy_board[rows+2][columns+2];                  /* Holds the new board */
    int temp_board[rows][columns];                      /* Temp board to hold only the rows and columns - no formating */

    /* Create a copy of the old board */
    for (j = 0; j < rows + 2; j++)
    {
        for (i = 0; i < columns + 2; i++)
        {
            copy_board[j][i] = board[j][i];
        }
    }

    /* Copies only the board's rows and columns into a temporary board*/
    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < columns; i++)
        {
            temp_board[j][i] = board[j+1][i+1];
        }
    }
    

    /* Loop through the whole board, check for live cells and apply rules */
    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < columns; i++)
        {
            /* Reset neighbour counter once a cell has been analyized */
            no_neighbours = 0;

            /* Check for neighbours in top and bottom 3 neighbouring grids */
            for (k = -1; k < 2; k++)
            {
                if (temp_board[j-1][i+k] > -1)
                {
                    if (j > 0 && j < rows-1 && i > 0 && i < columns-1)
                    {
                        no_neighbours++;
                    }
                }
                if (temp_board[j+1][i+k] > -1)
                {
                    if (j > 0 && j < rows-1 && i > 0 && i < columns-1)
                    {
                        no_neighbours++;
                    }
                }
            }

            /* Check for neighbours on the left and right of the current cell */
            if (temp_board[j][i-1] > -1)
            {
                if (j > 0 && j < rows && i > 0 && i < columns)
                {
                    no_neighbours++;
                }
            }
            if (temp_board[j][i+1] > -1)
            {
                if (j > 0 && j < rows && i > 0 && i < columns)
                {
                    no_neighbours++;
                }
            }

            /* ---- Implement rules ---- */

            /* Under population rule and over population */
            if (temp_board[j][i] > -1)
            {
                if (no_neighbours < 2 || no_neighbours > 3)
                {
                    copy_board[j+1][i+1] = -1;
                }
            }
            /* Reporduction */
            if (temp_board[j][i] == -1)
            {
                if (no_neighbours == 3)
                {
                    copy_board[j+1][i+1] = 0;
                }
            }
        }
    }

    /* Loops through both the last and new board and checks for duplicates.
     * If duplicates are found, 1 is added to their position, showing its age
     */
    for (j = 1; j < rows+1; j++)
    {
        for (i = 1; i < columns+1; i++)
        {
            if (board[j][i] == copy_board[j][i] && (board[j][i] > -1 || copy_board[j][i] > -1))
            {
                copy_board[j][i]++;
            }
        }
    }

    /* Copy new board onto the board to be printed */
    for (j = 0; j < rows + 2; j++)
    {
        for (i = 0; i < columns + 2; i++)
        {
            board[j][i] = copy_board[j][i];
        }
    }
}








static int check_repeat(int rows, int columns, int board[rows+2][columns+2], int init_board[rows+2][columns+2])
{
    int i, j;                               /* Indices for the for loop */
    int temp_board[rows+2][columns+2];      /* Temporary array to hold the board */

    /* Copy the board into a temporary array as to not
     * change the values of the current board which could
     * interfere with the rest of the code, namely the update
     * function.
     */
    for (j = 0; j < rows + 2; j++)
    {
        for (i = 0; i < columns + 2; i++)
        {
            temp_board[j][i] = board[j][i];
        }
    }

    /* Normalize the temporary array - i.e. set all the live cells
     * to the same value to make it simpler to compare with the
     * initial board array.
     */
    for (j = 1; j < rows + 1; j++)
    {
        for (i = 1; i < columns + 1; i++)
        {
            if (temp_board[j][i] > 0)
            {
                temp_board[j][i] = 0;
            }
        }
    }

    /* Check for repetition */
    for (j = 0; j < rows+2; j++)
    {
        for (i = 0; i < columns+2; i++)
        {
            if (temp_board[j][i] != init_board[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}








int main(int argc, char *argv[])
{
    int i;                  /* Index for the for loops */
    int no_lines = 0;       /* Number of lines in the file */  

    /* Check if the correct amount of arguments were
     * taken when the program is executed. If not,
     * end the program and print to stdout as stated in the spec.
     */
    if (argc != 5)
    {
        printf("Error: Incorrect input\n");
        exit(EXIT_FAILURE);
    }

    /* Assign the data from when the program is executed */
    char *input_file_name = argv[1];
    int columns = atoi(argv[2]);
    int rows = atoi(argv[3]);
    int generations = atoi(argv[4]);
    int board[rows+2][columns+2];

    int init_rows[MAX_ROWS], init_columns[MAX_COLUMNS];     /* Arrays to hold the initial row and column in int form*/
    int init_board[rows+2][columns+2];                          /* The initial board. Will be used to detect repetition */

    /* Checks if the rows and columns are within their max lengths
     * and if the rows, columns and generations are integers
     */
    if (rows >= MAX_ROWS || columns >= MAX_COLUMNS)
    {
        printf("Error: Incorrect input max\n");
        exit(EXIT_FAILURE);
    }



    /* Creates and open the input file and assign it to fp */
    FILE *fp = fopen(input_file_name, "r");

    /* Check for valid input file */
    if (fp == NULL)
    {
        printf("Error: Couldn't open file %s\n", input_file_name);
        exit(EXIT_FAILURE);
    }

    /* Call appropriate functions */
    read_file(fp, &no_lines, rows, columns, init_rows, init_columns);
    create_board(rows, columns, &no_lines, board, init_rows, init_columns, init_board);
    print_board(rows, columns, board);
    printf("\n\n");

    for (i = 0; i < generations; i++)
    {
        update_board(rows, columns, board);
        print_board(rows, columns, board);

        /* If 4 generations have not passed, then check for repeat */
        if (i < 4)
        {
            /* Boolean to decide whether repetition was detected or not */
            int repetition_detected = check_repeat(rows, columns, board, init_board);

            if (repetition_detected == 1)
            {
                printf("Period detected (%i): exiting\n", i+1);
                exit(EXIT_FAILURE);
            }
        }

        /* As stated in spec; "two blank lines should be printed 
        * between the outputs for each time step."
        */
        if (i < generations - 1)
        {
            printf("\n\n");
        }
        else
        {
            printf("\n");
        }
    }

    printf("Finished\n");

    /* Close the open file */
    fclose(fp);
    return 0;
}