#include<stdio.h>
#include<string.h>


int error = 0;
// void showops(char s[],int t) {
//     while(t >= 0) printf("%c  ",  s[t--]);
// }

int isDigit(char a) {
    return a <= '9' && a >= '0';
}

int isAlpha(char a) {
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

int isOp(char a) {
    return a == '+' || a == '-' || a == '*' || a == '/' ;
}

int cal(int a,int b,char o) {
    switch(o) {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': if(b==0) {
            //printf("Invalid Expression : cannot divide by 0 \n");
            error = 1;
            return 3;

        } else return a / b;
        default: error =2;
        //"Invalid Expression \n";
                
                    return 3;
    }
}

int precedence(char a) {
    if(a == '+' || a == '-') return 1;
    if(a == '/' || a == '*') return 2;
    return 0;

}

int explore(char s[]) {
    char op[100];   //for operands
    int t=-1;
    int num[100];     //for numbers
    int n = -1;
    

    for(int i = 0; i<strlen(s); i++) {
        if(s[i] == ' ') continue;

        if(isAlpha(s[i])) {
            //printf("Invalid expression: must only contain integers and operators (found variable )\n");
            error = 2;
            return 3;
        }

        if(isDigit(s[i])) {
            n++;
            num[n] = s[i] - '0';
            while(isDigit(s[i+1])) {
                i++;
                num[n] *= 10;
                num[n] += s[i] - '0';
            }
            continue;
        }

        if(isOp(s[i])) {
            
            while(t != -1 && precedence(op[t]) >= precedence(s[i])) {
                int x = num[n--];
                int y = num[n--];

                //printf("\n %d %c %d = ",y,op[t],x);
                num[++n] = cal(y,x,op[t--]); 
                //printf("%d",num[n]);

            }
           
            op[++t] = s[i];
            
            continue;
        }

        if(s[i] == '(') {
            //printf("\n open");
            t++;
            op[t] = s[i];
            continue;
        }

        if(s[i] == ')') {

            // printf("\n close");
            // printf("\n printing operators : \n");
            // showops(op,t);

            while(t != -1 && op[t] != '(') {
                int x = num[n--];
                int y = num[n--];

                //printf("\n %d %c %d = ",y,op[t],x);

                num[++n] = cal(y,x,op[t--]); 

                //printf("%d",num[n]);

            }
            //printf("\ndone %d",t);
            //t = (t >= 0)? t - 1: -1;
            if(t != -1) t--;

            //printf("   %d",t);

            continue;
        }

        //printf("invalid expression : should not contain %c",s[i]);
        error = 2;
        return 3;

    }

    while(t != -1) {
        int x = num[n--];
        int y = num[n--];

        //printf("\n %d %c %d = ",y,op[t],x);

        num[++n] = cal(y,x,op[t--]);

        //printf("%d",num[n]);
    }

    return num[n];
}



int main() {

    char x[100];
    int l = 0,ans = 0;
    
    scanf("%[^\n]",x);

    l = strlen(x);

    // printf("%s",x);

    // printf("\n %d",l);

    // printf("  %d",isDigit(x[2]));

    //printf("%d", cal(x[0] - '0',x[2] - '0',x[1]));

    ans = explore(x);

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
    //printf("\n\n %s = %d",x,explore(x));

    return 0;
}