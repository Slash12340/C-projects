#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* maximum number of employees that can be stored at once (relevant only
   to storage using an array) */
#define MAX_EMPLOYEES 200

#define MAX_NAME_LENGTH    100
#define MAX_JOB_LENGTH     100
#define MAX_INPUT_LENGTH   100         /* Max length of the input array */

/* Employee structure
 */
struct Employee
{
   /* Employee details */
   char name[MAX_NAME_LENGTH+1];       /* name string */
   char sex;                           /* sex identifier, either 'M' or 'F' */
   int  age;                           /* age */
   char job[MAX_JOB_LENGTH+1];         /* job string */
   
   /* pointers to previous and next employee structures in the linked list
      (for if you use a linked list instead of an array) */
   struct Employee *prev, *next;
};

/* Global array for employees */
static struct Employee employee_array[MAX_EMPLOYEES];
/* Number of employees stored in the array */
static int no_employees = 0;

/* read_line():
 *
 * Read line of characters from file pointer "fp", copying the characters
 * into the "line" string, up to a maximum of "max_length" characters, plus
 * one for the string termination character '\0'. Reading stops upon
 * encountering the end-of-line character '\n', for which '\0' is substituted
 * in the string. If the end of file character EOF is reached before the end
 * of the line, the failure condition (-1) is returned. If the line is longer
 * than the maximum length "max_length" of the string, the extra characters
 * are read but ignored. Success is returned (0) on successfully reading
 * a line.
 */
static int read_line ( FILE *fp, char *line, int max_length )
{
   int i;
   char ch;

   /* initialize index to string character */
   i = 0;

   /* read to end of line, filling in characters in string up to its
      maximum length, and ignoring the rest, if any */
    for(;;)
    {
        /* read next character */
        ch = fgetc(fp);

        /* check for end of file error */
        if ( ch == EOF )
            return -1;

        /* check for end of line */
        if ( ch == '\n' )
        {
        /* terminate string and return */
            line[i] = '\0';
            return 0;
        }

        /* fill character in string if it is not already full*/
        if ( i < max_length )
            line[i++] = ch;
    }

   /* the program should never reach here */
   return -1;
}

/* read_string():
 *
 */
