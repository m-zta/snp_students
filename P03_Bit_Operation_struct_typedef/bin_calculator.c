#include <stdio.h>
#include <stdlib.h>

#define OPERAND_BUFFER_SIZE 10

typedef struct {
    /*
    Students: The Expression struct should hold the two operands and
    the operation (use a char for the operation)
    */
    // DONE

    int  first;
    int  second;
    char operator;
} Expression;

int bits_per_int() {
    return sizeof(unsigned int) * 8;
}

unsigned int parse_operand(char operand_str[]) {
    unsigned int operand;

    if (operand_str[0] == '0' && operand_str[1] == 'x') {
        sscanf(&operand_str[2], "%x", &operand);
    } else if (operand_str[0] == '0') {
        sscanf(&operand_str[1], "%o", &operand);
    } else {
        sscanf(operand_str, "%u", &operand);
    }

    return operand;
}

void print_binary(unsigned int value) {
    // Students: Print a single number as a binary string
    // DONE

    // Example: 00000000'00000000'00000000'01001100
    // Push the bits to the right and mask with 1 to get the last bit
    for (int i = bits_per_int() - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);

        // Print a ' after every 8 bits
        if (i % 8 == 0) {
            printf("'");
        }
    }

    printf("\n");
}

void print_bit_operation_bin(Expression expression, unsigned int result) {
    /*
    Students: Print the entire operation in bin including the result

    Bin:
    00000000'00000000'00000000'00001100
    00000000'00000000'00000000'00001111 ^
    -----------------------------------
    00000000'00000000'00000000'00000011
    */
    // DONE

    printf("Bin:\n");
    print_binary(expression.first);
    printf("%c\n", expression.operator);
    print_binary(expression.second);
    printf("-----------------------------------\n");
    print_binary(result);
    printf("\n");
}

void print_bit_operation_hex(Expression expression, unsigned int result) {
    /*
    Students: Print the entire operation in hex including the result

    Hex:
    0x0c ^ 0x0f = 0x03
    */
    // DONE

    printf("Hex:\n");
    printf("0x%02x %c 0x%02x = 0x%02x\n", expression.first, expression.operator,
           expression.second, result);

    printf("\n");
}

void print_bit_operation_dec(Expression expression, unsigned int result) {
    /*
    Students: Print the entire operation in hex including the result

    Dec:
    12 ^ 15 = 3
    */
    // DONE

    printf("Dec:\n");
    printf("%d %c %d = %d\n", expression.first, expression.operator,
           expression.second, result);

    printf("\n");
}

unsigned int bit_operation(Expression expression) {
    // Students: Do the actual bit operation and return the result
    // DONE

    switch (expression.operator) {
        case '&':
            return expression.first & expression.second;
        case '|':
            return expression.first | expression.second;
        case '^':
            return expression.first ^ expression.second;
        default:
            return 0;
    }
}

int main() {
    char operand1_str[10];
    char operand2_str[10];
    char operation;

    unsigned int operand1, operand2;

    do {
        printf("Geben sie die Bit-Operation ein:\n");

        scanf("%s %c %s", operand1_str, &operation, operand2_str);

        operand1 = parse_operand(operand1_str);
        operand2 = parse_operand(operand2_str);

        // Students: Create an expression
        // DONE
        Expression expression = {operand1, operand2, operation};

        unsigned int result = bit_operation(expression);
        print_bit_operation_bin(expression, result);
        print_bit_operation_hex(expression, result);
        print_bit_operation_dec(expression, result);

        // Clear the input buffer
        while (getchar() != '\n')
            ;

        printf("\nMöchten sie weiter machen oder abbrechen? [(n)ext|(q)uit] ");

    } while (getchar() == 'n');

    printf("Bye..\n");
    return EXIT_SUCCESS;
}
