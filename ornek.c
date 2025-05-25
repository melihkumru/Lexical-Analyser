#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
struct students
{
    char name[50];
    char major[50];
    int age;
    double gpa;
};
*/


int main(){


    //USER INPUT
    /*
    int age;
    printf("enter your age:");
    scanf("%d", &age);
    printf("your age is %d",age);

    double age;
    printf("enter your age:");
    scanf("%lf", &age);
    printf("your age is %f",age);

    char age;
    printf("enter your age:");
    scanf("%c", &age);
    printf("your age is %c",age);

    char name[20];
    printf("enter your name:");
    fgets(name, 20, stdin);
    printf("your name is %s",name);
    */
    

    //ARRAYS
    /*
    int luckyNumber[] = {4,8,15,16,23,42};
    printf("%d", luckyNumber[2]);
    return 0;
    */

    
    //CALCULATOR
    /*
    double num1;
    double num2;
    char op;
    
    printf("input num1: ");
    scanf("%lf", &num1);
    printf("input op code: ");
    scanf(" %c", &op);
    printf("input num2: ");
    scanf("%lf", &num2);
    

    if(op == '+'){
        printf("%f",num1+num2);
    }
    else if (op == '-'){
        printf("%f",num1-num2);
    }
    else if (op == '/'){
        printf("%f",num1/num2);
    }
    else if (op == '*'){
        printf("%f",num1*num2);
    }
    else{
        printf("ınvalid operator");
    }
    */


    //SWITCH
    /*
    char grade;
    printf("input your grade:");
    scanf( "%c", &grade);

    switch(grade){
    case 'A':
        printf("you did great");
        break;
    case 'B':
        printf("you did alright");
        break;
    case 'C':
        printf("you did poorly");
        break;
    case 'D':
        printf("you did very bad");
        break;
    case 'F':
        printf("you failed");
        break;
    default:
        printf("invalid grade");
        break;
    }
    */


    //STRUCTS
    /*
    struct students student1;
    student1.age = 10;
    student1.gpa = 3.5;
    strcpy( student1.name, "jim");
    strcpy( student1.major, "business");
    printf("%f", student1.gpa);
    */

    
    //TWO DİMENSİOL ARRAY
    /*
    int nums[3][2] = {
                    {1,2},
                    {3,4},
                    {5,6}
                    };

    printf("%d\n", nums[0][0]);
    printf("%d\n", nums[2][1]);
    int i, j;
    for ( i = 0; i < 3; i++)
    {
        for ( j = 0; j < 2; j++)
        {
            printf("%d", nums[i][j]);
        }
        printf("\n");
        
    }
    */


    //MEMORY ADDRESSES
    /*
    int age = 30;
    double gpa = 3.4;
    char grade = 'A';

    printf("age: %p\ngpa: %p\ngrade: %p", &age, &gpa, &grade);
    */


    //POINTERS
    /*
    int age = 30;
    int *pAge = &age;

    double gpa = 3.4;
    double *pGpa = &gpa;
    
    char grade = 'A';
    char *pGrade = &grade;

    printf("age's memory address: %p\n", *pAge);
    printf("age's memory address: %d\n", *pAge);
    */

   
}

