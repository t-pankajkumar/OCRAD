#include <climits>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <stdint.h>

#include "ocradlib.h"
#include "common.h"
#include "rectangle.h"
#include "ucs.h"
#include "track.h"
#include "bitmap.h"
#include "blob.h"
#include "character.h"
#include "page_image.h"
#include "textline.h"
#include "textblock.h"
#include "textpage.h"
#include "com_ocrad_Main.h"

struct OCRAD_Descriptor {
	Page_image * page_image;
	Textpage * textpage;
	OCRAD_Errno ocr_errno;
	Control control;
	std::string text;

	OCRAD_Descriptor() :
			page_image(0), textpage(0), ocr_errno(OCRAD_ok) {
		control.outfile = 0;
	}
};
bool verify_descriptor( OCRAD_Descriptor * const ocrdes,
                        const bool result = false )
  {
  if( !ocrdes ) return false;
  if( !ocrdes->page_image || ( result && !ocrdes->textpage ) )
    { ocrdes->ocr_errno = OCRAD_sequence_error; return false; }
  return true;
  }
JNIEXPORT jstring JNICALL Java_com_ocrad_Main_OCRAD_1version(JNIEnv* env,
		jobject thisObject) {
	return env->NewStringUTF(OCRAD_version_string);
}

JNIEXPORT jlong JNICALL Java_com_ocrad_Main_OCRAD_1open(JNIEnv* env,
		jobject thisObject) {
	verbosity = -1;			// keep library silent
	OCRAD_Descriptor * const ocrdes = new (std::nothrow) OCRAD_Descriptor;
	if (!ocrdes)
		return 0;
	return (long) ocrdes;
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1close(JNIEnv* env,
		jobject thisObject, jlong ocrdesptr) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	if (!ocrdes)
		return -1;
	if (ocrdes->textpage)
		delete ocrdes->textpage;
	if (ocrdes->page_image)
		delete ocrdes->page_image;
	delete ocrdes;
	return 0;
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1get_1errno(JNIEnv* env,
		jobject thisObject, jlong ocrdesptr) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	if (!ocrdes)
		return OCRAD_bad_argument;
	return ocrdes->ocr_errno;
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1set_1image
  (JNIEnv* env, jobject thisObject, jlong ocrdesptr, jobject pixmap, jboolean invert){
  	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1set_1image_1from_1file(
		JNIEnv* env, jobject thisObject, jlong ocrdesptr, jstring filenamestr,
		jboolean invert) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	const char *filename = env->GetStringUTFChars(filenamestr, JNI_FALSE);
	if (!ocrdes)
		return -1;
	FILE * infile = 0;
	if (filename && filename[0]) {
		if (std::strcmp(filename, "-") == 0)
			infile = stdin;
		else
			infile = std::fopen(filename, "rb");
	}
	if (!infile) {
		ocrdes->ocr_errno = OCRAD_bad_argument;
		return -1;
	}
	int retval = 0;
	try {
		Page_image * const page_image = new Page_image(infile, invert);
		if (ocrdes->textpage) {
			delete ocrdes->textpage;
			ocrdes->textpage = 0;
		}
		if (ocrdes->page_image)
			delete ocrdes->page_image;
		ocrdes->page_image = page_image;
	} catch (std::bad_alloc ) {
		ocrdes->ocr_errno = OCRAD_mem_error;
		retval = -1;
	} catch (...) {
		ocrdes->ocr_errno = OCRAD_bad_argument;
		retval = -1;
	}
	std::fclose(infile);
	env->ReleaseStringUTFChars(filenamestr, filename);
	return retval;
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1recognize(JNIEnv* env,
		jobject thisObject, jlong ocrdesptr, jboolean layout) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	if (!verify_descriptor(ocrdes))
		return -1;
	Textpage * const textpage = new (std::nothrow) Textpage(*ocrdes->page_image,
			"", ocrdes->control, layout);
	if (!textpage) {
		ocrdes->ocr_errno = OCRAD_mem_error;
		return -1;
	}
	if (ocrdes->textpage)
		delete ocrdes->textpage;
	ocrdes->textpage = textpage;
	return 0;
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1result_1blocks(JNIEnv* env,
		jobject thisObject, jlong ocrdesptr) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	if (!verify_descriptor(ocrdes, true))
		return -1;
	return ocrdes->textpage->textblocks();
}

JNIEXPORT jint JNICALL Java_com_ocrad_Main_OCRAD_1result_1lines(JNIEnv* env,
		jobject thisObject, jlong ocrdesptr, jint blocknum) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	if (!verify_descriptor(ocrdes, true))
		return -1;
	if (blocknum < 0 || blocknum >= ocrdes->textpage->textblocks()) {
		ocrdes->ocr_errno = OCRAD_bad_argument;
		return -1;
	}
	return ocrdes->textpage->textblock(blocknum).textlines();
}

JNIEXPORT jstring JNICALL Java_com_ocrad_Main_OCRAD_1result_1line(JNIEnv* env,
		jobject thisObject, jlong ocrdesptr, jint blocknum, jint linenum) {
	OCRAD_Descriptor * const ocrdes = (OCRAD_Descriptor*) ocrdesptr;
	if (!verify_descriptor(ocrdes, true))
		return 0;
	if (blocknum < 0 || blocknum >= ocrdes->textpage->textblocks()
			|| linenum < 0
			|| linenum >= ocrdes->textpage->textblock(blocknum).textlines()) {
		ocrdes->ocr_errno = OCRAD_bad_argument;
		return 0;
	}
	const Textline & textline = ocrdes->textpage->textblock(blocknum).textline(
			linenum);
	ocrdes->text.clear();
	if (!ocrdes->control.utf8)
		for (int i = 0; i < textline.characters(); ++i)
			ocrdes->text += textline.character(i).byte_result();
	else
		for (int i = 0; i < textline.characters(); ++i)
			ocrdes->text += textline.character(i).utf8_result();
	ocrdes->text += '\n';
	return env->NewStringUTF(ocrdes->text.c_str());
}
