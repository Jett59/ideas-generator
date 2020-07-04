#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef char *String;
typedef struct FileInfo
{
    const char *name;
    unsigned int size;
    void *ankor;
    char *chars;
    void *end;
} FileInfo;

FileInfo *openFile(const char *);
String *wordsIn(FileInfo *);

int main()
{
    FileInfo *categories = openFile("nounlist.txt");
    FileInfo *purposes = openFile("purpose.txt");
    printf("%s\n", purposes->chars);
    printf("%s\n\n", categories->chars);
    String *wordsInCategories = wordsIn(categories);
    void *wordsInCategoriesAnkor = (void *)wordsInCategories;
    while (*wordsInCategories != 0)
    {
        printf("%s ", *wordsInCategories);
        wordsInCategories++;
    }
    wordsInCategories = (String *)wordsInCategoriesAnkor;
    printf("\n");
    free(categories->chars);
    free(categories);
    free(wordsInCategories);
    free(purposes->chars);
    free(purposes);
}

FileInfo *openFile(const char *name)
{
    FILE *file = fopen(name, "r");
    if (file == 0)
    {
        fprintf(stderr, "error while openning file %s\n", name);
        exit(-1);
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    FileInfo *fileInfo = malloc(1 * sizeof(FileInfo));
    if (fileInfo == 0)
{
        fprintf(stderr, "unable to allocate new filepointer for file %s\n", name);
        exit(-2);
    }
    fileInfo->name = name;
    fileInfo->size = size;
    char *chars = malloc((size + 1) * sizeof(char));
    if (chars == 0)
    {
        fprintf(stderr, "unable to allocate required memory for %s's chars\n", name);
        exit(-2);
    }
    void *ankor = (void *)chars;
    while ((*chars = getc(file)) != -1)
    {
        chars++;
    }
    fclose(file);
    *chars = '\0';
    fileInfo->end = (void *)chars;
    chars = (char *)ankor;
    fileInfo->ankor = ankor;
    fileInfo->chars = chars;
    return fileInfo;
}

String *wordsIn(FileInfo *fileInfo)
{
    unsigned int wordCount = 0;
    for (unsigned int i = 0; i < fileInfo->size; i++)
    {
        if (*(fileInfo->chars) == '\n')
        {
            wordCount++;
        }
        fileInfo->chars++;
    }
    fileInfo->chars = (char *)(fileInfo->ankor);
    String *words = malloc((wordCount + 2) * sizeof(String));
    if (words == 0)
    {
        fprintf(stderr, "error while allocating required memory for storing %d words from file %s\n", fileInfo->size, fileInfo->name);
        exit(-2);
    }
    void *wordsAnkor = (void *)words;
    *words = fileInfo->chars;
    words++;
    for (unsigned int i = 0; i < wordCount; i++)
    {
        while (*(fileInfo->chars++) != '\n')
        {
            continue;
        }
        *(fileInfo->chars - 1) = '\0';
        *words = fileInfo->chars;
        words++;
    }
    fileInfo->chars = (char *)(fileInfo->ankor);
    *words = 0;
    words = (String *)wordsAnkor;
    return words;
}
