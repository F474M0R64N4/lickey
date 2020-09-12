#include "stdafx.h"
#include "FileUtility.h"

#include <fstream>
#include <Windows.h>


namespace lickey {
  std::string GetExtension(const std::string &filePath) {
	  const auto pos = filePath.find_last_of('.');

    if (std::string::npos == pos) {
      return "";
    }

    return filePath.substr(pos, filePath.size() - pos);
  }


  std::string GetBaseFilePath(const std::string &filePath) {
	  const auto pos = filePath.find_last_of('.');

    if (std::string::npos == pos) {
      return filePath;
    }

    return filePath.substr(0, pos);
  }


  std::string GetFolderPath(const std::string &filePath) {
	  const auto pos = filePath.find_last_of('\\');

    if (std::string::npos == pos) {
      return "";
    }

    return filePath.substr(0, pos);
  }


  std::string GetFilename(const std::string &filePath) {
	  const auto pos = filePath.find_last_of('\\');

    if (std::string::npos == pos) {
      return filePath;
    }

    return filePath.substr(pos + 1, filePath.size() - pos - 1);
  }


  std::string GetExeFilePath() {
    static const auto BUF_SIZE = 2048;
    char path[BUF_SIZE];
    const auto status = GetModuleFileName(nullptr, path, BUF_SIZE);
    assert(0 != status);
    return path;
  }


  std::string GetExeFolderPath() {
    return GetFolderPath(GetExeFilePath());
  }


  std::string GivePostfix(const std::string &filepath, const std::string &postfix) {
	  const auto pos = filepath.find_last_of('.');
	  auto ans = std::string::npos == pos
		             ? filepath + "_" + postfix
		             : filepath.substr(0, pos) + "_" + postfix + filepath.substr(pos, filepath.size() - pos);
    return ans;
  }


  std::string ChangeExtension(const std::string &filepath, const std::string &newExt) {
	  const auto pos = filepath.find_last_of('.');

    if (std::string::npos == pos) {
      return filepath + "." + newExt;
    }

    return filepath.substr(0, pos) + "." + newExt;
  }


  std::string JoinPath(const std::string &folderpath, const std::string &filepath) {
    if (folderpath.empty()) {
      return filepath;
    }

    if ('\\' == folderpath.back()) {
      return folderpath + filepath;
    }

    return folderpath + "\\" + filepath;
  }


  bool ReadLines(const std::string &filepath, std::vector<std::string> &lines) {
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
