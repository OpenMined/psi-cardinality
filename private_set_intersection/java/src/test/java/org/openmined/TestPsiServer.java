package org.openmined;

import static org.junit.Assert.assertArrayEquals;

import org.junit.Test;

import org.openmined.PsiServer;

public class TestPsiServer {
    @Test
    public void testCreate() throws Exception {
        testCreateReveal(false);
        testCreateReveal(true);
    }

    private void testCreateReveal(boolean reveal) throws Exception {
        PsiServer server = new PsiServer(reveal);
        byte[] key = server.getPrivateKeyBytes();
        PsiServer new_server = new PsiServer(key, reveal);
        assertArrayEquals(server.getPrivateKeyBytes(), new_server.getPrivateKeyBytes());

        byte[] key2 = new byte[32];

        for (int i = 0; i < key2.length; i++) {
            key2[i] = 1;
        }

        PsiServer server2 = new PsiServer(key2, reveal);
        assertArrayEquals(server2.getPrivateKeyBytes(), key2.clone());
    }
}
