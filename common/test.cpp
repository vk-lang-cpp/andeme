#define _CRT_SECURE_NO_WARNINGS
#include "RSAKey.h"
#include <fstream>
#include <assert.h>
int main(void)
{
	// New key
	RSAKey acc;

	std::string pem;
	// Get public key
	acc.PublicKey(pem);
	std::cout << pem << "\n";
	/* Get private key*/
	acc.PrivateKey(pem, "hello");
	std::cout << pem << "\n";

	/* Load a private key using pass 'hello'*/
	acc.Load(pem, "hello");
	acc.PublicKey(pem);
	std::cout << pem << "\n";


    FILE * privateKey = nullptr, *publicKey = nullptr;
    privateKey = fopen("private.pem", "wb");
    publicKey = fopen("public.pem", "wb");

	// 
	PEM_write_RSAPublicKey(publicKey, acc.getRSAKey());
    PEM_write_RSAPrivateKey(privateKey, acc.getRSAKey(), acc.getCipher(), nullptr, 0, nullptr, StringAsVoid("hello"));
    return 0;
}
