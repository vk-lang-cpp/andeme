#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <random>
#include "keygen.h"

namespace andeme {

	//PublicKey

	PublicKey::PublicKey(const std::string key) noexcept : pubkey{ std::move(key) } {}

	bool PublicKey::verify(const std::string& sign, const std::string& data) const {

		const unsigned char* message = reinterpret_cast<const unsigned char*>(data.data());

		unsigned char* sigbuf = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(sign.data()));

		return RSA_verify(NID_sha256, message, data.length(), sigbuf, sign.length(), reinterpret_cast<RSA*>(pubkey.data()));
	}

	//PrivateKey

	PrivateKey::PrivateKey(const std::string key) noexcept : privkey{ std::move(key) } {}

	std::string PrivateKey::sign(const std::string& data) const {

		const unsigned char* temp = reinterpret_cast<const unsigned char*>(data.data());

		unsigned char* res = new unsigned char[RSAUtil::size / 8];
		unsigned int siglen;

		RSA_sign(NID_sha256, temp, data.length(), res, &siglen, reinterpret_cast<RSA*>(privkey.data()));

		return std::string{ reinterpret_cast<char*>(res), siglen };
	};

	//RSAUtil

	std::pair<PublicKey, PrivateKey> RSAUtil::generate() {

		srand(std::random_device{}());

		BIGNUM* bne = BN_new();
		BN_set_word(bne, RSA_F4);

		RSA* rsa_keys = RSA_new();

		RSA_generate_key_ex(rsa_keys, size, bne, nullptr);

		std::string byte_rep{ reinterpret_cast<char*>(rsa_keys), static_cast<uint64_t>(RSA_size(rsa_keys)) };

		return { PublicKey{ byte_rep }, PrivateKey{ byte_rep } };
	}

}