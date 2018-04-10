#include <stdio.h>
#include <jpeglib.h>
#include <malloc.h>
#include "compress.h"
#include <png.h>
#include <setjmp.h>
#include <unistd.h>
#include <libpng/pngstruct.h>
#include <libpng/pnginfo.h>

//#####################jpeg begin##################

void read_jpeg(char * filename, unsigned char * * data, int * width, int * height) {
    FILE * infile = fopen(filename, "rb");
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error( & jerr);
    jpeg_create_decompress( & cinfo);
    jpeg_stdio_src( & cinfo, infile);
    jpeg_read_header( & cinfo, TRUE);
    jpeg_start_decompress( & cinfo); * width = cinfo.output_width; * height = cinfo.output_height; * data = (unsigned char * ) malloc(
            cinfo.output_height * cinfo.output_width * cinfo.output_components);
    unsigned char * line_pointer;
    int i = 0;
    while (cinfo.output_scanline < cinfo.image_height) {
        line_pointer = * data + i * cinfo.output_width * cinfo.output_components;
        jpeg_read_scanlines( & cinfo, & line_pointer, 1);
        i++;
    }
    jpeg_finish_decompress( & cinfo);
    jpeg_destroy_decompress( & cinfo);

    fclose(infile);
    //free(&cinfo);
    //free(&jerr);
    //    free(line_pointer);
}

void write_jpeg(char * filename, unsigned char * * data, int * width, int * height, int quality) {
    FILE * outfile = fopen(filename, "wb");
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error( & jerr);
    jpeg_create_compress( & cinfo);
    jpeg_stdio_dest( & cinfo, outfile);
    cinfo.image_width = * width;
    cinfo.image_height = * height;
    cinfo.in_color_space = JCS_RGB; //JCS_GRAYSCALE:灰度图,JCS_RGB：彩色图
    cinfo.input_components = 3; // 在此为1,表示灰度图, 如果是彩色位图,则为3
    jpeg_set_defaults( & cinfo);
    jpeg_set_quality( & cinfo, quality, TRUE);
    jpeg_start_compress( & cinfo, TRUE);
    unsigned char * line_pointer;
    int i = 0;
    while (cinfo.next_scanline < cinfo.image_height) {
        line_pointer = * data + i * cinfo.image_width * cinfo.input_components;
        jpeg_write_scanlines( & cinfo, & line_pointer, 1);
        i++;
    }
    jpeg_finish_compress( & cinfo);
    jpeg_destroy_compress( & cinfo);

    fclose(outfile);

    //free(&cinfo);
    //free(&jerr);
    //free(line_pointer);
}

void
get_pixel_of(int x, int y, unsigned char * dest, unsigned char * * data, int * width, int * height) {
    if ((x >= * width) || (x < 0) || (y >= * height) || (y < 0)) {
        dest[0] = 0;
        dest[1] = 0;
        dest[2] = 0;
        return;
    }
    unsigned char * pos;
    pos = * data + (y * * width + x) * 3;
    dest[0] = pos[0];
    dest[1] = pos[1];
    dest[2] = pos[2];
}

void
set_pixel_of(int x, int y, unsigned char * dest, unsigned char * * data, int * width, int * height) {
    if ((x >= * width) || (x < 0) || (y >= * height) || (y < 0)) {
        return;
    }
    unsigned char * pos;
    pos = * data + (y * * width + x) * 3;
    pos[0] = dest[0];
    pos[1] = dest[1];
    pos[2] = dest[2];
}

int compress_jpeg(char * in , char * out, int quality) {

    if (quality < 1 && quality >= 100)
        return -1;

    int width, height = 0;
    unsigned char * data;

    read_jpeg( in , & data, & width, & height);
    unsigned char pixel[3] = {
            0,
            0,
            0
    };
    set_pixel_of(0, 0, pixel, & data, & width, & height);
    write_jpeg(out, & data, & width, & height, quality);

    free(data);
    //free(width);
    //free(height);
    return 0;
}

//############## jpeg end ##############

// #############png begin ###############

