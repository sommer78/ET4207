package com.etek.et4207demo;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Typeface;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.etek.ircore.IRCore;
import com.etek.utils.Tools;

/**
 * Created by jiangs on 2016/1/6.
 */
public class SendTest extends Activity implements View.OnClickListener{
    String TAG = "SendTest";
    static String nec6122 = "54005c200024000001af015d00ab001700140016003f0017062d015d005500171e0600460f0200160f030000000000000000000000000000000000000000000000000000000000000000011111111222222122122112112112212345";
//    static String learnData = "ca0014ffff802100000001211554084101010701010701010107010107010101070101070100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e07c3c0f8781f0800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000013b0515001400ab001401c600140511001400ab001401ca00141a80013c051a001400aa001401c600140512001400aa001401ca00141a80013b0516001400aa001401c600140512001400aa001401ca0014ffff0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

    String   etekcode  ="54 00 83 20 00 72 00 00 01 ca 00 71 00 43 00 10 00 2b 00 10 00 0f 00 10 05 f0 " +
            "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 " +
            "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 12 22 12 21 12 12 21 12 11 22 12 " +
            "21 22 22 22 22 22 22 22 22 22 22 12 22 22 12 22 21 21 22 22 22 22 22 21 22 22 22 22 22 22 " +
            "22 22 22 22 22 22 22 22 22 22 22 12 22 22 13 ";

    String tianjiacode ="55 00 84 20 00 72 00 00 01 53 d0 a2 06 44 d0 c1 06 2e d4 c3 02 08 d4 c1 07 f5" +
            "d8 d3 0e 07 d8 d1 0e 05 dc d3 0a 07 dc d1 0a 05 c0 d3 16 07 c0 d1 16 05 c4 d3 12 07 c4 d1" +
            "12 05 c8 d3 1e 07 c8 d1 1e 05 cc d3 1a 07 cc d1 1a 05 f1 c1 04 15 d1 c3 34 24 e6 c2 00 15" +
            "d5 f3 00 27 da f1 0c 25 da f3 0c 17 de f1 38 25 dd f0 08 27 c2 f1 14 26 c2 f3 14 27 c6 f1" +
            "10 25 c6 f3 10 27 ca f1 1c 25 ca c3 1c 27 ff d0";


    TextView tvShow;
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_send);
        Button btETEK = (Button)findViewById(R.id.send_etek);
        btETEK.setOnClickListener(this);
        Button btOld = (Button)findViewById(R.id.send_old);
        btOld.setOnClickListener(this);
        Button btTianjia = (Button)findViewById(R.id.send_tianjia);
        btTianjia.setOnClickListener(this);
        Button btBack = (Button)findViewById(R.id.send_back);
        btBack.setOnClickListener(this);
        tvShow = (TextView) findViewById(R.id.send_show);
        AssetManager mgr=getAssets();//得到AssetManager
        Typeface tf= Typeface.createFromAsset(mgr, "fonts/msyh.ttf");//根据路径得到Typeface
        tvShow.setTypeface(tf);//设置字体
    }

    @Override
    public void onClick(View v) {
        int ret = 0;
        switch(v.getId()) {
            case R.id.send_etek:
                String newCode = etekcode.replace(" ","");
                ret = IRCore.write(Tools.hexStringToBytes(newCode));
                Log.v(TAG,"return = "+ret);

                Log.v(TAG, etekcode);
                tvShow.setText(etekcode);
                break;
            case R.id.send_tianjia:
                String tianjia = tianjiacode.replace(" ","");
                ret = IRCore.write(Tools.hexStringToBytes(tianjia));
                Log.v(TAG,"return = "+ret);

                Log.v(TAG, tianjiacode);
                tvShow.setText(tianjiacode);
                break;
            case R.id.send_old:
                ret = IRCore.write(Tools.hexStringToBytes(nec6122));
                Log.v(TAG,"return = "+ret);

                Log.v(TAG, nec6122);
                tvShow.setText(nec6122);
                break;
            case R.id.send_back:
                finish();
                break;
        }
    }
}
