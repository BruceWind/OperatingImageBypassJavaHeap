
bypass java heap, completely avoid OOM
------------------
[中文](https://github.com/BruceWind/OperatingImageBypassDalvik/blob/master/README_CN.md)

***Stop maintaining announcement:***
> [In devices above Android 8.0, bitmaps will store in native heap](https://developer.android.com/topic/performance/graphics/manage-memory). In the future, devices will keep upgrading to newly and above 8.0 so that this repo will be useless. I'm very sorry for stoping maintaince.
------------------

It is fact that Android's jvm **Dalvik/ART** accords to JVM standard. So that Android JVM has set **xmx** in its configuration. Here I don't want to describe more infomation about what **xmx** is. You can look into docs or websites for learning it. 
It limits sise of java heap in one application. So that as you operrate large bitmap, device may throw OOM.

Although, size of physical memory, such as **RAM**, has been increasing for the decade, but the limitation of java heap still exist. Furthermore, the camera is improve though, so the images that we need operate keep incresing though.
Another way, even though some brand or company who produce phones has modified **xmx** of their Android phones, the limitation is still not enough for large images. 

On the other hand, **the native heap** has no limitation. It is able to allocate reach physical size.
So, operating images in native heap can avoid OOM. In this repo I bypass Bitmap. In case it bypass bitmap, it will bypass java heap.

To sum up, native heap is a free space I can use and the repo works in. All operations of image be in native heap that will compeletely avoid OOM. 



### The evidence for that the native heap doesn't have limitation.
I have tried to allocate numerous memory in native heap and don't execute `free`. The allocated memory is exceed 1G that didm't occur OOM.

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

