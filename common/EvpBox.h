#pragma once
#include "RSAKey.h"

class EvpBox : public RSAKey
{
public:
	EvpBox(RSA*);
	~EvpBox();
	EVP_PKEY* Key();
private:
	EVP_PKEY* evpkey;
};