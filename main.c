#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 100

typedef struct
{
    char* key;
    float value;
    struct Node *left;
    struct Node *right;

} Node;

Node *root = NULL;
Node *heapRoot = NULL;
Node *heapArray[max];
int heapArrayIndex;

Node* newNode(char* key, float value)
{
    Node *n = malloc(sizeof(Node));
    n -> key = malloc(100);
    strcpy(n->key,key);
    n -> value = value;
    n -> left = NULL;
    n -> right = NULL;
    return n;
}

Node* insert(Node*root,char* key, float value )
{
    if(root == NULL)
        return newNode(key, value);
    if(strcasecmp(key, root -> key) == 1)
        root -> right = insert(root -> right, key, value);
    else if(strcasecmp(key, root -> key) == -1)
        root -> left = insert(root -> left, key, value);
}

void put(char* key, float value)
{
    root = insert(root,key,value);
}

Node *search(Node *root, char* key)
{
    if (strcasecmp(key, root -> key) == 0)
        return root;

    if (strcasecmp(key, root -> key) == 1)
        return search(root->right, key);

    return search(root->left, key);
}

void inorder(Node* root)
{
    if(root!=NULL)
    {
        inorder(root->left);
        printf("%s = %.3f\n",root->key, root->value);
        inorder(root->right);
    }

}
int getLeftIndex(int i)
{
    return 2*i;
}

int getRightIndex(int i)
{
    return 2*i+1;
}

void exchange(int i, int largest)
{
    Node* temp=heapArray[i];
    heapArray[i]=heapArray[largest];
    heapArray[largest]=temp;
}

void heapBottomUp()
{
    int i;
    for(i=(heapArrayIndex/2); i>=1; i--)
    {
        maxHeapify(i,heapArrayIndex);
    }

}

void maxHeapify(int i,int size)
{
    int n = size;
    int l=2*i;
    int r=2*i+1;
    int largest;
    if(heapArray[l]!=NULL&&l<=n&&heapArray[l]->value>heapArray[i]->value)
        largest=l;
    else
        largest =i;
    if(heapArray[r]!=NULL&&r<=n&&heapArray[r]->value>heapArray[largest]->value)
        largest=r;
    if(largest!=i)
    {
        exchange(i,largest);
        maxHeapify(largest,n);
    }
}

void heapSort()
{
    int i;
    for( i=heapArrayIndex-1 ; i>=2 ; i--)
    {
        exchange(1,i);
        maxHeapify(1,i-1);
    }
}


typedef union
{
    char cData;
    float fData;
} Item;

typedef struct
{
    Item items[max];
    int top;
} stack;

stack* initialize()
{
    stack *s=malloc(sizeof(stack));
    s->top=0;
    return s;
}

void push(stack *s, Item value)
{
    s->items[s->top++] = value;
}

Item pop(stack *s)
{
    s->top--;
    return s->items[s->top];
}

Item top(stack *s)
{
    return s->items[s->top-1];
}

int isEmpty(stack *s)
{
    if (s->top == 0)
        return 1;
    else
        return 0;
}

int myIsDigit(char *token)
{
    int i;
    for(i=0; i<strlen(token); i++)
    {
        if(isdigit(token[i]))
            return 1;
    }
    return 0;
}

int myIsOperator(char op)
{
    if(op=='*'||op=='/'||op=='+'||op=='-')
        return 1;
    return 0;
}
int priority(char op)
{
    switch(op)
    {
    case '(':
        return 4;
        break;
    case '^':
        return 3;
    case '/':
    case '*':
    case '%':
        return 2;
        break;
    case '+':
    case '-':
        return 1;
        break;
    }
}

