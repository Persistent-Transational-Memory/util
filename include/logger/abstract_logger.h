/*
 * @author: BL-GS 
 * @date:   2023/4/16
 */

#pragma once
#ifndef PTM_ABSTRACT_LOGGER_H
#define PTM_ABSTRACT_LOGGER_H

#include <string>

#include <util/enum_operator.h>

namespace util {

	inline namespace logger {

		enum class Output_Type {
			CONSOLE,
			FILE
		};

		enum class Background_Color {
			BLACK     = 40,
			RED       = 41,
			GREEN     = 42,
			YELLOW    = 43,
			BLUE      = 44,
			PURPLE    = 45,
			DARKGREEN = 46,
			GRAY      = 47,
			NONE      = 48
		};

		enum class Font_Color {
			BLACK     = 30,
			RED       = 31,
			GREEN     = 32,
			YELLOW    = 33,
			BLUE      = 34,
			PURPLE    = 35,
			DARKGREEN = 36,
			GRAY      = 37,
			NONE      = 38
		};

		enum class Effect {
			OFF       = 0,
			HIGHLIGHT = 1,
			LOWLIGHT  = 2,
			UNDERLINE = 4,
			BRINK     = 5,
			INVERSE   = 7,
			FADE      = 8,
			NONE      = 9
		};


		class LoggerBase {
		public:
			template<Background_Color bc, Font_Color fc, Effect e>
			inline constexpr static std::string prefix_static() {
				std::string s("\033[");
				if constexpr (bc != Background_Color::NONE) {
					s += std::to_string((int) bc);
					s += ';';
				}
				if constexpr (fc != Font_Color::NONE) {
					s += std::to_string((int) fc);
					s += ';';
				}
				if constexpr (e != Effect::NONE) {
					s += std::to_string((int) e);
					s += ';';
				}
				*(s.end() - 1) = 'm';
				return s;
			}

			inline static std::string prefix(Background_Color bc, Font_Color fc, Effect e) {
				std::string s("\033[");
				if (bc != Background_Color::NONE) {
					s += std::to_string((int) bc);
					s += ';';
				}
				if (fc != Font_Color::NONE) {
					s += std::to_string((int) fc);
					s += ';';
				}
				if (e != Effect::NONE) {
					s += std::to_string((int) e);
					s += ';';
				}
				*(s.end() - 1) = 'm';
				return s;
			}

			inline static std::string suffix() {
				return { "\033[0m" };
			}
		};

		template<Output_Type output_type, bool coloring>
		class Logger {};


		enum class LoggerInfoType {
			Info   = 0,
			Warn   = 1,
			Error  = 2,
			Output = 3,
			None   = 4
		};

	}

	template<>
	struct EnableEnumCompareOperator<LoggerInfoType> {
		static constexpr bool enable = true;
	};

	inline namespace logger {

		/// The lowest level of which information is allowed to be outputted.
		constexpr LoggerInfoType GLOBAL_LEAST_LOGGER_LEVEL = LoggerInfoType::Info;

		//! @brief Whether information of this type is allowed to be outputted
		inline constexpr bool enable_logger_type(LoggerInfoType type) {
			return type >= GLOBAL_LEAST_LOGGER_LEVEL;
		}

	}
}

#endif //PTM_ABSTRACT_LOGGER_H
