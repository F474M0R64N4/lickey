#include "stdafx.h"
#include "FileUtility.h"

#include <fstream>
#include <Windows.h>


namespace lickey {
	auto get_extension(const std::string& filePath) -> std::string
	{
	  const auto pos = filePath.find_last_of('.');

    if (std::string::npos == pos) {
      return "";
    }

    return filePath.substr(pos, filePath.size() - pos);
  }


	auto get_base_file_path(const std::string& filePath) -> std::string
	{
	  const auto pos = filePath.find_last_of('.');

    if (std::string::npos == pos) {
      return filePath;
    }

    return filePath.substr(0, pos);
  }


	auto get_folder_path(const std::string& filePath) -> std::string
	{
	  const auto pos = filePath.find_last_of('\\');

    if (std::string::npos == pos) {
      return "";
    }

    return filePath.substr(0, pos);
  }


	auto get_filename(const std::string& filePath) -> std::string
	{
	  const auto pos = filePath.find_last_of('\\');

    if (std::string::npos == pos) {
      return filePath;
    }

    return filePath.substr(pos + 1, filePath.size() - pos - 1);
  }


	auto get_exe_file_path() -> std::string
	{
    static const auto buf_size = 2048;
    char path[buf_size];
    const auto status = GetModuleFileName(nullptr, path, buf_size);
    assert(0 != status);
    return path;
  }


	auto get_exe_folder_path() -> std::string
	{
    return get_folder_path(get_exe_file_path());
  }


	auto give_postfix(const std::string& filepath, const std::string& postfix) -> std::string
	{
	  const auto pos = filepath.find_last_of('.');
	  auto ans = std::string::npos == pos
		             ? filepath + "_" + postfix
		             : filepath.substr(0, pos) + "_" + postfix + filepath.substr(pos, filepath.size() - pos);
    return ans;
  }


	auto change_extension(const std::string& filepath, const std::string& new_ext) -> std::string
	{
	  const auto pos = filepath.find_last_of('.');

    if (std::string::npos == pos) {
      return filepath + "." + new_ext;
    }

    return filepath.substr(0, pos) + "." + new_ext;
  }


	auto join_path(const std::string& folderpath, const std::string& filepath) -> std::string
	{
    if (folderpath.empty()) {
      return filepath;
    }

    if ('\\' == folderpath.back()) {
      return folderpath + filepath;
    }

    return folderpath + "\\" + filepath;
  }


	auto read_lines(const std::string& filepath, std::vector<std::string>& lines) -> bool
	{
    std::ifstream in(filepath.c_str());

    if (in)
    {
	    while (!in.eof())
	    {
		    std::string line;
		    getline(in, line);
		    lines.push_back(line);
	    }

	    in.close();
	    return true;
    }
    return false;
  }
}
