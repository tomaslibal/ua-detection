/*
  A simple reader program

  Reads the input data file from <repo_root>/data/user_agents.txt and prints
  each line's user-agent string out.

*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("../data/user_agents.txt", "r");
    if(fp == NULL) {
        exit(EXIT_FAILURE);
    }

    // keeps the count of how many user-agents have been read
    int counter = 0;
    // data column reserved for future use
    int n1;
    // data column reserved for future use
    int n2;
    // buffer for the user-agent strings
    char ua_string[256];
    while(fscanf(fp, "%d %d %256[^\t\n]", &n1, &n2, &ua_string) == 3) {
        printf("user-agent string = %s\n", ua_string);
        counter++;
    }

    printf("Done\nRead %d user-agent values\n", counter);

    fclose(fp);
    return 0;
}