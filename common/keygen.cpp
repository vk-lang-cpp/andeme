#include <openssl/pem.h>
#include <memory>
#include <random>
#include "keygen.h"

namespace {

	const int kKeySize = 4096;

	std::string getPubKey(RSA* rsa) {
		
		std::unique_ptr<BIO, decltype(&BIO_free)> buf(BIO_new(BIO_s_mem()), BIO_free);
		PEM_write_bio_RSAPublicKey(buf.get(), rsa);

		size_t length = BIO_ctrl_pending(buf.get());

		std::string res;
		res.resize(length);

		BIO_read(buf.get(), res.data(), length);

		return res;
	}

	std::string getPrivKey(RSA* rsa) {
		
		std::unique_ptr<BIO, decltype(&BIO_free)> buf(BIO_new(BIO_s_mem()), BIO_free);
		PEM_write_bio_RSAPrivateKey(buf.get(), rsa, nullptr, nullptr, 0, nullptr, nullptr);

		size_t length = BIO_ctrl_pending(buf.get());
		std::string res;
		res.resize(length);

		BIO_read(buf.get(), res.data(), length);

		return res;
	}

}

namespace andeme {

	//PublicKey

	PublicKey::PublicKey(std::string&& key) noexcept : public_key_{ std::move(key) } {}

	bool PublicKey::verify(const std::string& sign, const std::string& data) const {

		std::unique_ptr<BIO, decltype(&BIO_free)> keybio(BIO_new_mem_buf(public_key_.data(), -1), BIO_free);
		std::unique_ptr<RSA, decltype(&RSA_free)> pubKey(nullptr, RSA_free);
		RSA* rsa_key = nullptr;

		PEM_read_bio_RSAPublicKey(keybio.get(), &rsa_key, nullptr, nullptr);
		pubKey.reset(rsa_key);

		return RSA_verify(NID_sha256, reinterpret_cast<const unsigned char*>(data.data()), data.length(), 
			reinterpret_cast<const unsigned char*>(sign.data()), sign.length(), pubKey.get());
	}

	//PrivateKey

	PrivateKey::PrivateKey(std::string&& key) noexcept : private_key_{ std::move(key) } {}

	std::string PrivateKey::sign(const std::string& data) const {

		std::unique_ptr<BIO, decltype(&BIO_free)> keybio(BIO_new_mem_buf(private_key_.data(), -1), BIO_free);
		std::unique_ptr<RSA, decltype(&RSA_free)> privKey(nullptr, RSA_free);
		RSA* rsa_key = nullptr;

		PEM_read_bio_RSAPrivateKey(keybio.get(), &rsa_key, nullptr, nullptr);
		privKey.reset(rsa_key);

		std::string res;
		res.resize(kKeySize / 8);
		unsigned int siglen;

		RSA_sign(NID_sha256, reinterpret_cast<const unsigned char*>(data.data()), data.length(),
			reinterpret_cast<unsigned char*>(res.data()), &siglen, privKey.get());

		return res;
	};

	//RSAUtil

	std::pair<PublicKey, PrivateKey> RSAUtil::generate() {

		srand(std::random_device{}());

		std::unique_ptr<BIGNUM, decltype(&BN_free)> bne(BN_new(), BN_free);
		BN_set_word(bne.get(), RSA_F4);

		std::unique_ptr<RSA, decltype(&RSA_free)> rsa_keys(RSA_new(), RSA_free);
		RSA_generate_key_ex(rsa_keys.get(), kKeySize, bne.get(), nullptr);

		return { PublicKey{ getPubKey(rsa_keys.get()) }, PrivateKey{ getPrivKey(rsa_keys.get()) } };
	}

}