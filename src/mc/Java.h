#pragma once

#include <jni/jni.h>
#include <jni/jvmti.h>

namespace java
{
    inline JavaVM* jvm = nullptr;

    JavaVM* GetJavaVM();
    JNIEnv* GetEnv();

    jclass FindClass(JNIEnv* env, const char* name);
    jmethodID FindMethod(JNIEnv* env, jclass klass, const char* name, const char* sig);
    jmethodID FindStaticMethod(JNIEnv* env, jclass klass, const char* name, const char* sig);
    jfieldID FindField(JNIEnv* env, jclass klass, const char* name, const char* sig);
}