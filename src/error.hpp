#pragma once

#include <string>

namespace artha {

enum TransactionError: uint8_t {
	NONE,
	MISSING_INPUTS,
	SIG_MISMATCH
};

std::string ErrorToString(TransactionError code);

}
