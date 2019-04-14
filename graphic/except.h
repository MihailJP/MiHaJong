#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace mihajong_graphic {

class GraphicModuleError : public std::exception {};

class ModuleInitializationError : public GraphicModuleError, std::runtime_error {
public:
	explicit ModuleInitializationError(const std::string& what) : std::runtime_error(what) {}
};

class KeyboardInitializationError : public ModuleInitializationError {
public:
	explicit KeyboardInitializationError(const std::string& what) : ModuleInitializationError(what) {}
};

class MouseInitializationError : public ModuleInitializationError {
public:
	explicit MouseInitializationError(const std::string& what) : ModuleInitializationError(what) {}
};

class GamepadInitializationError : public ModuleInitializationError {
public:
	explicit GamepadInitializationError(const std::string& what) : ModuleInitializationError(what) {}
};

class NoGamepadsAvailable : public GamepadInitializationError {
public:
	explicit NoGamepadsAvailable(const std::string& what) : GamepadInitializationError(what) {}
};

class WindowInitializationError : public ModuleInitializationError {
public:
	explicit WindowInitializationError(const std::string& what) : ModuleInitializationError(what) {}
};

class TextureCreationError : public GraphicModuleError, std::runtime_error {
public:
	explicit TextureCreationError(const std::string& what) : std::runtime_error(what) {}
#ifdef WITH_DIRECTX
	explicit TextureCreationError(const std::string& desc, std::intptr_t resourceID);
#endif /* WITH_DIRECTX */
};

class SpriteCreationError : public GraphicModuleError, std::runtime_error {
public:
	explicit SpriteCreationError(const std::string& what) : std::runtime_error(what) {}
};

class UninitializedObject : public GraphicModuleError, std::runtime_error {
public:
	explicit UninitializedObject(const std::string& what) : std::runtime_error(what) {}
};

class UnexpectedControlFlow : public GraphicModuleError, std::logic_error {
public:
	explicit UnexpectedControlFlow(const std::string& what) : std::logic_error(what) {}
};

class InvalidArgument : public GraphicModuleError, std::logic_error {
public:
	explicit InvalidArgument(const std::string& what) : std::logic_error(what) {}
};

class InvalidGameType : public InvalidArgument {
public:
	explicit InvalidGameType(const std::string& what) : InvalidArgument(what) {}
};

class InvalidScene : public InvalidArgument {
public:
	explicit InvalidScene(const std::string& what) : InvalidArgument(what) {}
};

class InvalidPlayerCode : public InvalidArgument {
public:
	explicit InvalidPlayerCode(const std::string& what) : InvalidArgument(what) {}
};

class DataLoadError : public GraphicModuleError, std::runtime_error {
public:
	explicit DataLoadError(const std::string& what) : std::runtime_error(what) {}
#ifdef _WIN32
	explicit DataLoadError(const std::string& desc, unsigned long err, int num);
#else /*_WIN32*/
	explicit DataLoadError(const std::string& desc, int num, const std::string& filename);
#endif /*_WIN32*/
};

class FontMapLoadError : public DataLoadError {
public:
	explicit FontMapLoadError(const std::string& what) : DataLoadError(what) {}
#ifdef _WIN32
	explicit FontMapLoadError(unsigned long err, int num) : DataLoadError("フォントマップ", err, num) {}
#else /*_WIN32*/
	explicit FontMapLoadError(int num, const std::string& filename) : DataLoadError("フォントマップ", num, filename) {}
#endif /*_WIN32*/
};

class TextureLoadError : public DataLoadError {
public:
	explicit TextureLoadError(const std::string& what) : DataLoadError(what) {}
#ifdef _WIN32
	explicit TextureLoadError(unsigned long err, int num) : DataLoadError("テクスチャ", err, num) {}
#else /*_WIN32*/
	explicit TextureLoadError(int num, const std::string& filename) : DataLoadError("テクスチャ", num, filename) {}
#endif /*_WIN32*/
};

}
