//
//  main.c
//  VTables
//
//  Created by Scott Roberts on 3/19/18.
//  Copyright © 2018 Scott Roberts. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Employee Types Enumeration
enum Emp_Types {Employee, HourlyEmployee, CommissionEmployee, SeniorSalesman};

// Commission Employee constants
static const double commission_factor = 0.1;
static const int commission_bonus = 40000;

// Senior Salesman constants
static const double s_salesman_factor = 0.2;
static const int s_salesman_bonus = 50000;
static const double s_salesman_age_factor = 0.05;
static const int s_salesman_min_age = 40;


// EMPLOYEE: base "class"
struct Employee {
    void** v_table;
    int age;
};

// HOURLY EMPLOYEE: derived "class" from Employee
struct HourlyEmployee {
    void** v_table;
    int age;
    double hourly_rate;
    double hours;
};
// Hourly Employee prints how much money they make.
void Speak_Hourly(struct Employee* e_ptr) {
    struct HourlyEmployee *h_ptr = (struct HourlyEmployee*) e_ptr;
    printf("I am %d years old.\n", h_ptr -> age);
    printf("I make %lf dollars an hour.\n", h_ptr -> hourly_rate);
    printf("I have worked %lf hours.\n", h_ptr -> hours);
}
// Hourly Employee returns the money they have made.
double GetPay_Hourly (struct Employee* e_ptr) {
    struct HourlyEmployee *h_ptr = (struct HourlyEmployee*) e_ptr;
    return (h_ptr -> hours) * (h_ptr -> hourly_rate);
}
// Hourly Employee vtable that holds function pointers to the "class's" functions.
void* Vtable_Hourly[2] = {Speak_Hourly, GetPay_Hourly};

// Hourly Employee default constructor.
void Construct_Hourly(struct HourlyEmployee* h_ptr) {
    h_ptr -> v_table = Vtable_Hourly;
    h_ptr -> age = 0;
    h_ptr -> hourly_rate = 0;
    h_ptr -> hours = 0;
}

// COMMISSION EMPLOYEE: derived "class" from Employee
struct CommissionEmployee {
    void** v_table;
    int age;
    double sales_amount;
};

// Commission Employee prints how much money they make.
void Speak_Commission(struct Employee* e_ptr) {
    struct CommissionEmployee *c_ptr = (struct CommissionEmployee*) e_ptr;
    printf("I am %d years old.\n", c_ptr -> age);
    printf("I have made %lf in sales\n", c_ptr -> sales_amount);
}

// Commission Employee returns the money they have made.
double GetPay_Commission (struct Employee* e_ptr) {
    struct CommissionEmployee *c_ptr = (struct CommissionEmployee*) e_ptr;
    return ((c_ptr -> sales_amount) * commission_factor) + commission_bonus;
}

// Commission Employee vtable that holds function pointers to the "class's" functions.
void* Vtable_Commission[2] = {Speak_Commission, GetPay_Commission};

// Commission Employee default constructors
void Construct_Commission(struct CommissionEmployee* c_ptr) {
    c_ptr -> v_table = Vtable_Commission;
    c_ptr -> age = 0;
    c_ptr -> sales_amount = 0;
}

// SENIOR SALESMAN: derived "class" from CommissionEmployee
struct SeniorSalesman {
    void** v_table;
    int age;
    double sales_amount;
};

// Senior salesman "GetPay" returns the money they have made.


// NOT SURE IF THE PARAMETER SHOULD BE EMPLOYEE POINTER OR COMMISSION POINTER
double GetPay_Senior (struct Employee* e_ptr) {
    struct SeniorSalesman *s_ptr = (struct SeniorSalesman*) e_ptr;
    double earnings = ((s_ptr -> sales_amount) * s_salesman_factor) + s_salesman_bonus;
    if (s_ptr -> age >= s_salesman_min_age) {
        earnings += ((s_ptr -> sales_amount) * s_salesman_age_factor);
    }
    return earnings;
}

// Senior salesman vtable that holds function pointers to the "class's" functions.
void* Vtable_Senior[2] = {Speak_Commission, GetPay_Senior};

// Senior salesman default constructors
void Construct_Senior(struct SeniorSalesman* s_ptr) {
    s_ptr -> v_table = Vtable_Senior;
    s_ptr -> age = 0;
    s_ptr -> sales_amount = 0;
}

