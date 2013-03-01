package com.cryptodroid.crypto;

public class AES {
	public static int KEY_SIZE = 16;
	public static int IV_SIZE = 16;
	
	static public class CBC {
		
		private byte[] __key;
		private byte[] __iv;
		
		public CBC(byte[] key, byte[] iv) {
			__key = key;
			__iv = iv;
		}
		
		public CBC() {
			__key = GenerateKey();
			__iv = GenerateIV();
		}
		
		public native byte[] Encrypt(byte[] data);
		public native byte[] Decrypt(byte[] data);
		
		public byte[] GetKey() {
			return __key;
		}
		
		public byte[] GetIV() {
			return __iv;
		}
	}
	
	public static native byte[] GenerateIV();
	public static native byte[] GenerateKey();
	
	static {
		try {
			System.loadLibrary("nativecryptowrapper");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

