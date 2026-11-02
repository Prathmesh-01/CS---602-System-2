#include<stdio.h>

int match_word(char *buff, int size, int *i, char *word)
{
    int done = 0;
    int prev = *i;
    int j = 0;

    while(*i < size && buff[*i] == word[j] && word[j] != '\0')
    {
        *i = *i + 1;
        j++;
    }

    if(word[j] == '\0' &&
       (*i == size || buff[*i] == ' ' ||
        buff[*i] == '\t' || buff[*i] == '\r' ||
        buff[*i] == '\n'))
    {
        done = 1;
    }

    if(done != 1)
        *i = prev;

    return done;
}

int my_strlen(char *s)
{
    int i = 0;

    while(s[i] != '\0')
        i++;

    return i;
}

int my_strcmp(char *s1, char *s2)
{
    int i = 0;

    while(s1[i] != '\0' && s2[i] != '\0')
    {
        if(s1[i] != s2[i])
            return 1;

        i++;
    }

    if(s1[i] == '\0' && s2[i] == '\0')
        return 0;

    return 1;
}

void my_strcpy(char *dest, char *src)
{
    int i = 0;

    while(src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
}

void skip_spaces(char *buff, int size, int *i)
{
    while(*i < size &&
          (buff[*i] == ' ' || buff[*i] == '\t' || buff[*i] == '\r'))
        *i = *i + 1;
}

void read_word(char *buff, int size, int *i, char *word)
{
    int j = 0;

    while(*i < size &&
          buff[*i] != ' ' &&
          buff[*i] != '\t' &&
          buff[*i] != '\n' &&
          buff[*i] != ',' &&
          buff[*i] != '\r')
    {
        word[j] = buff[*i];
        j++;
        *i = *i + 1;
    }

    word[j] = '\0';
}

void expand_macro(char body[][256], int body_count,
                  char args[][50], int arg_count)
{
    for(int i = 0; i < body_count; i++)
    {
        int j = 0;

        while(body[i][j] != '\0')
        {
            if(body[i][j] == '%' &&
               body[i][j+1] >= '1' &&
               body[i][j+1] <= '9')
            {
                int index = body[i][j+1] - '1';

                if(index < arg_count)
                    printf("%s", args[index]);

                j = j + 2;
            }
            else
            {
                printf("%c", body[i][j]);
                j++;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./getmacro <Assembly_File>\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if(fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    char line[256];

    char macro_name[50];
    int macro_arg_count = 0;

    char macro_body[50][256];

    int body_count = 0;
    int inside_macro = 0;

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        int i = 0;
        int size = my_strlen(line);

        // Find macro definition
        if(match_word(line, size, &i, "%macro"))
        {
            skip_spaces(line, size, &i);

            read_word(line, size, &i, macro_name);

            skip_spaces(line, size, &i);

            char number[10];

            read_word(line, size, &i, number);

            macro_arg_count = 0;

            for(int j = 0; number[j] >= '0' && number[j] <= '9'; j++)
            {
                macro_arg_count = macro_arg_count * 10 +
                                  (number[j] - '0');
            }

            inside_macro = 1;
            body_count = 0;

            continue;
        }

        // Find endmacro
        i = 0;

        if(match_word(line, size, &i, "%endmacro"))
        {
            inside_macro = 0;
            continue;
        }

        // Store macro body
        if(inside_macro)
        {
            if(body_count < 50)
            {
                my_strcpy(macro_body[body_count], line);
                body_count++;
            }
            else
            {
                printf("Macro body limit exceeded\n");
                fclose(fp);
                return 1;
            }

            continue;
        }

        // Check macro invocation
        i = 0;

        char word[50];

        skip_spaces(line, size, &i);

        read_word(line, size, &i, word);

        if(my_strcmp(word, macro_name) == 0)
        {
            char args[10][50];

            if(macro_arg_count > 10)
            {
                printf("Maximum 10 arguments supported\n");
                fclose(fp);
                return 1;
            }

            for(int j = 0; j < macro_arg_count; j++)
            {
                skip_spaces(line, size, &i);

                if(i < size && line[i] == ',')
                    i++;

                skip_spaces(line, size, &i);

                read_word(line, size, &i, args[j]);
            }

            expand_macro(macro_body, body_count,
                         args, macro_arg_count);
        }
        else
        {
            printf("%s", line);
        }
    }

    fclose(fp);

    return 0;
}
