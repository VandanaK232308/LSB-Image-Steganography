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

#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo  
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/*Encode seceret file extension size*/
Status encode_secret_file_extn_size(long extn_size, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/*Encode a size into LSB of image data array.*/
Status encode_size_to_lsb(long size, char *image_buffer);
#endif
