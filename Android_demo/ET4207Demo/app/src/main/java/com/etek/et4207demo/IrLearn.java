package com.etek.et4207demo;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Typeface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.etek.ircore.IRCore;
import com.etek.ircore.LearnDecode;
import com.etek.utils.Tools;

/**
 * Created by jiangs on 2016/1/7.
 */
public class IrLearn extends Activity implements View.OnClickListener{

    private LayoutInflater inflater;
    Context mContext ;
    Dialog dialog;
    String TAG = "IrLearn";
    private Handler mMainHandler, mLearnHandler;
    LearnLoop learnLoop;
    byte[] data;
    TextView tvShow;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_learn);
        mContext = this;
        Button learnStart = (Button)findViewById(R.id.learn_start);
        learnStart.setOnClickListener(this);
        Button learnSend = (Button)findViewById(R.id.learn_test);
        learnSend.setOnClickListener(this);
        Button learnRec= (Button)findViewById(R.id.learn_rec_start);
        learnRec.setOnClickListener(this);
        Button read = (Button)findViewById(R.id.read);
        read.setOnClickListener(this);
        Button back = (Button)findViewById(R.id.learn_back);
        back.setOnClickListener(this);
        tvShow = (TextView) findViewById(R.id.text);
        learnLoop = new LearnLoop();

        mMainHandler = new Handler() {

            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(Message msg) {
                int ret = msg.getData().getInt("state");

                Log.d(TAG, "finish button learn = " + ret);

                if (ret  == 1) {
                    byte[] learnCode = IRCore.read(10);


                    StringBuilder sb = new StringBuilder();
                    if(learnCode!=null&&learnCode.length>64) {
                        if (learnCode[10] == 0x01) {
                            for (int i = 0; i < 448; i++) {
                                if(i%32==0){
                                    int index = i+32;
                                    sb.append("REG[ "+index+"] =");
                                }
                                sb.append(Tools.bytesToHex(learnCode[i]));
                                sb.append(" ");
                                if(i%32==31){
                                    sb.append("\n");
                                }

                            }
                            Log.v(TAG, "read = " + sb.toString());
                            Log.v(TAG, "learn = " + Tools.bytesToHexString(learnCode));
                            data = LearnDecode.getET4207LearnCode(learnCode);
                            Log.v(TAG, "data = " + data);
                            tvShow.setText(sb.toString());

                        }
                        dialog.dismiss();
                    }
                }else if (ret  == 0){
                    Toast.makeText(mContext, "learn time out", Toast.LENGTH_SHORT).show();
                    ret =IRCore.learnStop();
                    dialog.dismiss();
                }

                //

            }

        };
        learnLoop.start();


        AssetManager mgr=getAssets();//得到AssetManager
        Typeface tf= Typeface.createFromAsset(mgr, "fonts/msyh.ttf");//根据路径得到Typeface
        tvShow.setTypeface(tf);//设置字体
    }
    @Override
    public void onClick(View v) {
        int ret = 0;
        int i = 0;
        switch (v.getId()){
            case R.id.learn_start:
                ret =IRCore.learnStart();
                learningPopup();
                Message m = new Message();
                Bundle bundle = new Bundle();
                bundle.putInt("learn", 1);
                m.setData(bundle);
                mLearnHandler.sendMessage(m);
                tvShow.setText("start learn");
                break;
            case R.id.learn_test:
                if(data!=null) {
                    ret = IRCore.write(data);

                    tvShow.setText(Tools.bytesToHexString(data));
                }
                break;
            case R.id.learn_rec_start:
                ret =IRCore.recLearnStart();
                learningPopup();
                m = new Message();
                bundle = new Bundle();
                bundle.putInt("learn", 1);
                m.setData(bundle);
                mLearnHandler.sendMessage(m);
                tvShow.setText("start learn");
                tvShow.setText(Tools.bytesToHexString(data));
                break;
            case R.id.read:
                byte[] readData =IRCore.read(100);

                StringBuilder sb = new StringBuilder();
                for( i=0;i<448;i++) {
//                    sb.append("d["+i+"]=");
                    sb.append(Tools.bytesToHex(readData[i]));
                    sb.append(",");

                }
                Log.v(TAG, "read = " + sb.toString());
                tvShow.setText( sb.toString());
                break;
            case R.id.learn_back:
                finish();
                break;

        }

    }


    class LearnLoop extends Thread {

//        String str = "";
//        int i = 0;

        public void run() {

            this.setName("LearnLoop");

            // 初始化消息循环队列，需要在Handler创建之前
            Looper.prepare();

            mLearnHandler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    Message toMain = mMainHandler.obtainMessage();
                    Bundle bundle = new Bundle();

                    int ret = IRCore.learnLoop();
                    //    Log.v(TAG, "data is " + i);
//						ETLogger.debug("device state is " + ret);
                    //       i++;
                    bundle.putInt("state", ret);

                    toMain.setData(bundle);
                    mMainHandler.sendMessage(toMain);


                }

            };

            Looper.loop();
        }
    }

    public void learningPopup() {

        if (inflater == null)
            inflater = getLayoutInflater();
        View localView = inflater.inflate(R.layout.learning_popup, null);

        dialog = new Dialog(mContext, android.R.style.Theme_DeviceDefault_Panel);
        // dialog.setCanceledOnTouchOutside(true);
        dialog.setContentView(localView);
        dialog.show();
        final Button quit = (Button) localView.findViewById(R.id.learning_quit);

        quit.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                int ret =IRCore.learnStop();
                // TODO Auto-generated method stub
                dialog.dismiss();
            }
        });

    }

}
