#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef char *String;
typedef struct fileinfo
{
    const char *name;
    unsigned int size;
    void *ankor;
    char *chars;
    void *end;
} fileinfo;

fileinfo *openFile(const char *);
String *wordsIn(fileinfo *);

int main()
{
    fileinfo *categories = openFile("categories.txt");
    fileinfo *purposes = openFile("purpose.txt");
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

fileinfo *openFile(const char *name)
{
    FILE *file = fopen(name, "r");
    if (file == 0)
    {
        fprintf(stderr, "error while openning file %s\n", name);
        exit(-1);
    }
    unsigned int size = 0;
    while (getc(file) != -1)
    {
        size++;
    }
    rewind(file);
    fileinfo *fp = malloc(1 * sizeof(fileinfo));
    if (fp == 0)
    {
        fprintf(stderr, "unable to allocate new filepointer for file %s\n", name);
        exit(-2);
    }
    fp->name = name;
    fp->size = size;
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
    fp->end = (void *)chars;
    chars = (char *)ankor;
    fp->ankor = ankor;
    fp->chars = chars;
    return fp;
}

String *wordsIn(fileinfo *fp)
{
    unsigned int wordCount = 0;
    for (unsigned int i = 0; i < fp->size; i++)
    {
        if (*(fp->chars) == '\n')
        {
            wordCount++;
        }
        fp->chars++;
    }
    fp->chars = (char *)(fp->ankor);
    String *words = malloc((wordCount + 2) * sizeof(String));
    if (words == 0)
    {
        fprintf(stderr, "error while allocating required memory for storing %d words from file %s\n", fp->size, fp->name);
        exit(-2);
    }
    void *wordsAnkor = (void *)words;
    *words = fp->chars;
    words++;
    for (unsigned int i = 0; i < wordCount; i++)
    {
        while (*(fp->chars++) != '\n')
        {
            continue;
        }
        *(fp->chars - 1) = '\0';
        *words = fp->chars;
        words++;
    }
    fp->chars = (char *)(fp->ankor);
    *words = 0;
    words = (String *)wordsAnkor;
    return words;
}
