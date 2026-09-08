#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define MAX_LINE_LENGTH 500

typedef struct {
    char **lines;
    int line_count;
    int capacity;
} Document;

void initializeEditor(Document *doc)
{
    doc->line_count = 0;
    doc->capacity = INITIAL_CAPACITY;

    doc->lines = malloc(doc->capacity * sizeof(char *));

    if (doc->lines == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
}

void insertLine(Document *doc)
{
    int position;
    char text[MAX_LINE_LENGTH];

    printf("Enter line number to insert: ");
    scanf("%d", &position);
    getchar();  

    if (position < 1 || position > doc->line_count + 1)
    {
        printf("Invalid line number.\n");
        return;
    }

    printf("Enter text: ");
    fgets(text, MAX_LINE_LENGTH, stdin);

    text[strcspn(text, "\n")] = '\0';

    if (doc->line_count == doc->capacity)
    {
        doc->capacity *= 2;

        char **temp = realloc(
            doc->lines,
            doc->capacity * sizeof(char *)
        );

        if (temp == NULL)
        {
            printf("Memory allocation failed.\n");
            return;
        }

        doc->lines = temp;
    }

    for (int i = doc->line_count; i >= position; i--)
    {
        doc->lines[i] = doc->lines[i - 1];
    }

    doc->lines[position - 1] = malloc(strlen(text) + 1);

    if (doc->lines[position - 1] == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(doc->lines[position - 1], text);

    doc->line_count++;

    printf("Line inserted successfully.\n");
}

void deleteLine(Document *doc)
{
    int position;

    printf("Enter line number to delete: ");
    scanf("%d", &position);
    getchar();

    if (position < 1 || position > doc->line_count)
    {
        printf("Invalid line number.\n");
        return;
    }

    free(doc->lines[position - 1]);

    for (int i = position - 1; i < doc->line_count - 1; i++)
    {
        doc->lines[i] = doc->lines[i + 1];
    }

    doc->line_count--;

    printf("Line deleted successfully.\n");
}