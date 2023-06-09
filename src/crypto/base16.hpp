#pragma once

#include <vector>
#include <string>

namespace artha {

template <typename type = unsigned char>
std::vector<type> EncodeBase16(std::vector<type> str, bool capital = false)
{
	std::vector<type> hexstr;
	hexstr.resize(str.size() * 2);
	const size_t a = capital ? 'A' - 1 : 'a' - 1;

	for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF) {
		hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
		hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
	}
	return hexstr;
}

template <typename type = unsigned char>
std::vector<type> DecodeBase16(const std::vector<type> hexstr)
{
	std::vector<type> str;
	str.resize((hexstr.size() + 1) / 2);

	for (size_t i = 0, j = 0; i < str.size(); i++, j++) {
		str[i] = (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
		str[i] |= (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) & 0xF;
	}

	return str;
}

}
