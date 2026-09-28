#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
    /* stego Image info */
    char *steg_image_fname;
    FILE *fptr_steg_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_buffer[MAX_IMAGE_BUF_SIZE];

    /* decoded msg file info */
    uint extnsize;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    uint size_secret_file;
    uint data_byte_offset;
    

    /*  msg decoded file*/
    char *msg_decode_fname;
    FILE *fptr_msg_decode;

} DecodeInfo;


/* decoding function prototype */

/* Check operation type */
OperationType decode_check_operation_type(char *argv[]);

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status decode_open_files(DecodeInfo *decInfo);

/* check at what byte data starts */

Status check_start_of_data(FILE *fptr_steg_file,DecodeInfo *decInfo);


/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

Status decode_data_from_image(char *buffer ,int size,DecodeInfo *decInfo);

Status decode_extn_size(int size, DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(int size,char *file_extn, DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

char decode_byte_from_img(DecodeInfo *decInfo);

Status  decode_int_from_lsb(int *ptrArr,int size,DecodeInfo *decInfo);




#endif