static void read_string (FILE *fp, char *string, char *prefix)
{
    /* Temorary strings to hold input data */
    char name[MAX_INPUT_LENGTH][MAX_INPUT_LENGTH];  
    char sex[MAX_INPUT_LENGTH][MAX_INPUT_LENGTH];  
    char age[MAX_INPUT_LENGTH][MAX_INPUT_LENGTH];  
    char job[MAX_INPUT_LENGTH][MAX_INPUT_LENGTH];  
    
    /* Intitialize index for for loops */
    int i, j;

    /* Array counters */
    int name_counter = 0;
    int sex_counter = 0;
    int age_counter = 0;
    int job_counter = 0;
    
    int array_size = 0;     /* Size of the name array */

    /* Loop through the whole file and read lines */
    for (i = 0; !feof(fp); i++)
    {
        read_line(fp, string, 100);

        /* If line is not empty, then read the prefix */
        if (string[0] != 0)
        {
            sscanf(string, "%[^:]:", prefix);       /* Read the prefix of current line */
        }

        /* If the length of the prefix array is more than 0 AND the last element in
         * the array is a '\n' then set it to '\0' to remove the "new line." */
        if (strlen(prefix) > 0 && prefix[strlen(prefix) - 1] == '\n')
        {
            prefix[strlen(prefix) - 1] = '\0';
        }
        
        /* If line is not empty, then read the data */
        if (string[0] != 0)
        {
            /* Check what the prefix at the current line is and use the correct
            * sscanf formating to assign it to the input array */
            if (strcmp(prefix, "Name") == 0)
            {
                sscanf(string, "%[^:]: %[^\t\n]", prefix, name[name_counter]);

                /* If the first character of the array is 0 or a linefeed, then
                 * exit the program */
                if (name[name_counter][0] == 0 || name[name_counter][0] == '\n')
                {
                    fprintf(stderr, "Incorrect name input at line %i\n", i);
                    exit(EXIT_FAILURE);
                }

                /* If there is an extra line feed at the end of the input, remove it */
                if (strlen(name[name_counter]) > 0 && name[name_counter][strlen(name[0]) - 1] == '\n')
                {
                    name[name_counter][strlen(name[0]) - 1] = '\0';
                }

                /* Use these counters to index the arrays that hold the input data.
                 * Using i does not work as, for example, the "name" prefix is not
                 * on every line so, if i was to be used as the indexing for the 
                 * arrays, there would be a gap in memory. Having the counters increment
                 * only when this block of code is called stop the code from having any
                 * gaps in array memory. */
                name_counter++; /* Increment counter each time */
                array_size++;   /* Increment array size after each time */
            }

            if (strcmp(prefix, "Sex") == 0)
            {
                sscanf(string, "%[^:]: %s", prefix, sex[sex_counter]);

                /* If input is a empty or not M or F, end program */
                if (sex[sex_counter][0] == 0 || strlen(sex[sex_counter]) != 1 || (sex[sex_counter][0] != 'M' && sex[sex_counter][0] != 'F'))
                {
                    fprintf(stderr, "Incorrect sex at light %i\n", i);
                    exit(EXIT_FAILURE);
                }
                sex_counter++;
            }

            if (strcmp(prefix, "Age") == 0)
            {
                int correct_input = 0;  /* Boolean to check if value age is corrected */
                
                sscanf(string, "%[^:]: %s", prefix, age[age_counter]);

                /* While loop to check if a legal value for input has been achieved */
                while (correct_input == 0)
                {
                    /* Set boolean to true (1). This might be can to false (0) as
                    * the code progresses
                    */
                    correct_input = 1;
                    int j;

                    /* If age not entered, ask for input again */
                    if (age[age_counter][0] == '\n')
                    {
                        fprintf(stderr, "Incorrect age input at line %i\n", i);
                        exit(EXIT_FAILURE);

                        /* Set boolean to false as the correct value has not yet been achieved */
                        correct_input = 0;
                    }    

                    /* Loop through the user input and check if any characters in the array are
                    * not digits. If there are non-digit characters, then ask for input again
                    */
                    for (j = 0; j < strlen(age[age_counter]) - 1; j++)
                    {
                        if (!isdigit(age[age_counter][j]) || age[age_counter][j] == ' ')
                        {
                            fprintf(stderr, "Incorrect age input at line %i\n", i);
                            exit(EXIT_FAILURE);
                            j = 0;   /* Reset counter for new input */

                            /* Set boolean to false as the correct value has not yet been achieved */
                            correct_input = 0;
                        }
                    }

                    if (atoi(age[age_counter]) < 1)
                    {
                        fprintf(stderr, "Incorrect age input at line %i\n", i);
                        exit(EXIT_FAILURE);

                        /* Set boolean to false as the correct value has not yet been achieved */
                        correct_input = 0;
                    }
                }

                age_counter++;
            }

            if (strcmp(prefix, "Job") == 0)
            {
                sscanf(string, "%[^:]: %[^\t\n]", prefix, job[job_counter]);

                if (job[job_counter][0] == 0)
                {
                    fprintf(stderr, "Incorrect job input at line %i\n", i);
                    exit(EXIT_FAILURE);
                }

                job_counter++;
            }
            
        }
    }

    /* Add the input data into the database */
    for (i = 0; i < array_size; i++, no_employees++)
    {
        strcpy(employee_array[no_employees].name, name[i]);
        employee_array[no_employees].sex = sex[i][0];
        employee_array[no_employees].age = atoi(age[i]);
        strcpy(employee_array[no_employees].job, job[i]);
    }
}

/* read_employee():
 *
 * uses fgets to read and input employee data into database.
 */
