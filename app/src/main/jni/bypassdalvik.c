/*
 * Copyright 2014 http://Bither.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include "jpeglib.h"
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */
#include "config.h"
#include "rotate.c"
#include "compress.h"
#include "resize.h"


jint Java_net_bither_util_NativeUtil_compressJpeg(JNIEnv* env,jobject thiz,jstring file,jstring fileout,jint quality)
{
    const char *in = (*env)->GetStringUTFChars(env, file, JNI_FALSE);
    const char *out = (*env)->GetStringUTFChars(env, fileout, JNI_FALSE);

    return compress(in,out,(int)quality);
}


jint Java_net_bither_util_NativeUtil_resizeJpeg(JNIEnv* env,jobject thiz,jstring file,jstring fileout,jfloat factor)
{
    const char *in = (*env)->GetStringUTFChars(env, file, JNI_FALSE);
    const char *out = (*env)->GetStringUTFChars(env, fileout, JNI_FALSE);

    return zoom_jpeg_file(in,out,(float)factor);
}

jint Java_net_bither_util_NativeUtil_rotateJpeg(JNIEnv* env,jobject thiz,jstring file,jstring fileout) {
    const char *in = (*env)->GetStringUTFChars(env, file, JNI_FALSE);
    const char *out = (*env)->GetStringUTFChars(env, fileout, JNI_FALSE);

    return rotate(in,out);
}

