/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ocrad_Main */

#ifndef _Included_com_ocrad_Main
#define _Included_com_ocrad_Main
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_version
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ocrad_Main_OCRAD_1version
  (JNIEnv *, jobject);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_open
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_ocrad_Main_OCRAD_1open
  (JNIEnv *, jobject);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_close
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1close
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_get_errno
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1get_1errno
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_set_image_from_file
 * Signature: (JLjava/lang/String;Z)I
 */
JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1set_1image_1from_1file
  (JNIEnv *, jobject, jlong, jstring, jboolean);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_recognize
 * Signature: (JZ)I
 */
JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1recognize
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_result_blocks
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1result_1blocks
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_result_lines
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1result_1lines
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_ocrad_Main
 * Method:    OCRAD_result_line
 * Signature: (JII)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ocrad_Main_OCRAD_1result_1line
  (JNIEnv *, jobject, jlong, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
