#include <stdio.h>
#include <cs50.h>

int numberLength(long h);

int main(void)
{
    // Declare CC number from user input
    long cc_number;
    cc_number = get_long("Please enter your credit card number\n");

    if (numberLength(cc_number) < 13 || numberLength(cc_number) > 16)
    {
        printf("INVALID\n");
    }

    // Declare length variable from length function
    int length = numberLength(cc_number);

    // Isolate common bank identification numbers
    long bank_ID = cc_number;
    while (bank_ID > 100)
    {
        bank_ID /= 10;
    }

    if ((bank_ID / 10) == 4 || bank_ID == 34 || bank_ID == 37 || (bank_ID > 50 && bank_ID < 56))
    {
        // Initiate Luhn Algorithm
        long temp_cc = cc_number;
        long digit = 0;
        int x;
        int even_sum = 0;
        int odd_sum = 0;
        int luhn = 0;
        for (int i = 0; i < length; i++)
        {
            digit = (temp_cc % 10);
            temp_cc = (temp_cc - digit) / 10;

            // Generate sum of even digits
            if (i % 2 == 1)
            {
                x = (digit * 2);

                if (x >= 10)
                {
                    x = ((digit * 2) % 10) + 1;
                }
                even_sum += x;
            }

            // Generate sum of odd digits
            if (i % 2 != 1)
            {
                odd_sum += digit;
            }

            luhn = odd_sum + even_sum;
        }

        // Feeds back validity and issuing institution
        if (luhn % 10 != 0)
        {
            printf("INVALID\n");
        }
        else if (luhn % 10 == 0 && ((length == 15) && (bank_ID == 34 || bank_ID == 37)))
        {
            printf("AMEX\n");
        }
        else if (luhn % 10 == 0 && ((length == 13 || length == 16) && (bank_ID / 10 == 4)))
        {
            printf("VISA\n");
        }
        else if (luhn % 10 == 0 && ((length == 16) && (bank_ID > 50 && bank_ID < 56)))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }

    }
    else
    {
        printf("INVALID\n");
    }
}

// Function to isolate user input length
int numberLength(long input)
{
    int length = 0;
    while (input)
    {
        length++;
        input /= 10;
    }
    return length;
}