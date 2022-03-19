#include <stdexcept>
#include <string>
#include <sense/fb.hpp>

sense::bitmap loadImage(const std::string& filename) throw (std::exception);
sense::bitmap drawImage(const std::string& filename) throw (std::exception);