/* Braden Switzer 115984660 */
#include "hw7.h"

/* Struct to act as linked list for my stack in evaluate */
typedef struct stack {
    matrix_sf *mat;
    struct stack *next;
} stack;

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    if (root == NULL) {
        root = malloc(sizeof(bst_sf));
        root->mat = mat;
        root->left_child = NULL;
        root->right_child = NULL;
    }
    else {
        /* Assuming all name values are unique */
        if (mat->name < root->mat->name) {
            root->left_child = insert_bst_sf(mat, root->left_child);
        }
        else {
            root->right_child = insert_bst_sf(mat, root->right_child);
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
    if (root == NULL) {
        return;
    }
    else {
        free_bst_sf(root->left_child);
        free_bst_sf(root->right_child);
        if (root->mat->name == '$') {
            free(root);
        }
        else {
            free(root->mat);
            free(root);
        }
    }
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

    sumMat->name = '?';
    sumMat->num_rows = mat1->num_rows;
    sumMat->num_cols = mat1->num_cols;
    
    /* Trying to run Make didn't like int, needs unsigned int. Stack overflow reccomended size_t */
    for (size_t i = 0; i < (mat1->num_rows*mat1->num_cols); i++) {
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

    prodMat->name = '?';
    prodMat->num_rows = mat1->num_rows;
    prodMat->num_cols = mat2->num_cols;

    /* i, j for indexing through prodMat and k uses iSum to accumulate the mar1 row and mat2 column products */
    for (size_t i = 0; i < prodMat->num_rows; i++) {
        for (size_t j = 0; j < prodMat->num_cols; j++) {
            int iSum = 0;
            for (size_t k = 0; k < mat1->num_cols; k++){
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
    matrix_sf *transMat = malloc(sizeof(matrix_sf) + (sizeof(int) * mat->num_rows * mat->num_cols));
    if (transMat == NULL) {
        perror("TRANS: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }
    
    transMat->name = '?';
    transMat->num_rows = mat->num_cols;
    transMat->num_cols = mat->num_rows;

    for (size_t i = 0; i < transMat->num_rows; i++) {
        for (size_t j = 0; j < transMat->num_cols; j++) {
            
            transMat->values[j + (transMat->num_cols * i)] = mat->values[i + (mat->num_cols * j)];
        }
    }

    return transMat;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {  
    int nrow, ncol;

    int dims = sscanf(expr, " %d %d", &nrow, &ncol);
    
    /* Allocate storage/initialize matrix */
    matrix_sf *mat = malloc(sizeof(matrix_sf) + sizeof(int) * nrow * ncol);
    if (mat == NULL) {
        perror("CREATE: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }
    mat->name = name;
    mat->num_rows = nrow;
    mat->num_cols = ncol;

    if ((dims == 2)) {
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
        while((*cur != '\0') && (*cur != ']') && (rowCt < nrow)) {
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
            
            if (colCt != ncol) {
                fprintf(stderr, "colCt: %d and ncol: %d", colCt, ncol);
                perror("MATRIX CREATE: COLUMN AMOUNT ERROR");
                exit(EXIT_FAILURE);
            }
            rowCt++;
            
            while (isspace(*cur)) {
                cur++;
            }

            if (*cur != ';') {
                perror("MATRIX CREATE: MISSING SEMICOLON ERROR");
                exit(EXIT_FAILURE);
            }
            cur++;

            while (isspace(*cur)) {
                cur++;
            }
        }
        
        /* To get rid of whitespace after final ';' */
        while (isspace(*cur)) {
            cur++;
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

    return mat;
}

/* For use in creating postfix, assisted by posted link (as is infix2postfix_sf()) */
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

char* infix2postfix_sf(char *infix) {
    /* Top so that I know where in my array I am to be able to modify the correct values,
    starts at -1 because will increment before first call to 0 */
    int top = -1;
    size_t len = strlen(infix);
    /* Made all 0 so that I could check for 0, EDIT: Didn't need check, but keeping in to avoid uncomfortable pointer situations */
    char opStk[len + 1];

    for (size_t stkInd = 0; stkInd < len; stkInd++) {
        opStk[stkInd] = 0;
    }
    char *postfix = malloc(sizeof(char) * (len + 1));
    
    if (postfix == NULL) {
        perror("IFP: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }

    /* To make sure I can return a pointer to the beginning, not the end */
    char *beginning = postfix;

    char curVal;

    while (*infix != '\0') {
        /* *postfix = '\0'; Without NULL terminating postfix my debug code for POSTFIX just wrote whatever*/ 
        /* printf("TOP: %d\nINFIX: %sPOSTFIX: %s\n\n", top, infix, beginning);
        if (top >= 0) {
            printf("^^^^OPSTK[top]:%c\n\n", opStk[top]);
        }
        */

        /* sscanf search until find value (Either matrix name, operation or parenthesis), REMOVED AND REPLACED BY POINTER INDEX
        int need1 = sscanf(infix, " %c", &curVal); */
        
        while (isspace(*infix)) {
            infix++;
        }

        /* Pointer index*/
         curVal = *infix;

        if ((curVal >= 'A') && (curVal <= 'Z')) {
            *postfix = curVal;
            postfix++;
        }
        else if (curVal == '(') {
            /* Mimics push */
            top++;
            opStk[top] = '(';
        }
        else if (curVal == ')') {
            while ((top >= 0) && (opStk[top] != '(')) {
                *postfix = opStk[top];
                postfix++;
                /* Mimics pop */
                opStk[top] = 0;
                top--;  
            }
            /* Makes sure there is starting parenthesis */  
            if (top < 0) {
                perror("IFP: COLUMN AMOUNT ERROR");
                exit(EXIT_FAILURE);
            }
            /* Get rid of starting parenthesis */
            opStk[top] = 0;
            top--;
        }
        else if (curVal == '\'') {
            /* while ((top >= 0) && (opStk[top] != '(')) {
                *postfix = opStk[top];
                postfix++;
                opStk[top] = 0;
                top--;
            }
            top++;
            opStk[top] = curVal; Kept running into issues using this, until I realized that in every situation, ' is just added to postfix 
            no need for stack */
            *postfix = curVal;
            postfix++;
        }
        else if ((curVal == '*') || (curVal == '+')) {
            while ((top >= 0) && ((prec(curVal)) <= (prec(opStk[top])))) {
                *postfix = opStk[top];
                postfix++;
                opStk[top] = 0;
                top--;
            }
            top++;
            opStk[top] = curVal;
        }
        else {
            perror("IFP: INVALID CHARACTER ERROR");
            exit(EXIT_FAILURE);
        }
        infix++;
        /* Gets rid of whitespace before '\0' so loop stops */
        while (isspace(*infix)) {
            infix++;
        }

    }
    while (top >= 0) {
        *postfix = opStk[top];
        postfix++;
        opStk[top] = 0;
        top--;
    }

    *postfix = '\0';

    /* DEBUG: printf("\n\n FINAL ---- TOP: %d\nINFIX: %sPOSTFIX: %s\n", top, infix, beginning);
        if (top >= 0) {
            printf("OPSTK[top]:%c\n\n", opStk[top]);
        }
    */

    return beginning;
}


/* Push/Pop heavily inspired by lecture slides (add_to_list(), etc) */
stack *push(stack *head, matrix_sf *mat) {
    stack *new_head = malloc(sizeof(stack));

    if (new_head == NULL) {
        perror("PUSH: ALLOCATION ERROR");
        exit(EXIT_FAILURE);
    }

    new_head->mat = mat;
    new_head->next = head;

    return new_head;
}

stack *pop(stack *head) {
    if (head == NULL) {
        perror("POP: NULL STACK POINTER ERROR");
        exit(EXIT_FAILURE);
    }

    stack *next = head->next;
    free(head);

    return next;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char *postfix = NULL;
    /* If postfix has zeroes, I iterated wrong in I2P, can check in debug */
    postfix = infix2postfix_sf(expr);

    char *beginning = postfix;
    /* Initialize new empty head so that can be pushed and popped */
    stack *head = NULL;

    while (*postfix != '\0') {
        if ((*postfix >= 'A') && (*postfix <= 'Z')) {
            matrix_sf *mat = find_bst_sf(*postfix, root);
            
            if (mat == NULL) {
                perror("EVAL: MATRIX NOT FOUND ERROR");
                exit(EXIT_FAILURE);
            }
            
            head = push(head, mat);
            postfix++;
        } 
        /* Unary, only pop one matrix */
        else if (*postfix == '\'') {
            matrix_sf *new = transpose_mat_sf(head->mat);
            
            /* Frees space taken up by intermediate matrices, now named '?' */
            if (head->mat->name == '?') {
                free(head->mat);
            }
            /* Remove original, add new */
            head = pop(head);
            
            head = push(head, new);
            postfix++;
        }
        else if (*postfix == '*') {
            matrix_sf *temp1 = head->mat;
            
            head = pop(head);
            
            /* Switch head->mat and temp because was backwards before, postfix does in reverse order */
            matrix_sf *prod = mult_mats_sf(head->mat, temp1);
            if (temp1->name == '?') {
                free(temp1);
            }
            if (head->mat->name == '?') {
                free(head->mat);
            }
            head = pop(head);

            head = push(head, prod);
            postfix++;
        }
        else if (*postfix == '+') {
            matrix_sf *temp1 = head->mat;
            
            head = pop(head);

            matrix_sf *sum = add_mats_sf(temp1, head->mat);
            if (temp1->name == '?') {
                free(temp1);
            }
            if (head->mat->name == '?') {
                free(head->mat);
            }
            head = pop(head);

            head = push(head, sum);
            postfix++;
        }
        else {
            perror("EVAL: POSTFIX CHARACTER INVALID ERROR");
            exit(EXIT_FAILURE);
        }
    }

    if ((head == NULL) || (head->next != NULL)) {
        perror("EVAL: STACK REMAINDER ERROR");
        exit(EXIT_FAILURE);
    } 
    
    if (head->mat == NULL) {
        perror("EVAL: STACK MATRIX NULL ERROR");
        exit(EXIT_FAILURE);
    }

    /* The (Should be) only left, finished matrix, record in final */
    matrix_sf *final = head->mat;

    final->name = name;

    head = pop(head);

    if (head != NULL) {
        perror("EVAL: STACK REMAINDER ERROR");
        exit(EXIT_FAILURE);
    }

    free(beginning);

    return final;
}

matrix_sf *execute_script_sf(char *filename) {
    /* Initialize first root */
    bst_sf *root = NULL;
    char *str = NULL;
    ssize_t line;
    /* To make sure that I can return the final referenced mat in the loop */
    matrix_sf *mat = NULL;
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
            if ((expr == NULL) || (*(expr + 1) == '\0')) {
                perror("MCREATE EXPRESSION NULL ERROR");
                exit(EXIT_FAILURE);
            }
            
            /* Increment to after = */
            expr++;

            /* Make Matrix */
            mat = create_matrix_sf(name, expr);
            
            if (mat == NULL) {
                perror("CREATED MATRIX NULL ERROR");
                exit(EXIT_FAILURE);
            }
        
            /* DEBUG: printf("MATRIX CREATED: %c\n", name);
            print_matrix_sf(mat); */

            /* Add to BST */
            root = insert_bst_sf(mat, root);
        }
        /* Else either a matrix operation or an error */
        else {
            char evCh;
            int isEval = sscanf(str, " %c = %c ", &name, &evCh);
            /* printf("Name: %c, First: %c\n", name, evCh); */ 
            if (isEval == 2) {
                if (!(((evCh >= 'A') && (evCh <= 'Z')) || (evCh == '('))) {
                    perror("EVAL CHARACTER VALIDITY ERROR");
                    exit(EXIT_FAILURE);
                }

                /* Copied from above */
                char *expr = strchr(str, '=');
                
                if ((expr == NULL) || (*(expr + 1) == '\0')) {
                    perror("EVAL EXPRESSION NULL ERROR");
                    exit(EXIT_FAILURE);
                }
                expr++;

                /* Evaluate Matrix */
                mat = evaluate_expr_sf(name, expr, root);

                if (mat == NULL) {
                    perror("EVAL NULL OUTPUT ERROR");
                    exit(EXIT_FAILURE);
                }

                root = insert_bst_sf(mat, root);
            }
            else {
                perror("INVALID FORMAT CRITICAL ERROR");
                exit(EXIT_FAILURE);
            }
        }

    }
    free(str);

    char tempName = mat->name;
    /* Trying to make sure that free_bst_sf() doesn't deallocate the final matrix, temporarily renamed as '$' */
    mat->name = '$';
    free_bst_sf(root);

    mat->name = tempName;

    fclose(file);

    return mat;
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

/* MAIN FUNCTION MOVED TO STUDENT_TESTS.C */


