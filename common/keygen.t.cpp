#include <gtest/gtest.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/ossl_typ.h>
#include <memory>
#include <iostream>

TEST(KeyGen, OpenSSLTest) {
    
    std::string public_key, private_key;
    
    // Генерация ключей
    {
        std::unique_ptr<BIGNUM, decltype(&BN_free)> e(BN_new(), BN_free);
        BN_set_word(e.get(), RSA_F4);

        std::unique_ptr<RSA, decltype(&RSA_free)> rsa(RSA_new(), RSA_free);

        // Генерируем public и private ключи в RSA структуру
        
        // https://www.openssl.org/docs/man1.1.1/man3/RSA_generate_key_ex.html
        // int RSA_generate_key_ex(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb);
        ASSERT_EQ(1, RSA_generate_key_ex(rsa.get(), 2048, e.get(), nullptr));

        // Вытаскиваем public и private ключи из RSA структуры

        {
            std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);
            ASSERT_EQ(1, PEM_write_bio_RSAPublicKey(bio.get(), rsa.get()));

            public_key.resize(BIO_ctrl_pending(bio.get()));
            ASSERT_EQ(public_key.size(), BIO_read(bio.get(), public_key.data(), public_key.size()));
        }

        std::cout << public_key << std::endl;
        
        {
            std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);
            ASSERT_EQ(1,
                      PEM_write_bio_RSAPrivateKey(bio.get(), 
                                                  rsa.get(),
                                                  nullptr,
                                                  nullptr,
                                                  0,
                                                  nullptr, 
                                                  nullptr));

            private_key.resize(BIO_ctrl_pending(bio.get()));
            ASSERT_EQ(private_key.size(), BIO_read(bio.get(), private_key.data(), private_key.size()));
        }

        std::cout << private_key << std::endl;
    }

    const std::string test_message = 
        "Hae elēni ā iepagon\n"
        "Tcheret gere vēja pelothos\n"
        "Tchaïmè leda kivio yn tegon\n"
        "Tchere iā mili bano";

    std::string signature;

    // Подписываение сообщения
    {
        std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);
        ASSERT_EQ(private_key.size(), BIO_write(bio.get(), private_key.data(), private_key.size()));

        // RSA *PEM_read_bio_RSAPrivateKey(BIO *bp, RSA **x,
        //                                 pem_password_cb *cb, void *u);
        std::unique_ptr<RSA, decltype(&RSA_free)> rsa(
            PEM_read_bio_RSAPrivateKey(bio.get(), nullptr, nullptr, nullptr),
            RSA_free);

        // https://www.openssl.org/docs/man1.1.1/man3/EVP_DigestSignFinal.html

    }
}
