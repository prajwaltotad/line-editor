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