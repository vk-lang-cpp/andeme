#include "EvpBox.h"

EvpBox::EvpBox(RSA* keyp) {
	evpkey = EVP_PKEY_new();
	if (!EVP_PKEY_set1_RSA(evpkey, keyp)) {
		throw ReadError();
	}
}
EvpBox::~EvpBox() {
	EVP_PKEY_free(evpkey);
}
EVP_PKEY* EvpBox::Key() {
	return evpkey;
}