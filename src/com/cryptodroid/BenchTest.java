package com.cryptodroid;

import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import com.cryptodroid.crypto.AES;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class BenchTest extends Activity {
	private TextView m_result_label;
	private Button m_start_button;
	
	private static int DATA_SIZE = 2000000;
	
	void test_native(byte[] iv, byte[] key, byte[] data) {
		AES.CBC e = new AES.CBC(iv, key);
		byte[] encrypted = e.Encrypt(data);
		byte[] decrypted = e.Decrypt(encrypted);
	}
	
	void test_managed(byte[] iv, byte[] key, byte[] data) throws Exception {
		Cipher _e = Cipher.getInstance("AES/CBC/PKCS5Padding");
		SecretKeySpec skeySpec = new SecretKeySpec(key, "AES");
		IvParameterSpec ivspec = new IvParameterSpec(iv);	    	
		_e.init(Cipher.ENCRYPT_MODE, skeySpec, ivspec);
		byte[] encrypted = _e.doFinal(data);
		
		Cipher _d = Cipher.getInstance("AES/CBC/PKCS5Padding");	    	
		_d.init(Cipher.DECRYPT_MODE, skeySpec, ivspec);
		byte[] decrypted = _d.doFinal(encrypted);
	}
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bench_test);
        
        m_result_label = (TextView)findViewById(R.id.textViewResult);
        m_start_button = (Button)findViewById(R.id.buttonStart);
        
        m_start_button.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				String text = "";
				try {
					byte[] iv = AES.GenerateIV();
					byte[] key = AES.GenerateKey();
					byte[] data = new byte[DATA_SIZE];
					long start_time = System.nanoTime();
					test_native(iv, key, data);
					long end_time = System.nanoTime();
					text += "Native time: " + (end_time - start_time)/1000000 + "ms;\n";
					
					start_time = System.nanoTime();
					test_managed(iv, key, data);
					end_time = System.nanoTime();
					text += " Managed time: " + (end_time - start_time)/1000000 + "ms";
				} catch (Exception e) {
					text = e.getMessage();
				}
				
				m_result_label.setText(text);
			}
		});
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_bench_test, menu);
        return true;
    }
    
}
