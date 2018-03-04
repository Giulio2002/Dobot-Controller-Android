package com.learning;

import android.util.Log;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.AsyncTask;
import android.view.View;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import android.view.OrientationEventListener;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private TcpClient mTcpClient;
    private int rotation;
    TextView tvId;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        new ConnectTask().execute("");


    }

    void onOrientationChanged (int orientation){
        tvId.setText(Integer.toString(orientation));
    }

    public void Up(View v) {
        mTcpClient.sendMessage("1");
    }

    public void Down(View v) {
        mTcpClient.sendMessage("0");
    }

    public void Right(View v) {
        mTcpClient.sendMessage("2");
    }

    public void Left(View v) {
        mTcpClient.sendMessage("3");
    }

    public void reset(View v) {
        mTcpClient.sendMessage("A");
    }
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