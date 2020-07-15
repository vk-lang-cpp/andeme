#include "RSAKey.h"

namespace andeme {
	RSAKey::RSAKey() : bio(BIO_new(BIO_s_mem())),
		pKeyPair(RSA_new()),
		cipher(EVP_get_cipherbyname("aes-256-cbc"))
	{
		BIGNUM* e = BN_new();
		BN_set_word(e, RSA_F4);
			int ret = RSA_generate_key_ex(pKeyPair, RSA_KEYLENGTH, e, 0);
			if (cipher == NULL)
				OpenSSL_add_all_algorithms();

	}

	RSAKey::~RSAKey()
	{
		if (pKeyPair != nullptr)
			RSA_free(pKeyPair);
		if (bio !=nullptr)
			BIO_free(bio);
	}

	std::string RSAKey::getPublicKey()
	{
		PEM_write_bio_RSAPublicKey(bio, pKeyPair);
		size_t length = BIO_ctrl_pending(bio);

		std::string pstr;
		void* buf = malloc(length);

		BIO_read(bio, buf, length);

		std::string pem = std::string(reinterpret_cast<const char*>(buf), length);
		free(buf);
		return pem;
	}
	std::string RSAKey::getPrivate()
	{
		int ret = PEM_write_bio_RSAPrivateKey(bio, pKeyPair, nullptr, nullptr, 0, nullptr, nullptr);

		size_t length = BIO_ctrl_pending(bio);

		std::string pstr;
		void* buf = malloc(length);

		BIO_read(bio, buf, length);

		std::string pem = std::string(reinterpret_cast<const char*>(buf), length);
		free(buf);
		return pem;
	}
}