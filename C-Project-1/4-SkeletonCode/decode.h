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

#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include "common.h"

/*
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

#define DECODE_FILE_EXTN_SIZE 32  //Get file extension size by decoding 32 bytes
#define DECODE_FILE_SIZE 32  //Get file size by decoding 32 bytes.

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
   // uint image_capacity;
    //uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    FILE *output;

    int extn_size;
    

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type_decode(char argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* check capacity */
//Status check_capacity(DecodeInfo *decInfo);

/* Get image size */
//uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size_decode(FILE *fptr);

/* Copy bmp image header */
Status skip_bmp_header(FILE *fptr_src_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/*Encode seceret file extension size*/
Status decode_secret_file_extn_size(long extn_size, DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_from_image(int size, FILE *fptr_src_image, DecodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
Status decode_lsb_to_byte(char *data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
//Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/*Encode a size into LSB of image data array.*/
Status decode_lsb_to_size(int* size, char *image_buffer);
#endif
