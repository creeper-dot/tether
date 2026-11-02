// tether.util:error 的实现
module;

#include <cstring>
#include <string>

module tether.util;

namespace tether {

std::string Error::message() const {
    // TODO: 按 code 拼一句可读的话；sys != 0 时附上 strerror(sys)，detail 非空时也附上
    //       Windows 上 sys 是 GetLastError()，要用 FormatMessageA 转成文字，不能用 strerror
    return {};
}

}  // namespace tether
