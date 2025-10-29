#include<stdio.h>
#include<string.h>


int error = 0;


int isDigit(char token) {
    return token<= '9' && token>= '0';
}

int isAlaphabate(char token) {
    return (token>= 'a' && token<= 'z') || (token>= 'A' && token<= 'Z');
}

int isOperator(char token) {
    return token== '+' || token== '-' || token== '*' || token== '/' ;
}

int applyArithematicOperation(int a,int b,char operator) {
    switch(operator) {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': if(b==0) {
            
                        error = 1;
                        return 3;

                    } else return a/ b;

        default: error =2;
                return 3;
    }
}

int precedence(char token) {
    if(token== '+' || token== '-') return 1;
    if(token== '/' || token== '*') return 2;
    return 0;

}

int explore(char expression[]) {
    char operatorStack[100];   
    int operatorTop=-1;
    int integerStack[100];     
    int integerTop = -1;
    int l = strlen(expression);                      // length of String expression
    

    for(int i = 0; i<l; i++) {
        if(expression[i] == ' ') continue;

        if(isAlaphabate(expression[i])) {
            
            error = 2;
            return 3;
        }

        if(isDigit(expression[i])) {
            integerTop++;
            integerStack[integerTop] = expression[i] - '0';

            while(isDigit(expression[i+1])) {                      // for multidigit number
                i++;
                integerStack[integerTop] *= 10;
                integerStack[integerTop] += expression[i] - '0';
            }
            continue;
        }
        
        if(isOperator(expression[i])) {
            
            while(operatorTop != -1 && precedence(operatorStack[operatorTop]) >= precedence(expression[i])) {
                //calculate the result of higherprecedence operation ad bodmass says
                int x = integerStack[integerTop--];
                int y = integerStack[integerTop--];

                integerStack[++integerTop] = applyArithematicOperation(y,x,operatorStack[operatorTop--]); 
                
            }
           
            operatorStack[++operatorTop] = expression[i];
            
            continue;
        }

        if(expression[i] == '(') {
            
            operatorTop++;
            operatorStack[operatorTop] = expression[i];
            continue;
        }

        if(expression[i] == ')') {

           
            while(operatorTop != -1 && operatorStack[operatorTop] != '(') {
                //solve parentheses first
                int x = integerStack[integerTop--];
                int y = integerStack[integerTop--];

                integerStack[++integerTop] = applyArithematicOperation(y,x,operatorStack[operatorTop--]); 

            }
            
            if(operatorTop != -1) operatorTop--;

            continue;
        }

        
        error = 2;
        return 3;

    }

    while(operatorTop != -1) {
        int x = integerStack[integerTop--];
        int y = integerStack[integerTop--];

        integerStack[++integerTop] = applyArithematicOperation(y,x,operatorStack[operatorTop--]);
        
    }

    return integerStack[integerTop];
}



int main() {

    char expression[100];
    int ans = 0;
    
    scanf("%[^\n]",expression);


    ans = explore(expression);

    if(error == 0) {
        printf("%d",ans);
    } else {
        switch(error) {
            case 1: printf("Error: Division by zero");
                    break;
            case 2: printf("Error: Invalid expression");
                    break;
        }
    }
    
    return 0;
}