package com.androidyuan.jpg_rotate;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import net.bither.util.NativeUtil;

public class MainActivity extends AppCompatActivity {


    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        testCompress();
    }

    private void testCompress() {


        for(int i=0;i<1;i++) {

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            new Thread(
            new Runnable() {
                @Override
                public void run() {
                    Log.d("TEST", "begin");

                    String in = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test.png";
                    String out = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test2.png";

                    //这里可以循环几十次测试内存释放
//                NativeUtil.compressJpeg(in, out,20);
//                NativeUtil.resizeJpeg(in, out,0.6f);
                    int result = NativeUtil.compressPng(in, out, 9);

                    Log.d("TEST", "end : " + result);

                }
            }).start();

        }

    }


}
