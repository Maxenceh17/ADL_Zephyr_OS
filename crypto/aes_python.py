from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad

# Clé AES (128 bits)
aes_key = bytes([
    0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81
])

# Texte en clair
plaintext = bytes([
    ord('T'), ord('h'), ord('i'), ord('s'), ord(' '), ord('i'), ord('s'), ord(' '),
    ord('a'), ord(' '), ord('t'), ord('e'), ord('s'), ord('t'), ord('!'), ord(' ')
])

# Chiffrement
cipher = AES.new(aes_key, AES.MODE_ECB)
encrypted = cipher.encrypt(plaintext)  # Pas besoin de pad car le texte fait déjà 16 octets
print("Message chiffré :", encrypted.hex())

# Déchiffrement
decipher = AES.new(aes_key, AES.MODE_ECB)
decrypted = decipher.decrypt(encrypted)
print("Message déchiffré :", decrypted.decode('utf-8'))
