#include "EncryptManager.h"

#include <openssl/evp.h>
#include <openssl/x509.h>
void Encryptor::encrypt() {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  unsigned char key[EVP_MAX_KEY_LENGTH];
  unsigned char iv[EVP_MAX_IV_LENGTH];
  memset(iv, 0, EVP_MAX_IV_LENGTH);
  for (int i = 0; i < 8; i++) {
    iv[i] = i;
  }
  memset(key, 0, EVP_MAX_KEY_LENGTH);
  memcpy(key, password.c_str(), password.size());
  EVP_CIPHER_CTX_init(ctx);
  EVP_EncryptInit_ex(ctx, EVP_des_ede3_cbc(), NULL, key, iv);
  unsigned char input[1024];
  unsigned char output[1024];
  int inl;
  int outl;
  while ((inl = read(this->ori_fp, input, 1024)) > 0) {
    EVP_EncryptUpdate(ctx, output, &outl, input, inl);
    write(this->res_fp, output, outl);
  }
  EVP_EncryptFinal_ex(ctx, output, &outl);
  write(this->res_fp, output, outl);
  EVP_CIPHER_CTX_cleanup(ctx);
}
void Decryptor::decrypt() {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  unsigned char key[EVP_MAX_KEY_LENGTH];
  unsigned char iv[EVP_MAX_IV_LENGTH];
  memset(iv, 0, EVP_MAX_IV_LENGTH);
  for (int i = 0; i < 8; i++) {
    iv[i] = i;
  }
  memset(key, 0, EVP_MAX_KEY_LENGTH);
  memcpy(key, password.c_str(), password.size());
  EVP_CIPHER_CTX_init(ctx);
  EVP_DecryptInit_ex(ctx, EVP_des_ede3_cbc(), NULL, key, iv);
  unsigned char input[1024];
  unsigned char output[1024];
  int inl;
  int outl;
  while ((inl = read(this->ori_fp, input, 1024)) > 0) {
    EVP_DecryptUpdate(ctx, output, &outl, input, inl);
    write(this->res_fp, output, outl);
  }
  EVP_DecryptFinal_ex(ctx, output, &outl);
  write(this->res_fp, output, outl);
  EVP_CIPHER_CTX_cleanup(ctx);
}
