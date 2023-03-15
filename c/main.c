#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "ragesh2004"
#define DB_NAME "bankmanagements"

void auth(MYSQL*);
void menu(MYSQL*);
void new_acc(MYSQL*);
void check_balance(MYSQL*);
void transfer(MYSQL*);
void deposit(MYSQL*);
void withdraw(MYSQL*);
void check_details(MYSQL*);
void delete_customer(MYSQL*);
void apply_loan(MYSQL*);
void check_loan_status(MYSQL*);
void pay_back_loan(MYSQL*) ;


void menu(MYSQL* con) {
    int a;

        printf("\n\n\t\t==========Welcome to RB bank of india ==========\n\n");
        printf("\n\t\t----Please choose one of the options below----\n");
        printf("\n\t\t< 1 >  create new Accounts \n");
        printf("\n\t\t< 2 >  Check Balance\n");
        printf("\n\t\t< 3 >  bank transfer\n");
        printf("\n\t\t< 4 >  Deposit\n");
        printf("\n\t\t< 5 >  Withdraw\n");
        printf("\n\t\t< 6 >  check details \n");
        printf("\n\t\t< 7 >  delete customer\n");
        printf("\n\t\t< 8 >  apply loan\n");
        printf("\n\t\t< 9 > check loan status\n");
        printf("\n\t\t< 10 >  pay_back_loan\n");
        printf("\n\t\t< 11 >  Exit\n\n");
        printf("\n\n\t\t===============================================\n\n");

        printf("enter the choice: ");
        scanf("%d", &a);

        switch (a) {
            case 1:
                new_acc(con);
                break;
            case 2:
                check_balance(con);
                break;
            case 3:
                transfer(con);
                break;
            case 4:
                deposit(con);
                break;
            case 5:
                withdraw(con);
                break;
            case 6:
                check_details(con);
                break;
            case 7:
                delete_customer(con);
                break;
        	case 8:
        		apply_loan(con);
        		break;
            case 9:
                check_loan_status(con);
                break;
            case 10:
                pay_back_loan(con);
            case 11:
                exit(0);
            default:
                printf("Invalid choice.\n");
                break;
        }
}

void auth(MYSQL* con) {
    char name[50];
    char emp_pin[10];
    char query[200];

    printf("Enter your name: ");
    scanf("%s", name);

    printf("Enter your employee pin: ");
    scanf("%s", emp_pin);

    sprintf(query, "SELECT * FROM employee WHERE name='%s' AND emp_pin='%s'", name, emp_pin);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result))) {
        printf("Authentication successful.\n");
    } else {
        printf("Authentication failed.\n");
        mysql_close(con);
        exit(1);
    }

    mysql_free_result(result);
}


void new_acc(MYSQL* con){
    char name[50];
    char email[50];
    char phone_number[20];
    char address[1000];
    char adhar_number[20];
    char query[500];
    char pin[6];
    char amount[20];
	int num_fields;
    char accountno[11];

    printf("Enter customer name: ");
    scanf("%s", name);

    printf("Enter customer email: ");
    scanf("%s", email);

    printf("Enter customer phone number: ");
    scanf("%s", phone_number);

    printf("Enter customer address: ");
    scanf(" %[^\n]", address);

    printf("Enter customer adhar number: ");
    scanf("%s", adhar_number);

    printf("Enter initial amount: ");
    scanf("%s", &amount);

    printf("Enter account number: ");
    scanf("%s", accountno);

    printf("Enter your pin: ");
    scanf("%s", &pin);

    sprintf(query, "INSERT INTO customers (name, email, phone_number, address, adharnumber, amount, pin, accountnumber) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')", name, email, phone_number, address, adhar_number, amount, pin, accountno);
    

    int status = mysql_query(con, query);

    if (status != 0) {
        printf("Failed to create account.\n");
    } else {
        if (mysql_affected_rows(con) > 0) {
            printf("Account created successfully.\n");
        } else {
            printf("Failed to create account.\n");
        }
    }

    menu(con);

}


void check_balance(MYSQL* con) {
    char from_acc[12],pin[4];
    char query[200];
	printf("Enter the account number to check balance: ");
    scanf("%s", from_acc);
    printf("Enter your pin: ");
    scanf("%s", pin);

    sprintf(query, "SELECT * FROM customers WHERE accountnumber='%s' AND pin='%s' ", from_acc,pin);;

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result))) {
        printf("Your balance is: %s\n", row[6]);
    } else {
        printf("No customer found with adhar number %s.\n", from_acc);
    }

    mysql_free_result(result);
    menu(con);
}



