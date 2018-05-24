# 绕过Bitmap操作图片,100%防止OOM 

------------------
我们都知道JVM xmx参数，这个`xmx`不做具体介绍，这是jvm的基本参数，不了解的请自行查询Java虚拟机原理。

android的Dalvik/ART遵守了JVM的规范，有堆内存限制，遇到大图操作缩放旋转Bitmap的时候，就很容易OOM。

虽然，现在新的android系统提供的这个限制都在调大，我记得6.0官方ROM好像是96M,非官方ROM要大一些，因为非官方ROM在编译时修改了xmx参数配置，但是新出的手机相机拍摄出来的图片更大，相机技术的增长速度远大于这个限制的开放程度，进行大图操作，你懂得。


上面说的核心原因在于你的bitmap内存申请都在dalvik heap中，这个最大内存的限制。
但是native内存处于不太受管理的状态，这一点很多人不知道。



这个库的目的在于绕过Dalvik heap 限制，去操作图片，保证无需担心OOM。怎么绕过Dalvik呢？就是绕过bitmap去直接对图片进行操作，毕竟Android 4.0以后的机器bitmap内存都在Dalvik中,我们所有操作图片时的内存申请都在Native heap中。


### 证明native heap内存申请无限制 
我尝试用native写了个申请巨大内存又不执行`free()`的代码进行测试，申请了1G多的内存还是不会OOM，下图是我用```adb shell dumpsys meminfo PACKAGENAME ```打印出来的当前memory信息。

![](https://github.com/weizongwei5/my_blog_datasave/raw/62e952490c7fc3ef1f478c52985d4686331d17e0/img/native_memory_show.png)


## 当前仓库进度

- [x] rotate  jpeg
- [x] compress jpeg
- [x]  resize jpeg

- [x] compress  png          (2018.04.10)  
    > - [ ] 提供除了libpng单纯压缩level之外的算法,参考optipng这个古老的开源库
- [ ]  rotate  png
- [ ]  resize png

## 编译运行提示
该项目需要在外部存储空间(SD卡)根目录下存储一个test.jpg 的大图。
运行项目之后:
```
    test.jpg ----------compress/rotate---------> test2.jpg
      
```


## 了解更多
 
访问[http://androidyuan.com/post/bypassjavadalvikheapopratingimg](http://androidyuan.com/post/bypassjavadalvikheapopratingimg)


特别鸣谢
------------------------------

[base on::bither-android-lib](https://github.com/bither/bither-android-lib)


[github:libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo)


[source.android.com/...../dalvik_vm_properties](https://source.android.com/devices/tech/dalvik/configure?hl=zh-cn#dalvik_vm_properties)

[sourceforge:libjpeg](http://libjpeg.sourceforge.net/)




