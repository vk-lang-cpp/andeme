#ifndef KEYGEN_H
#define KEYGEN_H

#include <string>

namespace andeme {

	class PublicKey {

		mutable std::string pubkey;

	public:

		PublicKey(const std::string key) noexcept;

		bool verify(const std::string& sign, const std::string& data) const;

	};

	class PrivateKey {

		mutable std::string privkey;

	public:

		PrivateKey(const std::string key) noexcept;

		std::string sign(const std::string& data) const;

	};

	class RSAUtil {

	public:

		RSAUtil() = delete;
		RSAUtil(const RSAUtil&) = delete;
		RSAUtil& operator=(const RSAUtil&) = delete;
		RSAUtil(RSAUtil&&) = delete;
		RSAUtil& operator=(RSAUtil&&) = delete;

		static constexpr int size{ 4096 };

		static std::pair<PublicKey, PrivateKey> generate();

	};
}

#endif 