void Display_Menu() {
    printf("\nWhich Employee would you like to choose?\n");
    printf("1. Hourly Employee\n");
    printf("2. Commission Employee\n");
    printf("3. Senior Salesman\n");
    printf("0. Quit\n");
}

int Get_Int_Choice() {
    int num;
    scanf("%d", &num);
    // Handle errors here if you care.
    return num;
}

double Get_Double_Choice() {
    double num;
    scanf("%lf", &num);
    // Handle errors here if you care.
    return num;
}

int Ask_Age () {
    printf("\nHow old is the employee?\n");
    return Get_Int_Choice();
}

double Ask_PayRate() {
    printf("\nWhat is the hourly rate of the employee?\n");
    return Get_Double_Choice();
}

double Ask_Hours() {
    printf("\nWhat are the hours worked by the employee?\n");
    return Get_Double_Choice();
}

double Ask_Sales_Amount() {
    printf("\nWhat was your total sales amount?\n");
    return Get_Double_Choice();
}

// I TRIED THE FUNCTIONAL WAY... didn't work out as planned.

int main(int argc, const char * argv[]) {
    // All Employee types
    struct Employee *e_ptr = NULL;
    struct HourlyEmployee *h_ptr = malloc(sizeof(struct HourlyEmployee*));
    struct CommissionEmployee *c_ptr = malloc(sizeof(struct CommissionEmployee*));
    struct SeniorSalesman *s_ptr = malloc(sizeof(struct SeniorSalesman*));
    
    // All Fields
    double hourly_rate, hours, sales_amount;
    int age;
    int choice = -1;
    do {
        Display_Menu();
        choice = Get_Int_Choice();
        
        switch (choice) {
            case 0:
                break;
            case 1:
                // Allocate Memory
                e_ptr = malloc(sizeof(struct HourlyEmployee*));
                
                // Construct Appropriate Class
                Construct_Hourly(h_ptr);
                age = Ask_Age();
                h_ptr -> age = age;
                hourly_rate = Ask_PayRate();
                h_ptr -> hourly_rate = hourly_rate;
                hours = Ask_Hours();
                h_ptr -> hours = hours;
                
                // Set Employee to Appropriate Class
                e_ptr = (struct Employee*) h_ptr;
                
                // Speak
                e_ptr -> v_table = h_ptr -> v_table;
                ((void(*)(struct Employee*))Vtable_Hourly[0])((struct Employee*)e_ptr);
                printf("The total pay: %lf\n", GetPay_Hourly(e_ptr));
                
                break;
            case 2:
                // Allocate Memory
                e_ptr = malloc(sizeof(struct CommissionEmployee*));
                
                // Construct Appropriate Class
                Construct_Commission(c_ptr);
                age = Ask_Age();
                c_ptr -> age = age;
                sales_amount = Ask_Sales_Amount();
                c_ptr -> sales_amount = sales_amount;
                
                // Set Employee to Appropriate Class
                e_ptr = (struct Employee*) c_ptr;
                
                // Speak
                e_ptr -> v_table = c_ptr -> v_table;
                
                ((void(*)(struct Employee*))Vtable_Commission[0])((struct Employee*)e_ptr);
                printf("The total pay: %lf\n", GetPay_Commission(e_ptr));
                
                break;
            case 3:
                // Allocate Memory
                e_ptr = malloc(sizeof(struct SeniorSalesman*));
                
                // Construct Appropriate Class
                Construct_Senior(s_ptr);
                age = Ask_Age();
                s_ptr -> age = age;
                sales_amount = Ask_Sales_Amount();
                s_ptr -> sales_amount = sales_amount;
                
                // Set Employee to Appropriate Class
                e_ptr = (struct Employee*) s_ptr;
                e_ptr -> v_table = s_ptr -> v_table;
                
                // Speak
                ((void(*)(struct Employee*))Vtable_Senior[0])((struct Employee*)e_ptr);
                printf("The total pay: %lf\n", GetPay_Senior(e_ptr));
                break;
            default:
                printf("\nThere are no employees of that type.\n");
        }
    } while (choice != 0);

    // Free the memory space for employee pointer.
    free(e_ptr);
    free(h_ptr);
    free(c_ptr);
    free(s_ptr);
}