static void read_employee(void)
{
    /* Create new employee variable and set it to the nth employee in the array*/
    struct Employee *new = &employee_array[no_employees];

    /* Get name input */
    {
        /* Prompt user for input */
        fprintf(stderr, "Enter name: ");
        
        /* Get name input */
        fgets(new->name, MAX_NAME_LENGTH, stdin);
        fflush(stdin);    /* Clear buffer incase of overflow. Truncates the array*/

        /* If name is not entered or if the input doesn't start with a alphabetical char, 
        * send message to stderr and ask for name again */
        while (new->name[0] == '\n' || !isalpha(new->name[0]))
        {
            fprintf(stderr, ("Incorrect input\nEnter name: "));
            fgets(new->name, MAX_NAME_LENGTH, stdin);
            fflush(stdin);
        }

        /* If the length of the name array is more than 0 AND the last element in
        * the array is a '\n' then set it to '\0' to remove the "new line." */
        if (strlen(new->name) > 0 && new->name[strlen(new->name) - 1] == '\n')
        {
            new->name[strlen(new->name) - 1] = '\0';
        }
    }

    /* Get sex input */
    {
        char input[100];       /* Create string to take in input for the sex */

        /* Prompt user for input */
        fprintf(stderr, "Enter sex: ");

        /* Get sex input */
        fgets(input, 100, stdin);
        fflush(stdin);

        while (input[0] == '\n' || strlen(input) != 2 || (input[0] != 'M' && input[0] != 'F'))
        {
            fprintf(stderr, "Incorrect input\nEnter sex: ");
            fgets(input, 100, stdin);
            fflush(stdin);
        }

        new->sex = input[0];
    }

    /* Get age input*/
    {
        char input[100];        /* Create a string to take in input for the age */
        int i;                  /* Create int for the for loop */
        int correct_input = 0;  /* Boolean to check if value age is corrected */

        /* Prompt user for input */
        fprintf(stderr, "Enter age: ");

        /* Get age input */
        fgets(input, 100, stdin);
        fflush(stdin);

        /* While loop to check if a legal value for input has been achieved */
        while (correct_input == 0)
        {
            /* Set boolean to true (1). This might be changed to false (0) as
            * the code progresses
            */
            correct_input = 1;

            /* If age not entered, ask for input again */
            if (input[0] == '\n')
            {
                fprintf(stderr, "Incorrect input\nEnter age: ");
                fgets(input, 100, stdin);
                fflush(stdin);

                /* Set boolean to false as the correct value has not yet been achieved */
                correct_input = 0;
            }    

            /* Loop through the user input and check if any characters in the array are
            * not digits. If there are non-digit characters, then ask for input again
            */
            for (i = 0; i < strlen(input) - 1; i++)
            {
                if (!isdigit(input[i]) || input[i] == ' ')
                {
                fprintf(stderr, "Incorrect input\nEnter age: ");
                fgets(input, 100, stdin);
                fflush(stdin);
                i = 0;   /* Reset counter for new input */

                /* Set boolean to false as the correct value has not yet been achieved */
                correct_input = 0;
                }
            }

            /* Convert input (string) to an int and assign it to the age variable in
            * the employee struct
            */
            new->age = atoi(input);

            if (new->age < 1)
            {
                fprintf(stderr, "Incorrect input\nEnter age: ");
                fgets(input, 100, stdin);
                fflush(stdin);

                /* Set boolean to false as the correct value has not yet been achieved */
                correct_input = 0;
            }
        }

    }

    /* Get job input */
    {
        int i;         /* Create int for the for loop */

        /* Prompt user for input */
        fprintf(stderr, "Enter job: ");

        /* Get job input */
        fgets(new->job, MAX_JOB_LENGTH, stdin);
        fflush(stdin);

        /* If nothing is entered into job, ask for input again */
        while (new->job[0] == '\n')
        {
            fprintf(stderr, ("Incorrect input\nEnter job: "));
            fgets(new->job, MAX_NAME_LENGTH, stdin);
            fflush(stdin);
        }

        /* Loop through all the characters in the job array */
        for (i = 0; i<strlen(new->job)-1; i++)
        {
            /* If job is not entered or if there is a non-alphabetical char
            * in the array, ask for input again */
            while (!isalpha(new->job[i]) && new->job[i] != ' ')
            {
                fprintf(stderr, "Incorrect input\nEnter job: ");
                fgets(new->job, MAX_JOB_LENGTH, stdin);
                fflush(stdin);
                i = 0;   /* Reset counter for new input */
            }
        }
    }



    /* Increments the number of employees in the array by 1 */
    no_employees++;
}

static void sort(void)
{
   char temp[MAX_NAME_LENGTH];            /* Array to temporarily hold name */
   int j, i;                              /* Create int for the for loops */

   /* Bubble sort */
   for (j = 0; j < no_employees - 1; j++)       /* Create primary for loop */
   {
      for (i = j+1; i < no_employees; i++)      /* Create nested secondary for loop */
      {
         if (strcmp(employee_array[j].name, employee_array[i].name) > 0)      /* Swap the string if the jth string is larger than ith string */
         {
            strcpy(temp, employee_array[j].name);                             /* Copy jth element of the employee_array into the temporary array*/
            strcpy(employee_array[j].name, employee_array[i].name);           /* Copy ith element of the employee_arry into the jth value */
            strcpy(employee_array[i].name, temp);                             /* Copy the temporary array into the ith value of employee array */
         }
      }
   }
}

/* menu_add_employee():
 *
 * Add new employee to database
 */
static void menu_add_employee(void)
{
   read_employee();
}

/* menu_print_database():
 *
 * Print database of employees to standard output.
 */
static void menu_print_database(void)
{
   int i;

   sort();

   for (i = 0; i < no_employees; i++)
   {
      printf("Name: %s\n", employee_array[i].name);
      printf("Sex: %c\n", employee_array[i].sex);
      printf("Age: %i\n", employee_array[i].age);
      printf("Job: %s\n\n", employee_array[i].job);
   }
}	    

