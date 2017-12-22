package com.etek.et4207demo;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Typeface;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.etek.ircore.IRCore;
import com.etek.ircore.LearnDecode;
import com.etek.utils.Tools;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{
String TAG = "ET4207demo";
    TextView tvShow;




    Context mContext ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        int ret = IRCore.init();
        Log.v(TAG,"init = "+ret);
        mContext = this;
        tvShow = (TextView) findViewById(R.id.text);
        Button bt = (Button)findViewById(R.id.version);
        bt.setOnClickListener(this);
        Button write = (Button)findViewById(R.id.write);
        write.setOnClickListener(this);
        Button learn = (Button)findViewById(R.id.ir_learn);
        learn.setOnClickListener(this);

        Button current = (Button)findViewById(R.id.set_current);
        current.setOnClickListener(this);

//        byte[] bs = Tools.hexStringToBytes(learnData);
//        for(int i=0;i<bs.length;i++){
//          Log.v(TAG, "data["+i+"] ="+bs[i]);
////
//        }
    }

    @Override
    public void onClick(View v) {
        int ret = 0;
        int i = 0;
        switch(v.getId()){
            case R.id.version:
                ret = IRCore.readVersion();
                Log.v(TAG, "version = " + ret);
                byte[] version = new byte[4];
                version[0] = (byte) (ret/16777216);
                version[1] = (byte) (ret/65536);
                version[2] = (byte) (ret/256);
                version[3] = (byte) (ret);
                tvShow.setText(Tools.bytesToHexString(version));
            break;

            case R.id.write:

                Intent intent = new Intent(MainActivity.this,SendTest.class);
                startActivity(intent);
                break;

            case R.id.set_current:
               ret =  IRCore.setCurrent(4);
               Log.v(TAG, "current return = " + ret);
                break;
            case R.id.ir_learn:
                Intent intent1 = new Intent(MainActivity.this,IrLearn.class);
                startActivity(intent1);
                break;

        }
    }





}
