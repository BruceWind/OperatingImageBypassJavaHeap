OperatingImageBypassJavaHeap
------------------
[中文](https://github.com/BruceWind/OperatingImageBypassDalvik/blob/master/README_CN.md)

***Announcement regarding discontinued maintenance at 2021:***
> [In devices above Android 8.0, bitmaps store in native heaps](https://developer.android.com/topic/performance/graphics/manage-memory). 
> In the future, devices will keep upgrading to new above 8.0, so this repo will be useless. This is why I have decided to stop maintaining it.
> I apologize to anyone who looks forward to updates.
------------------


Although the size of physical memory **RAM** has been increasing for a decade, the limitation of the Java heap still exists. Furthermore, the camera is improved, and the images that we need to operate keep increasing, though.
By the way, even though some brands or companies produce phones whose **xmx** has been modified, the limitation on those devices are still insufficient for immensely large images. 

It is a fact that Android's JVMs such as **Dalvik** and **ART** accord with JVM standards. So that the Android JVM has a configuration with **xmx**. 
> In the README, I don't want to describe more information about what **xmx** is. You can look into docs or websites for learning it.
> 
Due to the limitation of Java heap for one application, while you operate large bitmaps, the device may throw OOM.

On the other hand, **the native heap** has no limitation. It is able to allocate reach physical size.
So, operating images in the native heap can avoid OOM. In this repo, I bypass Bitmap. In case it bypasses bitmap, it will bypass the Java heap.

To sum up, the native heap is unlimited for every developer. This is why the repo works. All operations of images are in the native heap, which will completely avoid OOM. 



### The evidence for that the native heap doesn't have limitations.
I have tried to allocate numerous memory in the native heap and don't execute `free`. The allocated memory exceeds 1G, which didn't give rise to OOM.

Below a picture, I run ```adb shell dumpsys meminfo PACKAGENAME ``` to print that memory information.

![](https://github.com/weizongwei5/my_blog_datasave/raw/62e952490c7fc3ef1f478c52985d4686331d17e0/img/native_memory_show.png)


## The repo progress

- [x] rotate  jpeg
- [x] compress jpeg
- [x]  resize jpeg

- [x] compress  png          (2018.04.10)  
    > - [ ] provide other algorithms about compression, may introduce `optipng`, which has been used in most services or applications.
- [ ]  rotate  png
- [ ]  resize png

## Build tips
1. before you run on the device, you need to put a `test.jpg` on external storage.

2. After you start this app: 
```
    test.jpg ----------compress/rotate---------> test2.jpg
```


Thanks
------------------------------
[base on::bither-android-lib](https://github.com/bither/bither-android-lib)

[github:libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo)

[source.android.com/...../dalvik_vm_properties](https://source.android.com/devices/tech/dalvik/configure?hl=zh-cn#dalvik_vm_properties)

[sourceforge:libjpeg](http://libjpeg.sourceforge.net/)

