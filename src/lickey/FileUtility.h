#ifndef FILEUTILITY_H
#define FILEUTILITY_H

namespace lickey
{
	auto get_extension(const std::string& filepath) -> std::string;
	auto get_base_file_path(const std::string& filepath) -> std::string;
	//auto get_filename(const std::string& filepath) -> std::string;
	auto get_folder_path(const std::string& filepath) -> std::string;
	auto get_exe_file_path() -> std::string;
	//auto get_exe_folder_path() -> std::string;
	//auto give_postfix(const std::string& filepath, const std::string& postfix) -> std::string;
	//auto change_extension(const std::string& filepath, const std::string& new_ext) -> std::string;
	//auto join_path(const std::string& folderpath, const std::string& filepath) -> std::string;
	auto read_lines(const std::string& filepath, std::vector<std::string>& lines) -> bool;
}
#endif // FILEUTILITY_H
