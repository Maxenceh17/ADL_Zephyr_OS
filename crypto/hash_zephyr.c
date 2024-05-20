#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <tinycrypt/constants.h>
#include <tinycrypt/sha256.h>

static const uint8_t message[] = "Ceci est un texte a hasher ! ";

void hash_sha256(void) {
    struct tc_sha256_state_struct s;
    uint8_t hash[TC_SHA256_DIGEST_SIZE];

    // Initialiser le contexte SHA-256
    if (tc_sha256_init(&s) != TC_CRYPTO_SUCCESS) {
        printk("Erreur d'initialisation du contexte SHA-256.\n");
        return;
    }

    // Mettre à jour le contexte avec les données
    if (tc_sha256_update(&s, message, sizeof(message) - 1) != TC_CRYPTO_SUCCESS) {
        printk("Erreur lors de la mise à jour des données pour SHA-256.\n");
        return;
    }

    // Finaliser le hachage
    if (tc_sha256_final(hash, &s) != TC_CRYPTO_SUCCESS) {
        printk("Erreur lors de la finalisation du hachage SHA-256.\n");
        return;
    }

    // Afficher le hachage
    printk("Hachage SHA-256: ");
    for (int i = 0; i < TC_SHA256_DIGEST_SIZE; i++) {
        printk("%02x", hash[i]);
    }
    printk("\n");
}

void main(void) {
    printk("Exemple de hachage SHA-256 avec TinyCrypt:\n");
    hash_sha256();
}
