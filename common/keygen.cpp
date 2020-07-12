#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <iostream>
#include <memory>
#include <random>
#include "keygen.h"

namespace {

	const int kKeySize = 4096;
	const std::string kKeyPass = "privateKey";

	std::string getPubKey(EVP_PKEY* evp) {
		
		std::unique_ptr<BIO, decltype(&BIO_free)> buf(BIO_new(BIO_s_mem()), BIO_free);
		PEM_write_bio_PUBKEY(buf.get(), evp);

		size_t length = BIO_ctrl_pending(buf.get());
		std::unique_ptr<char[]> res(new char[length]);

		BIO_read(buf.get(), res.get(), length);

		return std::string{ res.get(), length };
	}

	std::string getPrivKey(EVP_PKEY* evp) {
		
		std::unique_ptr<BIO, decltype(&BIO_free)> buf(BIO_new(BIO_s_mem()), BIO_free);
		PEM_write_bio_PKCS8PrivateKey(buf.get(), evp, EVP_aes_256_cbc(),
			const_cast<char*>(kKeyPass.c_str()), kKeyPass.length(), nullptr, nullptr);

		size_t length = BIO_ctrl_pending(buf.get());
		std::unique_ptr<char[]> res(new char[length]);
		BIO_read(buf.get(), res.get(), length);

		return std::string{ reinterpret_cast<char*>(res.get()), length };
	}

}

namespace andeme {

	//PublicKey

	PublicKey::PublicKey(const std::string&& key) noexcept : public_key_{ std::move(key) } {}

	bool PublicKey::verify(const std::string& sign, const std::string& data) const {

		const unsigned char* message = reinterpret_cast<const unsigned char*>(data.data());

		const unsigned char* sigbuf = reinterpret_cast<const unsigned char*>(sign.data());

		std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> m_RSAVerifyCtx(EVP_MD_CTX_new(), EVP_MD_CTX_free);
		std::unique_ptr<BIO, decltype(&BIO_free)> keybio(BIO_new_mem_buf(public_key_.c_str(), -1), BIO_free);
		std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> pubKey(EVP_PKEY_new(), EVP_PKEY_free);
		EVP_PKEY* evp_key = nullptr;

		PEM_read_bio_PUBKEY(keybio.get(), &evp_key, nullptr, const_cast<char*>(kKeyPass.data()));
		pubKey.reset(evp_key);

		if (EVP_DigestVerifyInit(m_RSAVerifyCtx.get(), nullptr, EVP_sha256(), nullptr, pubKey.get()) <= 0) {
			return false;
		}
		if (EVP_DigestUpdate(m_RSAVerifyCtx.get(), message, data.length()) <= 0) {
			return false;
		}

		return EVP_DigestVerifyFinal(m_RSAVerifyCtx.get(), sigbuf, sign.length());
	}

	//PrivateKey

	PrivateKey::PrivateKey(const std::string&& key) noexcept : private_key_{ std::move(key) } {}

	std::string PrivateKey::sign(const std::string& data) const {

		const unsigned char* temp = reinterpret_cast<const unsigned char*>(data.data());

		unsigned char* res = new unsigned char[kKeySize / 8];
		size_t siglen;

		std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> m_RSASignCtx(EVP_MD_CTX_new(), EVP_MD_CTX_free);
		std::unique_ptr<BIO, decltype(&BIO_free)> keybio(BIO_new_mem_buf(private_key_.c_str(), -1), BIO_free);
		std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> privKey(EVP_PKEY_new(), EVP_PKEY_free);
		EVP_PKEY* evp_key = nullptr;

		PEM_read_bio_PrivateKey(keybio.get(), &evp_key, nullptr, const_cast<char*>(kKeyPass.data()));
		privKey.reset(evp_key);

		EVP_DigestSignInit(m_RSASignCtx.get(), nullptr, EVP_sha256(), nullptr, privKey.get());
		EVP_DigestUpdate(m_RSASignCtx.get(), temp, data.length());
		EVP_DigestSignFinal(m_RSASignCtx.get(), nullptr, &siglen);
		EVP_DigestSignFinal(m_RSASignCtx.get(), res, &siglen);

		return std::string{ reinterpret_cast<char*>(res), siglen };
	};

	//RSAUtil

	std::pair<PublicKey, PrivateKey> RSAUtil::generate() {

		srand(std::random_device{}());

		std::unique_ptr<BIGNUM, decltype(&BN_free)> bne(BN_new(), BN_free);
		BN_set_word(bne.get(), RSA_F4);

		std::unique_ptr<RSA, decltype(&RSA_free)> rsa_keys(RSA_new(), RSA_free);
		RSA_generate_key_ex(rsa_keys.get(), kKeySize, bne.get(), nullptr);

		std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> evp(EVP_PKEY_new(), EVP_PKEY_free);
		EVP_PKEY_set1_RSA(evp.get(), rsa_keys.get());

		return { PublicKey{ std::move(getPubKey(evp.get())) }, PrivateKey{ std::move(getPrivKey(evp.get())) } };
	}

}