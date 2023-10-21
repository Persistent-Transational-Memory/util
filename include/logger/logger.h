/*
 * @author: BL-GS
 * @date:   2022/11/23
 */

#ifndef ALGORITHM_LOG_H
#define ALGORITHM_LOG_H

#include <logger/abstract_logger.h>

#include <logger/console_logger.h>
#include <logger/file_logger.h>

namespace util {

	inline namespace logger {

		/*!
		 * @brief Global configuration of logger
		 */

		#ifndef LOGGER_OUTPUT
			#define LOGGER_OUTPUT CONSOLE
		#endif
		#define LOGGER_TYPE_ENUM util::logger::Output_Type::LOGGER_OUTPUT
		#define LOGGER_TYPE util::logger::Logger<LOGGER_TYPE_ENUM, true>

		using GlobalLoggerType = LOGGER_TYPE;

		inline GlobalLoggerType &get_global_logger() {
			return GlobalLoggerType::get_instance();
		}

		template<class ...Args>
		inline void logger_info(Args &&...args) {
			auto &logger = get_global_logger();
			logger.info(std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_warn(Args &&...args) {
			auto &logger = get_global_logger();
			logger.warn(std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_error(Args &&...args) {
			auto &logger = get_global_logger();
			logger.error(std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_info_format(const char *fmt, Args &&...args) {
			auto &logger = get_global_logger();
			logger.info_format(fmt, std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_warn_format(const char *fmt, Args &&...args) {
			auto &logger = get_global_logger();
			logger.warn_format(fmt, std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_error_format(const char *fmt, Args &&...args) {
			auto &logger = get_global_logger();
			logger.error_format(fmt, std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_print_property(std::string_view header_name, Args &&...args) {
			auto &logger = get_global_logger();
			logger.print_property(header_name, std::forward<Args>(args)...);
		}

		template<class ...Args>
		inline void logger_exception(Args &&...args) {
			auto &logger = get_global_logger();
			logger.error(std::forward<Args>(args)...);
			exit(-1);
		}
	}
}

#endif //ALGORITHM_LOG_H
