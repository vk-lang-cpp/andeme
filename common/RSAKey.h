#pragma once
#include <openssl/rsa.h> /* core library */
#include <openssl/pem.h> /* errors */
#include <openssl/bio.h> /* BasicInput/Output streams */
#include <iostream>
#include <exception>
#include <string> 


#define RSA_KEYLENGTH 512 /* buffer size  */
#define RSA_E 65537

/*Rsa-generate class*/
class RSAKey {
public:
	RSAKey();
	~RSAKey();
	RSA* getRSAKey() const;
	const EVP_CIPHER* getCipher();
	void CheckKey() const;

	void Load(const std::string&, const std::string&);
	void PublicKey(std::string&) const;
	void PrivateKey(std::string&, const std::string&) const;

private:
	RSA* pKeyPair; /*Rivest Shamir Adleman*/
	const EVP_CIPHER* cipher;
};

/*Exception classes*/
class ReadError : public std::exception
{
public:
	virtual const char* what() const throw();
};

class NoKeypairLoaded : public std::exception
{
public:
	virtual const char* what() const throw();
};

static int pass_cb(char* buf, int size, int rwflag, void* u)
{
	const std::string pass = reinterpret_cast<char*>(u);
	int len = pass.size();
	/* if too long, truncate*/
	if (len > size)
		len = size;
	pass.copy(buf, len);
	return len;
}
/*convert string to void*/
static char *LoseStringConst(const std::string& str)
{
	return const_cast<char*>(str.c_str());
}
static void* StringAsVoid(const std::string& str)
{
	return reinterpret_cast<void*>(LoseStringConst(str));
}