/* menu_delete_employee():
 *
 * Delete new employee from database.
 */
static void menu_delete_employee(void)
{
    /* Variables */
    char employee_to_delete[MAX_NAME_LENGTH+1];     /* Employee to delete name string */
    int i;                      /* Int for for loop index */
    int delete_pos;             /* position of employee to delete in the employee_array */ 
    int employee_exists = 0;    /* Boolean to check if the selected employee to delete exists or not */

    /* Prompt user to input name of the employee to delete */
    fprintf(stderr, "Enter name of employee to delete: ");

    /* Get name of employee to delete */
    fgets(employee_to_delete, MAX_NAME_LENGTH, stdin);
    fflush(stdin);

    /* If name is not entered or if the input doesn't start with a alphabetical char, 
    * send message to stderr and ask for name again */
    while (employee_to_delete[0] == '\n' || !isalpha(employee_to_delete[0]))
    {
        fprintf(stderr, ("Incorrect input\nEnter name: "));
        fgets(employee_to_delete, MAX_NAME_LENGTH, stdin);
        fflush(stdin);
    }

    /* If the length of the name array is more than 0 AND the last element in
    * the array is a '\n' then set it to '\0' to remove the "new line." */
    if (strlen(employee_to_delete) > 0 && employee_to_delete[strlen(employee_to_delete) - 1] == '\n')
    {
        employee_to_delete[strlen(employee_to_delete) - 1] = '\0';
    }

    for (i = 0; i < no_employees; i++)
    {
        if (strcmp(employee_to_delete, employee_array[i].name) == 0)
        {
            delete_pos = i;
            employee_exists = 1;
        }
    }

    /* If employee to delete exists, delete. Else, print to stderr and do nothing. */
    if (employee_exists)
    {
        for (i = delete_pos; i < no_employees-1; i++)
        {
            /* Replace the name string in employee_array with the next
            * string from the delete_pos up to the end of the end of the
            * employee_array
            * */
            strcpy(employee_array[i].name, employee_array[i+1].name);
        }

        /* Decrement the number of employees by 1 after the employee is deleted */
        no_employees--;
    }
    else
    {
        fprintf(stderr, "Employee not found\n");
    }
}

/* read file containing database of employees */
static void read_employee_database (char *file_name)
{
    char string[MAX_NAME_LENGTH];   /* String to hold data of the current line */
    char prefix[MAX_NAME_LENGTH];   /* String to hold prefixes in the file */

    FILE *fp;
    fp = fopen(file_name, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error: Couldn't open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    read_string(fp, string, prefix);

    fclose(fp);
}







/* codes for menu */
#define ADD_CODE    0
#define DELETE_CODE 1
#define PRINT_CODE  2
#define EXIT_CODE   3

int main ( int argc, char *argv[] )
{
   /* check arguments */
   if ( argc != 1 && argc != 2 )
   {
      fprintf ( stderr, "Usage: %s [<database-file>]\n", argv[0] );
      exit(-1);
   }

   /* read database file if provided, or start with empty database */
   if ( argc == 2 )
      read_employee_database ( argv[1] );

   for(;;)
   {
      int choice, result;
      char line[301];

      /* print menu to standard error */
      fprintf ( stderr, "\nOptions:\n" );
      fprintf ( stderr, "%d: Add new employee to database\n", ADD_CODE );
      fprintf ( stderr, "%d: Delete employee from database\n", DELETE_CODE );
      fprintf ( stderr, "%d: Print database to screen\n", PRINT_CODE );
      fprintf ( stderr, "%d: Exit database program\n", EXIT_CODE );
      fprintf ( stderr, "\nEnter option: " );

      if ( read_line ( stdin, line, 300 ) != 0 ) continue;

      result = sscanf ( line, "%d", &choice );
      if ( result != 1 )
      {
	      fprintf ( stderr, "corrupted menu choice\n" );
	      continue;
      }

      switch ( choice )
      {
         case ADD_CODE: /* add employee to database */
	         menu_add_employee();
	         break;

         case DELETE_CODE: /* delete employee from database */
	         menu_delete_employee();
	         break;

         case PRINT_CODE: /* print database contents to screen
			     (standard output) */
            menu_print_database();
            break;

	      /* exit */
         case EXIT_CODE:
	         break;

         default:
      fprintf ( stderr, "illegal choice %d\n", choice );
      break;
         }

         /* check for exit menu choice */
         if ( choice == EXIT_CODE )
      break;
   }

   return 0;   
}
