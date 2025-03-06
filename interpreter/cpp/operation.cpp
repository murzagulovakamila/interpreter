#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <any>
#include <variant>

#include "visitor.hpp"

std::string Analyzer::any_to_string(const std::any& el) {
        try {
            return std::to_string(std::any_cast<int>(el));
        }
        catch (...) {
            try {
                return std::to_string(std::any_cast<float>(el));
            }
            catch (...) {
                try {
                    return std::to_string(std::any_cast<bool>(el));
                }
                catch (...) {
                    try {
                        return std::any_cast<std::string>(el);
                    }
                    catch (...) {
                        return "unknown";
                    }
                }
            }
        }
    }
    
const std::unordered_map<std::string, std::function<std::any(const std::any&)>> Analyzer::unary_op = {
    std::make_pair("+", [](const std::any& x) -> std::any {
        return x;
    })
};


const std::unordered_map<std::string, std::function<std::any(const std::any&, const std::any&)>> Analyzer::binary_op = {
    {"+", [](const std::any& x, const std::any& y) -> std::any {
        try {
            auto left = std::any_cast<int>(x);
            auto right = std::any_cast<int>(y);
            return left + right;
        } catch (...) {
            try {
                auto left = std::any_cast<float>(x);
                auto right = std::any_cast<float>(y);
                return left + right;
            } catch (...) {
                try {
                    auto left = std::any_cast<std::string>(x);
                    auto right = std::any_cast<std::string>(y);
                    return left + right;
                } catch (...) {
                    std::cout << "No such operand ";
                }
            }
        }
        return std::any();
    }},

    {"-", [](const std::any& x, const std::any& y) -> std::any {
        try {
            auto left = std::any_cast<int>(x);
            auto right = std::any_cast<int>(y);
            return left - right;
        } catch (...) {
            try {
                auto left = std::any_cast<float>(x);
                auto right = std::any_cast<float>(y);
                return left - right;
            } catch (...) {
                try {
                    auto left = std::any_cast<bool>(x);
                    auto right = std::any_cast<bool>(y);
                    return left - right;
                } catch (...) {
                    std::cout << "No such operand ";
                }
            }
        }
        return std::any();
    }},


    {"==", [](const std::any& x, const std::any& y) -> std::any {
        try {
            auto left = std::any_cast<int>(x);
            auto right = std::any_cast<int>(y);
            return left == right;
        } catch (...) {
            try {
               auto left = std::any_cast<float>(x);
                auto right = std::any_cast<float>(y);
                return left == right;
            } catch (...) {
                try {
                    auto left = std::any_cast<std::string>(x);
                    auto right = std::any_cast<std::string>(y);
                    return left == right;
                } catch (...) {
                    try {
                        auto left = std::any_cast<bool>(x);
                        auto right = std::any_cast<bool>(y);
                        return left == right;
                    } catch (...) {
                        std::cout << "No such operand ";
                    }
                }
            }
        }
        return std::any();
    }},
    
  {"=", [](const std::any&, const std::any& y) -> std::any {
        return y;
    }},
};
