#include "BioBox.h"

BioBox::BioBox()
	: bio(NULL) {
	buf.buf = NULL;
	buf.size = 0;
}

BioBox::~BioBox() {
	BIO_free(bio);
	free(buf.buf);
}

void BioBox::ConstructSink(const std::string& str) {
	BIO_free(bio);
	bio = BIO_new_mem_buf(StringAsVoid(str), -1);
	if (!bio)
		throw ReadError();
}

void BioBox::NewBuffer() {
	BIO_free(bio);
	bio = BIO_new(BIO_s_mem());
	if (!bio)
		throw ReadError();
}

BIO* BioBox::Bio() const {
	return bio;
}
BioBox::Buffer BioBox::ReadAll() {
	buf.size = BIO_ctrl_pending(bio);
	buf.buf = malloc(buf.size);
	if (BIO_read(bio, buf.buf, buf.size) < 0) {
		throw ReadError();
	}
	return buf;
}