#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include<string.h>
#include "types.h"

int main(int argc , char **argv)
{

    if(check_operation_type(argv)== e_encode){
        printf("selected encoding\n");
        EncodeInfo encoding;
        if(read_and_validate_encode_args(argv,&encoding) == e_success){
            printf("read and validate encode  args is success\n");

            if(do_encoding(&encoding) == e_success){
                printf("Encoding is successfully done\n");
                
            }
            else{
                printf("Encoding is failed\n");
                return 1;
            }


        }
        else{
            printf("read and validate encode  args is failure\n");
        }

    }
    else if(decode_check_operation_type(argv)== e_decode){
        printf("selected decoding\n");
        DecodeInfo decoding;
        if(read_and_validate_decode_args(argv,&decoding) == e_success){
            printf("read and validate function is success\n");

            if(do_decoding(&decoding) == e_success){
                printf("decoding successfully completed\n");
            }
            else{
                printf("decoding is failed\n");
                return 0;
            }


        }
        else{
            printf("read and validate function failed\n");
            return 0;
        }

        
    }
    else
        printf("Invalid arguments\nFor endcode : ./a.out beautiful.bmp secret.txt [steg.bmp] , For decode : ./a.out steg.bmp\n");

    


    return 0;


}