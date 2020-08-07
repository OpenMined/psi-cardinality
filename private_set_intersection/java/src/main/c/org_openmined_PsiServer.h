#include <jni.h>
/* Header for class org_openmined_PsiServer */

#ifndef _Included_org_openmined_PsiServer
#define _Included_org_openmined_PsiServer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_openmined_PsiServer
 * Method:    create_with_new_key
 * Signature: (Z)J
 */
JNIEXPORT jlong JNICALL Java_org_openmined_PsiServer_create_1with_1new_1key
  (JNIEnv *, jclass, jboolean);

/*
 * Class:     org_openmined_PsiServer
 * Method:    create_from_key
 * Signature: ([BZ)J
 */
JNIEXPORT jlong JNICALL Java_org_openmined_PsiServer_create_1from_1key
  (JNIEnv *, jclass, jbyteArray, jboolean);

/*
 * Class:     org_openmined_PsiServer
 * Method:    get_private_key_bytes
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_openmined_PsiServer_get_1private_1key_1bytes
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
