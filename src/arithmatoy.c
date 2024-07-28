#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int VERBOSE = 0;

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} DynamicArray;

// Here are some utility functions that might be helpful to implement add, sub
// and mul:

unsigned int get_digit_value(char digit) {
  // Convert a digit from get_all_digits() to its integer value
  if (digit >= '0' && digit <= '9') {
    return digit - '0';
  }
  if (digit >= 'a' && digit <= 'z') {
    return 10 + (digit - 'a');
  }
  return -1;
}

char to_digit(unsigned int value) {
  // Convert an integer value to a digit from get_all_digits()
  if (value >= ALL_DIGIT_COUNT) {
    debug_abort("Invalid value for to_digit()");
    return 0;
  }
  return get_all_digits()[value];
}

char *reverse(char *str) {
  // Reverse a string in place, return the pointer for convenience
  // Might be helpful if you fill your char* buffer from left to right
  const size_t length = strlen(str);
  const size_t bound = length / 2;
  for (size_t i = 0; i < bound; ++i) {
    char tmp = str[i];
    const size_t mirror = length - i - 1;
    str[i] = str[mirror];
    str[mirror] = tmp;
  }
  return str;
}

const char *drop_leading_zeros(const char *number) {
  // If the number has leading zeros, return a pointer past these zeros
  // Might be helpful to avoid computing a result with leading zeros
  if (*number == '\0') {
    return number;
  }
  while (*number == '0') {
    ++number;
  }
  if (*number == '\0') {
    --number;
  }
  return number;
}

void debug_abort(const char *debug_msg) {
  // Print a message and exit
  fprintf(stderr, debug_msg);
  exit(EXIT_FAILURE);
}


DynamicArray *createDynamicArray() {
    DynamicArray *array = (DynamicArray *) malloc(sizeof(DynamicArray));
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
    return array;
}

void append(DynamicArray *array, char value) {
    if (array->size >= array->capacity) {
        size_t new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        char *new_data = (char *) realloc(array->data, new_capacity * sizeof(char));
        if (new_data == NULL) {
            fprintf(stderr, "Error: could not allocate memory\n");
            exit(EXIT_FAILURE);
        }
        array->data = new_data;
        array->capacity = new_capacity;
    }
    array->data[array->size++] = value;
}

void setDynamicArray(DynamicArray *array, char *data, size_t size, size_t capacity){
  array->data = data;
  array->size = size;
  array->capacity = capacity;
}

void destroyDynamicArray(DynamicArray *array) {
    free(array->data);
    free(array);
}

const char *get_all_digits() { return "0123456789abcdefghijklmnopqrstuvwxyz"; }
const size_t ALL_DIGIT_COUNT = 36;

void arithmatoy_free(char *number) { free(number); }

char* get_reverse(const char* to_reverse, size_t length){
  char *rev = calloc(length, sizeof(char));
  memcpy(rev, to_reverse, length * sizeof(char));
  return reverse(rev);
}

int abs(int value){
  return value < 0 ? -value : value;
}

// Fill the function, the goal is to compute lhs + rhs
// You should allocate a new char* large enough to store the result as a
// string Implement the algorithm Return the result
char *arithmatoy_add(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "add: entering function\n");
  }
  DynamicArray *result = createDynamicArray();

  const char* charset = get_all_digits();

  const char *n_rhs = drop_leading_zeros(rhs);
  const char *n_lhs = drop_leading_zeros(lhs);
  size_t lhs_len = strlen(n_lhs);
  size_t rhs_len = strlen(n_rhs);
  size_t max = lhs_len > rhs_len ? lhs_len : rhs_len;
  char *rev_rhs = get_reverse(n_rhs, rhs_len);
  char *rev_lhs = get_reverse(n_lhs, lhs_len);

  char a, b;
  int retenu = 0;
  char calcul;
  
  for(int i=0; i < max; i++){
    a = lhs_len <= i ? '0' : rev_lhs[i];
    b = rhs_len <= i ? '0' : rev_rhs[i];
    calcul = charset[(get_digit_value(a) + get_digit_value(b) + retenu) % base];
    retenu = (get_digit_value(a) + get_digit_value(b) + retenu) / base;
    if(VERBOSE)
      fprintf(stderr, "add: digit %c digit %c carry %d\n", a, b, retenu);
    append(result, calcul);
  }

  while(retenu != 0){
    calcul = charset[retenu % base];
    retenu = retenu / base;
    append(result, calcul);
  }
  
  append(result, '\0');
  size_t length = result->size;
  char* final_result = calloc(length, sizeof(char));
  memcpy(final_result, result->data, length * sizeof(char));
  destroyDynamicArray(result);
  free(rev_lhs);
  free(rev_rhs);
  return reverse(final_result);
}


