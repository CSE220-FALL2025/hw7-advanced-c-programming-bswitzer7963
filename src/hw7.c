#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    return NULL;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    return NULL;
}

void free_bst_sf(bst_sf *root) {
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    return NULL;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
   return NULL;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    return NULL;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {  
    int nrow, ncol;

    int dims = sscanf(expr, " %d %d [", &name, &nrow, &ncol);

    if (dims == 2) {
        int cur = strchr(str, '[');
        if (cur == NULL) {
            perror("MATRIX ENTRY NULL ERROR");
            exit(EXIT_FAILURE);
        }


        while((cur != '\0') && (cur != ']')) {
            int indyrow = 
        }

        }
    return NULL;
}

char* infix2postfix_sf(char *infix) {
    return NULL;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    return NULL;
}

matrix_sf *execute_script_sf(char *filename) {
    char *str = NULL;
    size_t max_line_size = MAX_LINE_LEN;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("OPEN SCRIPT ERROR");
        exit(EXIT_FAILURE);
    }
    
    /* Initialized bst root to NULL so that it can be pointed to on call */
    bst_sf *root = NULL;
    
    while ((line = getline(&str, &max_line_size, file)) != 1) {
        if (str == NULL) {
            perror("BUFFER NULL ERROR");
            exit(EXIT_FAILURE);
        }
        
        char name;
        /* If row exists, then we move to create, value irrelevant */
        int row;
        /* Get number of inputs recieved by sscanf to figure out if line is a creation or operation */
        int isCreate = sscanf(str, " %c = %d ", &name, &row);

        if (isCreate == 2)  {
            char *expr = strchr(str, '=');
            /* I dont think it can be NULL because isCreate already == 2, but just in case */
            if ((*expr == NULL) || (*(expr + 1) == NULL)) {
                perror("MCREATE EXPRESSION NULL ERROR");
                exit(EXIT_FAILURE);
            }
            
            /* Increment to after = */
            expr++;

            matrix_sf *mat = create_matrix_sf(name,*expr);
            
            /* Add to BST now */






        }
        /* Else either a matrix operation or an error */
        else: {

        }


    }
    
    free(str);
    fclose(file);
    exit(EXIT_SUCCESS);
   
    return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
