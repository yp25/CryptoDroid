#include "jni.h"
#include <iostream>
#include <stdexcept>

#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/randpool.h"

#include "com_cryptodroid_crypto_AES.h"
#include "com_cryptodroid_crypto_AES_CBC.h"

const size_t KEY_SIZE = 16;
const size_t IV_SIZE = 16;

std::vector<jbyte> to_vector(JNIEnv* env, jbyteArray data) {
    size_t data_len = env->GetArrayLength(data);
    std::vector<jbyte> result(data_len);
    if (data_len) {
	env->GetByteArrayRegion(data, 0, data_len, &*result.begin());
    }
    return result;
}

class jbytearray_holder {
public:
    typedef jbyte* iterator;
    
    jbytearray_holder(jbyteArray& ar, JNIEnv* env): m_env(env), m_ar(ar) {
	jboolean is_copy;
	m_data = m_env->GetByteArrayElements(m_ar, &is_copy);
    }
    template<typename T>
    T get_as() {
	return reinterpret_cast<T>(m_data);
    }
    iterator begin() {
	return reinterpret_cast<iterator>(m_data);
    }
    iterator end() {
	return begin() + size();
    }
    size_t size() {
	return m_env->GetArrayLength(m_ar);
    }
    ~jbytearray_holder() {
	m_env->ReleaseByteArrayElements(m_ar, m_data, 0);
    }
private:
    JNIEnv* m_env;
    jbyte* m_data;
    jbyteArray& m_ar;
  
    jbytearray_holder(jbytearray_holder&);
    jbytearray_holder& operator= (jbytearray_holder&);
};


void throw_jni_exception(JNIEnv* env, const std::exception& e) {
    jclass excClass = env->FindClass("java/lang/IllegalArgumentException");
    if (excClass) {
	std::string message = "Exception from native code: ";
	message += e.what();

	env->ThrowNew(excClass, message.c_str());
    }
}

template<typename T>
T get_field_value(JNIEnv* env, jobject obj, const std::string& field_name);

template<>
jbyteArray get_field_value<jbyteArray>(JNIEnv* env, jobject obj, const std::string& field_name) {
    jclass clazz = env->GetObjectClass(obj);
    if (!clazz)
	throw std::runtime_error("No class!");

    jfieldID fld = env->GetFieldID(clazz, field_name.c_str(), "[B");
    jbyteArray result = static_cast<jbyteArray>(env->GetObjectField(obj, fld));
    return result;
}

JNIEXPORT jbyteArray JNICALL Java_com_cryptodroid_crypto_AES_00024CBC_Encrypt(JNIEnv* env, jobject obj, jbyteArray source) {
    try {
	std::vector<jbyte> key = to_vector(env, get_field_value<jbyteArray>(env, obj, "__key"));
	std::vector<jbyte> iv  = to_vector(env, get_field_value<jbyteArray>(env, obj, "__iv" ));

	CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption e;
	e.SetKeyWithIV(
	    reinterpret_cast<byte*>(&key.front()),
	    KEY_SIZE,
	    reinterpret_cast<byte*>(&iv.front())
	    );

	CryptoPP::StreamTransformationFilter filter (e);
	jbytearray_holder data_holder(source, env);
	filter.Put(reinterpret_cast<byte*>(&*data_holder.begin()), data_holder.size());
	filter.MessageEnd();
	
	jbyteArray result = env->NewByteArray(filter.MaxRetrievable());
	if (!result) throw std::runtime_error("No memory!");
	
	jbytearray_holder result_holder(result, env);
	filter.Get(reinterpret_cast<byte*>(&*result_holder.begin()), result_holder.size());

	return result;
    } catch (std::exception& e) {
	throw_jni_exception(env, e);
    }
    return NULL;
}

JNIEXPORT jbyteArray JNICALL Java_com_cryptodroid_crypto_AES_00024CBC_Decrypt(JNIEnv* env, jobject obj, jbyteArray source) {
    try {
	std::vector<jbyte> key = to_vector(env, get_field_value<jbyteArray>(env, obj, "__key"));
	std::vector<jbyte> iv  = to_vector(env, get_field_value<jbyteArray>(env, obj, "__iv" ));

	CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
	d.SetKeyWithIV(
	    reinterpret_cast<byte*>(&key.front()),
	    KEY_SIZE,
	    reinterpret_cast<byte*>(&iv.front())
	    );

	CryptoPP::StreamTransformationFilter filter (d);
	jbytearray_holder data_holder(source, env);
	filter.Put(reinterpret_cast<byte*>(&*data_holder.begin()), data_holder.size());
	filter.MessageEnd();
	
	jbyteArray result = env->NewByteArray(filter.MaxRetrievable());
	if (!result) throw std::runtime_error("No memory!");
	
	jbytearray_holder result_holder(result, env);
	filter.Get(reinterpret_cast<byte*>(&*result_holder.begin()), result_holder.size());

	return result;
    } catch (std::exception& e) {
	throw_jni_exception(env, e);
    }
    return NULL;
}

JNIEXPORT jbyteArray JNICALL Java_com_cryptodroid_crypto_AES_GenerateIV(JNIEnv* env, jclass clazz) {
    try {
	jbyteArray result = env->NewByteArray(IV_SIZE);
	jbytearray_holder holder(result, env);

	CryptoPP::RandomPool rng;
	rng.Put(reinterpret_cast<byte*>(holder.begin()), IV_SIZE);

	return result;
    } catch (std::exception& e) {
	throw_jni_exception(env, e);
    }
    return NULL;
}

JNIEXPORT jbyteArray JNICALL Java_com_cryptodroid_crypto_AES_GenerateKey(JNIEnv* env, jclass clazz) {
    try {
	jbyteArray result = env->NewByteArray(IV_SIZE);
	jbytearray_holder holder(result, env);

	CryptoPP::RandomPool rng;
	rng.Put(reinterpret_cast<byte*>(holder.begin()), IV_SIZE);

	return result;
    } catch (std::exception& e) {
	throw_jni_exception(env, e);
    }
    return NULL;
}
