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
#include "types.h"
#include "common.h"
#include <string.h>

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);  // Seek to 18th byte

    fread(&width, sizeof(int), 1, fptr_image);      // Read the width (an int)
    printf("width = %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image);      // Read the height (an int)
    printf("height = %u\n", height);

    return width * height * 3;      // Return image capacity
}

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");  //Open src_image_fname in read mode and store it back to fptr_src_image which is present in structure.
    if (encInfo->fptr_src_image == NULL)  //Check if fptr_src_image is empty or not.
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);  //If empty show error.

	return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");  //Open secret_fname in read mode and store it in fptr_secret which is in structure.
    if (encInfo->fptr_secret == NULL)   //Check if the secret file is empty or not.
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);  //If empty then display errors.

	return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");   //Now, open the stego image file name in write mode and store it in stego image which is in the structure.
    if (encInfo->fptr_stego_image == NULL)   //Check if the stego image is empty or not.
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);  //If empty show error.

	return e_failure;
    }

    return e_success;
}

OperationType check_operation_type (char argv[])
{

    if (argv == NULL )   //Check if the argument vector is empty or not.
    {
	printf("Error: Select encoding(-e) or decoding(-d) and the image file and secret text.\n");  //If empty show error.
    }
    else
    {
    if ( (strcmp(argv, "-e")) == 0)  //check ooption is equal to "-e"(strcmp)
    {
	return e_encode;      //If yes => return e_encode.
    }
    else if ( (strcmp(argv, "-d")) == 0)      //Checkmoption is equal to "-d"(strcmp)
    {
	return e_decode;      //If yes => return e_decode
    }
    else      //else
    {
	return e_unsupported;       //return e_unsupported
    }
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{

    if (argv[2] == NULL)
    {
	fprintf(stderr, "Error: Pass the image file and secret file.\n");
    return e_failure;
	}
	
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)      //check argv[2] => .bmp
    {
	encInfo -> src_image_fname = argv[2];      //If yes => store into structure
    }
    else
    {
	fprintf(stderr, "Error: Source image %s format should be .bmp\n", argv[2]);
	return e_failure;
    }

    if(argv[3] == NULL )
    {
	fprintf(stderr, "Error: Pass the secret file.\n");
	return e_failure;
    }
    if (strcmp(strstr(argv[3], "."), ".txt") == 0)      //check argv[3] os having extension or not.
    {
	encInfo -> secret_fname = argv[3];      //If yes => store into structure, take the extension and store
    }
    else
    {
	fprintf(stderr, "Error: Source image %s format should be .txt\n", argv[3]);
	return e_failure;
    }
    if (argv[4])      //check argv[4] passed or not
    {
	if (strcmp(strstr(argv[4],"."), ".bmp") == 0)      //passed => validate
	{
	    encInfo->stego_image_fname = argv[4];
	}
	else
	{
	    fprintf(stderr,"Error: Output image %s format should be .bmp\n", argv[4]);  //If the image extension is not .bmp then show error.
	    return e_failure;
	}
    }
    else
    {
	encInfo->stego_image_fname = "stego.bmp";      //not passed => store default name
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)  //Check for all function calls and output messages here.
{
    if( open_files(encInfo) == e_success )
    {
	printf("Opened File Successfully.\n");
	if(check_capacity(encInfo) == e_success)
	{
	    printf("Source capacity verified\n");
	    if (copy_bmp_header(encInfo->fptr_src_image, encInfo-> fptr_stego_image) == e_success)
	    {
		printf("bmp header copied successfully.\n");
		if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
		{
		    printf("Encoded magic string successfully.\n");
		    if ( encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
		    {
			printf("Extension size encoded successfully.\n");
			if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
			{
			    printf("Secret file extension encoded successfully.\n");
			    if ( encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success )
			    {
				printf("Secret file size encoded successfully.\n");
				if ( encode_secret_file_data(encInfo) == e_success )
				{
				    printf("Secret file data encoded successfully.\n");
				    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success )
				    {
					printf("Remaining image copied successfully.\n");
					return e_success;
				    }
				    else
				    {
					printf("Remaining image copied unsuccessfully.\n");
				    }
				}
				else
				{
				    printf("Secret file data encoded unsuccessfully.\n");
				}
			    }
			    else
			    {
				printf("Secret file size encoded unsuccessfully.\n");
			    }
			}
			else
			{
			    printf("Secret file extension encoded unsuccessfully.\n");
			}
		    }
		    else
		    {
			printf("Extension size encode unsuccessfully.\n");
		    }
		}
		else
		{
		    printf("Encoded magic string not successfull.\n");
		}
	    }
	    else
	    {
		printf("bmp header was not copied successfully.\n");
	    }
	}
	else
	{
	    printf("Source capicity not verified.\n");
	}
    }
    else
    {
	printf("Error in opening file.\n");
    }
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);   //Check for image capacity.
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);   //Check for secret file size.

    if( encInfo->image_capacity > ((( strlen(MAGIC_STRING) + strlen( encInfo->extn_secret_file ) + encInfo->size_secret_file + 4 + 4) * 8)))  //image capacity.
	return e_success;
    else
	return e_failure;                                                                                                                             
}

