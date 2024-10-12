#include "abc.c"
#include <stdio.h>

int main()
{
    char *str1;
    char *str2;
    char *result;
    char s1[100];
    const char *s2;
    int return_code;
    char num_str[100];

    /* Test cases for MyStrStr */
    printf("=== Testing MyStrStr Function ===\n\n");

    /* Test 1: Normal case */
    printf("Test 1: Normal case\n");
    str1 = "Hello, World!";
    str2 = "World";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in '%s': %s\n\n", str2, str1, result ? result : "Not Found");

    /* Test 2: Substring at the beginning */
    printf("Test 2: Substring at the beginning\n");
    str1 = "Hello";
    str2 = "He";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in '%s': %s\n\n", str2, str1, result ? result : "Not Found");

    /* Test 3: Substring at the end */
    printf("Test 3: Substring at the end\n");
    str1 = "Hello";
    str2 = "lo";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in '%s': %s\n\n", str2, str1, result ? result : "Not Found");

    /* Test 4: Substring not present */
    printf("Test 4: Substring not present\n");
    str1 = "Hello";
    str2 = "World";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in '%s': %s\n\n", str2, str1, result ? result : "Not Found");

    /* Test 5: Empty substring */
    printf("Test 5: Empty substring\n");
    str1 = "Hello";
    str2 = "";
    result = MyStrStr(str1, str2);
    printf("Searching empty string in '%s': %s\n\n", str1, result ? result : "Not Found");

    /* Test 6: Empty base string */
    printf("Test 6: Empty base string\n");
    str1 = "";
    str2 = "Hello";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in empty string: %s\n\n", str2, result ? result : "Not Found");

    /* Test 7: Both strings empty */
    printf("Test 7: Both strings empty\n");
    str1 = "";
    str2 = "";
    result = MyStrStr(str1, str2);
    printf("Searching empty string in empty string: %s\n\n", result ? result : "Not Found");

    /* Test 8: Substring longer than main string */
    printf("Test 8: Substring longer than main string\n");
    str1 = "Hi";
    str2 = "Hello";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in '%s': %s\n\n", str2, str1, result ? result : "Not Found");

    /* Test 9: NULL inputs */
    printf("Test 9: NULL inputs\n");
    str1 = NULL;
    str2 = "Hello";
    result = MyStrStr(str1, str2);
    printf("Searching '%s' in NULL: %s\n\n", str2, result ? result : "Not Found");

    str1 = "Hello";
    str2 = NULL;
    result = MyStrStr(str1, str2);
    printf("Searching NULL in '%s': %s\n\n", str1, result ? result : "Not Found");

    /* Test cases for StringDecToBinary */
    printf("=== Testing StringDecToBinary ===\n\n");

    /* Test 1: Positive number */
    printf("Test 1: Positive number\n");
    strcpy(num_str, "10");
    printf("Binary of '%s': ", num_str);
    return_code = StringDecToBinary(num_str);
    if (return_code != OK)
    {
        printf("Error: Return code %d\n", return_code);
    }
    printf("\n");

    /* Test 2: Zero */
    printf("Test 2: Zero\n");
    strcpy(num_str, "0");
    printf("Binary of '%s': ", num_str);
    return_code = StringDecToBinary(num_str);
    if (return_code != OK)
    {
        printf("Error: Return code %d\n", return_code);
    }
    printf("\n");

    /* Test 3: Negative number (should be invalid input) */
    printf("Test 3: Negative number\n");
    strcpy(num_str, "-5");
    printf("Binary of '%s': ", num_str);
    return_code = StringDecToBinary(num_str);
    if (return_code == OK)
    {
        printf("\n");
    }
    else
    {
        printf("Error: Return code %d\n\n", return_code);
    }

    /* Test 4: Large number */
    printf("Test 4: Large number\n");
    strcpy(num_str, "123456789");
    printf("Binary of '%s': ", num_str);
    return_code = StringDecToBinary(num_str);
    if (return_code != OK)
    {
        printf("Error: Return code %d\n", return_code);
    }
    printf("\n");

    /* Test 5: Non-numeric string */
    printf("Test 5: Non-numeric string\n");
    strcpy(num_str, "abc");
    printf("Binary of '%s': ", num_str);
    return_code = StringDecToBinary(num_str);
    if (return_code == OK)
    {
        printf("\n");
    }
    else
    {
        printf("Error: Return code %d\n\n", return_code);
    }

    /* Test 6: Empty string */
    printf("Test 6: Empty string\n");
    strcpy(num_str, "");
    printf("Binary of empty string: ");
    return_code = StringDecToBinary(num_str);
    if (return_code == OK)
    {
        printf("\n");
    }
    else
    {
        printf("Error: Return code %d\n\n", return_code);
    }

    /* Test cases for Squeeze */
    printf("=== Testing Squeeze ===\n\n");

    /* Test 1: Normal case */
    printf("Test 1: Normal case\n");
    strcpy(s1, "hello world");
    s2 = "lo";
    return_code = Squeeze(s1, s2);
    printf("Squeezing '%s' from '%s': Result: '%s' (Return code: %d)\n\n", s2, "hello world", s1, return_code);

    /* Test 2: Empty s2 */
    printf("Test 2: Empty s2\n");
    strcpy(s1, "hello world");
    s2 = "";
    return_code = Squeeze(s1, s2);
    printf("Squeezing empty string from '%s': Result: '%s' (Return code: %d)\n\n", "hello world", s1, return_code);

    /* Test 3: Empty s1 */
    printf("Test 3: Empty s1\n");
    strcpy(s1, "");
    s2 = "abc";
    return_code = Squeeze(s1, s2);
    printf("Squeezing '%s' from empty string: Result: '%s' (Return code: %d)\n\n", s2, s1, return_code);

    /* Test 4: All characters in s1 are in s2 */
    printf("Test 4: All characters in s1 are in s2\n");
    strcpy(s1, "abc");
    s2 = "abc";
    return_code = Squeeze(s1, s2);
    printf("Squeezing '%s' from '%s': Result: '%s' (Return code: %d)\n\n", s2, "abc", s1, return_code);

    /* Test 5: No characters in s1 are in s2 */
    printf("Test 5: No characters in s1 are in s2\n");
    strcpy(s1, "abc");
    s2 = "xyz";
    return_code = Squeeze(s1, s2);
    printf("Squeezing '%s' from '%s': Result: '%s' (Return code: %d)\n\n", s2, "abc", s1, return_code);

    /* Test 6: s2 contains some characters from s1 */
    printf("Test 6: s2 contains some characters from s1\n");
    strcpy(s1, "hello world");
    s2 = "aeiou";
    return_code = Squeeze(s1, s2);
    printf("Squeezing '%s' from '%s': Result: '%s' (Return code: %d)\n\n", s2, "hello world", s1, return_code);

    /* Test 7: NULL s1 */
    printf("Test 7: NULL s1\n");
    s1[0] = '\0';
    s2 = "abc";
    return_code = Squeeze(NULL, s2);
    printf("Squeezing '%s' from NULL s1: Return code: %d\n\n", s2, return_code);

    /* Test 8: NULL s2 */
    printf("Test 8: NULL s2\n");
    strcpy(s1, "hello world");
    return_code = Squeeze(s1, NULL);
    printf("Squeezing NULL s2 from '%s': Return code: %d\n\n", s1, return_code);

    /* Test 9: Both s1 and s2 are NULL */
    printf("Test 9: Both s1 and s2 are NULL\n");
    return_code = Squeeze(NULL, NULL);
    printf("Squeezing NULL s2 from NULL s1: Return code: %d\n\n", return_code);

    return 0;
}