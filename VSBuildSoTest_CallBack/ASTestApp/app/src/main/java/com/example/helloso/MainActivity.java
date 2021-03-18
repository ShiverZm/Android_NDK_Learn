package com.example.helloso;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getName();

    public static Context mContext;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("SharedObject1");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this;
        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(getPlatformABI());


        TextView testTv = findViewById(R.id.test_text);

        HelloWorld();
    }



    public static native String getPlatformABI();

    public static native String HelloWorld();


}