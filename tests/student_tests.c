#include "unit_tests.h"

/* Was interfering with my compilation */
TestSuite(student_tests, .timeout=TEST_TIMEOUT);


/* Put a lot of time to figure out how to compile my source code and the student tests together in order to run my main
Added to gitignore because I am kind of reaching out of my bag here, I didn't want to acidentally modify something I wasnt supposed to
Ran gcc -g -std=gnu11 -I include src/hw7.c tests/student_tests.c -o bin/mtxOut -lm 
To run for specific script, scriptx, type ./bin/mtxOut -p ./tests.in/scriptx.txt into terminal*/

/* MAIN FUNCTION SO THAT I CAN PRINT OUT MATRICES USING TERMINAL COMMANDS, FOR DEBUGGING */
/* int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("MAIN: ARUGMENT COUNT ERROR");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-p") == 0) {
        char *script = argv[2];
        
        matrix_sf *final = execute_script_sf(script);

        if (final == NULL) {
            perror("MAIN: MATRIX OUTPUT NULL ERROR");
            exit(EXIT_FAILURE);
        }
        else {
            print_matrix_sf(final);
        }
    }
    else {
        perror("MAIN: DIDNT DO DA P ERROR");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
} */
