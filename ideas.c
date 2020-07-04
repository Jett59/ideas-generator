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

typedef struct WordsInfo
{
    int wordCount;
    String *words;
} WordsInfo;

FileInfo *openFile(const char *);
WordsInfo *wordsIn(FileInfo *);

int main()
{
    FileInfo *categories = openFile("categories.txt");
    FileInfo *purposes = openFile("nounlist.txt");
    printf("%s\n", purposes->chars);
    printf("%s\n\n", categories->chars);
    WordsInfo *wordsInCategories = wordsIn(categories);
    void *wordsInCategoriesAnkor = (void *)wordsInCategories->words;
    while (*wordsInCategories->words != 0)
    {
        printf("%s ", *wordsInCategories->words);
        wordsInCategories->words++;
    }
    wordsInCategories->words = (String *)wordsInCategoriesAnkor;
    printf("\n\n");
    printf("Categories count: %d\n", wordsInCategories->wordCount);

    WordsInfo *wordsInPurposes = wordsIn(purposes);
    printf("Purpose count: %d\n", wordsInPurposes->wordCount);

    free(wordsInPurposes->words);
    free(wordsInPurposes);
    
    free(wordsInCategories->words);
    free(wordsInCategories);

    free(categories->chars);
    free(categories);
    
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

WordsInfo *wordsIn(FileInfo *fileInfo)
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

    WordsInfo *wordsInfo = malloc(sizeof(WordsInfo));
    wordsInfo->wordCount=wordCount;
    wordsInfo->words=words;
    return wordsInfo;
}
