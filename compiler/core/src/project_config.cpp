// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "project_config.hpp"

#include "../../../external/RapidXML/RapidXML/rapidxml.hpp"

const auto binary_type_map = std::unordered_map<std::string_view, BinaryType>{
    {"Exe", BinaryType::Exe},
    {"Lib", BinaryType::Lib},
    {"Dyn", BinaryType::Dyn}
};

auto get_binary_type(const std::string_view view) -> BinaryType {
  if (auto it = binary_type_map.find(view); it != binary_type_map.end()) {
    return it->second;
  }

  std::cerr << "Error : Unknown binary type \"" << view << '"' << std::endl;
  exit(-1);
}

using xml_document = rapidxml::xml_document<char>;
using xml_node     = rapidxml::xml_node<char>;
using xml_attr     = rapidxml::xml_attribute<char>;

auto find_project_file(const fs::path& dir_path) -> fs::path {
  auto project_file_paths = std::vector<fs::path>{};

  {
    auto it = fs::directory_iterator{dir_path};
    std::copy_if(
        begin(it), end(it), std::back_inserter(project_file_paths), [](auto entry) -> bool {
          return entry.is_regular_file() && entry.path().extension() == project_file_ext;
        });
  }

  if (project_file_paths.empty()) {
    std::cerr << "No project file found!" << std::endl;
    std::exit(-1);
  } else if (project_file_paths.size() > 1) {
    std::cerr << "Only one project file allowed!" << std::endl;
    std::exit(-1);
  }

  return project_file_paths.front();
}

auto read_all(const fs::path& path) -> std::string {
  auto buffer = std::stringstream{};
  auto file   = std::ifstream{path};
  buffer << file.rdbuf();
  return buffer.str();
}

using project_node_handler = void (*)(ProjectConfig& config, const xml_node& node);

auto project_name_handler(ProjectConfig& config, const xml_node& node) -> void {
  auto name = std::string_view{node.value(), node.value_size()};
  if (name.empty()) {
    std::cerr << "Error : Empty \"ProjectName\" not allowed." << std::endl;
    exit(-1);
  }

  config.set_name(name);
}

auto project_source_dir_handler(ProjectConfig& config, const xml_node& node) -> void {
  auto name = std::string_view{node.value(), node.value_size()};
  if (name.empty()) {
    std::cerr << "Error : Empty \"SourceDir\" not allowed." << std::endl;
    exit(-1);
  }
  config.set_source_dir(name);
}

auto project_build_dir_handler(ProjectConfig& config, const xml_node& node) -> void {
  auto name = std::string_view{node.value(), node.value_size()};
  if (name.empty()) {
    std::cerr << "Error : Empty \"BuildDir\" not allowed." << std::endl;
    exit(-1);
  }
  config.set_build_dir(name);
}

auto project_binary_dir_handler(ProjectConfig& config, const xml_node& node) -> void {
  auto name = std::string_view{node.value(), node.value_size()};
  if (name.empty()) {
    std::cerr << "Error : Empty \"BinaryDir\" not allowed." << std::endl;
    exit(-1);
  }
  config.set_binary_dir(name);
}

auto project_binary_type_handler(ProjectConfig& config, const xml_node& node) -> void {
  auto value    = std::string_view{node.value(), node.value_size()};
  auto bin_type = get_binary_type(value);
  config.set_binary_type(bin_type);
}

constexpr auto true_string  = std::string_view{"true"};
constexpr auto false_string = std::string_view{"false"};

auto project_link_std_handler(ProjectConfig& config, const xml_node& node) -> void {
  auto value = std::string_view{node.value(), node.value_size()};
  auto lower = std::string{};
  for (auto c : value) {
    lower += tolower(c);
  }

  if (lower == true_string) {
    config.set_link_std(true);
  } else if (lower == false_string) {
    config.set_link_std(false);
  } else {
    std::cerr << "Error : Unknown NoStd value \"" << value << '"' << std::endl;
    exit(-1);
  }
}

auto project_configurations_handler(ProjectConfig& config, const xml_node& node) -> void {
  // todo : implement
}

auto project_references_handler(ProjectConfig& config, const xml_node& node) -> void {
  // todo : implement
}

const auto project_node_handlers = std::unordered_map<std::string_view, project_node_handler>{
    {"ProjectName",    project_name_handler          },
    {"BinaryType",     project_binary_type_handler   },
    {"LinkStd",        project_link_std_handler      },
    {"Configurations", project_configurations_handler},
    {"References",     project_references_handler    },
    {"SourceDir",      project_source_dir_handler    },
    {"BuildDir",       project_build_dir_handler     },
    {"BinaryDir",      project_binary_dir_handler    },
};

auto create_project_config(const xml_node& xml) -> ProjectConfig::ConstPointer {
  auto project = std::make_unique<ProjectConfig>();

  for (auto pnode = xml.first_node(); pnode; pnode = pnode->next_sibling()) {
    auto& node     = deref(pnode);
    auto node_name = std::string_view{node.name(), node.name_size()};

    if (auto it = project_node_handlers.find(node_name); it != project_node_handlers.end()) {
      auto handler = it->second;
      handler(*project, node);
    } else {
      std::cerr << "Error : Unknown project node \"" << node_name << '"' << std::endl;
      exit(-1);
    }
  }

  return project;
}

constexpr auto project_node_name = std::string_view{"Project"};

auto ProjectConfig::load(const fs::path& dir_path) -> ConstPointer {
  auto file_path = find_project_file(dir_path);
  auto buffer    = read_all(file_path);

  auto doc       = xml_document{};
  doc.parse<0>(buffer.data());

  auto project_node = doc.first_node(project_node_name.data(), project_node_name.size());
  if (!project_node) {
    std::cerr << "Error : Project file missing root \"Project\" xml node." << std::endl;
    exit(-1);
  }

  return create_project_config(deref(project_node));
}
