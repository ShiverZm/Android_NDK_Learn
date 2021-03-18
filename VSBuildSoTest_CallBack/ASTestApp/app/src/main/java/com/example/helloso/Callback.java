package com.example.helloso;

import android.util.Log;
import android.widget.Toast;

public class Callback {
    public static final String TAG = Callback.class.getName();


    public int  Callback_TestMethodVoid(){

        Log.i(TAG,"java Callback_TestVoid");

        System.out.println("java Callback_TestVoid");

        return 0;
    }

    public static void  Callback_TestVoid(){

        Log.i(TAG,"java Callback_TestVoid");

        System.out.println("java Callback_TestVoid");
    }


    public static String Callback_TestString(String str){

        Log.i(TAG,"java Callback_TestString content:"+str);

        System.out.println("java Callback_TestString content:"+str);

        return "你好 在测试Callback_TestString";
    }


    public static byte[] Callback_TestByteArray(byte[] byteArray,int size){
        Log.i(TAG,"java Callback_TestByteArray byteArraySize:"+size);

        try {
            String strBytes = new String(byteArray,"UTF-8");
            Log.i(TAG,"java Callback_TestByteArray content:"+strBytes);

        }catch (Exception e){
            e.printStackTrace();
        }


        byte []retArray = new byte[10];

        retArray[0] = 'h';
        retArray[1] = 'e';
        retArray[2] = 'l';
        retArray[3] = 'l';
        retArray[4] = 'o';
        retArray[5] = 'w';
        retArray[6] = 'o';
        retArray[7] = 'r';
        retArray[8] = 'l';
        retArray[9] = 'd';

        return retArray;
    }
}
