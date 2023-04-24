#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define BI_CONV(b) b[3] | b[2] << 8 | b[1] << 16 | b[0] << 24


int main()
{
    const char* path = "test_image.png";
    FILE* f = fopen(path, "rb");
    assert(f != NULL);

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET); 

    char* buff = malloc(fsize);
    assert(fread(buff, fsize, 1, f) != 0 );

    int current_pos = 0;
    int start_offset = 0;

    // signature
    unsigned char signature[9];
    memmove(signature, buff + start_offset, 8);
    signature[8] = '\0';
    start_offset += 8;
    printf("%s", signature);

    unsigned char length_in_bytes[4];
    memmove(length_in_bytes, buff + start_offset, 4);
    start_offset += 4;
    unsigned int length = BI_CONV(length_in_bytes);
    assert(length == 13);
    printf("%d\n", length);

    unsigned char ihdr[5];
    memmove(ihdr, buff+start_offset, 4);
    ihdr[4] = '\0';
    start_offset += 4;
    printf("%s\n", ihdr);

    unsigned char w[4];
    memmove(w, buff + start_offset, 4);
    start_offset += 4;

    unsigned char h[4];
    memmove(h, buff+start_offset, 4);
    start_offset += 4;

    unsigned int width = BI_CONV(w);
    unsigned int heigth= BI_CONV(h);

    printf("width: %d - heigth: %d\n",  width, heigth);

    unsigned char depth[1];
    memmove(depth, buff + start_offset, 1);
    start_offset += 1;
    printf("bit depth: %d\n", depth[0]);

    unsigned char color_type[1];
    memmove(color_type, buff+start_offset, 1);
    start_offset += 1;
    printf("color_type: RGBA(%d)\n", color_type[0]);

    unsigned char compression_method[1];
    memmove(compression_method, buff+start_offset, 1);
    start_offset+=1;
    printf("compression_method: DEFLATE(%d)\n", compression_method[0]);

    unsigned char filter_method[1];
    memmove(filter_method, buff+start_offset, 1);
    start_offset+=1;
    printf("filter method: %d\n", filter_method[0]);
    
    unsigned char interlace_method[1];
    memmove(interlace_method, buff+start_offset, 1);
    start_offset+=1;
    printf("interlace_method: %d\n", interlace_method[0]);

    unsigned char checksum[5];
    memmove(checksum, buff+start_offset, 4);
    checksum[4] = '\0';
    start_offset+=4;
    printf("checksum %s\n", checksum);

    unsigned char l[4];
    memmove(l, buff+start_offset, 4);
    start_offset+=4;
    unsigned int length2 = BI_CONV(l);
    printf("%d\n", length2);

    unsigned char type2[5];
    memmove(type2, buff+start_offset, 4);
    start_offset+=4;
    type2[4] = '\0';
    printf("%s\n", type2);
    printf("start_offset: %d\n", start_offset);

    int i = 0;
    unsigned char * keyword = malloc(length2);
    memset(keyword, 0, length2);
    while( *(buff+start_offset) != '\0')
    {
        *(keyword + i) = *(buff+start_offset);
        start_offset+=1;
        i++;
    }
    printf("keyword: %s\n", keyword);
    printf("start_offset: %d\n", start_offset);

    unsigned char text_string[18];
    memmove(text_string, buff+start_offset, length2-i);
    text_string[17] = '\0';

    for (int i = 0; i < 18; i++) {
        printf("%c", text_string[i]);
    }
    printf("\n");
    start_offset += length2-i;
    start_offset += 4; // checksum

    printf("%d\n", start_offset);   

    free(buff);
    fclose(f);


    return EXIT_SUCCESS;
}
