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