#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

// Fonction pour vérifier si un caractère est un délimiteur
bool isDelimiter(char chr) {
    return (chr == ' ' || chr == '+' || chr == '-' ||
            chr == '*' || chr == '/' || chr == ',' ||
            chr == ';' || chr == '%' || chr == '>' ||
            chr == '<' || chr == '=' || chr == '(' ||
            chr == ')' || chr == '[' || chr == ']' ||
            chr == '{' || chr == '}' || chr == '"' || chr == '\'');
}

// Fonction pour vérifier si un caractère est un opérateur
bool isOperator(char chr) {
    return (chr == '+' || chr == '-' || chr == '*' ||
            chr == '/' || chr == '>' || chr == '<' ||
            chr == '=' || chr == '&' || chr == '|');
}

// Fonction pour vérifier si une chaîne est un identifiant valide
bool isValidIdentifier(char *str) {
    return (str[0] != '0' && str[0] != '1' && str[0] != '2' &&
            str[0] != '3' && str[0] != '4' && str[0] != '5' &&
            str[0] != '6' && str[0] != '7' && str[0] != '8' &&
            str[0] != '9' && !isDelimiter(str[0]));
}

// Fonction pour vérifier si une chaîne est un mot-clé
bool isKeyword(char *str) {
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while", "printf", "scanf"
    };
    for (long long unsigned int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Fonction pour vérifier si une chaîne est un entier
bool isInteger(char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i++;
    }
    while (isdigit(str[i])) {
        i++;
    }
    return str[i] == '\0';
}

// Fonction pour vérifier si une chaîne est un nombre flottant
bool isFloat(char *str) {
    int pointCount = 0;
    if (str == NULL || *str == '\0') {
        return false;
    }
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i++;
    }
    while (str[i] != '\0') {
        if (str[i] == '.') {
            pointCount++;
        } else if (!isdigit(str[i])) {
            return false;
        }
        i++;
    }
    return pointCount == 1;
}

// Fonction pour extraire une sous-chaîne
char *getSubstring(char *str, int start, int end) {
    int subLength = end - start + 1;
    char *subStr = (char *)malloc((subLength + 1) * sizeof(char));
    strncpy(subStr, str + start, subLength);
    subStr[subLength] = '\0';
    return subStr;
}

// Fonction pour ignorer les commentaires
void ignoreComment(char *input, int *index) {
    if (input[*index] == '/' && input[*index + 1] == '/') {
        while (input[*index] != '\n' && input[*index] != '\0') {
            (*index)++;
        }
    } else if (input[*index] == '/' && input[*index + 1] == '*') {
        *index += 2;
        while (!(input[*index] == '*' && input[*index + 1] == '/')) {
            (*index)++;
            if (input[*index] == '\0') break;
        }
        *index += 2;
    }
}

// Fonction pour analyser les tokens
int lexicalAnalyzer(char *input) {
    int left = 0, right = 0;
    int len = strlen(input);

    while (right <= len && left <= right) {
        if (input[right] == '/' && (input[right + 1] == '/' || input[right + 1] == '*')) {
            ignoreComment(input, &right);
            left = right;
        } else if (input[right] == '"' || input[right] == '\'') {
            char quote = input[right];
            right++;
            while (right <= len && input[right] != quote) {
                right++;
            }
            right++;
            char *subStr = getSubstring(input, left, right - 1);
            printf("Token: String, Value: %s\n", subStr);
            left = right;
        } else if (!isDelimiter(input[right])) {
            right++;
        }

        if (isDelimiter(input[right]) && left == right) {
            if (isOperator(input[right])) {
                printf("Token: Operator, Value: %c\n", input[right]);
            }
            right++;
            left = right;
        } else if ((isDelimiter(input[right]) && left != right) || (right == len && left != right)) {
            char *subStr = getSubstring(input, left, right - 1);

            if (isKeyword(subStr)) {
                printf("Token: Keyword, Value: %s\n", subStr);
            } else if (isInteger(subStr)) {
                printf("Token: Integer, Value: %s\n", subStr);
            } else if (isFloat(subStr)) {
                printf("Token: Float, Value: %s\n", subStr);
            } else if (isValidIdentifier(subStr) && !isDelimiter(input[right - 1])) {
                printf("Token: Identifier, Value: %s\n", subStr);
            } else if (!isValidIdentifier(subStr) && !isDelimiter(input[right - 1])) {
                printf("Token: Unidentified, Value: %s\n", subStr);
            }
            left = right;
        }
    }
    return 0;
}

// Fonction principale
int main() {
    // Entrée 01
    char lex_input[MAX_LENGTH] = "int a = b + c; // Commentaire";
    printf("For Expression \"%s\":\n", lex_input);
    lexicalAnalyzer(lex_input);
    printf(" \n");

    // Entrée 02
    char lex_input01[MAX_LENGTH] = "float x = 3.14; /* Commentaire \n sur plusieurs lignes */";
    printf("For Expression \"%s\":\n", lex_input01);
    lexicalAnalyzer(lex_input01);
    return 0;
}

//vérification