/** \file strings_int.h
 */
#ifndef STRINGS_INT_H_INCLUDED
#define STRINGS_INT_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>

#define DELETE( var ) ({free(var); var = NULL;})

//==========================__ISTEXT__==========================
/** \brief Checks if given code of a symbol is a letter of English or Russian alphabet
 *
 * \param n int [in] - a symbol presented as int
 * \return (0) if it is not a letter, (1) if it if English, (2) if it is Russian
 *
 */
int istext(char c);

//==========================__PRINTTEXT__=======================
/** \brief Prints given number of strings (as integer arrays) from array - a string in a line
 *
 * \param txt_arr int** [in] - array of strings (as integer arrays) to print
 * \param num_of_lines int [in] - number of strings to print
 * \return (1) if array not found, (number of failed strings printing) if occurred, (0) if OK
 *
 */
void printtext(char** txt_arr, int num_of_lines);

//==========================__PRINT_ORIGINAL_TEXT__=============
/** \brief Prints given number of strings (as integer arrays) from the place that used to be a single string
 *
 * \param text int* [in] - continuous set of strings (as integer arrays) to print
 * \param num_of_lines int [in] - number of strings to print
 * \return (1) if set (the place) not found, (0) if OK
 *
 */
int print_original_text(char* text, int num_of_lines);

//==========================__FGETFILE__========================
/** \brief Creates a string (as integer array) which contains entire given file
 *
 * \param filename char* [in] - name of the file to read from
 * \param num_of_lines int* [out] - number of lines in the file
 * \return int* - string (as integer array) which have been read
 *
 * \warning Created memory for string must be deleted
 */
char* fgetfile(char* filename, int* num_of_lines);

//==========================__SLICEDTEXT__======================
/** \brief 1. Slices given string (as integer array) into continuous set of strings by changing '\n' to '\0'
 * \brief 2. Creates array of strings (as integer arrays) containing strings from the sliced one
 *
 * \param text int* [in] - string (as integer array) to slice
 * \param num_of_lines int [in] - number of substrings (lines)
 * \return int** - array of ready strings
 *
 * \note Any change of original string will lead to change of array data
 *
 * \warning Created memory for array of strings must be deleted
 */
char** slicedtext(char* text, int num_of_lines);

//==========================__LINE_COMP__=======================
/** \brief Compares strings (as integer arrays) from their beginning
 *
 * \param a int* [in] - first string (as integer array)
 * \param b int* [in] - second string
 * \return (<0) if a < b, (>0) if a > b, (0) if a = b
 *
 * \note Ignores non-text symbols
 */
int linecomp(const char* a, const char* b);

//==========================__REV_LINE_COMP__===================
/** \brief Compares strings (as integer arrays) from their ending
 *
 * \param a int* [in] - first string (as integer array)
 * \param b int* [in] - second string
 * \return (<0) if a < b, (>0) if a > b, (0) if a = b
 *
 * \note Ignores non-text symbols
 */
int rev_linecomp(const char* a, const char* b);

//==========================__QSORT_TEXT_ARRAY__================
/** \brief Sorts some number of strings (as integer arrays) from given array by given comparing function
 *
 * \param txt_arr int** [in] - array of strings (as integer arrays) to sort
 * \param num_of_lines int [in] - number of string to sort
 * \param line_comp_func int (*)(int*, int*) [in] - comparing function
 * \param arg char [in] - detailed info trigger
 * \return int - number of self-recourses
 *
 * \note Arg = '+' for detailed info
 */
int qsort_text_array(char** txt_arr, int num_of_lines, int (*line_comp_func)(const char*, const char*), char arg);


#endif // STRINGS_INT_H_INCLUDED

