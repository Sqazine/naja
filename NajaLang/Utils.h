#pragma once
#include <string>
#include <string_view>
#include <memory>
namespace NajaLang
{
    std::string ReadFile(std::string_view path);


	template <typename T>
	using SharedRef = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr SharedRef<T> CreateShared(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using UniqueRef = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr UniqueRef<T> CreateUnique(Args &&...args)
	{
		return std::move(std::make_unique<T>(std::forward<Args>(args)...));
	}

}