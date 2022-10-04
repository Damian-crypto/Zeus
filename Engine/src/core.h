#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <thread>
#include <functional>
#include <array>
#include <fstream>
#include <sstream>
#include <string_view>
#include <algorithm>

#define GLValidate(x) x; if (GLenum error; (error = glGetError()) != GL_NO_ERROR) __debugbreak();
