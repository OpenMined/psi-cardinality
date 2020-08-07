package org.openmined;

// TODO: this import is just to test linking to PSI protobuf library
import org.openmined.proto.PsiProto.*;

public class PsiServer {
    static {
        System.loadLibrary("c_PsiServer");
    }

    // actually contains a void*
    private long psiServerCtx;

    // use Java conventions for constructors

    // TODO: finish JNI C++ code that does the heavy-lifting. This class only provides a Java wrapper.

    private static native long create_with_new_key(boolean reveal_intersection) throws Exception;
    private static native long create_from_key(byte[] key_bytes, boolean reveal_intersection) throws Exception;

    public PsiServer(boolean reveal_intersection) throws Exception {
        this.psiServerCtx = create_with_new_key(reveal_intersection);
    }

    public PsiServer(byte[] key_bytes, boolean reveal_intersection) throws Exception {
        this.psiServerCtx = create_from_key(key_bytes, reveal_intersection);
    }

    // TODO: methods that take protobufs as parameters should first serialize the protobufs into
    // byte arrays, before handing them off through JNA to the C++ code

    public native byte[] get_private_key_bytes() throws Exception;
}
