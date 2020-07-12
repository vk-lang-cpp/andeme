#ifndef ANDEME_COMMON_KEYGEN_H_
#define ANDEME_COMMON_KEYGEN_H_

#include <string>

namespace andeme {

	class PublicKey {

		const std::string public_key_;

	public:

		PublicKey(const std::string&& key) noexcept;

		bool verify(const std::string& sign, const std::string& data) const;

	};

	class PrivateKey {

		const std::string private_key_;


	public:

		PrivateKey(const std::string&& key) noexcept;

		std::string sign(const std::string& data) const;

	};

	class RSAUtil {

	public:

		static std::pair<PublicKey, PrivateKey> generate();

	};

}

#endif 