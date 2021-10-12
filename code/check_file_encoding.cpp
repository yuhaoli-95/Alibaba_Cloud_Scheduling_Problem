#include "../heads/common.h"


std::string check_file_encoding(std::string path)
{
	std::string result;
	std::ifstream ifs(path.c_str(), std::ios::binary);
	std::stringstream ss;
	std::string encoding = ENCODING_ASCII;

	if (!ifs.is_open()) {
		// Unable to read file
		cout << "不能打开文件！！！" << endl;
		return "-1";
	}
	else if (ifs.eof()) {
		result.clear();
	}
	else {
		int ch1 = ifs.get();
		int ch2 = ifs.get();
		if (ch1 == 0xff && ch2 == 0xfe) {
			// The file contains UTF-16LE BOM
			encoding = ENCODING_UTF16LE;
		}
		else if (ch1 == 0xfe && ch2 == 0xff) {
			// The file contains UTF-16BE BOM
			encoding = ENCODING_UTF16BE;
		}
		else {
			int ch3 = ifs.get();
			if (ch1 == 0xef && ch2 == 0xbb && ch3 == 0xbf) {
				// The file contains UTF-8 BOM
				encoding = ENCODING_UTF8_WITH_BOM;
			}
			else {
				// The file does not have BOM
				encoding = ENCODING_ASCII;
				// ifs.seekg(0);
			}
		}
	}
	ifs.close();
	return encoding;
	// ss << ifs.rdbuf() << "";
	// if (encoding == ENCODING_UTF16LE) {
	// 	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
	// 	result = utfconv.to_bytes(std::wstring((wchar_t *)ss.str().c_str()));
	// }
	// else if (encoding == ENCODING_UTF16BE) {
	// 	cout << "暂时没有windows的<io.h>库" << endl;
	// 	throw new logic_error("");
	// 	// std::string src = ss.str();
	// 	// std::string dst = src;
	// 	// // Using Windows API
	// 	// _swab(&src[0u], &dst[0u], src.size() + 1);
	// 	// std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
	// 	// result = utfconv.to_bytes(std::wstring((wchar_t *)dst.c_str()));
	// }
	// else if (encoding == ENCODING_UTF8) {
	// 	result = ss.str();
	// }
	// else {
	// 	result = ss.str();
	// }
	// return result;
}