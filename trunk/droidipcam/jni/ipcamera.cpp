#include "ipcamera.h"

#define  JNIDEFINE(fname) Java_teaonly_projects_droidipcam_NativeAgent_##fname

extern "C" {
    JNIEXPORT jint JNICALL JNIDEFINE(nativeCheckMedia)(JNIEnv* env, jclass clz, jstring file_path);
    JNIEXPORT jint JNICALL JNIDEFINE(nativeStartFormatingMedia)(JNIEnv* env, jclass clz, jobject infdesc, jobject outfdesc);
};

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;

    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeCheckMedia)(JNIEnv* env, jclass clz, jstring file_path) {
    std::string mp4_file = convert_jstring(env, file_path);
    int ret = CheckMedia(mp4_file);

    return ret;
}

int getNativeFd(JNIEnv* env, jclass clz, jobject fdesc) { 
    jclass clazz;
    jfieldID fid;

    /* get the fd from the FileDescriptor */
    if (!(clazz = env->GetObjectClass(fdesc)) ||
            !(fid = env->GetFieldID(clazz,"descriptor","I"))) return -1; 

    /* return the descriptor */
    return env->GetIntField(fdesc,fid);
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeStartFormatingMedia)(JNIEnv* env, jclass clz, jobject infdesc, jobject outfdesc) {
    int infd = getNativeFd(env, clz, infdesc);
    int outfd = getNativeFd(env, clz, outfdesc);
    if ( (infd == -1) || (outfd == -1) ) {
        return -1;
    }

    return StartFormatingMedia(infd, outfd);
}
