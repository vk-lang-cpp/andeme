#ifndef ANDEME_COMMON_RSAUTIL_H_
#define ANDEME_COMMON_RSAUTIL_H_

#include <string>

namespace andeme {

	class PublicKey {

		std::string public_key_;

	public:

		PublicKey(std::string&& key) noexcept;

		bool Verify(const std::string& sign, const std::string& data) const;
		const std::string& KeyValue();
	};


	class PrivateKey {

		std::string private_key_;

	public:

		PrivateKey(std::string&& key) noexcept;

		std::string Sign(const std::string& data) const;
		const std::string& KeyValue();
	};


	class RSAUtil {

	public:

		static std::pair<PublicKey, PrivateKey> Generate();
	};

}

#endif 