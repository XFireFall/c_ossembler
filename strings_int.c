#include "strings_int.h"

//==========================__ISTEXT__==========================
// Checks if given code of a symbol is a letter of English or Russian alphabet
int istext(char c)
{
    return 1;

    if(c > 'z')
        return 2;
    else
        return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))) ? 1 : 0;
}

//==========================__PRINTTEXT__=======================
// Prints given number of strings (as integer arrays) from array
void printtext(char** txt_arr, int num_of_lines)
{
    assert(txt_arr != NULL);

    if(!txt_arr)
        return;

    for(int i = 0; i < num_of_lines; ++i)
        printf("%s\n", txt_arr[i]);

    return;
}

//==========================__PRINT_ORIGINAL_TEXT__=============
// Prints given number of strings (as integer arrays) from the place that used to be a single string (as integer array)
int print_original_text(char* text, int num_of_lines)
{
    assert(text != NULL);

    if(!text)
        return 1;

    while(num_of_lines > 0)
    {
        printf("%c", (*text) ? *text : '\n');
        ++text;
        if(!*text)
            --num_of_lines;
    }
    return 0;
}

//==========================__FGETFILE__========================
// Creates a string (as integer array) which contains entire given file
char* fgetfile(char* filename, int* num_of_lines)
{
    assert(filename != NULL);
    assert(num_of_lines != NULL);

    if(!filename)
        return NULL;
    if(!num_of_lines)
        return NULL;

    struct stat st = {};
    stat(filename, &st);

    int len = (int) st.st_size;
    if(!len)
    {
        printf("No file or it is empty\n");
        return NULL;
    }

    FILE *infile = fopen(filename, "r");
    char* text = (char*) calloc(len, sizeof(*text));
    char* txt_p = text - 1;

    char c;
    while((c = getc(infile)) != EOF)
        if((*++txt_p = c) == '\n')
            ++*num_of_lines;

    *txt_p = '\0';

    fclose(infile);
    return text;
}

//==========================__SLICEDTEXT__======================
// Slices given string (as integer array) into continuous set of strings (as integer arrays) by changing '\n' to '\0'
// Creates array of strings (as integer arrays) containing strings (as integer arrays) from the sliced one
char** slicedtext(char* text, int num_of_lines)
{
    assert(text != NULL);

    if(!text)
        return NULL;

    char** txt_arr= (char**) calloc(num_of_lines, sizeof(*txt_arr));
    char** txt_arr_p = txt_arr;

    *txt_arr_p++ = text;
    while(*text)
        if(*text++ == '\n')
        {
            *(text - 1) = '\0';
            /*while(isspace(*text))
                text++;
            if(*text == '\0')
                text -= 2;*/
            *txt_arr_p++ = text;
        }
    return txt_arr;
}

//==========================__LINE_COMP__=======================
// Compares strings (as integer arrays) from their beginning
// Ignores non-text symbols
int linecomp(const char* a, const char* b)
{
    assert(a != b);

    assert(a != NULL);
    assert(b != NULL);

    while(!istext(*a))
        a++;
    while(!istext(*b))
        b++;

    while(*a == *b)
    {
        if(*a == '\0')
            return 0;

        do
        {
            ++a;
        } while(!istext(*a) && *a);

        do
        {
            ++b;
        } while(!istext(*b) && *b);
    }
    return *a - *b;
}

//==========================__REV_LINE_COMP__===================
// Compares strings (as integer arrays) from their ending
// Ignores non-text symbols
int rev_linecomp(const char* a, const char* b)
{
    assert(a != b);

    assert(a != NULL);
    assert(b != NULL);

    const char* a0 = a;
    const char* b0 = b;

    while(*a)
        a++;
    while(*b)
        b++;

    if (a == a0 || b == b0)
        return *a - *b;

    while(*a == *b)
    {
        do
        {
            --a;
        } while(!istext(*a) && a != a0);

        do
        {
            --b;
        } while(!istext(*b) && b != b0);

        if (a == a0 || b == b0)
            break;
    }
    return *a - *b;
}

//==========================__QSORT_TEXT_ARRAY__================
// Sorts some number of strings (as integer arrays) from given array by given comparing function
// Arg = '+' for detailed info
int qsort_text_array(char** txt_arr, int num_of_lines, int (*line_comp_func)(const char*, const char*), char arg)
{
    assert(txt_arr != NULL);
    assert(line_comp_func != NULL);

    if(!txt_arr || !line_comp_func)
        return 0;

    //**

    if(num_of_lines <= 1)
        return 1;

    int base = num_of_lines - 1;
    int check = 0;
    int num_of_self_recourses = 1;

    if(arg == '+')
    {
        printf("___\n"
               "-> new reference, <base> = (");
        printf("%s", txt_arr[base]);
        printf("):\n");
        printtext(txt_arr, num_of_lines);
    }

    while(check < base)
        if((*line_comp_func)(txt_arr[check], txt_arr[base]) >= 0)
        {
            char*            t = txt_arr[check];
            txt_arr[check]    = txt_arr[base - 1];
            txt_arr[base - 1] = txt_arr[base];
            txt_arr[base]     = t;
            --base;
            if(arg == '+')
            {
                printf("%s", txt_arr[check]);
                printf(" >= ");
                printf("%s", txt_arr[base]);
                printf("\n");
                printtext(txt_arr, num_of_lines);
            }
        }
        else
            ++check;

    //**

    num_of_self_recourses += qsort_text_array(txt_arr, base, line_comp_func, arg);
    num_of_self_recourses += qsort_text_array(txt_arr + base + 1, num_of_lines - base - 1, line_comp_func, arg);
    return num_of_self_recourses;
}