float evaluate(float x, float y, char op)
{
    switch(op)
    {
    case '+':
        return x+y;
        break;
    case '-':
        return x-y;
        break;
    case '*':
        return x*y;
        break;
    case '/':
        return x/y;
        break;
    case '^':
        return pow(x,y);
    }
}
void infixToPostfix(char* infix, char* postfix)
{
    postfix[0]= '\0';
    stack *s = initialize();
    int i;
    char newInfix[max];
    newInfix[0]='\0';

    for(i=0; i<strlen(infix); i++)
    {
        if(infix[i]=='-'&&myIsOperator(infix[i-1]))
        {
            char *c;
            c[0]=infix[i];
            c[1]=infix[i+1];
            strcat(newInfix,c);
            char c2[2];
            c2[0]=' ' ;
            c2[1]='\0';
            char c3 = c2[0];
            strncat(newInfix,&c3,1);
            i++;
        }
        else if(isdigit(infix[i]))
        {
            while(isdigit(infix[i])||infix[i]=='.')
            {
                char c1 = infix[i];
                strncat(newInfix,&c1,1);
                i++;
            }
            i--;
            char c2[2];
            c2[0]=' ' ;
            c2[1]='\0';
            char c3 = c2[0];
            strncat(newInfix,&c3,1);
        }
        else
        {
            char c1 = infix[i];
            strncat(newInfix,&c1,1);
            char c2[2];
            c2[0]=' ' ;
            c2[1]='\0';
            char c3 = c2[0];
            strncat(newInfix,&c3,1);

        }
    }
    char *token = strtok(newInfix, " ");
    while(token!=NULL)
    {
        if(myIsDigit(token)||strcmp(token,".")==0)
        {
            strcat(postfix, token);
            strcat(postfix, " ");
        }
        else if(isEmpty(s))
        {
            Item x;
            x.cData = token[0];
            push(s, x);
        }
        else if((top(s).cData == '(')&&( token[0]!= ')'))
        {
            Item x;
            x.cData = token[0];
            push(s, x);
        }
        else if (token[0] == ')')
        {
            while(top(s).cData != '(')
            {
                Item x = pop(s);
                strncat(postfix, &x.cData, 1);
                strcat(postfix, " ");
            }
            pop(s);
        }
        else
        {
            while(priority(top(s).cData)>=priority(token[0]) && (top(s).cData) != '(' && !isEmpty(s))
            {
                Item x = pop(s);
                strncat(postfix, &x.cData, 1);
                strcat(postfix, " ");
            }
            Item x;
            x.cData = token[0];
            push(s, x);
        }
        token = strtok(NULL, " ");
    }
    while(!isEmpty(s))
    {
        Item x = pop(s);
        strncat(postfix, &x.cData, 1);
        strcat(postfix, " ");
    }
}
float evaluatePostfix(char *postfix)
{
    stack *s = initialize();
    char *token = strtok(postfix, " ");
    while(token!=NULL)
    {
        if(myIsDigit(token))
        {
            float x = atof(token);
            Item y;
            y.fData = x;
            push(s, y);
        }
        else if(strcmp(token,".")==0)
        {

        }
        else
        {
            float x = pop(s).fData;
            float y = pop(s).fData;
            float result = evaluate(y, x, token[0]);
            Item z;
            z.fData = result;
            push(s, z);
        }
        token = strtok(NULL, " ");
    }
    float result = pop(s).fData;
    return result;
}

void deblank(char str[])
{
    char *token;
    char *line = (char *) malloc(100);
    strcpy(line, str);
    strcpy(str, "");
    token = strtok(line, " ");
    while (token != NULL)
    {
        strcat(str, token);
        token = strtok(NULL, " ");
    }

}

int checkValidKey(char key[])
{
    int i,flag=1;
    for(i = 0; i<strlen(key); i++)
    {
        if(!isalpha(key[i]))
            flag = 0 ;
    }
    return flag;
}

void sortByValue()
{
    heapBottomUp();
    heapSort();
    int k;
    for(k=1; k<=heapArrayIndex-1; k++)
    {
        printf("%s = %.3f\n",heapArray[k]->key,heapArray[k]->value);
    }
}

void readFromFile()
{
    char line[200];
    char infix[256]="";
    char postfix[256] = "";
    char *token;
    int i, flag=0;
    heapArrayIndex=1;
    FILE *f = fopen("src.txt", "r");
    if (!f)
    {
        printf("Could not open file");
    }
    else
    {
        printf("------READING FROM FILE------\n");
        while(!feof(f))
        {
            //read each line in line string
            fscanf(f,"%[^\n]",line);
            fscanf(f,"\n");
            deblank(line);

            //use strtok to get before = (key)
            token  = strtok(line, "=");
            char key[max];
            strcpy(key,token);

            //use strtok to get after = (value or expression)
            token = strtok(NULL, "=");

            //to check if there are no operators before =, only alphabet
            if(!checkValidKey(key))
            {
                printf("Expression including %s invalid\n", key);
            }
            else
            {
                for(i = 0; i < strlen(token); i++)
                {
                    //check if there's a character in the expression is present in the BST
                    if(isalpha(token[i]))
                    {
                        if(root!=NULL)
                        {
                            char newToken[max]="";
                            while(isalpha(token[i]))
                            {
                                char c = token[i];
                                strncat(newToken, &c, 1);
                                i++;
                            }
                            newToken[i]='\0';
                            i--;
                            Node *searchNode = search(root, newToken);
                            if(search==NULL)
                            {
                                flag = 1;
                                break;
                            }
                        }
                    }
                }

                if(!flag)
                {
                    strcpy(infix,"");
                    for(i = 0; i < strlen(token); i++)
                    {
                        if(isalpha(token[i])&&!myIsOperator(token[i]))
                        {
                            char newToken[max]="";
                            while(isalpha(token[i]))
                            {
                                char c = token[i];
                                strncat(newToken, &c, 1);
                                i++;
                            }
                            newToken[i]='\0';
                            i--;
                            Node *searchNode = search(root, newToken);
                            float f = searchNode->value;
                            char s[max];
                            gcvt(f,3,s);
                            strcat(infix,s);
                        }
                        else
                        {
                            char c = token[i];
                            strncat(infix,&c,1);
                        }
                    }
                    printf("%s = %s \n", line, token);
                    infixToPostfix(infix, postfix);
                    float result = evaluatePostfix(postfix);
                    put(key,result);
                    heapArray[heapArrayIndex]=newNode(key,result);
                    heapArrayIndex++;
                }

            }
        }
    }
    fclose(f);
}
int main()
{
    readFromFile();
    printf("------SORT BY VARIABLE NAME------\n");
    inorder(root);
    printf("------SORT BY VALUE------\n");
    sortByValue();
    return 0;
}