void withdraw(MYSQL* con) {
    char from_acc[12],pin[4];
    char query[200],new_balances[20];
    int amount;
    printf("Enter the account number to withdraw: ");
    scanf("%s", from_acc);
	printf("Enter your pin: ");
    scanf("%s", pin);

    sprintf(query, "SELECT * FROM customers WHERE accountnumber='%s' AND pin='%s'", from_acc,pin);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result))) {
        printf("Enter amount to withdraw: ");
        scanf("%d", &amount);

        int balance = atoi(row[6]);
        int new_balance = balance - amount;
        
        if (new_balance < 0) {
            printf("Withdrawal amount exceeds balance. Please try again.\n");
        } else {
        	sprintf(new_balances, "%d", new_balance);
            sprintf(query, "UPDATE customers SET amount=%s WHERE accountnumber='%s'", new_balances, from_acc);

            if (mysql_query(con, query)) {
                fprintf(stderr, "%s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
            } else {
                printf("Withdrawal successful. New balance is: %d\n", new_balance);
            }
        }
    } else {
        printf("No customer found with accountnumber %s.\n", from_acc);
    }

    mysql_free_result(result);
    menu(con);
}



void deposit(MYSQL* con) {
    char from_acc[12],pin[4];
    char amount_str[20];
    int amount;
    char query[200];

 printf("Enter the account number to DEPOSIT : ");
    scanf("%s", from_acc);
printf("Enter your pin: ");
    scanf("%s", pin);


    sprintf(query, "SELECT * FROM customers WHERE accountnumber='%s' AND pin='%s'", from_acc,pin);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result))) {
        printf("Enter amount to deposit: ");
        scanf("%s", amount_str);
        amount = atoi(amount_str);

        int current_balance = atoi(row[6]);
        int new_balance = current_balance + amount;
        sprintf(query, "UPDATE customers SET amount=%d WHERE adharnumber='%s'", new_balance, from_acc);

        if (mysql_query(con, query)) {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            exit(1);
        }
        
        printf("deposit successful. New balance is: %d\n", new_balance);

    } else {
        printf("No customer found with account number %s.\n", from_acc);
    }
    
    mysql_free_result(result);
    menu(con);
}




void transfer(MYSQL* con) {
    char from_acc[20];
    char to_acc[20];
    int amount;
    char query[500],pin[4];
    char from_acc_amount[20];
    char to_acc_amount[20];
    int flag=0;
    
    printf("Enter the account number to transfer from: ");
    scanf("%s", from_acc);
    
    printf("Enter the pin: ");
    scanf("%s", pin);
    
    printf("Enter the account number to transfer to: ");
    scanf("%s", to_acc);
    
    printf("Enter the amount to transfer: ");
    scanf("%d", &amount);

    sprintf(query, "SELECT amount FROM customers WHERE accountnumber='%s' AND pin='%s'", from_acc,pin);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
  }

    MYSQL_ROW row;
    if ((row = mysql_fetch_row(result))) {
        int from_amount = atoi(row[0]);
        if(from_amount>=amount){
            from_amount-=amount;
            sprintf(from_acc_amount, "%d", from_amount);
            sprintf(query, "UPDATE customers SET amount='%s' WHERE accountnumber='%s'", from_acc_amount, from_acc);
            if (mysql_query(con, query)) {
                fprintf(stderr, "%s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
            }
            flag=1;
        }
        else {
            printf("Insufficient balance.\n");
        }
    } else {
 printf("No customer found with account number %s.\n", from_acc);
    }

    if(flag==1){
        sprintf(query, "SELECT amount FROM customers WHERE accountnumber='%s'", to_acc);

        if (mysql_query(con, query)) {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            exit(0);
        }

        result = mysql_store_result(con);

        if (result == NULL) {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            exit(0);
        }

        if ((row = mysql_fetch_row(result))) {
            int to_amount = atoi(row[0]);
            to_amount+=amount;
            sprintf(to_acc_amount, "%d", to_amount);
            sprintf(query, "UPDATE customers SET amount='%s' WHERE accountnumber='%s'", to_acc_amount, to_acc);
            if (mysql_query(con, query)) {
                fprintf(stderr, "%s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
 }
            printf("Amount transferred successfully.\n");
        } else {
            printf("No customer found with account number %s.\n", to_acc);
        }
    }

    mysql_free_result(result);
    menu(con);
}



void check_details(MYSQL* con) {
    char from_acc[12], pin[4];
    char query[200];

    printf("Enter the account number to check details: ");
    scanf("%s", from_acc);

    sprintf(query, "SELECT * FROM customers WHERE accountnumber='%s'", from_acc);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result))) {
        printf("Name: %s\n", row[1]);
        printf("Email: %s\n", row[2]);
        printf("Phone number: %s\n", row[3]);
        printf("Address: %s\n", row[4]);
        printf("Adhar number: %s\n", row[5]);
        printf("Account balance: %s\n", row[6]);
        printf("Account number: %s\n", row[8]);
    } else {
        printf("No customer found with account number %s.\n", from_acc);
    }

    mysql_free_result(result);
    menu(con);
}


void delete_customer(MYSQL* con) {
    char account_number[11];
    char query[500];

    printf("Enter the account number of the customer you want to delete: ");
    scanf("%s", account_number);

    sprintf(query, "DELETE FROM customers WHERE accountnumber = '%s'", account_number);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    int num_rows_affected = mysql_affected_rows(con);

    if (num_rows_affected > 0) {
        printf("Customer deleted successfully.\n");
    } else {
        printf("Customer not found.\n");
    }

    menu(con);
}


