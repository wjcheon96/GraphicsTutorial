#ifndef __COMMON_HPP__
#define __COMMON_HPP__


#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

std::optional<std::string> LoadTextFile(const std::string& filename);

#define CLASS_PTR(className) \
class className; \
using className ## UPtr = std::unique_ptr<className>; \
using className ## Ptr = std::shared_ptr<className>; \
using className ## WPtr = std::weak_ptr<className>;

#endif
