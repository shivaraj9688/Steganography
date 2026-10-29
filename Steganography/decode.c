#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "common.h"
#include "types.h"

/* Open files */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

/* Read command line arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Invalid BMP file\n");
        return e_failure;
    }

    if (argv[3] != NULL)
        decInfo->output_fname = argv[3];
    else
        decInfo->output_fname = "decoded.txt";

    return e_success;
}

/* Decode one byte from 8 image bytes */
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;

    for(int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}

/* Decode integer */
Status decode_integer_from_lsb(int *num, DecodeInfo *decInfo)
{
    char image_buffer[32];

    fread(image_buffer, 32, 1, decInfo->fptr_stego_image);

    *num = 0;

    for(int i = 0; i < 32; i++)
    {
        *num = (*num << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}

/* Decode data */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char image_buffer[8];

    for(int i = 0; i < size; i++)
    {
        fread(image_buffer,8,1,decInfo->fptr_stego_image);
        decode_byte_from_lsb(&data[i],image_buffer);
    }

    return e_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic[3];

    decode_data_from_image(magic,2,decInfo);
    magic[2]='\0';

    if(strcmp(magic,MAGIC_STRING)==0)
    {
        printf("Magic String Matched\n");
        return e_success;
    }

    printf("Magic String Not Matched\n");
    return e_failure;
}

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    int size;

    decode_integer_from_lsb(&size,decInfo);

    decode_data_from_image(decInfo->extn_secret_file,size,decInfo);

    decInfo->extn_secret_file[size]='\0';

    return e_success;
}

/* Decode file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_integer_from_lsb(&decInfo->size_secret_file,decInfo);

    return e_success;
}

/* Decode secret file */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for(int i=0;i<decInfo->size_secret_file;i++)
    {
        char image_buffer[8];

        fread(image_buffer,8,1,decInfo->fptr_stego_image);

        decode_byte_from_lsb(&ch,image_buffer);

        fwrite(&ch,1,1,decInfo->fptr_output);
    }

    return e_success;
}

/* Main decode function */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("Opening files...\n");

    if(open_decode_files(decInfo)==e_failure)
        return e_failure;

    /* Skip BMP header */
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    printf("Decoding Magic String...\n");

    if(decode_magic_string(decInfo)==e_failure)
    {
        printf("Decoding Failed\n");
        return e_failure;
    }

    printf("Decoding Extension...\n");
    decode_secret_file_extn(decInfo);

    printf("Extension : %s\n",decInfo->extn_secret_file);

    printf("Decoding File Size...\n");
    decode_secret_file_size(decInfo);

    printf("File Size : %d bytes\n",decInfo->size_secret_file);

    printf("Decoding Secret File...\n");
    decode_secret_file_data(decInfo);

    printf("Decoding Completed Successfully\n");

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    return e_success;
}