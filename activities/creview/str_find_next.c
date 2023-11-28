#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 *
 * find_next_field()
 *
 * Given an input string (represented by a char pointer), the string can be
 * divided into multiple fields by using a certain separator. In this problem,
 * the separator is defined as as DOUBLE SPACE, i.e., "  ".
 *
 * Given the input pointer as the starting point, the function will find the
 * position of the beginning of the next field. Namely, you need to find the
 * position of the next double space and then return the pointer that points to
 * the character right after the double space.
 *
 * If no separator is found, the function returns the original pointer.
 *
 * For example:
 * 1)
 *  char* message = "0123  45."
 *  char* ret = find_next_field(message)
 *
 *  "0123  45."
 *         |
 *         |
 *         v
 *        ret
 *  //ret == message + 6
 *  //now ret points to the position shown above
 *
 * 2)
 *  char* message = "find next  field."
 *  char* ret = find_next_field(message)
 *
 *  "find next  field."
 *              |
 *              |
 *              v
 *             ret
 *  //ret == message + 11
 *  //now ret points to the position shown above
 *
 * YOU MAY NOT CALL OTHER FUNCTIONS FROM string.h EXCEPT FOR strlen() and
 * strncmp().
 *
 * @param input - the input string to search
 * @returns a pointer to the beginning of the next field.
 */
char *find_next_field(char *input) {
  // Your code goes here.
  
  //Initialize vars
  int len = strlen(input);
  int foundSeparator = 0;
  int start = 0;

  //Search for '  '
  for (int i = 0; i < len - 1; i++){
    if (input[i] == 32){
      if (input[i+1] == 32){
        foundSeparator = 1;
        start = i;
        start++;
        start++;
        break;
      }
    }
  }

  //Begin alt exit
  if (foundSeparator == 1){
    char *s = malloc(len - start);

    //Copy string
    for (int i = start; i < len; i++){
      s[i-start] = input[i];
    }
 
    //Alt exit
    return s;
  }

   //Default exit
  return input;
}

//
// add a main function to test the following inputs:
//
// Test case 1:
//  Input: "a  b"
//  Expected output: "b"
//
// Test case 2:
//  Input: "A  big long test  string with* things    in it."
//  Expected output: "big long test  string with* things    in it."
//
// Test case 3:
//  Input: "big long test  string with* things    in it."
//  Expected output: "string with* things    in it."
//
//  Note: Do not create a new test string for test 3, use the output of test 2.
//

// Your code starts here.

int  main(void){
  char test1[] = "a  b";
  char sol1[] = "b";
  char *output = find_next_field(test1);

  printf("Test case 1:\n");
  printf("\tInput: \"%s\"\n", test1);
  printf("\tExpected output: \"%s\"\n", sol1);
  printf("\tActual output: \"%s\"\n", output);

  char test2[] = "A  big long test  string with* things    in it.";
  char sol2[] = "big long test  string with* things    in it.";
  output = find_next_field(test2);
  
  printf("Test case 2:\n");
  printf("\tInput: \"%s\"\n", test2);
  printf("\tExpected output: \"%s\"\n", sol2);
  printf("\tActual output: \"%s\"\n", output);
 

  char sol3[] = "string with* things    in it.";
  output = find_next_field(sol2);

  printf("Test case 3:\n");
  printf("\tInput: \"%s\"\n", sol2);
  printf("\tExpected output: \"%s\"\n", sol3);
  printf("\tActual output: \"%s\"\n", output);
}
