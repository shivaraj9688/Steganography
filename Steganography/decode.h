#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

/*
 * Structure to store decoding information
 */
typedef struct _DecodeInfo
{
    /* Stego Image File */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output Secret File */
    char *output_fname;
    FILE *fptr_output;

    /* Secret File Details */
    char extn_secret_file[10];
    int size_secret_file;

} DecodeInfo;

/* Read and validate command line arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open required files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode operations */
Status decode_magic_string(DecodeInfo *decInfo);

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

Status decode_byte_from_lsb(char *data, char *image_buffer);

Status decode_integer_from_lsb(int *num, DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo);

#endif