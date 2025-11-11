#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *nmat, bst_sf *root) {
    if (root == NULL) {
        root = malloc(sizeof(bst_sf));
        root->mat = nmat;
        root->left_child = NULL;
        root->right_child = NULL;
    }
    else {
        /* Assuming all name values are unique */
        if (nmat->name < root->mat->name) {
            root->left_child = insert_bst_sf(nmat, root->left_child);
        }
        else {
            root->right_child = insert_bst_sf(nmat, root->right_child);
        }
    }
    /* Initially returns original root, recursively calls function to see if NULL, if yes, place there, if not, recursively calls... */
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if (root == NULL) {
        return NULL;
    }
    else if (name < root->mat->name) {
        return find_bst_sf(name, root->left_child);
    }
    else if (name > root->mat->name) {
        return find_bst_sf(name, root->right_child);
    }
    /* Only terminates if root == NULL or root has the same name */
    return root->mat;
}

void free_bst_sf(bst_sf *root) {
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    if ((mat1 == NULL) || (mat2 == NULL)) {
        perror("ADD: MATRIX VALIDITY ERROR");
        exit(EXIT_FAILURE);
    }
    if (!((mat1->num_rows == mat2->num_rows) && (mat1->num_cols == mat2->num_cols))) {
        perror("ADD: MATRIX COMPATIBILITY ERROR");
        exit(EXIT_FAILURE);
    }
    
    matrix_sf *sumMat = malloc(sizeof(matrix_sf) + sizeof(int) * mat1->num_rows * mat1->num_cols);
    if (sumMat == NULL) {
        perror("ADD: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }
    sumMat->num_rows = mat1->num_rows;
    sumMat->num_cols = mat1->num_cols;
    for (int i = 0; i < (mat1->num_rows*mat1->num_cols); i++) {
        sumMat->values[i] = (mat1->values[i] + mat2->values[i]);
    }

    return sumMat;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
   return NULL;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    return NULL;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {  
    int nrow, ncol;

    /* Verify structure, irrelevant value beyond that */
    char isBrkt;

    int dims = sscanf(expr, " %d %d %c", &nrow, &ncol, &isBrkt);
    
    /* Allocate storage/initialize matrix */
    matrix_sf *mat = malloc(sizeof(matrix_sf) + sizeof(int) * nrow * ncol);
    mat->name = name;
    mat->num_rows = nrow;
    mat->num_cols = ncol;

    if ((dims == 3) && (isBrkt == '[')) {
        char *cur = strchr(expr, '[');
        if (cur == NULL) {
            perror("MATRIX CREATE: MISSING BRACKET ERROR");
            exit(EXIT_FAILURE);
        }
        cur++;
        
        /* Index for adding values to matrix structure */
        int i = 0;
        int rowCt = 0;
        /* LINK: https://en.cppreference.com/w/c/string/byte/strtol.html is insanely helpful */
        while((*cur != '\0') && (*cur != ']') && (*rowCt < nrow)) {
            int colCt = 0;
            while ((*cur != ';') && (colCt < ncol)) {
                char *end;
                if (!((isspace(*cur) || (isdigit(*cur)) || (*cur == '-')))) {
                    perror("MATRIX CREATE: INVALID ENTRY ERROR");
                    exit(EXIT_FAILURE);
                }
                const long valLong = strtol(cur, &end, 10);
                
                if (cur == end) {
                    perror("MATRIX CREATE: INVALID ENTRY ERROR");
                    exit(EXIT_FAILURE);
                }
                
                /* Noticed that the structs "values" array was int */
                int val = (int)valLong;
                /* The manual for strtol had error handling for this, but I am going to assume that no entry has
                too big of a value for long */
                cur = end;
                
                mat->values[i] = val;
                
                /* Handling for if there is whitespace after final entry */ 
                while (isspace(*cur)) {
                    cur++;
                }
                /* Makes sure that there isnt too many inputs per row */
                colCt++;
                i++;
            }
            
            if ((*cur != ';') || (colCt != ncol)) {
                perror("MATRIX CREATE: COLUMN AMOUNT ERROR");
                exit(EXIT_FAILURE);
            }
            rowCt++;
            
            /* To get rid of whitespace after ';' */
            while (isspace(*cur)) {
                cur++;
        }
        }
        
        if (*cur != ']') {
            perror("MATRIX CREATE: UNCLOSED ENTRIES ERROR");
            exit(EXIT_FAILURE);
        }

        if (rowCt != nrow) {
            perror("MATRIX CREATE: ROW AMOUNT ERROR");
            exit(EXIT_FAILURE);
        }

        }
    else {
        perror("MATRIX CREATE: IMMEDIATE FORMAT ERROR");
        exit(EXIT_FAILURE);
    }
    return *mat;
}

char* infix2postfix_sf(char *infix) {
    return NULL;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    return NULL;
}

matrix_sf *execute_script_sf(char *filename) {
    /* Initialize first root */
    bst_sf *root = NULL;
    char *str = NULL;
    size_t max_line_size = MAX_LINE_LEN;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("OPEN SCRIPT ERROR");
        exit(EXIT_FAILURE);
    }
    
    while ((line = getline(&str, &max_line_size, file)) != -1) {
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

            /* Make Matrix */
            matrix_sf *mat = create_matrix_sf(name, expr);
            
            /* Add to BST */
            root = insert_bst_sf(mat, root)




        }
        /* Else either a matrix operation or an error */
        else {

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

/* BEFORE SUBMITTING CONSIDER THIS: The function must insert  mat  into the BST even if any of 
num_rows  ,  num_cols,  or  values  is invalid. */