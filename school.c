#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct info
{
    char first_name[20];
    char last_name[20];
    int rollNumber;
    float Marks;
};

// Defining Functions...
void search();
void print(struct info *printStudent);
void viewAll();
int isRollNumberExists(int roll);
void App();
void Exit();

int main()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    int input;
    // Heading
    printf("***********************************************\n");
    printf("\tWelcome to Student Record App.\n");
    printf("***********************************************\n");
    
    while (1)
    { // Loop until valid input
        printf("\n[1] Add student details.\n");
        printf("[2] Exit the App.\n");
        printf("[3] View All Student Records\n");
        printf("[4] Search for student by roll No.\n");
        printf("Input(1-4): ");
        scanf("%d", &input);
        printf("\n");
        
        switch (input)
        {
        case 1:
        App();
        break;
        case 2:
        exit(0); // Exit program
        case 3:
        viewAll();
        break;
        case 4:
        search();
        break;
        default:
        printf("Invalid input! Please enter 1, 2, 3, or 4.\n");
        break;
    }
}

return 0;
}

void print(struct info *printStudent)
{
    printf("\nStudent Details:\n");
    printf("-----------------\n");
    printf("First Name : %s\n", printStudent->first_name);
    printf("Last Name  : %s\n", printStudent->last_name);
    printf("Roll Number: %d\n", printStudent->rollNumber);
    printf("Marks      : %.2f\n", printStudent->Marks);
}

void viewAll()
{
    FILE *fp = fopen("record.txt", "r");
    if (fp == NULL)
    {
        printf("No records found yet.\n");
        return;
    }

    char line[100];
    printf("\n===== All Student Records =====\n\n");
    while (fgets(line, sizeof(line), fp))
    {
        // Skip separator lines
        if (strstr(line, "------") == NULL)
        {
            printf("%s", line);
        }
    }
    fclose(fp);
}

void App()
{
    struct info student;

    // Opening File...
    FILE *fp = fopen("record.txt", "a");
    if (fp == NULL)
    { // Check for error...
        printf("Error in file opening.\n");
        exit(1); // Exit program with error code
    }

    // Taking User inputs...
    printf("Enter Name of Student First name: ");
    scanf("%s", student.first_name);

    printf("Enter Name of Student Last name: ");
    scanf("%s", student.last_name);

    int exists;
    do {
        printf("Enter the roll Number: ");
        scanf("%d", &student.rollNumber);
        exists = isRollNumberExists(student.rollNumber);
        if (exists) {
            printf("Roll Number %d already exists! Please enter a unique one.\n", student.rollNumber);
        }
    } while (exists);
    
    do
    {
        printf("Enter the Marks (0-100): ");
        scanf("%f", &student.Marks);
        if (student.Marks < 0 || student.Marks > 100)
        {
            printf("Invalid marks! Please enter valid marks...\n");
        }
    } while (student.Marks < 0 || student.Marks > 100);

    // Asking user if they want to print student info
    printf("Do you want to check Student details (y/n): ");

    while ((getchar()) != '\n')
        ; // Clear input buffer
    char ch = getchar();

    ch = toupper(ch);

    if (ch == 'Y')
    {
        print(&student);
    }

    // Writing to file
    fprintf(fp, "First name: %s\nLast name: %s\nRoll Number: %d\nMarks: %.2f\n\n",
            student.first_name, student.last_name, student.rollNumber, student.Marks);
    fprintf(fp, "------------------------------\n");

    printf("\nStudent details successfully saved to file.\n");

    fclose(fp); // Close file
}

void search()
{
    FILE *fp = fopen("record.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file or file does not exist.\n");
        return;
    }

    int targetRoll, currentRoll, found = 0;
    printf("Enter the roll number to search: ");
    scanf("%d", &targetRoll);

    char line[200];
    char record[5][200]; // store 5 lines of current record
    int i = 0;

    while (fgets(line, sizeof(line), fp))
    {
        strcpy(record[i % 5], line); // save current line
        i++;

        if (strstr(line, "Roll Number:") != NULL)
        {
            sscanf(line, "Roll Number: %d", &currentRoll);

            if (currentRoll == targetRoll)
            {
                printf("\nStudent Found:\n");
                int start = i >= 4 ? i - 4 : 0;
                for (int j = start; j < i; j++)
                {
                    printf("%s", record[j % 5]);
                }
                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        printf("Student with roll number %d not found.\n", targetRoll);
    }

    fclose(fp);
}

int isRollNumberExists(int roll)
{
    FILE *fp = fopen("record.txt", "r");
    if (fp == NULL)
        return 0; // No records yet

    char line[100];
    int currentRoll;

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, "Roll Number:") != NULL)
        {
            sscanf(line, "Roll Number: %d", &currentRoll);
            if (currentRoll == roll)
            {
                fclose(fp);
                return 1; // Found match
            }
        }
    }

    fclose(fp);
    return 0; // Not found
}
