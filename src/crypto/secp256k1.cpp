#include <crypto/secp256k1.hpp>

namespace artha {

Secp256k1Context::Secp256k1Context()
{
	_ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
}

Secp256k1Context::~Secp256k1Context()
{
	secp256k1_context_destroy(_ctx);
}

}
