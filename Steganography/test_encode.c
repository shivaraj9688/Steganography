#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv[]);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Invalid arguments\n");
        printf("Usage:\n");
        printf("Encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("Decoding : ./a.out -d stego.bmp [output.txt]\n");
        return 1;
    }

    printf("Lets see what you have selected:\n");

    if (check_operation_type(argv) == e_encode)
    {
        printf("Congrats! you have selected encoding\n");

        EncodeInfo stegnography;

        if (read_and_validate_encode_args(argv, &stegnography) == e_success)
        {
            printf("Read and validated encode arguments successfully\n");
            printf("--------Lets start encoding--------\n");

            if (do_encoding(&stegnography) == e_success)
            {
                printf("Completed encoding successfully\n");
            }
            else
            {
                printf("Failed to perform encoding\n");
                return 1;
            }
        }
        else
        {
            printf("Failed to validate encode arguments\n");
            return 1;
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        printf("Congrats! you have selected decoding\n");

        DecodeInfo decInfo;

        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("Read and validated decode arguments successfully\n");

            if (do_decoding(&decInfo) == e_success)
            {
                printf("Completed decoding successfully\n");
            }
            else
            {
                printf("Failed to perform decoding\n");
                return 1;
            }
        }
        else
        {
            printf("Failed to validate decode arguments\n");
            return 1;
        }
    }
    else
    {
        printf("Invalid option\n");
        printf("Usage:\n");
        printf("Encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("Decoding : ./a.out -d stego.bmp [output.txt]\n");
        return 1;
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}