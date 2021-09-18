
bypass java heap, completely avoid OOM
------------------
[中文](https://github.com/BruceWind/OperatingImageBypassDalvik/blob/master/README_CN.md)

***Stop maintaining announcement:***
> [Due to the fact that devices above Android 8.0, bitmap store in  native heap](https://developer.android.com/topic/performance/graphics/manage-memory), devices will keep upgrade to newly, most of devices will above 8.0 so that this repo is no use. I'm very sorry about that.
------------------
Actually, it bypass Java heap to avoid OOM.

It is fact that Android JVM has set **xmx** param. Here I don't want to descripe what **xmx** is. You can look into some doc for learning it.

android's jvm Dalvik/ART has follow JVM standard，it has limitation of java heap size. So as you oprate large bitmap, device may throw OOM。

Although, the devices has going to improve the physical RAM, but the limitation of java heap still not enough. Furthermore, the camera is improve though, so the bitmap keep incresing though.
Beside, some brand or company who produce phones modify **xmx**, but the limitation still not enough for large bitmap. 

On the other hand, native heap has not limitation. It is able to allocate reach physical size.

To sum up, native heap is a free space I can use and the repo works in. All operations of bitmap in native heap that will compeletely avoid OOM. 



### assure the native heap doesn't have limitation.
I have tried to allocate numerous memory in native heap and don't execute `free`. The allocated memory is exceed 1G that still hasn't occured OOM.

Below a picture, I run ```adb shell dumpsys meminfo PACKAGENAME ``` to printed that memory information.

![](https://github.com/weizongwei5/my_blog_datasave/raw/62e952490c7fc3ef1f478c52985d4686331d17e0/img/native_memory_show.png)


## the repo progress

- [x] rotate  jpeg
- [x] compress jpeg
- [x]  resize jpeg

- [x] compress  png          (2018.04.10)  
    > - [ ] provide other algrithm about compression, may intererence `optipng` which has been used in most services or applications.
- [ ]  rotate  png
- [ ]  resize png

## build tips
before you run on device, you need put a `test.jpg` on external storage.

After you run: 
```
    test.jpg ----------compress/rotate---------> test2.jpg
```


Thanks
------------------------------
[base on::bither-android-lib](https://github.com/bither/bither-android-lib)

[github:libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo)

[source.android.com/...../dalvik_vm_properties](https://source.android.com/devices/tech/dalvik/configure?hl=zh-cn#dalvik_vm_properties)

[sourceforge:libjpeg](http://libjpeg.sourceforge.net/)

