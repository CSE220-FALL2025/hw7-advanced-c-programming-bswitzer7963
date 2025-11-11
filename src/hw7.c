/* Braden Switzer 115984660 */
#include "hw7.h"

/* Struct to act as linked list for my stack in evaluate, can access next so I can recursively call for push or pop like functions  */
/* NEEDS AN INITIALIZED BASE, PLACE IN EVALUATE (I THINK) */
/* Next 3 heavily inspired by lecture slides */
// typedef struct stack {
//     matrix_sf *mat;
//     struct stack *next;
// } stack;

// /* Similar to add_to_list() in lecture slides, probably have to free using loop at end */
// stack *push(stack *stk, i) {

// }

// stack *pop() {

// }

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
   if ((mat1 == NULL) || (mat2 == NULL)) {
        perror("MULT: MATRIX VALIDITY ERROR");
        exit(EXIT_FAILURE);
    }
    if (mat1->num_cols != mat2->num_rows) {
        perror("MULT: MATRIX COMPATIBILITY ERROR");
        exit(EXIT_FAILURE);
    }

    matrix_sf *prodMat = malloc(sizeof(matrix_sf) + sizeof(int) * mat1->num_rows * mat2->num_cols);
    if (prodMat == NULL) {
        perror("MULT: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }
    prodMat->num_rows = mat1->num_rows;
    prodMat->num_cols = mat2->num_cols;

    /* i, j for indexing through prodMat and k uses iSum to accumulate the mar1 row and mat2 column products */
    for (int i = 0; i < prodMat->num_rows; i++) {
        for (int j = 0; j < prodMat->num_cols; j++) {
            int iSum = 0;
            for (int k = 0; k < mat1->num_cols; k++){
                iSum += (mat1->values[k + mat1->num_cols * i]) * (mat2->values[j + mat2->num_cols * k]);
            }
            prodMat->values[j + (prodMat->num_cols * i)] = iSum;
        }
    }

    return prodMat;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    if ((mat == NULL)) {
        perror("TRANS: MATRIX VALIDITY ERROR");
        exit(EXIT_FAILURE);
    }

    /* Values[] will be same size as original */
    matrix_sf *transMat = malloc(sizeof(matrix_sf) + sizeof(int) * mat->num_rows * mat->num_cols);
    if (transMat == NULL) {
        perror("TRANS: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }
    
    transMat->num_rows = mat->num_cols;
    transMat->num_cols = mat->num_rows;

    for (int i = 0; i < transMat->num_rows; i++) {
        for (int j = 0; j < transMat->num_cols; j++) {
            transMat->values[i + (transMat->num_cols * j)] = mat->values[j + (mat->num_cols * i)]
        }
    }

    return transMat;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {  
    int nrow, ncol;

    /* Verify structure, irrelevant value beyond that */
    char isBrkt;

    int dims = sscanf(expr, " %d %d %c", &nrow, &ncol, &isBrkt);
    
    /* Allocate storage/initialize matrix */
    matrix_sf *mat = malloc(sizeof(matrix_sf) + sizeof(int) * nrow * ncol);
    if (mat == NULL) {
        perror("CREATE: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }
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

/* For use in creating postfix */
int prec(char op) {
    if (op == '\'') {
        return 3;
    }
    else if (op == '*') {
        return 2;
    }
    else if (op == '+') {
        return 1;
    }
    return -1;
}

/* MAKE SURE TO GET RID OF SPACE TAKEN UP BY THIS AFTER FUNCTION IS DONE */
char* infix2postfix_sf(char *infix) {
    /* sscanf search until find value (Either matrix name, operation or parenthesis) */
    /* Top so that I know where in my array I am to be able to modify the correct values */
    int top = 0;
    size_t len = strlen(*infix);
    /* Made all 0 so that I could check for 0 */
    char opStk[len] = {0};
    char *postfix = malloc(sizeof(char) * (len + 1));
    if (postfix == NULL) {
        perror("IF2P: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }

    while (*infix != '\0') {
        char curVal;
        char num = sscanf(infix, " %c ", &curVal);

        if ((curVal >= 'A') && (curVal <= 'Z')) {
            *postfix = curVal;
            postfix++;
        }
        else if (curVal == '(') {
            opStk[top] = '(';
            top++;
        }
        else if (curVal == ')') {
            while (top != '(') {
                *postfix = opStk[top];
                postfix++;
                opStk[top] = 0;
                if (top != 0) {
                    top--;
                }
                /* Makes sure there is starting parenthesis */
                else {
                    perror("IF2P: FORMAT ERROR");
                    exit(EXIT_FAILURE);
                }
            }

        else if () {

        }

        
        }
        while (isspace(*infix)) {
            infix++;
        }
    }
    
    
    
    return *postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    /* Before call change to postfix, initialize a stack and a string to hold the postfix values. After, deallocate stack (??) */
    char *postfix = NULL;
    stack *head = NULL;

    
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
            char evCh;
            int isEval = sscanf(str, " %c = %c ", &name, &evCh);

            if (isEval == 2) {
                if (!((evCh > 'A') && (evCh < 'Z')) || (evCh == '(')) {
                    perror("EVAL CHARACTER VALIDITY ERROR");
                    exit(EXIT_FAILURE);
                }

                /* Copied from above */
                char *expr = strchr(str, '=');
                
                if ((*expr == NULL) || (*(expr + 1) == NULL)) {
                    perror("EVAL EXPRESSION NULL ERROR");
                    exit(EXIT_FAILURE);
                }
                expr++;

                /* Evaluate Matrix */
                matrix_sf *mat = evaluate_expr_sf(name, expr);



            }
            else {
                perror("INVALID FORMAT CRITICAL ERROR");
                exit(EXIT_FAILURE);
            }
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