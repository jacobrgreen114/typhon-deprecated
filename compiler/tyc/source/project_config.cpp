// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "project_config.hpp"

std::unique_ptr<ProjectConfig> ProjectConfig::load(const fs::path& dir_path) {
  auto project_file_paths = std::vector<fs::path>{};

  for (auto& entry : fs::directory_iterator{dir_path}) {
    if (entry.is_regular_file() && entry.path().extension() == project_file_ext) {
      project_file_paths.emplace_back(entry.path());
    }
  }

  if (project_file_paths.empty()) {
    std::cerr << "No project file found!" << std::endl;
    std::exit(-1);
  } else if (project_file_paths.size() > 1) {
    std::cerr << "Only one project file allowed!" << std::endl;
    std::exit(-1);
  }

  // todo : implement project file parsing

  return std::make_unique<ProjectConfig>();
}
