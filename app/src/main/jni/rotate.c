#include <jpeglib.h>
#include <jerror.h>
#include <logger.h>
#include "transupp.h"       /* Support routines for jpegtran */



/* various flags */
#define JFLAG_TRANSFORM_IMAGE      0x0001
#define JFLAG_TRANSFORM_THUMBNAIL  0x0002
#define JFLAG_TRANSFORM_TRIM       0x0004

#define JFLAG_UPDATE_COMMENT       0x0010
#define JFLAG_UPDATE_ORIENTATION   0x0020
#define JFLAG_UPDATE_THUMBNAIL     0x0040

#define JFLAG_FILE_BACKUP          0x0100
#define JFLAG_FILE_KEEP_TIME       0x0200

int rotate(char *in,char *out)
{
int tsize = 0;
JXFORM_CODE transform = JXFORM_ROT_90;
unsigned int flags =
	JFLAG_TRANSFORM_IMAGE     |
	JFLAG_TRANSFORM_THUMBNAIL |
	JFLAG_TRANSFORM_TRIM      |
	JFLAG_UPDATE_ORIENTATION;

flags &= ~JFLAG_UPDATE_ORIENTATION;


JCOPY_OPTION copyoption;
jpeg_transform_info transformoption;

struct jpeg_decompress_struct srcinfo;
 struct jpeg_compress_struct dstinfo;
 struct jpeg_error_mgr jsrcerr, jdsterr;
 jvirt_barray_ptr * src_coef_arrays;
 jvirt_barray_ptr * dst_coef_arrays;
//int file_index;
 srcinfo.err = jpeg_std_error(&jsrcerr);
 jpeg_create_decompress(&srcinfo);
 dstinfo.err = jpeg_std_error(&jdsterr);
 jpeg_create_compress(&dstinfo);
 jsrcerr.trace_level = jdsterr.trace_level;
 srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;

 copyoption = JCOPYOPT_DEFAULT;
 transformoption.transform = JXFORM_NONE;
 transformoption.trim = FALSE;
 transformoption.crop = FALSE;
 transformoption.force_grayscale = FALSE;
 transformoption.transform = JXFORM_ROT_90;


   FILE * infile = fopen(in, "rb");

 jpeg_stdio_src(&srcinfo, infile);
 jcopy_markers_setup(&srcinfo, copyoption);
 (void) jpeg_read_header(&srcinfo, TRUE);

 jtransform_request_workspace(&srcinfo, &transformoption);

 src_coef_arrays = jpeg_read_coefficients(&srcinfo);
 jpeg_copy_critical_parameters(&srcinfo, &dstinfo);
 dst_coef_arrays = jtransform_adjust_parameters(&srcinfo, &dstinfo,src_coef_arrays,&transformoption);


 FILE * outfile = fopen(out, "wb");
 jpeg_stdio_dest(&dstinfo, outfile);
 jpeg_write_coefficients(&dstinfo, dst_coef_arrays);
 jcopy_markers_execute(&srcinfo, &dstinfo, copyoption);
 jtransform_execute_transformation(
 &srcinfo,
  &dstinfo,
 src_coef_arrays,
 &transformoption);

 jpeg_finish_compress(&dstinfo);
 jpeg_destroy_compress(&dstinfo);
 (void) jpeg_finish_decompress(&srcinfo);
 jpeg_destroy_decompress(&srcinfo);
//
 if (infile!= stdin)
    fclose(infile);
 if (outfile!= stdout)
    fclose(outfile);


return 0;
}
