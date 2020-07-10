#pragma once
#include "RSAKey.h"

/* BasicInput/Output streams class for RsaKey class*/

class BioBox : public RSAKey
{
public:
	struct Buffer
	{
		void* buf;
		int size;
	};

	BioBox();
	~BioBox();
	void ConstructSink(const std::string&);
	void NewBuffer();
	BIO* Bio() const;
	Buffer ReadAll();
private:
	BIO* bio;
	Buffer buf;
};