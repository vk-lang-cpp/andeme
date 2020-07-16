#pragma once
#include <openssl/rsa.h> 
#include <openssl/evp.h>
#include <openssl/pem.h> 
#include <openssl/bio.h> 
#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#include <iostream>
#include <string> 
#include <memory>


#define RSA_KEYLENGTH 512 /* buffer size  */
#define RSA_E 65537

/*Rsa-generate class*/
namespace andeme {
	class RSAKey {
	public:

		RSAKey();
		RSAKey(RSAKey const &) = delete;
		RSAKey& operator=(RSAKey const &) = delete;
		~RSAKey();
		std::pair<std::string, std::string> generate();

	private:
		std::string getPublicKey();
		std::string getPrivate();
		RSA* pKeyPair;
		BIO* bio;
		
	};

}