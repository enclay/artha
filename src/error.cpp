#include <error.hpp>

namespace artha {

std::string ErrorToString(TransactionError code)
{
	switch (code) {
		case TransactionError::MISSING_INPUTS: return "Inputs missing or spent";
		case TransactionError::SIG_MISMATCH: return "Signatures do not match";
		case TransactionError::NONE: default: return "No error";
	}
}

}
