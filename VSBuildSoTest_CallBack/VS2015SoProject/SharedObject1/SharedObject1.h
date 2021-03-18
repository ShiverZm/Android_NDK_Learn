#ifndef _SHAREDOBJECT1_
#define _SHAREDOBJECT1_

extern "C"  jstring Java_com_example_helloso_MainActivity_HelloWorld(JNIEnv* env, jobject thiz);


extern "C"  jstring Java_com_example_helloso_MainActivity_getPlatformABI(JNIEnv* env, jobject thiz);

#endif

