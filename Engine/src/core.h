#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <array>
#include <fstream>
#include <sstream>
#include <string_view>

#define GLValidate(x) x; if (GLenum error; (error = glGetError()) != GL_NO_ERROR) __debugbreak();
