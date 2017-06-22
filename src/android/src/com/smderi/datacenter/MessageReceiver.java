package com.smderi.datacenter;  

import android.os.Bundle;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.content.Intent;

import io.yunba.android.manager.YunBaManager;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.json.JSONException;

public class MessageReceiver extends BroadcastReceiver {
    private static NotificationManager m_notificationManager;
    private static int count = 0;

    @Override
    public void onReceive(Context context, Intent intent) {

        if (YunBaManager.MESSAGE_RECEIVED_ACTION.equals(intent.getAction())) {

            String topic = intent.getStringExtra(YunBaManager.MQTT_TOPIC);
            String msg = intent.getStringExtra(YunBaManager.MQTT_MSG);

            if (m_notificationManager == null) {
                m_notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
            }

            Notification.Builder m_builder = new Notification.Builder(context);
            m_builder.setSmallIcon(R.drawable.icon);

            Intent appIntent = new Intent(Intent.ACTION_MAIN);
            appIntent.addCategory(Intent.CATEGORY_LAUNCHER);
            appIntent.setComponent(new ComponentName("com.smderi.datacenter","com.smderi.datacenter.DatacenterActivity" ));
            appIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK| Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
            PendingIntent contentIntent = PendingIntent.getActivity(context, 0, appIntent, PendingIntent.FLAG_UPDATE_CURRENT);
            m_builder.setContentIntent( contentIntent );
            m_builder.setOngoing(true);

            if(count < 1){
                m_builder.setContentTitle("船舶数据中心");
                count++;
            }else{
                m_notificationManager.cancel(count);
                StringBuilder title = new StringBuilder();
                title.append(count+1)
                        .append("条")
                        .append(" ")
                        .append("船舶数据中心");
                m_builder.setContentTitle( title );
                count++;
            }
            m_builder.setContentText( msg );

            Notification m_notification = m_builder.build();
            m_notification.defaults |= Notification.DEFAULT_SOUND;
            m_notification.defaults |= Notification.DEFAULT_VIBRATE;
            m_notification.flags =Notification.FLAG_ONLY_ALERT_ONCE;

            m_notificationManager.notify(count, m_notification);
        }
    }

    public static void acknowledge(){
        if(count != 0){
            m_notificationManager.cancel(count);
            count = 0;
        }

    }
}  

//com.smderi.datacenter.datacenterapplication
//org.qtproject.qt5.android.bindings.QtApplication
