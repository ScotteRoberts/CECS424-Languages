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

// Commission Employee constants
static const int commission_factor = 0.1;
static const int commission_bonus = 40000;

// Senior Salesman constants
static const int s_salesman_factor = 0.2;
static const int s_salesman_bonus = 50000;
static const int s_salesman_age_factor = 0.05;
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
    printf("I work for %f dollars per hour", h_ptr -> hourly_rate);
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
    printf("I make commission on %f dollars in sales!", c_ptr -> sales_amount);
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


int main(int argc, const char * argv[]) {
    
    printf("Hello, World!\n");
    return 0;
}

