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

package net.bither.util;


/**
 *  所有native return 0 都代表操作成功
 */
public class NativeUtil {

	public static native int compressJpeg(String in,String out,int quality);

	//该方法比较耗时 较大图片4s左右
	public static native int resizeJpeg(String in,String out,float factor);

	public static native int rotateJpeg(String in,String out);


	static {
		System.loadLibrary("jpeg");
		System.loadLibrary("bypassdalvik");
	}

}