// Fill the function, the goal is to compute lhs - rhs (assuming lhs > rhs)
// You should allocate a new char* large enough to store the result as a
// string Implement the algorithm Return the result
char *arithmatoy_sub(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "sub: entering function\n");
  }
  DynamicArray *result = createDynamicArray();

  const char* charset = get_all_digits();

  const char *n_rhs = drop_leading_zeros(rhs);
  const char *n_lhs = drop_leading_zeros(lhs);
  size_t lhs_len = strlen(n_lhs);
  size_t rhs_len = strlen(n_rhs);
  size_t max = lhs_len > rhs_len ? lhs_len : rhs_len;
  char *rev_rhs = get_reverse(n_rhs, rhs_len);
  char *rev_lhs = get_reverse(n_lhs, lhs_len);

  char a, b;
  int retenu = 0;
  int next_retenu = 0;
  int intermediaire;
  int index;
  char calcul;
  for(int i=0; i < max; i++){
    a = lhs_len <= i ? '0' : rev_lhs[i];
    b = rhs_len <= i ? '0' : rev_rhs[i];
    intermediaire = get_digit_value(a) - get_digit_value(b) - retenu;
    if(intermediaire < 0){
      index = base - abs(intermediaire);
      next_retenu = 1;
    } else {
      index = intermediaire;
      next_retenu = 0;
    }
    calcul = charset[index % base];
    retenu = next_retenu;
    if(VERBOSE)
      fprintf(stderr, "add: digit %c digit %c carry %d\n", a, b, retenu);
    append(result, calcul);
  }

  append(result, '\0');
  size_t length = result->size;
  char* final_result = calloc(length, sizeof(char));
  memcpy(final_result, result->data, length * sizeof(char));
  destroyDynamicArray(result);
  free(rev_lhs);
  free(rev_rhs);

  // lhs < rhs
  if(retenu > 0){
    free(final_result);
    return NULL;
  }

  final_result = reverse(final_result);
  final_result = (char*) drop_leading_zeros((const char*) final_result);
  return final_result;
}


char *multiple_adding(DynamicArray **results, size_t length, unsigned int base){
  char *final_result;
  if(length==1){
    return results[0]->data;
  }
  final_result = results[0]->data;
  for(int i=1; i<length; i++){
    final_result = arithmatoy_add(base, (const char*) final_result, (const char*) results[i]->data);
  }
  return final_result;
}

int my_pow(int base, int exp){
  if(exp == 0)
    return 1;
  int res = 1;
  for(int i = 0; i < exp; i++)
    res *= base;
  return res;
}

// Fill the function, the goal is to compute lhs * rhs
// You should allocate a new char* large enough to store the result as a
// string Implement the algorithm Return the result
char *arithmatoy_mul(unsigned int base, const char *lhs, const char *rhs) {
    if (VERBOSE) {
        fprintf(stderr, "mul: entering function\n");
    }

    const char* charset = get_all_digits();

    const char *n_rhs = drop_leading_zeros(rhs);
    const char *n_lhs = drop_leading_zeros(lhs);
    size_t lhs_len = strlen(n_lhs);
    size_t rhs_len = strlen(n_rhs);
    char *rev_rhs = get_reverse(n_rhs, rhs_len);
    char *rev_lhs = get_reverse(n_lhs, lhs_len);

    // Initialize result array with zeroes
    size_t result_len = lhs_len + rhs_len;
    int *result = calloc(result_len, sizeof(int));

    for (size_t i = 0; i < lhs_len; i++) {
        for (size_t j = 0; j < rhs_len; j++) {
            int lhs_digit = get_digit_value(rev_lhs[i]);
            int rhs_digit = get_digit_value(rev_rhs[j]);
            int product = lhs_digit * rhs_digit;
            result[i + j] += product;

            // Handle carry
            size_t k = i + j;
            while (result[k] >= base) {
                result[k + 1] += result[k] / base;
                result[k] %= base;
                k++;
            }
        }
    }

    // Convert result array back to string
    DynamicArray *result_str = createDynamicArray();
    int start = result_len - 1;
    while (start >= 0 && result[start] == 0) {
        start--; // Remove trailing zeros
    }
    for (int i = start; i >= 0; i--) {
        append(result_str, to_digit(result[i]));
    }
    if (start < 0) {
        append(result_str, '0'); // If the result is 0
    }
    append(result_str, '\0');

    // Clean up
    free(result);
    free(rev_lhs);
    free(rev_rhs);

    char *final_result = strdup(result_str->data);
    destroyDynamicArray(result_str);
    return final_result;
}
