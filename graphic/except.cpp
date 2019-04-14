#include "except.h"

#ifdef _WIN32
#define NWL "\r\n"
#else
#define NWL "\n"
#endif

#ifdef _WIN32
mihajong_graphic::DataLoadError::DataLoadError(const std::string& desc, unsigned long err, int num) : std::runtime_error(
	desc + std::string("の読み込みに失敗しました" NWL "エラーコード ") +
	std::to_string(err) +
	std::string(NWL "リソース番号 ") +
	std::to_string(num)
) {}
#else /*_WIN32*/
mihajong_graphic::DataLoadError::DataLoadError(const std::string& desc, int num, const std::string& filename) : std::runtime_error(
	desc + std::string("の読み込みに失敗しました" NWL "リソース番号 ") +
	std::to_string(num) +
	std::string(NWL "ファイル名 ") +
	std::to_string(filename)
) {}
#endif /*_WIN32*/

#ifdef WITH_DIRECTX
mihajong_graphic::TextureCreationError::TextureCreationError(const std::string& desc, std::intptr_t resourceID) : std::runtime_error(
	std::string("テクスチャの生成に失敗しました。") +
	desc +
	std::string(NWL "リソース番号 ") +
	std::to_string(resourceID)
) {}
#endif /* WITH_DIRECTX */
