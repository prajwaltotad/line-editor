#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define MAX_LINE_LENGTH 500

typedef struct
{
    char **lines;
    int line_count;
    int capacity;
} Document;

/* Initializes the document and allocates memory for the line array */
void initializeEditor(Document *doc)
{
    doc->line_count = 0;
    doc->capacity = INITIAL_CAPACITY;

    doc->lines = malloc(doc->capacity * sizeof(char *));

    if (doc->lines == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
}

/* Inserts a new line at the specified position */
void insertLine(Document *doc)
{
    int position;
    char text[MAX_LINE_LENGTH];

    printf("Enter line number to insert: ");

    /* Check whether the user entered a valid integer */
    if (scanf("%d", &position) != 1)
    {
        printf("Error: Invalid line number. Please enter an integer.\n");

        /* Clear invalid input */
        while (getchar() != '\n')
            ;

        return;
    }

    getchar();

    /* Check whether the position is valid */
    if (position < 1 || position > doc->line_count + 1)
    {
        printf("Error: Invalid line number.\n");
        return;
    }

    printf("Enter text: ");
    fgets(text, MAX_LINE_LENGTH, stdin);

    /* Remove newline added by fgets() */
    text[strcspn(text, "\n")] = '\0';

    /* Increase capacity if the array is full */
    if (doc->line_count == doc->capacity)
    {
        int new_capacity = doc->capacity * 2;

        char **temp = realloc(
            doc->lines,
            new_capacity * sizeof(char *));

        if (temp == NULL)
        {
            printf("Error: Memory allocation failed.\n");
            return;
        }

        doc->lines = temp;
        doc->capacity = new_capacity;
    }

    /* Shift lines to the right to create space */
    for (int i = doc->line_count; i >= position; i--)
    {
        doc->lines[i] = doc->lines[i - 1];
    }

    /* Allocate memory for the new line */
    doc->lines[position - 1] = malloc(strlen(text) + 1);

    if (doc->lines[position - 1] == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    strcpy(doc->lines[position - 1], text);

    doc->line_count++;

    printf("Line inserted successfully.\n");
}

/* Deletes a line from the document */
void deleteLine(Document *doc)
{
    int position;

    if (doc == NULL || doc->line_count == 0)
    {
        printf("Error: Document is empty.\n");
        return;
    }

    printf("Enter line number to delete: ");

    /* Check whether the user entered a valid integer */
    if (scanf("%d", &position) != 1)
    {
        printf("Error: Invalid line number. Please enter an integer.\n");

        /* Clear invalid input */
        while (getchar() != '\n')
            ;

        return;
    }

    getchar();

    /* Check whether the line number is valid */
    if (position < 1 || position > doc->line_count)
    {
        printf("Error: Invalid line number.\n");
        return;
    }

    /* Free memory occupied by the selected line */
    free(doc->lines[position - 1]);

    /* Shift remaining lines to the left */
    for (int i = position - 1;
         i < doc->line_count - 1;
         i++)
    {
        doc->lines[i] = doc->lines[i + 1];
    }

    doc->line_count--;

    /* Set the unused pointer to NULL */
    doc->lines[doc->line_count] = NULL;

    printf("Line deleted successfully.\n");
}

/* Displays all lines of the document */
void displayDocument(Document *doc)
{
    if (doc == NULL || doc->line_count == 0)
    {
        printf("Document is empty.\n");
        return;
    }

    for (int i = 0; i < doc->line_count; i++)
    {
        printf("%d: %s\n", i + 1, doc->lines[i]);
    }
}

/* Displays lines from startLine to endLine */
void displayRange(Document *doc, int startLine, int endLine)
{ /* Check whether the document is empty */
    if (doc == NULL || doc->line_count == 0)
    {
        printf("Error: Document is empty.\n");
        return;
    } /* Check whether start and end line numbers are valid */
    if (startLine < 1 || endLine < 1 || startLine > doc->line_count || endLine > doc->line_count || startLine > endLine)
    {
        printf("Error: Invalid line range.\n");
        return;
    } /* Display only the requested range */
    for (int i = startLine - 1; i < endLine; i++)
    {
        printf("%d: %s\n", i + 1, doc->lines[i]);
    }
}

/* Searches for a word or phrase in every line of the document */
void findText(Document *doc, const char *search)
{
    int found = 0;

    if (doc == NULL || doc->line_count == 0)
    {
        printf("Document is empty.\n");
        return;
    }

    /* Check every line in the document */
    for (int i = 0; i < doc->line_count; i++)
    {
        /* strstr() checks whether search exists in the current line */
        if (strstr(doc->lines[i], search) != NULL)
        {
            printf("Found \"%s\" at line %d:\n",
                   search, i + 1);

            printf("%d: %s\n",
                   i + 1, doc->lines[i]);

            found = 1;
        }
    }

    /* If the text was not found anywhere */
    if (!found)
    {
        printf("\"%s\" not found in the document.\n", search);
    }
}

/* Replaces all occurrences of oldText with newText in one string */
char *replaceSubstring(const char *original,
                       const char *oldText,
                       const char *newText)
{
    int count = 0;
    const char *ptr;

    /* Check for invalid input */
    if (original == NULL ||
        oldText == NULL ||
        newText == NULL)
    {
        return NULL;
    }

    /* Empty oldText is not allowed */
    if (strlen(oldText) == 0)
    {
        return NULL;
    }

    /* Count occurrences of oldText */
    ptr = original;

    while ((ptr = strstr(ptr, oldText)) != NULL)
    {
        count++;

        /* Move forward to search for the next occurrence */
        ptr += strlen(oldText);
    }

    /*
       If oldText does not occur,
       create and return a copy of the original string.
    */
    if (count == 0)
    {
        char *copy = malloc(strlen(original) + 1);

        if (copy == NULL)
        {
            return NULL;
        }

        strcpy(copy, original);

        return copy;
    }

    size_t oldLen = strlen(oldText);
    size_t newLen = strlen(newText);
    size_t originalLen = strlen(original);

    /*
       Calculate the memory required for the new string.
    */
    size_t resultLen =
        originalLen + count * (newLen - oldLen);

    /* Allocate memory for the new string */
    char *result = malloc(resultLen + 1);

    if (result == NULL)
    {
        return NULL;
    }

    char *dest = result;
    const char *src = original;

    /* Construct the new string */
    while (*src != '\0')
    {
        if (strncmp(src, oldText, oldLen) == 0)
        {
            /* Copy newText instead of oldText */
            memcpy(dest, newText, newLen);

            dest += newLen;
            src += oldLen;
        }
        else
        {
            /* Copy the current character */
            *dest = *src;

            dest++;
            src++;
        }
    }

    /* Add null terminator */
    *dest = '\0';

    return result;
}

/* Replaces a word or phrase only on the specified line */
void replaceTextOnLine(Document *doc,
                       int lineNumber,
                       const char *oldText,
                       const char *newText)
{
    /* Check whether the line number is valid */
    if (doc == NULL ||
        lineNumber < 1 ||
        lineNumber > doc->line_count)
    {
        printf("Error: Invalid line number.\n");
        return;
    }

    /* Check whether the text to replace is empty */
    if (oldText == NULL || strlen(oldText) == 0)
    {
        printf("Error: Text to replace cannot be empty.\n");
        return;
    }

    /* Create a new line with the replacement */
    char *newLine = replaceSubstring(
        doc->lines[lineNumber - 1],
        oldText,
        newText);

    if (newLine == NULL)
    {
        printf("Error: Replacement failed.\n");
        return;
    }

    /* Free the old line */
    free(doc->lines[lineNumber - 1]);

    /* Store the newly created line */
    doc->lines[lineNumber - 1] = newLine;

    printf("Replacement completed on line %d.\n",
           lineNumber);
}

/* Replaces a word or phrase throughout the entire document */
void replaceText(Document *doc,
                 const char *oldText,
                 const char *newText)
{
    int replacements = 0;

    if (doc == NULL || doc->line_count == 0)
    {
        printf("Document is empty.\n");
        return;
    }

    /* Check whether the text to replace is empty */
    if (oldText == NULL || strlen(oldText) == 0)
    {
        printf("Error: Text to replace cannot be empty.\n");
        return;
    }

    /* Go through every line */
    for (int i = 0; i < doc->line_count; i++)
    {
        char *newLine = replaceSubstring(
            doc->lines[i],
            oldText,
            newText);

        if (newLine == NULL)
        {
            printf("Error: Replacement failed on line %d.\n",
                   i + 1);

            continue;
        }

        /* Check whether the line actually changed */
        if (strcmp(doc->lines[i], newLine) != 0)
        {
            replacements++;
        }

        /* Free the old line */
        free(doc->lines[i]);

        /* Store the new line */
        doc->lines[i] = newLine;
    }

    if (replacements == 0)
    {
        printf("\"%s\" was not found.\n", oldText);
    }
    else
    {
        printf("Replacement completed.\n");
    }
}

/* Frees all dynamically allocated memory used by the document */
void freeDocument(Document *doc)
{
    if (doc == NULL)
    {
        return;
    }

    /* Free every individual line */
    for (int i = 0; i < doc->line_count; i++)
    {
        free(doc->lines[i]);
    }

    /* Free the dynamic array of pointers */
    free(doc->lines);

    /* Reset the document */
    doc->lines = NULL;
    doc->line_count = 0;
    doc->capacity = 0;
}

/* Main function controls the line editor */ 
int main()
{
    Document doc;
    char command;
    char search[MAX_LINE_LENGTH];
    char oldText[MAX_LINE_LENGTH];
    char newText[MAX_LINE_LENGTH];
    int lineNumber; /* Variables for display range */
    int startLine;
    int endLine; /* Initialize the document */
    initializeEditor(&doc);
    printf("===== NEXUS LINE EDITOR =====\n");
    while (1)
    {
        printf("\n");
        printf("Commands:\n");
        printf("I - Insert line\n");
        printf("D - Delete line\n");
        printf("P - Display document\n");
        printf("G - Display range of lines\n");
        printf("F - Find text\n");
        printf("R - Replace text\n");
        printf("Q - Quit\n");
        printf("\nEnter command: ");
        scanf(" %c", &command);
        getchar(); /* Convert lowercase command to uppercase */
        if (command >= 'a' && command <= 'z')
        {
            command = command - 'a' + 'A';
        } /* Check whether the entered command is one of the specified commands. */
        if (command != 'I' && command != 'D' && command != 'P' && command != 'G' && command != 'F' && command != 'R' && command != 'Q')
        {
            printf("Error: Invalid command.\n");
            printf("Please enter I, D, P, G, F, R, or Q.\n");
            continue;
        }
        switch (command)
        { /* Insert */
        case 'I':
            insertLine(&doc);
            break; /* Delete */
        case 'D':
            deleteLine(&doc);
            break; /* Display entire document */
        case 'P':
            displayDocument(&doc);
            break; /* Display a specific range of lines */
        case 'G':
            printf("Enter start line: ");
            if (scanf("%d", &startLine) != 1)
            {
                printf("Error: Invalid line number. "
                       "Please enter an integer.\n");
                while (getchar() != '\n')
                    ;
                break;
            }
            printf("Enter end line: ");
            if (scanf("%d", &endLine) != 1)
            {
                printf("Error: Invalid line number. "
                       "Please enter an integer.\n");
                while (getchar() != '\n')
                    ;
                break;
            }
            getchar(); /* Display the requested range */
            displayRange(&doc, startLine, endLine);
            break; /* Find */
        case 'F':
            printf("Enter text to find: ");
            fgets(search, MAX_LINE_LENGTH, stdin);
            search[strcspn(search, "\n")] = '\0';
            if (strlen(search) == 0)
            {
                printf("Error: Search text cannot be empty.\n");
                break;
            }
            findText(&doc, search);
            break; /* Replace */
        case 'R':
            printf("Enter line number (0 for entire document): ");
            if (scanf("%d", &lineNumber) != 1)
            {
                printf("Error: Invalid line number. "
                       "Please enter an integer.\n");
                while (getchar() != '\n')
                    ;
                break;
            }
            getchar(); /* 0 means replace throughout the entire document. */
            if (lineNumber < 0 || lineNumber > doc.line_count)
            {
                printf("Error: Invalid line number.\n");
                break;
            }
            printf("Enter text to replace: ");
            fgets(oldText, MAX_LINE_LENGTH, stdin);
            oldText[strcspn(oldText, "\n")] = '\0';
            if (strlen(oldText) == 0)
            {
                printf("Error: Text to replace cannot be empty.\n");
                break;
            }
            printf("Enter new text: ");
            fgets(newText, MAX_LINE_LENGTH, stdin);
            newText[strcspn(newText, "\n")] = '\0';
            if (lineNumber == 0)
            { /* Replace throughout the document */
                replaceText(&doc, oldText, newText);
            }
            else
            { /* Replace only on the specified line */
                replaceTextOnLine(&doc, lineNumber, oldText, newText);
            }
            break; /* Quit */
        case 'Q':  /* Free all dynamically allocated memory */
            freeDocument(&doc);
            printf("Exiting editor...\n");
            return 0;
        }
    }
}