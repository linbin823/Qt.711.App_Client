package com.smderi.datacenter;

public class DatacenterActivity extends org.qtproject.qt5.android.bindings.QtActivity{
    @Override
    public void onResume() {
        super.onResume();
        MessageReceiver.acknowledge();

    }
}


//com.smderi.datacenter.DatacenterActivity
