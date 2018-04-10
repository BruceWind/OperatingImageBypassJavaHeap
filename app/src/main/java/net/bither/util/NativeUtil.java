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


	/**
	 *
	 * @param in
	 * @param out
	 * @param quality   must between 1-99
	 * @return  0 : success else :failed
	 */
	public static native int compressJpeg(String in,String out,int quality);


	/**
	 *  该方法比较耗时 较大图片4s左右
	 * @param in
	 * @param out
	 * @param factor  : <1 && >0.5f
	 * @return 0 : success else :failed
	 */
	public static native int resizeJpeg(String in,String out,float factor);

	/**
	 * 顺时针旋转 90×n 度数
	 * @param in
	 * @param out
	 * @param degreee  must in 90 ,180 ,270
	 * @return  0 : success else :failed
	 */
	public static native int rotateJpeg(String in,String out,int degreee);




	/**
	 *
	 * test记录：循环200次的内存的释放回收通过
	 *
	 * @param in
	 * @param out
	 * @param level   must between 1-10,不同于jpeg的参数,这里是压缩等级
	 *                if(level == 10) 可能会修改颜色,透明度,像素深度
	 * @return  0 : success else :failed
	 */
	public static native int compressPng(String in,String out,int level);


	static {
		System.loadLibrary("jpeg");
		System.loadLibrary("bypassdalvik");
	}

}