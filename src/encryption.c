#include <linux/crypto.h>
#include <linux/slab.h>
#include <linux/random.h>
#include "../include/safe_device.h"

// Configurar el transform handle
static struct crypto_skcipher *tfm = NULL;

int init_crypto(void) {
    tfm = crypto_alloc_skcipher("cbc(aes)", 0, 0);
    return IS_ERR(tfm) ? PTR_ERR(tfm) : 0;
}

void cleanup_crypto(void) {
    if (tfm) crypto_free_skcipher(tfm);
}

// Configurar el vector de inicializacion y el unsigned 8 bit unit
int encrypt_data(const u8 *key, const char *plaintext, size_t len, 
                 u8 **ciphertext, size_t *ciphertext_len) 
{
    struct skcipher_request *req = NULL;
    struct scatterlist sg_in, sg_out;
    u8 *iv;
    int ret = 0;
    unsigned int total_len = IV_LEN + len; // IV + texto plano
    
    // Allocate output buffer (IV prepended to ciphertext)
    u8 *output = kmalloc(total_len, GFP_KERNEL);
    if (!output) return -ENOMEM;
    iv = output; // First 16 bytes = IV
    get_random_bytes(iv, IV_LEN);
    
    // Configurar crypto
    crypto_skcipher_setkey(tfm, key, KEY_SIZE);
    req = skcipher_request_alloc(tfm, GFP_KERNEL);
    
    // Configurar input (texto plano) y output (a partir de IV)
    sg_init_one(&sg_in, plaintext, len);
    sg_init_one(&sg_out, output + IV_LEN, len);
    skcipher_request_set_crypt(req, &sg_in, &sg_out, len, iv);
    
    // Ejecutar encriptacion
    ret = crypto_skcipher_encrypt(req);
    if (!ret) {
        *ciphertext = output;
        *ciphertext_len = total_len;
    } else {
        kfree(output);
    }
    
    skcipher_request_free(req);
    return ret;
}
    
