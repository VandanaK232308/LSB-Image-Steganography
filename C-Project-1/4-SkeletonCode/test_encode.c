/*
NAME : Vandana K
DATE : 26/03/2024
PROJECT TITLE : LSB Image Steganography.
DESCRIPTION : The art and science of hiding information by embedding messages within other, seemingly harmless messages. 
			  Bits of unused data are replaced by bits of valuable information using LSB mechanism. Sender and receiver 
			  will have individual key / secret based on which they will be able to extract the actual data from the image. 
			  This project also gives basic level understanding of image processing methodologies.
SAMPLE EXECUTION :  vandana@DESKTOP-RCS3BHE:~/23034B/AdvancedC/Project/C-Project-1 (4)/C-Project-1/C-Project-1/4-SkeletonCode$ gcc *.c
				    vandana@DESKTOP-RCS3BHE:~/23034B/AdvancedC/Project/C-Project-1 (4)/C-Project-1/C-Project-1/4-SkeletonCode$ ./a.out -e beautiful.bmp secret.txt
				    Selected Encoding
					Read and Validate successfull
					Opened File Successfully.
					width = 1024
					height = 768
					Source capacity verified
					bmp header copied successfully.
					Encoded magic string successfully.
					Extension size encoded successfully.
					Secret file extension encoded successfully.
					Secret file size encoded successfully.
					Secret file data encoded successfully.
					Remaining image copied successfully.
					Encoding Successfull
					vandana@DESKTOP-RCS3BHE:~/23034B/AdvancedC/Project/C-Project-1 (4)/C-Project-1/C-Project-1/4-SkeletonCode$ ./a.out -d stego.bmp decode
					Selected Decoding
					Read and Validate successfull
					Opened File Successfully.
					bmp header skipped successfully
					Magic string after decode #*
					#*Decoded magic string successfully.
					extn_size = 4096
					Extension size decoded successfully.
					Output file not mentioned creating a file from secret_fname
					out file name decode extn name.txt
					in extn name
					Opened secret_fname now validate
					Secret file extension decoded successfully.
					Secret File size : 20
					Secret file size decoded successfully.
					Decoded string =    This is Secret!.

					Secret file data decoded successfully.
					Decoding Successfull
					*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;  //Initialize encode structure.
    uint img_size;

    DecodeInfo decInfo;  //Initialize decode structure.

    int ret = check_operation_type(argv[1]);  //Initialize ret to check operation type function using argument 1.
    if(ret == e_encode)  //Check if ret is equal to encode.
    {
	printf("Selected Encoding\n");
	if(read_and_validate_encode_args(argv, &encInfo) == e_success)  //check the read and validate encode arguments.
	{
	    printf("Read and Validate successfull\n");  //If true print the message.
	    if(do_encoding(&encInfo) == e_success)  //If encoding is selected.
	    {
		printf("Encoding Successfull\n");  //Print message.
	    }
	    else
	    {
		printf("Encoding Unsuccessfull\n");  //Else print error.
	    }

	}
	else
	{
	    printf("Read and Validate was not successfull\n");  //else print error.
	}
    }
    else if (ret == e_decode)  //Check for decoding.
    {
	printf("Selected Decoding\n");
	if(read_and_validate_decode_args(argv, &decInfo) == e_success)  //Check for read and validate decode args function.
	{
	    printf("Read and Validate successfull\n");  //If true print message.
	    if(do_decoding(&decInfo) == e_success)  //If decoding properly.
	    {
		printf("Decoding Successfull\n");  //Print the message.
	    }
	    else
	    {
		printf("Decoding Unsuccessfull\n");  //else print error message.
	    }

	}
    }
    else
    {
	printf("ERROR\n");  //else print error.
    }

}

/*OperationType check_operation_type (char *argv[])
  {
//check ooption is equal to "-e"(strcmp)
//If yes => return e_encode.
//Checkmoption is equal to "-d"(strcmp)
//If yes => return e_decode
//else
//return e_unsupported

if ( (strcmp(argv[1], "-e")) == 0)
{
return e_encode;
}
else if ( (strcmp(argv[1], "-d")) == 0)
{
return e_decode;
}
else
{
return e_unsupported;

}
}*/
