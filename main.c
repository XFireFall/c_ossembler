#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings_int.h"

#define MAX_STRLEN (50)
#define MAX_OPNAME (10)
#define MAX_ARGNUM (10)


#define REG_NUM (1)
#define DIR_NUM (0)
#define RAM_POS (-1)

struct Operation
{
    char code;
    char name[MAX_OPNAME];
    int argc;
    int arg_types[MAX_ARGNUM];
    char decl_form[MAX_STRLEN];
};

int hex2(char s0, char s1)
{
    s0 = toupper(s0);
    s1 = toupper(s1);
    int d0 = (isdigit(s0)) ? s0 - '0' : s0 - 'A' + 10;
    int d1 = (isdigit(s1)) ? s1 - '0' : s1 - 'A' + 10;
    return d0 * 16 + d1;
}

int deftype(char c)
{
    int ans = -1;
    switch (c)
    {
    case 'R':
        ans = REG_NUM;
        break;
    case 'k':
        ans = RAM_POS;
        break;
    default:
        ans = DIR_NUM;
        break;
    }
    return ans;
}

int init_operation(char* line, struct Operation* op)
{
    if(!strchr(line, ' '))
        return 0;

    strcpy(op->decl_form, line);
    op->code = hex2(line[0], line[1]);

    sscanf(line, "%*s %s", op->name);

    op->argc = 0;
    int n_words = 0;

    line++;

    while(n_words < 2)
    {
        if(isspace(*line) > isspace(*(line - 1)))
            n_words++;

        if(*line++ == '\0')
            return 1;
    }
    while(isspace(*line))
        line++;

    op->arg_types[0] = deftype(*line);

    while(!isspace(*line))
    {
        if(*line == ',')
        {
            ++op->argc;
            op->arg_types[op->argc] = deftype(*(line + 1));
        }
        line++;
    }
    ++op->argc;

    return 1;
}

int fgetboldline(FILE* infile, char* outline)
{
    char c;
    do
    {
        c = getc(infile);
    } while(isspace(c) && c != EOF);

    ungetc(c, infile);

    while((c = getc(infile)) != '\n' && c != EOF)
        *outline++ = c;
    *outline++ = '\0';

    return c == EOF;
}

void writecode(FILE* codefile, char* line, struct Operation* op, int num_of_ops)
{
    char com_name[MAX_OPNAME];
    sscanf(line, "%s", com_name);
    //printf("-%s-\n", com_name);

    for(int i = 0; i < num_of_ops; ++i, ++op)
        if(!strcmp(op->name, com_name))
        {
            printf("\tfound: %s\n", op->name);
            putc(op->code, codefile);
            break;
        }
    if(op->argc == 0)
        return;

    line = strchr(line, ' ') + 1;
    while(isspace(*line))
        line++;

    int arg = 0;
    while(*line)
    {
        if(isdigit(*line))
            arg = 10 * arg + (*line - '0');
        if(*line == ',')
        {
            printf("(%d) ", arg);
            putc(arg, codefile);
            arg = 0;
        }
        line++;
    }
    printf("(%d)\n", arg);
    putc(arg, codefile);

    return;
}


int main()
{
    int num_of_lines = 0;

    char* text = fgetfile("ossembler.txt", &num_of_lines);
    if(!text)
        return 1;

    char** txt_arr = slicedtext(text, num_of_lines);
    if(!txt_arr)
    {
        DELETE(text);
        return 1;
    }

    //**

    print_original_text(text, num_of_lines);

    //**

    struct Operation ops[100];
    int num_of_operations = 0;

    for(int i = 0; i < num_of_lines; ++i)
        num_of_operations += init_operation(txt_arr[i], &ops[num_of_operations]);

    for(int i = 0; i < num_of_operations; ++i)
    {
        printf("%5s (0x%02X) with %d args with types:\n", ops[i].name, ops[i].code, ops[i].argc);
        for(int k = 0; k < ops[i].argc; ++k)
            printf("\t%d) %d\n", k, ops[i].arg_types[k]);
    }




    printf("\n");




    FILE* prog = fopen("input.txt", "r");
    FILE* code = fopen("code_b", "wb");

    char line[100];
    while(!fgetboldline(prog, line))
    {
        printf("%s:\n", line);
        writecode(code, line, ops, num_of_operations);
    }



    fclose(prog);
    fclose(code);

    //**

    DELETE(txt_arr);
    DELETE(text);
    return 0;
}
