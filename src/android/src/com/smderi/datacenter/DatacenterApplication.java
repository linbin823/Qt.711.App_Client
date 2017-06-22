package com.smderi.datacenter;  

import io.yunba.android.manager.YunBaManager;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.json.JSONException;

import android.app.Application;
import org.qtproject.qt5.android.bindings.QtApplication;  
import android.util.Log;
import android.os.Bundle;

  
public class DatacenterApplication extends QtApplication {
    private final static String TAG = "SMDERIDataCenterApplication";  
    @Override  
    public void onCreate() {

        super.onCreate();
        YunBaManager.start(getApplicationContext());

        YunBaManager.subscribe(getApplicationContext(), new String[]{"testTopic"}, new IMqttActionListener() {

            @Override
            public void onSuccess(IMqttToken arg0) {
                Log.d(TAG, "Subscribe topic succeed");
            }

            @Override
            public void onFailure(IMqttToken arg0, Throwable arg1) {
                Log.d(TAG, "Subscribe topic failed" );
            }
        });

    }
}  

//com.smderi.datacenter.datacenterapplication
//org.qtproject.qt5.android.bindings.QtApplication
