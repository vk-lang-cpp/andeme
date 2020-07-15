#pragma once
#include <openssl/rsa.h> 
#include <openssl/evp.h>
#include <openssl/pem.h> 
#include <openssl/bio.h> 
#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#include <iostream>
#include <exception>
#include <string> 
#include <memory>


#define RSA_KEYLENGTH 512 /* buffer size  */
#define RSA_E 65537

/*Rsa-generate class*/
namespace andeme {
	class RSAKey {
	public:

		RSAKey();
		~RSAKey();
		std::string getPublicKey();
		std::string getPrivate();

	private:

		RSA* pKeyPair;
		BIO* bio;
		const EVP_CIPHER* cipher;
	};

}