int compress_png(char * inname , char * outname,int level) {

    static png_FILE_p fpin;
    static png_FILE_p fpout;

    //读：
    png_structp read_ptr;
    png_infop read_info_ptr, end_info_ptr;
    //写
    png_structp write_ptr;
    png_infop write_info_ptr,write_end_info_ptr;
    //
    png_bytep row_buf;
    png_uint_32 y;
    int num_pass, pass;
    png_uint_32 width, height;//宽度，高度
    int bit_depth, color_type;//位深，颜色类型
    int interlace_type, compression_type, filter_type;//扫描方式，压缩方式，滤波方式
    //读
    row_buf = NULL;
    //打开读文件
    if ((fpin = fopen(inname, "rb")) == NULL)
    {
        LOGD(stderr,"Could not find input file %s\n", inname);
        return (1);
    }
    //打开写文件
    if ((fpout = fopen(outname, "wb")) == NULL)
    {
        LOGD("Could not open output file %s\n", outname);
        fclose(fpin);
        return (1);
    }
    //我们这里不处理未知的块unknown chunk
    //初始化1
    read_ptr =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    write_ptr =
            png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    read_info_ptr = png_create_info_struct(read_ptr);
    end_info_ptr = png_create_info_struct(read_ptr);
    write_info_ptr = png_create_info_struct(write_ptr);
    write_end_info_ptr = png_create_info_struct(write_ptr);
    //初始化2
    png_init_io(read_ptr, fpin);
    png_init_io(write_ptr, fpout);
    //读文件有high level(高层）和low level两种，我们选择从底层具体信息中读取。
    //这里我们读取所有可选。
    png_read_info(read_ptr, read_info_ptr);
    //（1）IHDR
    //读取图像宽度(width)，高度(height)，位深(bit_depth)，颜色类型(color_type)，压缩方法(compression_type)
    //滤波器方法(filter_type),隔行扫描方式(interlace_type)
    if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
                     &color_type, &interlace_type, &compression_type, &filter_type))
    {
        //我们采用默认扫描方式
        png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
                     color_type, PNG_INTERLACE_NONE, compression_type, filter_type);
    }


    //（2）cHRM
    //读取白色度信息  白/红/绿/蓝 点的x,y坐标，这里采用整形，不采用浮点数
    png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x,blue_y;

    if (png_get_cHRM_fixed(read_ptr, read_info_ptr, &white_x, &white_y,
                           &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y))
    {
        png_set_cHRM_fixed(write_ptr, write_info_ptr, white_x, white_y, red_x,
                           red_y, green_x, green_y, blue_x, blue_y);
    }
    //（3）gAMA
    png_fixed_point gamma;

    if (png_get_gAMA_fixed(read_ptr, read_info_ptr, &gamma))
        png_set_gAMA_fixed(write_ptr, write_info_ptr, gamma);
    //（4）iCCP
    png_charp name;
    png_bytep profile;
    png_uint_32 proflen;

    if (png_get_iCCP(read_ptr, read_info_ptr, &name, &compression_type,
                     &profile, &proflen))
    {
        png_set_iCCP(write_ptr, write_info_ptr, name, compression_type,
                     profile, proflen);
    }
    //(5)sRGB
    int intent;
    if (png_get_sRGB(read_ptr, read_info_ptr, &intent))
        png_set_sRGB(write_ptr, write_info_ptr, intent);
    //(7)PLTE
    png_colorp palette;
    int num_palette;

    if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette))
        png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
    //(8)bKGD
    png_color_16p background;

    if (png_get_bKGD(read_ptr, read_info_ptr, &background))
    {
        png_set_bKGD(write_ptr, write_info_ptr, background);
    }
    //(9)hist

    png_uint_16p hist;

    if (png_get_hIST(read_ptr, read_info_ptr, &hist))
        png_set_hIST(write_ptr, write_info_ptr, hist);
    //(10)oFFs
    png_int_32 offset_x, offset_y;
    int unit_type;

    if (png_get_oFFs(read_ptr, read_info_ptr, &offset_x, &offset_y,
                     &unit_type))
    {
        png_set_oFFs(write_ptr, write_info_ptr, offset_x, offset_y, unit_type);
    }
    //(11)pCAL
    png_charp purpose, units;
    png_charpp params;
    png_int_32 X0, X1;
    int type, nparams;

    if (png_get_pCAL(read_ptr, read_info_ptr, &purpose, &X0, &X1, &type,
                     &nparams, &units, &params))
    {
        png_set_pCAL(write_ptr, write_info_ptr, purpose, X0, X1, type,
                     nparams, units, params);
    }
    //(12)pHYs

    png_uint_32 res_x, res_y;

    if (png_get_pHYs(read_ptr, read_info_ptr, &res_x, &res_y, &unit_type))
        png_set_pHYs(write_ptr, write_info_ptr, res_x, res_y, unit_type);
    //(13)sBIT
    png_color_8p sig_bit;

    if (png_get_sBIT(read_ptr, read_info_ptr, &sig_bit))
        png_set_sBIT(write_ptr, write_info_ptr, sig_bit);
    //（14）sCAL
    int unit;
    png_charp scal_width, scal_height;

    if (png_get_sCAL_s(read_ptr, read_info_ptr, &unit, &scal_width,
                       &scal_height))
    {
        png_set_sCAL_s(write_ptr, write_info_ptr, unit, scal_width,
                       scal_height);
    }
    //(15)iTXt
    png_textp text_ptr;
    int num_text;

    if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0)
    {
        png_set_text(write_ptr, write_info_ptr, text_ptr, num_text);
    }
    //(16)tIME,这里我们不支持RFC1123
    png_timep mod_time;

    if (png_get_tIME(read_ptr, read_info_ptr, &mod_time))
    {
        png_set_tIME(write_ptr, write_info_ptr, mod_time);
    }
    //(17)tRNS
    png_bytep trans_alpha;
    int num_trans;
    png_color_16p trans_color;

    if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans,
                     &trans_color))
    {
        int sample_max = (1 << bit_depth);
        /* libpng doesn't reject a tRNS chunk with out-of-range samples */
        if (!((color_type == PNG_COLOR_TYPE_GRAY &&
               (int)trans_color->gray > sample_max) ||
              (color_type == PNG_COLOR_TYPE_RGB &&
               ((int)trans_color->red > sample_max ||
                (int)trans_color->green > sample_max ||
                (int)trans_color->blue > sample_max))))
            png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans,
                         trans_color);
    }

    //(18)控制压缩level
