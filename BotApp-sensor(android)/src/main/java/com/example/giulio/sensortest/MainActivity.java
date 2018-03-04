package com.example.giulio.sensortest;

import android.hardware.SensorEventListener;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.hardware.SensorManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import java.lang.Math;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity implements SensorEventListener{
    private TcpClient mTcpClient;
    private SensorManager manager;
    private Sensor accel;
    private int Sensibility;
    private int count;
    private final int max = 0;
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        manager = (SensorManager)getSystemService(SENSOR_SERVICE);
        accel = manager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION);
        Sensibility = 15;
        new ConnectTask().execute("");
        count = 0;
    }
    protected void onResume() {
        super.onResume();
        manager.registerListener(this, accel, SensorManager.SENSOR_DELAY_GAME);
    }
    public void onPause() {
        super.onPause();
        manager.unregisterListener(this);
    }
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    public void reset(View v) {
        mTcpClient.sendMessage("A");
    }


    public void onSensorChanged(SensorEvent event) {
        int xValue = (int)event.values[0];
        int yValue = (int)event.values[1];
        int zValue = (int)event.values[2];
        if(zValue >= Sensibility || zValue <= -Sensibility && count == max){
            String msg;
            msg = "z" + Integer.toString(zValue/2);
            Log.e("z", msg);
            mTcpClient.sendMessage(msg);
            count = 0;
            SystemClock.sleep(200);
            return;
        }
        if(xValue >= Sensibility || xValue <= -Sensibility && count == max){
            String msg = "y"+Integer.toString(xValue/2);
            Log.e("x", msg);
            mTcpClient.sendMessage(msg);
            mTcpClient.sendMessage(msg);
            count = 0;
            SystemClock.sleep(200);
            return;
        }
        if(yValue >= Sensibility || yValue <= -Sensibility && count == max){
            String msg = "x"+Integer.toString(yValue);
            Log.e("y", msg);
            mTcpClient.sendMessage(msg);
            count = 0;
            SystemClock.sleep(200);
            return;
        }
        count++;
    }
    /*      +
    *       Z
    *       -
    *<--- + X - ----->
    *       Y like x
    * */
    public class ConnectTask extends AsyncTask<String, String, TcpClient> {

        @Override
        protected TcpClient doInBackground(String... message) {

            //we create a TCPClient object
            mTcpClient = new TcpClient(new TcpClient.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            });
            mTcpClient.run();

            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            //response received from server
            Log.d("test", "response " + values[0]);
            //process server response here....

        }
    }
}
