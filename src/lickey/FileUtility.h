#pragma once

namespace lickey {
  /**
  * return base file path that does not include extension ("." is not included also).
  * if input file path does not include extension, return same value as input file path.
  * @param filePath [i] input file path to extract base file path
  * @param std::string [o] base file path that does not include extension
  */
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
