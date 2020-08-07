package org.openmined;

// TODO: this import is just to test linking to PSI protobuf library
import org.openmined.proto.PsiProto.*;

public class PsiServer {
    static {
        System.loadLibrary("c_PsiServer");
    }

    // actually contains a void*
    private long psiServerCtx;

    // TODO: finish JNI C++ code that does the heavy-lifting. This class only provides a Java wrapper.

    private static native long create_with_new_key(boolean reveal_intersection) throws Exception;
    private static native long create_from_key(byte[] key_bytes, boolean reveal_intersection) throws Exception;

    public PsiServer(boolean revealIntersection) throws Exception {
        this.psiServerCtx = create_with_new_key(revealIntersection);
    }

    public PsiServer(byte[] keyBytes, boolean revealIntersection) throws Exception {
        this.psiServerCtx = create_from_key(keyBytes, revealIntersection);
    }

    // TODO: methods that take protobufs as parameters should first serialize the protobufs into
    // byte arrays, before handing them off through JNI to the C++ code

    private native byte[] get_private_key_bytes(long psiServerCtx) throws Exception;

    public byte[] getPrivateKeyBytes() throws Exception {
        return get_private_key_bytes(this.psiServerCtx);
    }
}
