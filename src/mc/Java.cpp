#include "Java.h"

JavaVM* java::GetJavaVM()
{
    JavaVM* vm;

    jsize count;
	if (JNI_GetCreatedJavaVMs(&vm, 1, &count) != JNI_OK || count < 1)
		return nullptr;
    
    return vm;
}

JNIEnv* java::GetEnv()
{
    JNIEnv* env;

    jint status = jvm->GetEnv((void**)&env, JNI_VERSION_1_8);

	if (status == JNI_EDETACHED)
		jvm->AttachCurrentThread((void**)&env, NULL);
	else if (status == JNI_EVERSION || status != JNI_OK)
		return nullptr;

    return env;
}

jclass java::FindClass(JNIEnv* env, const char* name)
{
    if (env == nullptr)
        return nullptr;
    
    jclass classId = env->FindClass(name);

    if (classId == nullptr)
        return nullptr;

    return classId;
}

jmethodID java::FindMethod(JNIEnv* env, jclass klass, const char* name, const char* sig)
{
    if (env == nullptr)
        return nullptr;
    
    jmethodID methodId = env->GetMethodID(klass, name, sig);

    if (methodId == nullptr)
        return nullptr;

    return methodId;
}

jmethodID java::FindStaticMethod(JNIEnv* env, jclass klass, const char* name, const char* sig)
{
    if (env == nullptr)
        return nullptr;
    
    jmethodID methodId = env->GetStaticMethodID(klass, name, sig);

    if (methodId == nullptr)
        return nullptr;

    return methodId;
}

jfieldID java::FindField(JNIEnv* env, jclass klass, const char* name, const char* sig)
{
    if (env == nullptr)
        return nullptr;
    
    jfieldID fieldId = env->GetFieldID(klass, name, sig);

    if (fieldId == nullptr)
        return nullptr;

    return fieldId;
}