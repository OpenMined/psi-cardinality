package org.openmined;

import static org.junit.Assert.assertArrayEquals;

import org.junit.Test;

import org.openmined.PsiServer;

public class TestPsiServer {
    @Test
    public void test_create() throws Exception {
        test_create_reveal(false);
        test_create_reveal(true);
    }

    private void test_create_reveal(boolean reveal) throws Exception {
        PsiServer server = new PsiServer(reveal);
        byte[] key = server.get_private_key_bytes();
        PsiServer new_server = new PsiServer(key, reveal);
        assertArrayEquals(server.get_private_key_bytes(), new_server.get_private_key_bytes());

        byte[] key2 = new byte[32];

        for (int i = 0; i < key2.length; i++) {
            key2[i] = 1;
        }

        PsiServer server2 = new PsiServer(key2, reveal);
        assertArrayEquals(server2.get_private_key_bytes(), key2.clone());
    }
}
