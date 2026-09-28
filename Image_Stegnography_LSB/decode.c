#include <stdio.h>
#include "decode.h"
#include <string.h>
#include "types.h"
#include "common.h"
/* Function Definitions */


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status decode_open_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_steg_image = fopen(decInfo->steg_image_fname, "rb");
    // Do Error handling
    if (decInfo->fptr_steg_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->steg_image_fname);

    	return e_failure;
    }


    //  msg decode file
    decInfo->fptr_msg_decode = fopen(decInfo->msg_decode_fname, "wb");
    // Do Error handling
    if (decInfo->fptr_msg_decode == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->msg_decode_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


OperationType decode_check_operation_type(char *argv[]){

    if(strcmp(argv[1] , "-e")==0){
        return e_encode;

    }
    else if((strcmp(argv[1] , "-d"))==0){
        return e_decode;
    }
    else
        return e_unsupported;

    return 0;


}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo){

    if(strcmp(strchr(argv[2],'.'),".bmp") == 0){
        decInfo->steg_image_fname = argv[2];
    }
    else
        return e_failure;
   
    if(argv[3]!=NULL){
        decInfo->msg_decode_fname = argv[3];
    }
    else
        decInfo->msg_decode_fname = "msg_decoded_file.txt";

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo){

    if(decode_open_files(decInfo) == e_success){
        printf("open files fucntion success\n");

        if(check_start_of_data(decInfo->fptr_steg_image,decInfo) == e_success){
            printf("check start of data fucntion is success\n");

            if(decode_magic_string(decInfo) == e_success){
                printf("decode magic string is success\n");

                if (decode_extn_size(4,decInfo) == e_success)
                {
                    printf("decode extn size funciton success\n");

                    if(decode_secret_file_extn(4,decInfo->extn_secret_file,decInfo)==e_success){
                        printf("decode secret file extension success\n");

                        if(decode_secret_file_size(decInfo)==e_success){
                            printf("decode secret file size success\n");

                            if(decode_secret_file_data(decInfo) == e_success){
                                printf("decode secret file data success\n");
                            }
                            else{
                                printf("decode secret file data failed\n");
                            }
                        }
                        else{
                            printf("decode secret file size failed\n");
                            return e_failure;
                        }
                        
                    }
                    else{
                        printf("decode secret file extension failed\n");
                        return e_failure;
                    }

                    
                }
                else{
                    printf("decode extn size funciton failed\n");
                    return e_failure;
                }
                

            }
            else{
                printf("decode magic string function failed\n");
                return e_failure;
            }

        }
        else{
            printf("check start of data fucntion is failed\n");
            return e_failure;
        }

    }
    else{
        printf("open files fucntion failed\n");
        return e_failure;
    }

    return e_success;
}

Status check_start_of_data(FILE *fptr_steg_file,DecodeInfo *decInfo){

    fseek(fptr_steg_file,10,SEEK_SET);

    fread(&decInfo->data_byte_offset,sizeof(unsigned int), 1,fptr_steg_file);

    //printf("Pixel data starts at byte: %u\n", decInfo->data_byte_offset);

    return e_success;

}

Status decode_magic_string(DecodeInfo *decInfo){

    char str[3];
    
    fseek(decInfo->fptr_steg_image,decInfo->data_byte_offset,SEEK_SET);

    decode_data_from_image(str,2,decInfo);

    if(strcmp(str,MAGIC_STRING)==0)
    {
        printf("Magic string matched\n");
        /*printf("Decoded chars = '%c' '%c'\n", str[0], str[1]);
        printf("Hex = %02X %02X\n",
        (unsigned char)str[0],
        (unsigned char)str[1]);*/
            return e_success;
    }

    //printf("Magic string mismatch : %s\n",str);
    /*printf("Decoded chars = '%c' '%c'\n", str[0], str[1]);   //  this is for debugging
    printf("Hex = %02X %02X\n",
       (unsigned char)str[0],
       (unsigned char)str[1]);*/


    return e_failure;
}

Status decode_extn_size(int size, DecodeInfo *decInfo){

    int size_data[4]={0};
        

    decode_int_from_lsb(size_data,size,decInfo);
    decInfo->extnsize =0;
    for(int i=0;i<size;i++){
        decInfo->extnsize = (decInfo->extnsize | (size_data[size-i-1] << (i*8)));
    }

    //printf("extn size : %d\n",decInfo->extnsize);
    
    


    return e_success;



}

Status decode_int_from_lsb(int *ptrArr , int size,DecodeInfo *decInfo){

    for(int i=0;i<size;i++){
        fread(decInfo->image_buffer,1,8,decInfo->fptr_steg_image);
        ptrArr[i] = decode_byte_from_img(decInfo);
    }

    return e_success;

    
}

Status decode_secret_file_extn(int size, char *file_extn, DecodeInfo *decInfo){

    decode_data_from_image(file_extn,size,decInfo);

    //printf("secret extn name : %s\n",decInfo->extn_secret_file);
    return e_success;
    

}

Status decode_secret_file_size(DecodeInfo *decInfo){

    int size_data[4]={0};
        

    decode_int_from_lsb(size_data,4,decInfo);
    decInfo->size_secret_file =0;
    for(int i=0;i<4;i++){
        decInfo->size_secret_file = (decInfo->size_secret_file | (size_data[4-i-1] << (i*8)));
    }

    //printf("size_secret_file : %d\n",decInfo->size_secret_file);
    return e_success;

}

Status decode_secret_file_data(DecodeInfo *decInfo){

    char file_data[decInfo->size_secret_file +1];

    decode_data_from_image(file_data,decInfo->size_secret_file ,decInfo);
    
    //printf("secret msg :%s\n",file_data);

    //fputs(file_data,decInfo->fptr_msg_decode);

    fwrite(file_data,1,decInfo->size_secret_file,decInfo->fptr_msg_decode);
    
    return e_success;

}



Status decode_data_from_image(char *buffer,int size,DecodeInfo *decInfo){
     
    for(int i=0;i<size;i++){
        
        fread(decInfo->image_buffer,1,8,decInfo->fptr_steg_image);
        buffer[i] = decode_byte_from_img(decInfo);
        
        
    }
    buffer[size] = '\0';

    return e_success;
    





}

char decode_byte_from_img(DecodeInfo *decInfo){

        char data =0;

        for(int i=0;i<8;i++){
            data = data | ((decInfo->image_buffer[i] & 1) << (7-i));
        }
        

        return data;


    
}





