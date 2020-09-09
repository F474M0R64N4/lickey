#pragma once

namespace lickey {
  auto GetExtension(const std::string &filepath) -> std::string;
  auto GetBaseFilePath(const std::string &filepath) -> std::string;
  auto GetFilename(const std::string &filepath) -> std::string;
  auto GetFolderPath(const std::string &filepath) -> std::string;
  auto GetExeFilePath() -> std::string;
  auto GetExeFolderPath() -> std::string;
  auto GivePostfix(const std::string &filepath, const std::string &postfix) -> std::string;
  auto ChangeExtension(const std::string &filepath, const std::string &newExt) -> std::string; ///< newExt must be without "."
  auto JoinPath(const std::string &folderpath, const std::string &filepath) -> std::string;
  auto ReadLines(const std::string &filepath, std::vector<std::string> &lines) -> bool;
}
