#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <jni.h>

#include "private_set_intersection/java/src/main/c/org_openmined_PsiServer.h"

#include <algorithm>

#include "private_set_intersection/c/internal_utils.h"
#include "private_set_intersection/cpp/psi_server.h"

namespace {
    using private_set_intersection::PsiServer;
    using private_set_intersection::c_bindings_internal::generate_error;
}

typedef void* psi_server_ctx;

JNIEXPORT jlong JNICALL Java_org_openmined_PsiServer_create_1with_1new_1key
  (JNIEnv* env, jclass self, jboolean reveal_intersection) {
    auto result = PsiServer::CreateWithNewKey((bool)reveal_intersection);

    if (result.ok()) {
        psi_server_ctx ctx = std::move(result).ValueOrDie().release();

        if (ctx != nullptr) {
            return (jlong)ctx;
        }
    }

    char* null_ptr = nullptr;
    char** error_out = (char**)&null_ptr;
    generate_error(result.status(), error_out);
    jclass Exception = env->FindClass("java/lang/Exception");
    env->ThrowNew(Exception, *error_out);

    return 0;
}

JNIEXPORT jlong JNICALL Java_org_openmined_PsiServer_create_1from_1key
  (JNIEnv* env, jclass self, jbyteArray key_bytes, jboolean reveal_intersection) {
    auto key_str = std::string((char*)env->GetByteArrayElements(key_bytes, NULL), (size_t)env->GetArrayLength(key_bytes));
    auto result = PsiServer::CreateFromKey(key_str, (bool)reveal_intersection);

    if (result.ok()) {
        psi_server_ctx ctx = std::move(result).ValueOrDie().release();

        if (ctx != nullptr) {
            return (jlong)ctx;
        }
    }

    char* null_ptr = nullptr;
    char** error_out = (char**)&null_ptr;
    generate_error(result.status(), error_out);
    jclass Exception = env->FindClass("java/lang/Exception");
    env->ThrowNew(Exception, *error_out);

    return 0;
}

JNIEXPORT jbyteArray JNICALL Java_org_openmined_PsiServer_get_1private_1key_1bytes
  (JNIEnv* env, jobject self) {
    jclass self_class = env->GetObjectClass(self);
    jfieldID ctx_field_id = env->GetFieldID(self_class, "psiServerCtx", "J");
    jlong ctx_ptr = env->GetLongField(self, ctx_field_id);
    auto server = static_cast<PsiServer*>((psi_server_ctx)ctx_ptr);

    if (server == nullptr) {
        char* null_ptr = nullptr;
        char** error_out = (char**)&null_ptr;
        generate_error(::private_join_and_compute::InvalidArgumentError("invalid server context"), error_out);
        jclass Exception = env->FindClass("java/lang/Exception");
        env->ThrowNew(Exception, *error_out);
        return NULL;
    }

    auto value = server->GetPrivateKeyBytes();
    size_t len = value.size();

    jbyteArray out = env->NewByteArray(len);
    env->SetByteArrayRegion(out, 0, len, (jbyte*)value.data());

    return out;
}
