#include "SharedObject1.h"


#include <inttypes.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
#include <assert.h>
#include <string.h>



#define INVOKE_CALLBACK

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SharedObject1", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SharedObject1", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SharedObject1", __VA_ARGS__))


JavaVM*  g_pJavaVM;
jclass   g_jniHelperClz;
jobject  g_jniHelperObj;
jclass   g_mainActivityClz;
jobject  g_mainActivityObj;


jmethodID g_methodTestVoid;
jmethodID g_methodTestString;
jmethodID g_methodTestByteArray;

char* ConvertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray);
jstring Java_com_example_helloso_MainActivity_getPlatformABI(JNIEnv* env, jobject thiz)
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a/NEON (hard-float)"
#else
#define ABI "armeabi-v7a/NEON"
#endif
#else
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a (hard-float)"
#else
#define ABI "armeabi-v7a"
#endif
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif
	return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	LOGI("Start JNI_OnLoad");

	JNIEnv* env;

	g_pJavaVM = vm;
	if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
		return JNI_ERR; // JNI version not supported.
	}

	jclass  clz = env->FindClass(
		"com/example/helloso/Callback");

	g_jniHelperClz = (jclass)env->NewGlobalRef(clz);

	jmethodID  jniHelperCtor = env->GetMethodID(g_jniHelperClz,
		"<init>", "()V");

	jobject    handler = env->NewObject(g_jniHelperClz,
		jniHelperCtor);
	g_jniHelperObj = env->NewGlobalRef(handler);


	/////////
	g_methodTestVoid = env->GetStaticMethodID(g_jniHelperClz,
		"Callback_TestVoid", "()V");

	g_methodTestString = env->GetStaticMethodID(g_jniHelperClz,
		"Callback_TestString", "(Ljava/lang/String;)Ljava/lang/String;");


	g_methodTestByteArray = env->GetStaticMethodID(g_jniHelperClz,
		"Callback_TestByteArray", "([BI)[B");
	
	///////
	g_mainActivityObj = NULL;

	LOGI("End JNI_OnLoad");
	return  JNI_VERSION_1_6;
}



INVOKE_CALLBACK 
void Callback_StaticTestVoid() {
	LOGE("Start Callback_TestVoid");
	JNIEnv *env;
	jint res = g_pJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);


	if (g_methodTestVoid != NULL) {
		LOGE("Start CallStaticVoidMethod");
		env->CallStaticVoidMethod(g_jniHelperClz, g_methodTestVoid);
	}
	

	LOGE("End Callback_TestVoid");
}


INVOKE_CALLBACK
void Callback_StaticTestString() {
	LOGI("Start Callback_TestString");
	JNIEnv *env;
	LOGI("Start GetEnv");
	jint res = g_pJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

	LOGI("Start NewStringUTF");
	//jcharArray  jcharAarry = L"你好 hello i am Shiver";

	jstring str = env->NewStringUTF("hello i am Shiver");//!!!此处不能写中文 中文是4个字节的字符
	//jstring str = env->NewString(jcharAarry,20);//utf-16


	LOGI("Start GetStringUTFChars");
	//const jchar *strCont = (const jchar*)env->GetStringChars(str, JNI_FALSE);//utf-16
	const char *strCont = (const char*)env->GetStringUTFChars(str, JNI_FALSE);//utr-8
	
	jstring result;
	if (g_methodTestString != NULL) {
		LOGI("Start CallStaticStringMethod");
		result =  (jstring)env->CallStaticObjectMethod(g_jniHelperClz, g_methodTestString, env->NewStringUTF(strCont));//utf-8
		//result = (jstring)env->CallStaticObjectMethod(g_jniHelperClz, g_methodTestString, env->NewString(strCont));//转utf16

		const char *strResult = env->GetStringUTFChars(result,JNI_FALSE);

		LOGI(strResult);
	}

	LOGI("End Callback_TestString");
}

INVOKE_CALLBACK
void Callback_StaticTestByteArray() {
	LOGI("Start Callback_TestByteArray");
	JNIEnv *env;
	LOGI("Start GetEnv");
	jint res = g_pJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);


	const char* src = "i am shiver\0";

	//创建指定大小的byte数组
	jbyteArray retVal = env->NewByteArray(strlen(src));

	if (retVal) {
		env->SetByteArrayRegion(retVal, 0, strlen(src), (jbyte *)src);
	}
	
	jbyteArray result;
	if (g_methodTestString != NULL) {
		LOGI("Start CallStaticStringMethod");
		result = (jbyteArray)env->CallStaticObjectMethod(g_jniHelperClz, g_methodTestByteArray, retVal,strlen(src));//utf-8


		char* pResult = ConvertJByteaArrayToChars(env, result);

		LOGI("c++ receive content:%s", pResult);
		
	}

	LOGI("End Callback_TestByteArray");
}

INVOKE_CALLBACK
void CallBack_MemeberTestVoid() {
	LOGI("Start CallBack_MemeberTestVoid");
	JNIEnv *env;
	jint res = g_pJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

	//直接用findClass找到Class, 也就是Sdk.class.
	jclass jSdkClass = env->FindClass("com/example/helloso/Callback");
	if (jSdkClass == 0) {
		LOGE("Unable to find class");
		return;
	}
	//找到需要调用的方法ID
	jmethodID javaCallback = env->GetMethodID(jSdkClass,
		"Callback_TestMethodVoid", "()I");

	//这时候要回调还没有jobject，那就new 一个
	jmethodID sdkInit = env->GetMethodID(jSdkClass, "<init>", "()V");
	jobject jSdkObject = env->NewObject(jSdkClass, sdkInit);

	//进行回调，ret是java层的返回值（这个有些场景很好用）
	jint ret = (jint)env->CallIntMethod(jSdkObject, javaCallback);

	LOGI("ret:%d", (int)ret);
	LOGI("End CallBack_MemeberTestVoid");
}

jstring Java_com_example_helloso_MainActivity_HelloWorld(JNIEnv* env, jobject thiz) {

	LOGE("Start HelloWorld");

	//Callback_StaticTestVoid();
	//Callback_StaticTestString();
	//Callback_StaticTestByteArray();
	CallBack_MemeberTestVoid();



	LOGE("End HelloWorld");
	return env->NewStringUTF("Java_com_example_helloso_MainActivity_HelloWorld");
}



char* ConvertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray)
{
	char *chars = NULL;

	jbyte *bytes;

	bytes = env->GetByteArrayElements(bytearray, 0);

	int chars_len = env->GetArrayLength(bytearray);

	chars = new char[chars_len + 1];

	memset(chars, 0, chars_len + 1);

	memcpy(chars, bytes, chars_len);

	chars[chars_len] = 0;

	env->ReleaseByteArrayElements(bytearray, bytes, 0);

	return chars;

}