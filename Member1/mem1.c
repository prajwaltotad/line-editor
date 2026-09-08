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
        newText
    );


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
            newText
        );


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