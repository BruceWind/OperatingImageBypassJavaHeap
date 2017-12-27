package com.androidyuan.jpg_rotate;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import net.bither.util.NativeUtil;

public class MainActivity extends AppCompatActivity {


    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);


//        testJpeg();
        testCompress();
    }

    private void testCompress() {

        new Thread(new Runnable() {
            @Override
            public void run() {
                Log.d("TEST", "time");

                String in = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test.jpg";
                String out = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test2.jpg";

                for (int i = 0; i < 30; i++) {
                    try {//测试内存释放
                        Thread.sleep(800);
                        Log.d("TEST", "begin");
//                        NativeUtil.compressJpeg(in, out);
                        NativeUtil.rotateJpeg(in, out);
//                        Log.d("TEST", "end");
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

                System.gc();

            }
        }).start();

    }


}