//check params invalid.
    if(level<1 || level>9){
        level = Z_BEST_COMPRESSION;
    }
    png_set_compression_level(write_ptr,level);

    //写进新的png文件中
    png_write_info(write_ptr, write_info_ptr);
    //读真正的图像数据
    num_pass = 1;
    for (pass = 0; pass < num_pass; pass++)
    {
        for (y = 0; y < height; y++)
        {
            //分配内存
            row_buf = (png_bytep)png_malloc(read_ptr,
                                            png_get_rowbytes(read_ptr, read_info_ptr));
            png_read_rows(read_ptr, (png_bytepp)&row_buf, NULL, 1);
            png_write_rows(write_ptr, (png_bytepp)&row_buf, 1);
            png_free(read_ptr, row_buf);
            row_buf = NULL;
        }
    }
    //结束
    png_read_end(read_ptr, end_info_ptr);
    //
    //tTXt
    if (png_get_text(read_ptr, end_info_ptr, &text_ptr, &num_text) > 0)
    {
        png_set_text(write_ptr, write_end_info_ptr, text_ptr, num_text);
    }
    //tIME
    if (png_get_tIME(read_ptr, end_info_ptr, &mod_time))
    {
        png_set_tIME(write_ptr, write_end_info_ptr, mod_time);
    }
    //
    png_write_end(write_ptr, write_end_info_ptr);
    //回收
    png_free(read_ptr, row_buf);
    row_buf = NULL;
    png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
    png_destroy_info_struct(write_ptr, &write_end_info_ptr);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    //
    fclose(fpin);
    fclose(fpout);
    //测试，比较两个PNG文件是否相同
    if ((fpin = fopen(inname, "rb")) == NULL)
    {
        LOGD("Could not find file %s\n", inname);
        return (1);
    }

    if ((fpout = fopen(outname, "rb")) == NULL)
    {
        LOGD("Could not find file %s\n", outname);
        fclose(fpin);
        return (1);
    }
    char inbuf[256], outbuf[256];
    for (;;)
    {
        png_size_t num_in, num_out;

        num_in = fread(inbuf, 1, 1, fpin);
        num_out = fread(outbuf, 1, 1, fpout);

        if (num_in != num_out)
        {
            LOGD("\nFiles %s and %s 大小不同\n",
                   inname, outname);

            fclose(fpin);
            fclose(fpout);
            return (0);
        }

        if (!num_in)
            break;

        if (memcmp(inbuf, outbuf, num_in))
        {
            LOGD("\nFiles %s and %s 内容不同\n", inname, outname);
            fclose(fpin);
            fclose(fpout);
            return (0);
        }
    }

    fclose(fpin);
    fclose(fpout);

    return (0);
}

//############### png end #################
