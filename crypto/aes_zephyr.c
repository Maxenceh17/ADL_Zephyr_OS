#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <tinycrypt/aes.h>
#include <tinycrypt/constants.h>

// Clé pour AES
static const uint8_t aes_key[TC_AES_KEY_SIZE] = {
    0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81
};

static const uint8_t plaintext[TC_AES_BLOCK_SIZE] = {
    'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ',
    't', 'e', 's', 't', '!', ' '
};

void aes_encrypt_decrypt(void) {
    uint8_t encrypted[TC_AES_BLOCK_SIZE];
    uint8_t decrypted[TC_AES_BLOCK_SIZE];
    struct tc_aes_key_sched_struct s;

    // Initialiser la clé AES
    if (tc_aes128_set_encrypt_key(&s, aes_key) != TC_CRYPTO_SUCCESS) {
        printk("Erreur d'initialisation de la clé AES.\n");
        return;
    }

    // Chiffrement AES en mode ECB
    if (tc_aes_encrypt(encrypted, plaintext, &s) != TC_CRYPTO_SUCCESS) {
        printk("Erreur de chiffrement.\n");
        return;
    }

    printk("Données chiffrées: ");
    for (int i = 0; i < sizeof(encrypted); i++) {
        printk("%02x ", encrypted[i]);
    }
    printk("\n");

    // Déchiffrement AES en mode ECB
    if (tc_aes_decrypt(decrypted, encrypted, &s) != TC_CRYPTO_SUCCESS) {
        printk("Erreur de déchiffrement.\n");
        return;
    }

    printk("Données déchiffrées: ");
    for (int i = 0; i < sizeof(decrypted); i++) {
        printk("%c", decrypted[i]);
    }
    printk("\n");
}

void main(void) {
    printk("Exemple de chiffrement et déchiffrement AES avec TinyCrypt:\n");
    aes_encrypt_decrypt();
}
