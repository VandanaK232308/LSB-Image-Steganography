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
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>

Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->src_image_fname="stego.bmp";   //Give the src image file name as stego.bmp.
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");  //open the src image file name in read mode.
    if (decInfo->fptr_src_image == NULL)  //check if src image is empty or no.
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);  //if empty show error.

	return e_failure;
    }
    return e_success;
}

OperationType check_operation_type_decode (char argv[])
{

    if (argv == NULL )  //check if the argument vector is NULL or not.
    {
	printf("Error: Select encoding(-e) or decoding(-d) and the image file.\n");  //If NULL display error.
    return e_failure;
	}
		
    if ( (strcmp(argv, "-e")) == 0)      //check ooption is equal to "-e"(strcmp)
    {
	return e_encode;      //If yes => return e_encode.
    }
    else if ( (strcmp(argv, "-d")) == 0)      //Checkmoption is equal to "-d"(strcmp)
    {
	return e_decode;      //If yes => return e_decode
    }
    else      //else
    {
	return e_unsupported;      //return e_unsupported

    }
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (argv[2] == NULL )
    {
	fprintf(stderr, "Error: Pass the image file.\n");
    return e_failure;
	}
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)      //check argv[2] => .bmp
    {
	decInfo -> stego_image_fname = argv[2];      //If yes => store into structure
    }
    else
    {
	fprintf(stderr, "Error: Source image %s format should be .bmp\n", argv[2]);
	return e_failure;
    }
    if (argv[3])
    {
	strcpy(decInfo -> secret_fname, strtok(argv[3],"."));      //check argv[3] os having extension or not.
    }
    else
    {
	strcpy(decInfo -> secret_fname, "output1");          //not passed => store default name
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)   //Call the functions which are used for this code.
{
    if( open_files_decode(decInfo) == e_success )
   {
	printf("Opened File Successfully.\n");
	printf("bmp header skipped successfully\n");
	if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
	{
	    printf("Decoded magic string successfully.\n");
	    if ( decode_secret_file_extn_size((decInfo->extn_size), decInfo) == e_success)
	    {
		printf("Extension size decoded successfully.\n");
		if(decode_secret_file_extn(decInfo) == e_success)
		{
		    printf("Secret file extension decoded successfully.\n");
		    if ( decode_secret_file_size(decInfo) == e_success )
		    {
			printf("Secret file size decoded successfully.\n");
			if ( decode_secret_file_data(decInfo) == e_success )
			{
			    printf("Secret file data decoded successfully.\n");
			}
			else
			{
			    printf("Secret file data decoded unsuccessfully.\n");
			    return e_failure;
			}
		    }
		    else
		    {
			printf("Secret file size decoded unsuccessfully.\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Secret file extension decoded unsuccessfully.\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Extension size decode unsuccessfully.\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Decoded magic string not successfull.\n");
	    return e_failure;
	}

    }
    else
    {
	printf("Error in opening file.\n");
	return e_failure;
    }
    return e_success;
}

uint get_file_size_decode(FILE *fptr)  //decode file size.
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

Status skip_bmp_header (FILE *fptr_stego_image)   //skip the bmp header.
{
    fseek(fptr_stego_image, 54, SEEK_SET);  
    return e_success;
}
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[8];  //initialize buffer to 8.
    char ch[strlen(magic_string)+1];  //initialize ch.
    char ch_temp = 0;  //initialize ch_temp to 0.
    skip_bmp_header(decInfo ->fptr_src_image); 
    int i;
    for (i = 0; i < strlen(magic_string); i++ )
    {
	ch_temp = 0;
	fread(buffer, 8, 1, decInfo->fptr_src_image);  //read the src image to buffer.
	decode_lsb_to_byte(&ch_temp, buffer);  //decode lsb to byte.
	ch[i]=ch_temp;

    }
    ch[i] ='\0';  //Give null character at the end.
    printf("Magic string after decode %s\n",ch);  //Print magic string.

    printf("%s",ch);
    if(strcmp(ch,MAGIC_STRING) == 0)  //check if ch is equal to magic string or not.
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status decode_lsb_to_byte(char *data, char *image_buffer)
{
    int i;
    for ( i = 0; i < 8; i++ )
    {
	*data = ((image_buffer[i] & 1)<<7-i)|*data;  //decode lsb to byte using bit wise operator.
    }

    return e_success;
}

Status decode_lsb_to_size(int* size, char *image_buffer)
{
    int i;
    for ( i = 0; i < 32; i++ )
    {
	*size = ((image_buffer[i] & 1) << 31-i) | *size;  //decode lsb to size using bit wise operator.
    }

    return e_success;
}

Status decode_secret_file_extn_size(long extn_size, DecodeInfo *decInfo)
{
    int size=0;
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_src_image);  //read the src image to the buffer.
    decode_lsb_to_size(&size, buffer);  //decode lsb to size.
    printf("extn_size = %ld\n", extn_size);
    decInfo->extn_size=size;

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{

    char buffer[8];
    int i;
    char ch_temp =0;
    for ( i = 0; i < decInfo->extn_size; i++ )
    {
	ch_temp = 0 ;
	fread(buffer,8,1,decInfo->fptr_src_image);  //read the src image to buffer.
	decode_lsb_to_byte(&ch_temp,buffer);
	decInfo->extn_secret_file[i] = ch_temp;  //give extn secret file size to ch_temp.
    }
    decInfo->extn_secret_file[i] = '\0';

    printf("Output file not mentioned creating a file from secret_fname\n");
    printf("out file name %s extn name%s\n",decInfo-> secret_fname,".txt");
	
    decInfo -> secret_fname = strcat(decInfo->secret_fname, ".txt");  //decode secret file name as .txt
	
	printf("in extn name \n");
    decInfo -> fptr_secret = fopen(decInfo -> secret_fname,"w");  //open the file secret in write mode.
    printf("Opened secret_fname now validate\n");
    if(decInfo -> secret_fname == NULL)  //If nothing is present in secret file,
    {
	fprintf(stderr,"Error : unable to open file.\n");  //display error.
	return e_failure;
    }
    return e_success;
}


Status decode_secret_file_size( DecodeInfo *decInfo)
{

    char buffer[32];
    char buffer_temp[100];
    FILE *fp;  //Initialize the file pointer as fp.
    fp = fopen("secret.txt", "r");  //open the file secret.txt in read mode.
    fread(buffer, 32, 1, fp);  //read the file details from fp to buffer.
    int size = 0;  //initialize size to 0.
    decode_lsb_to_size(&size, buffer);  //decode lsb to size.
    decInfo->size_secret_file = ( strlen(buffer) - 2 );  //file the decoded secret file size.
    printf("Secret File size : %lld\n",decInfo -> size_secret_file);  //print the file size.
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch[decInfo->size_secret_file+1];
    char buffer[8];
    int i;
    char ch_temp =0 ;
    for ( i = 0; i < decInfo->size_secret_file; i++)
    {
	ch_temp = 0;
	fread (buffer, 8, 1, decInfo->fptr_src_image);  //read the src image to buffer.
	decode_lsb_to_byte (&ch_temp, buffer);  //decode lsb to byte.
	ch[i] = ch_temp;
	fwrite (&ch_temp, 1, 1, decInfo -> fptr_secret);  //write the ch_temp to secret.
    }
    ch[i]='\0';
    printf("Decoded string = %s\n",ch);
    return e_success;

}
