#include "RSAKey.h"
#include "BioBox.h"
#include "EvpBox.h"

RSAKey::RSAKey() : pKeyPair(nullptr), cipher(NULL) {

	cipher = EVP_get_cipherbyname("aes-256-cbc");
	if (cipher == NULL)
		OpenSSL_add_all_algorithms();

	/* Generate RSA  */
	if (pKeyPair != nullptr)
		RSA_free(pKeyPair);
	pKeyPair = RSA_generate_key(RSA_KEYLENGTH, RSA_E, NULL, NULL);
	/*Checking key for available*/
	CheckKey();
}

RSAKey::~RSAKey() {
	RSA_free(pKeyPair);
}

RSA * RSAKey::getRSAKey()const {
	return pKeyPair;
}

const EVP_CIPHER* RSAKey::getCipher() {
	return cipher;
}


void RSAKey::CheckKey() const {
	if (!pKeyPair)
		throw NoKeypairLoaded();
}

void RSAKey::Load(const std::string& pem, const std::string& pass) {
	RSA_free(pKeyPair);
	BioBox bio;
	bio.ConstructSink(pem);
	pKeyPair = PEM_read_bio_RSAPrivateKey(bio.Bio(), NULL, pass_cb,
		StringAsVoid(pass));
	CheckKey();
}

void RSAKey::PublicKey(std::string& pem) const {
	CheckKey();
	BioBox bio;
	bio.NewBuffer();
	int ret = PEM_write_bio_RSA_PUBKEY(bio.Bio(), pKeyPair);
	if (!ret)
		throw ReadError();
	const BioBox::Buffer& buf = bio.ReadAll();
	pem = std::string(reinterpret_cast<const char*>(buf.buf), buf.size);
}

void RSAKey::PrivateKey(std::string& pem, const std::string& passphrase) const {
	CheckKey();
	BioBox bio;
	bio.NewBuffer();

	EvpBox evp(pKeyPair);
	int ret = PEM_write_bio_PKCS8PrivateKey(bio.Bio(), evp.Key(),
		EVP_aes_256_cbc(),
		LoseStringConst(passphrase),
		passphrase.size(), NULL, NULL);
	if (!ret)
		throw ReadError();
	const BioBox::Buffer& buf = bio.ReadAll();
	pem = std::string(reinterpret_cast<const char*>(buf.buf), buf.size);
}


const char* ReadError::what() const throw() {
	return "Problem reading BIO.";
}

const char* NoKeypairLoaded::what() const throw() {
	return "No keypair loaded.";
}