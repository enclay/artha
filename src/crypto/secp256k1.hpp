#pragma once

#include <secp256k1.h>
#include <secp256k1_ecdh.h>
#include <secp256k1_recovery.h>

namespace artha {

class Secp256k1Context {
public:
	static Secp256k1Context &Get() {
		static Secp256k1Context ctx;
		return ctx;
	}
	
	operator secp256k1_context*() {
		return _ctx;
	}

private:
	Secp256k1Context();
	~Secp256k1Context();

	secp256k1_context* _ctx;
};

}
