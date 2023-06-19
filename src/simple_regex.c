/*
 * The simple implementation of a regex engine below mimics grep in UNIX.
 * This implemetation is, however, separable from it's surroundings.
 * It simply reads from standard input or a sequence of files and prints the lines that match the regex, as grep does.
 *
 * This very minimal implementation handles these constructs:
 *  c matches any literal character c
 *  . matches any single character
 *  ^ matches the beginning of the input string
 *  $ matches the end of the input string
 *  * matches zero or more occurences of the previous character
 * 
 * Feel free to extend functionality!
 */

#include <stdio.h>

/* 
 * Tests whether there is an occurrence of the regex anywhere in the input string.
 * If the regex starts with ^ then the match can only be at the beginning of the input string.
 * Otherwise, the match can happen anywhere within the input string
 * 
 * Parameters:
 *  regexp - pointer to a C character string with the regex pattern
 *  text - pointer to a C character string, the input string
 * 
 * Returns:
 *  1 if a match is found, 0 if not
 */
int match(char *regexp, char *text)
{
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* 
 * Tests whether the regular expression matches the text that begins at the current input string pointer.
 * If the match fails, there can be no match at this text position and matchhere returns 0.
 * If the match succeeds, however, it's possible to advance to the next character of the regular expression 
 * and the next character of the text. This is done by calling matchhere recursively.
 * 
 * Parameters:
 *  regexp - pointer to a C character string with the regex pattern
 *  text - pointer to a C character string, the input string
 * Returns:
 *  1 if a match is found, 0 if not
 */
int matchhere(char *regexp, char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return matchhere(regexp+1, text+1);
    return 0;
}

/* 
 * Tries to match repetitions of the text character c, beginning with zero repetitions and counting up, 
 * until it either finds a match of the rest of the text, or it fails and thus concludes that there is no match.
 * 
 * Parameters:
 *  c - character to match
 *  regexp - pointer to a C character string with the regex pattern
 *  text - pointer to a C character string, the input string
 * 
 * Returns:
 *  1 if a match is found, 0 if not
 */
int matchstar(int c, char *regexp, char *text)
{
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