void apply_loan(MYSQL* con) {
    char accountno[11];
    char loan_amount[20];
    char interest_rate[10];
    char duration[10];
    char query[500];
    char Query[500];
    int loan_id;
    int balance;
    printf("Enter account number: ");
    scanf("%s", accountno);

    printf("Enter loan amount: ");
    scanf("%s", loan_amount);

    printf("Enter interest rate (in percentage): ");
    scanf("%s", interest_rate);

    printf("Enter loan duration (in months): ");
    scanf("%s", duration);

    sprintf(query, "INSERT INTO loans (accountnumber, amount, interest_rate, duration) VALUES ('%s', '%s', '%s', '%s')", accountno, loan_amount, interest_rate, duration);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    loan_id = mysql_insert_id(con);
    printf("Loan application submitted successfully. Your loan id is: %d\n", loan_id);

    menu(con);
}


void check_loan_status(MYSQL* con) {
    char loanid[20];
    int loan_id;
    char query[200];
    char is_paid[15];
    
    printf("Enter loan ID: ");
    scanf("%s", loanid);

    loan_id = atoi(loanid);

    sprintf(query, "SELECT * FROM loans WHERE loan_id=%d", loan_id);

    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    MYSQL_RES *result = mysql_store_result(con);

if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(0);
    }

    int num_rows = mysql_num_rows(result);

    if (num_rows == 0) {
        printf("Loan not found.\n");
    } else {
        MYSQL_ROW row = mysql_fetch_row(result);

        printf("Loan ID: %s\n", row[0]);
        printf("accountn number: %srs\n", row[1]);
        printf("amount: %s%\n", row[2]);
        printf("Interest rate: %s%%\n", row[3]);
        printf("duration: %s\n", row[4]);

        strcpy(is_paid, row[5]);


        if (strcmp(is_paid, "paid") == 0) {
            printf("Loan status: paid\n");
        } else {
            printf("Loan status: unpaid\n");
        }
    }

    mysql_free_result(result);
    menu(con);
}


void pay_back_loan(MYSQL* con) {
char loan_id_str[20];
int loan_id, old, amounts, months, rate;
char query[200], confirm;
char from_acc[12], pin[4];
char new_balance_str[20];
  char Query[200];
printf("Enter loan ID: ");
scanf("%s", loan_id_str);


printf("Enter the months taken to pay: ");
scanf("%d", &months);

printf("Enter the interest rate in percentage: ");
scanf("%d", &rate);

loan_id = atoi(loan_id_str);

sprintf(query, "SELECT * FROM loans WHERE loan_id=%d", loan_id);

if (mysql_query(con, query)) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

MYSQL_RES* result = mysql_store_result(con);

if (result == NULL) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int num_rows = mysql_num_rows(result);

if (num_rows == 0) {
    printf("Loan not found.\n");
} else {
    MYSQL_ROW row = mysql_fetch_row(result);
    int is_paid = atoi(row[5]);

    if (is_paid == 1) {
        printf("This loan has already been paid.\n");
    } else {
        printf("Loan ID: %s\n", row[0]);
        printf("Amount given: $%s\n", row[2]);

        old = atoi(row[2]);
        amounts = old + (old * months * rate) / 100;

        printf("Amount to be paid: $%d\n", amounts);

        printf("Are you sure you want to pay back this loan? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            printf("Enter the account number to withdraw: ");
            scanf("%s", from_acc);
            printf("Enter your PIN: ");
            scanf("%s", pin);

            sprintf(query, "SELECT * FROM customers WHERE accountnumber='%s' AND pin='%s'", from_acc, pin);

            if (mysql_query(con, query)) {
                fprintf(stderr, "%s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
            }

            result = mysql_store_result(con);

            if (result == NULL) {
                fprintf(stderr, "%s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
            }

            num_rows = mysql_num_rows(result);

            if (num_rows == 0) {
                printf("Invalid account number or PIN.\n");
            } else {
                MYSQL_ROW row = mysql_fetch_row(result);

                int balance = atoi(row[6]);
                int new_balance = balance - amounts;

                if (new_balance < 0) {
                    printf("Withdrawal amount exceeds balance. Please try again.\n");
                } else {
                    sprintf(new_balance_str, "%d", new_balance);

                    sprintf(query, "UPDATE customers SET amount='%s' WHERE accountnumber='%s'", new_balance_str, from_acc);

if (mysql_query(con, query)) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

sprintf(Query, "UPDATE loans SET status='paid' WHERE loan_id=%d", loan_id);

if (mysql_query(con, Query)) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

printf("Loan successfully paid.\n");
}
}
}
}


mysql_free_result(result);
menu(con);
}
}


int main() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    auth(con);

    menu(con);

    mysql_close(con);

    return 0;
}

