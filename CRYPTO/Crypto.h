/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Crypto.h
 * Author: smorodin
 *
 * Created on July 16, 2023, 11:15 PM
 */

#ifndef CRYPTO_H
#define CRYPTO_H


#include "cryptopp/rsa.h"
#include "cryptopp/integer.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/osrng.h"
#include "cryptopp/sha.h"
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/crc.h"
#include "cryptopp/sha3.h"

using namespace CryptoPP;

///////////////////////////////////////////////////////////////////////////////////////////////////////

class MY_CRC
{
public:
    CRC32 crc32;
    unsigned int calc(unsigned char *src, int len);
    MY_CRC() {};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

class MY_SHA3
{
public:
    //CryptoPP::
    SHA3_256 sha3;
    void hash_32_byte(unsigned char *src_32_byte, unsigned char *dst_32_byte);
    MY_SHA3();
};


class MY_RSA
{
public:
    RSA::PrivateKey priv_Key;
    RSA::PublicKey  publ_Key;
    ModularArithmetic modn;
    Integer r, rInv;
    Integer re_base;
    Integer re;

    Integer _decode(Integer &x);
    void decode_128_byte(unsigned char *src_128_byte, unsigned char *dest_128_byte);

    void encode_128_byte(unsigned char *src_128_byte, unsigned char *dest_128_byte);

    void init();

    MY_RSA() {};
};

class MY_AES
{
    public:
    ECB_Mode<AES>::Decryption d;
    ECB_Mode<AES>::Encryption e;

    void set_key_16_byte(const unsigned char *key_16_byte, const wchar_t *info);
    void encrypt_16_byte(unsigned char *buf_16_byte);

    void encrypt_stream(unsigned char *buf, int buf_len);

    void decrypt_16_byte(unsigned char *buf_16_byte);
    void decrypt_128_byte(unsigned char *buf_16_byte);
    void decrypt_1120_byte(unsigned char *buf_16_byte);

    MY_AES();
};




#endif /* CRYPTO_H */