uint get_file_size(FILE *fptr)   //get file size.
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);  //Rewind the offset.
    char buffer[54];  //Initialize the buffer.
    fread(buffer, 54, 1, fptr_src_image);  //read the src image to buffer.
    fwrite(buffer, 54, 1, fptr_dest_image);  //write the buffer to dest image.
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];  //initialize buffer.
    for ( int i = 0; i < strlen(magic_string); i++ )
    {
	fread(buffer, 8, 1, encInfo->fptr_src_image);  //read the src image to buffer.
	encode_byte_to_lsb(magic_string[i], buffer);  //encode byte to lsb using the magic string and buffer.
	fwrite(buffer, 8, 1, encInfo->fptr_stego_image);  //write the buffer to stego image.
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;
    for ( i = 0; i <= 8; i++ )
    {
	image_buffer[i] = ((image_buffer[i] & 0xfe)|(data>>(7-i) & 0x01));  //encode byte to lsb using the bit wise operators.
    }
}

Status encode_size_to_lsb(long size, char *image_buffer)
{
    int i;
    for ( i = 0; i < 32; i++ )
    {
	image_buffer[i] = ((image_buffer[i] & 0xfe)|(size>>(31-i) & 0x01));  //encode size to lsb using bit wise operator.
    }

    return e_success;
}

Status encode_secret_file_extn_size(long extn_size, EncodeInfo *encInfo)
{
    char buffer[32];  //initialize butter.
    fread(buffer, 32, 1, encInfo->fptr_src_image);  //read the src image to buffer.
    encode_size_to_lsb(extn_size, buffer);  //encode size to lsb using extension size and buffer.
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);  //write the buffer to stego image.
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];  //initialize buffer.
    int i;
    for ( i = 0; i < strlen(file_extn); i++ )
    {
	fread(buffer, 8, 1, encInfo->fptr_src_image);  //read the src image to buffer.
	encode_byte_to_lsb(file_extn[i], buffer);  //encode the byte to lsb using file extension and buffer.
	fwrite (buffer, 8, 1, encInfo->fptr_stego_image);  //write the buffer to stego image.
    }
    return e_success;
}

Status encode_secret_file_size( long file_size, EncodeInfo *encInfo)
{
    char buffer[32];  //initialize buffer.
    encode_size_to_lsb(file_size, buffer);  //encode size to lsb using file size and buffer.
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file], temp[8];  //initialize buffer and temp.
    rewind(encInfo->fptr_secret);  //rewind the offset.
    fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret);  //read the secret file to buffer.

    int i;
    for ( i = 0; i < encInfo->size_secret_file; i++ )
    {
	fread(temp, 8, 1, encInfo->fptr_src_image);  //read the src image to temp.
	encode_byte_to_lsb(buffer[i], temp);  //encode byte to lsb using buffer and temp.
	fwrite(temp, 8, 1, encInfo->fptr_stego_image);  //write the temp to stego image.
    }
    return e_success;
}

//Status copy_remaining_img_data(encinfo->fptr_src, encInfo->fptr_dest)
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while( fread(&ch, 1, 1, fptr_src) > 0 )
	fwrite(&ch, 1, 1, fptr_dest);  //write the remaining characters to the destination.
    return e_success;
}
