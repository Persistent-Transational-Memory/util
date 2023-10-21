/*
 * @author: BL-GS 
 * @date:   2023/6/3
 */

#pragma once
#ifndef UTIL_MEM_ALLOCATOR_FILE_DESCRIPTOR_H
#define UTIL_MEM_ALLOCATOR_FILE_DESCRIPTOR_H

#include <cassert>
#include <filesystem>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <memory/memory_config.h>

inline namespace util_mem {

	struct FileDescriptor {
	public:
		/// The align size of start pointer
		static constexpr size_t ALIGN_SIZE = CACHE_LINE_SIZE;

	public:
		/// File descriptor
		int fd;
		/// The pathname of file
		std::filesystem::path file_path;
		/// The start pointer of mapped file
		uint8_t *start_ptr;
		/// The aligned start pointer of mapped file
		uint8_t *aligned_start_ptr;
		/// The total size of mapped area
		uint64_t total_size;
		/// The total size of aligned mapped area
		uint64_t aligned_total_size;

	public:
		FileDescriptor(std::string_view dir_name, std::string_view path, size_t alloc_size):
				fd(0), file_path(std::string(dir_name) + '/' + path.data()), start_ptr(nullptr), aligned_start_ptr(nullptr), total_size(alloc_size)  {
			// Create directories of the path
			if (!std::filesystem::exists(dir_name)) {
				std::filesystem::create_directories(dir_name);
			}

			// Open file and truncate the size of file
			fd = open(file_path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			int td = ftruncate(fd, alloc_size);
			if (fd < 0 || td < 0) {
				perror("Unable to create file");
				exit(-1);
			}

			// mmap() memory range
			// MAP_POPULATE avoid running-time page fault
			start_ptr = (uint8_t *)mmap(NULL, alloc_size, (PROT_READ | PROT_WRITE), MAP_SHARED | MAP_POPULATE, fd, 0);
			if (start_ptr == MAP_FAILED) {
				perror("ERROR: mmap() is not working !!! ");
				exit(-1);
			}

			aligned_start_ptr  = (uint8_t *)align_ptr(start_ptr);
			aligned_total_size = total_size - ((uint8_t *)aligned_start_ptr - (uint8_t *)start_ptr);
		}

		~FileDescriptor() {
			munmap(start_ptr, total_size);
			close(fd);
			std::filesystem::remove(file_path);
		}

	private:
		static void *align_ptr(void *ptr) {
			return reinterpret_cast<void *>(
					(reinterpret_cast<size_t>(ptr) + (ALIGN_SIZE - 1)) & (~(ALIGN_SIZE - 1))
			);
		}
	};

	/*!
	 * @brief Allocate a unique id for file
	 */
	std::string allocate_file_index() {
		static std::atomic<uint32_t> index_counter{0};
		uint32_t res = index_counter++;
		assert(res < std::numeric_limits<uint32_t>::max());
		return std::to_string(res);
	}

	/*!
	 * @brief Allocate a unique filename
	 */
	std::string allocate_file_name() {
		return std::string("Data_") + allocate_file_index();
	}

}

#endif //UTIL_MEM_ALLOCATOR_FILE_DESCRIPTOR_H
