/*******************************************************************
 * Description : Hello World program
 *
 *******************************************************************/

#include "stdio.h"
#include <stdlib.h>
#include "limits.h"

int getBmpFileSize(FILE * fpbmp)
{
	int size = 0;

    fseek(fpbmp, 2, SEEK_SET);
    fread(&size, sizeof(char), 4, fpbmp);

    return size;
}

int getBmpWidth(FILE* fpbmp)
{
    int width = 0;

    fseek(fpbmp, 18L, SEEK_SET);
    fread(&width, sizeof(char), 4, fpbmp);

    return width;
}

int getBmpHeight( FILE* fpbmp )
{
    int height = 0;

    fseek(fpbmp, 22L, SEEK_SET);
    fread(&height, sizeof(char), 4, fpbmp);

    return height;
}

int getOffset(FILE * fpbmp)
{
	int offset = 0;

    fseek(fpbmp, 10L, SEEK_SET);
    fread(&offset, sizeof(char), 4, fpbmp);

    return offset;
}

int readBmpData( FILE* fpbmp )
{
    int i = 0, j = 0;

    int offset = getOffset(fpbmp);
    int width = getBmpWidth(fpbmp);
    int height = getBmpHeight(fpbmp);

    printf("\noffset = %d\n", offset);
    printf("\nheight = %d\n", height);
    printf("\nwidth = %d\n", width);
    unsigned char * pix = NULL;

    //one pix have 3 byte data( R G B )
    pix = (unsigned char*)malloc( 4 * sizeof( unsigned char ) );

    fseek(fpbmp, offset, SEEK_SET); // Jump to data part

    for( i = 0; i < height; i++ )
    {
        for( j = 0; j < width; j++ )
        {
            fread(pix, 3, 1, fpbmp);
            printf("(%3d,%3d,%3d) \n",pix[0],pix[1],pix[2]);
        }

        /*printf(",");
        for(;(j)%4!=0;j++)
        {
			fread(pix, 3, 1, fpbmp);
            printf("(%3d,%3d,%3d) ",pix[0],pix[1],pix[2]);
		}*/
        printf("\n");
    }
    return 0;
}

int ReadFile(const char* fileName, int arrSize, int elementSize, void *dst)
{
	FILE* fp = fopen(fileName,"rb");
	if (fp == NULL) {
		perror("open file failed!\n");
		return -1;
	}

	fread(dst, arrSize, elementSize, fp);

	fclose(fp);

	return 0;
}

int ReadFileBMP(const char* fileName, int arrSize, int elementSize, void *dst)
{
	FILE* fp = fopen(fileName,"rb");
	if (fp == NULL) {
		perror("open file failed!\n");
		return -1;
	}

	fseek(fp, 54L, 0);

	fread(dst, arrSize, elementSize, fp);

	fclose(fp);

	return 0;
}

int WriteFile(const char* fileName, int arrSize, int elementSize, void *dst)
{
	FILE* fp = fopen(fileName,"wb");
	if (fp == NULL) {
		perror("open file failed!\n");
		return -1;
	}

	fwrite(dst, elementSize, arrSize, fp);

	fclose(fp);

	return 0;
}

int covertRgb888ToYUVNV21(uint8_t* src, uint8_t* dst, int height, int width)
{
	int rgbStride = width * 3;
//	int yuvStride = width;

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	uint16_t y = 0;
	uint16_t u = 0;
	uint16_t v = 0;

	int row = 0;
	int col = 0;

//	FILE* fp = fopen("log.txt","w");

	for (int i = 0; i < height; i = i + 1) {
		for (int j = 0; j < rgbStride; j = j + 3) {
			r = src[i * rgbStride + j + 2];
			g = src[i * rgbStride + j + 1];
			b = src[i * rgbStride + j + 0];

			y = ((74 * r + 150 * g + 29 * b) >> 8);
			u = ((-43 * r - 84 * g + 112 * b) / 255) + 128;
			v = ((128 * r - 107 * g - 21 * b) / 255) + 128;

			//fprintf(fp,"r = %d, g = %d, b = %d, y = %d, u = %d, v = %d\n", r, g, b, y, u, v);

			row = (height - i - 1);
			col = j / 3;

			dst[row * width + col] =  y > 255 ? 255 : y < 0 ? 0 : y;
			//fprintf(fp,"row * width + col = %d, dst[row * width + col] = %d\n", row * width + col, dst[row*width+col]);
			dst[height * width + row / 2 * width + (col / 2) * 2] = v > 255 ? 255 : v < 0 ? 0 : v;
			//printf("height * width + row / 2 * width + (col / 2) * 2 = %d, dst[height * width + row / 2 * width + (col / 2) * 2] = %d\n", height * width + row / 2 * width + (col / 2) * 2, dst[height * width + row / 2 * width + (col / 2) * 2]);
			dst[height * width + row / 2 * width + (col / 2) * 2 + 1] = u > 255 ? 255 : u < 0 ? 0 : u;
		}
	}

//	for (int i = 0; i< height; i++) {
//		for (int j = 0; j< width; j++) {
//			fprintf(fp, "i = %d, j = %d, y = %d\n", i, j, dst[i*width+j]);
//		}
//	}

	//fclose(fp);

	return 0;

}

int main(void) {

//	FILE * fp_read ;
//
//	fp_read = fopen("girl_with_hat.bmp","rb");
//
//	if(fp_read == NULL)
//	{
//		printf("open file failed!\n");
//		return 0;
//	}
//
//	printf("width£º%d\n",getBmpWidth(fp_read));
//	printf("height£º%d\n",getBmpHeight(fp_read));
//	printf("\nsize:%d byte\n",getBmpFileSize(fp_read));
////	printf("bits £º%d\n",getBmpPixBits(fp_read));
//	printf("offset£º%d\n",getOffset(fp_read));
//
//	readBmpData(fp_read);
//
//	return 0;

	int height = 512;
    int width = 512;

	int rgbLen = height * width * 3;
	int yuvLen = height * width * 3 / 2;

	uint8_t* src = (uint8_t*)malloc(rgbLen * sizeof(uint8_t));
	uint8_t* dst = (uint8_t*)malloc(yuvLen * sizeof(uint8_t));

	ReadFileBMP("girl_with_hat.bmp", rgbLen, sizeof(uint8_t), src);

	for (int i = 0; i < 100; i++) {
		printf("src[%d] = %d\n", i, src[i]);
	}

//	printf("src[0] = %d\n", src[0]);
//	printf("src[1] = %d\n", src[1]);
//	printf("src[2] = %d\n", src[2]);

	covertRgb888ToYUVNV21(src, dst, height, width);

	WriteFile("girl_with_hat01.yuv", yuvLen, sizeof(uint8_t), dst);





	printf("%d\n",src[0]);

	free(src);
	free(dst);

	printf("Hello World");
	return EXIT_SUCCESS;

}
