//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_STRINGS_H
#define COCCOC_STRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

#include "../include/Utils.h"
#include "../include/StringBuilder.h"


class Strings {
public:
    static bool isCapitalLetter(const char c) { return c >= 'A' && c <= 'Z'; }
    static bool isLowerCaseLetter(const char c) { return c >= 'a' && c <= 'z'; }
    static std::string toUpper(std::string str) {
        char c[str.length()];
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                c[i] = str[i] - 32;
            } else {
                c[i] = str[i];
            }
        }
        return std::string(c, str.length());
    }

    static std::string toLower(std::string str) {
        char c[str.length()];
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') {
                c[i] = str[i] + 32;
            } else {
                c[i] = str[i];
            }
        }
        return std::string(c, str.length());
    }

    static bool isLetterOrDigitOrUnderScore(const char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               (c >= '0' && c <= '9') ||
               (c == '_');
    }

    static std::string strip(std::string str) {
        if (str.length() <= 1) {
            return str;
        }
        unsigned int i, j;
        for (i = 0; i < str.length(); i++) {
            if (str[i] != ' ' && str[i] != '\t') {
                break;
            }
        }
        for (j = str.length() - 1; j >= 0; j--) {
            if (str[j] != ' ' && str[j] != '\t') {
                break;
            }
        }
        return str.substr(i, j + 1 - i);
    }

    static std::string strip(std::string str, std::string match) {
        std::string str1 = strip(str);
        if (match.length() == 0) {
            return str1;
        }
        // strip head.
        int limit = Utils::min(str.length(), match.length());
        int i = 0;
        for (; i < limit; i++) {
            if (str[i] != match[i]) {
                break;
            }
        }

        int j = str.length() - 1, k = match.length() - 1;
        for (; j >= 0 && k >= 0; j--, k--) {
            if (str[j] != match[k]) {
                break;
            }
        }
        // printf("i = %d, j = %d\n", i, j);
        return str.substr(i, j + 1 - i);
    }

    static bool startWith(std::string &str, std::string match) {
        if (match.length() > str.length()) {
            return false;
        }
        for (unsigned int i = 0; i < match.length(); i++) {
            if (str[i] != match[i]) {
                return false;
            }
        }
        return true;
    }

    static bool endWith(std::string &str, std::string match) {
        if (match.length() > str.length()) {
            return false;
        }
        int strLength = str.length(), matchLength = match.length();
        for (unsigned int i = 0; i < match.length(); i++) {
            if (str[strLength - 1 - i] != match[matchLength - 1 - i]) {
                return false;
            }
        }
        return true;
    }

    static std::vector<std::string> split(const std::string &str, const char c) {
        std::vector<std::string> result;
        unsigned int start = 0;
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] == c) {
                result.push_back(str.substr(start, i - start));
                start = i + 1;
            }
        }
        result.push_back(str.substr(start, str.length() - start));
        return result;
    }

    static std::vector<std::string> split(const std::string &str, const std::string &match) {
        std::vector<std::string> result;
        if (match.length() == 0 || str.length() <= match.length()) {
            return result;
        }

        unsigned int start = 0;
        for (unsigned int i = 0; i <= str.length() - match.length(); i++) {
            if (str.substr(i, match.length()) == match) {
                result.push_back(str.substr(start, i - start));
                start = i + match.length();
            }
        }
        result.push_back(str.substr(start, str.length() - start));
        return result;
    }

    static std::vector<std::string> splitGreedy(const std::string &str, const char c) {
        std::vector<std::string> result;
        unsigned int start = 0;
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] == c) {
                std::string piece = str.substr(start, i - start);
                if (piece.length() > 0) {
                    result.push_back(piece);
                }
                start = i + 1;
            }
        }
        if (start < str.length()) {
            result.push_back(str.substr(start, str.length() - start));
        }
        return result;
    }

    static std::vector<std::string> splitGreedy(const std::string &str, const std::string &match) {
        std::vector<std::string> result;
        if (match.length() == 0 || str.length() <= match.length()) {
            return result;
        }

        unsigned int start = 0;
        for (unsigned int i = 0; i <= str.length() - match.length(); i++) {
            if (str.substr(i, match.length()) == match) {
                std::string piece = str.substr(start, i - start);
                if (piece.length() > 0) {
                    result.push_back(piece);
                }
                start = i + match.length();
            }
        }
        if (start < str.length()) {
            result.push_back(str.substr(start, str.length() - start));
        }
        return result;
    }

    static bool isSingleWord(const std::string &str) {
        return splitGreedy(str, ' ').size() == 0;
    }

    static std::string strCat(std::vector<std::string> v, unsigned int start) {
        std::string result = "";
        for (unsigned int i = start; i < v.size(); i++) {
            result += v[i];
        }
        return result;
    }

    static int findFirstMatch(std::string str, std::string match) {
        if (match.length() > str.length()) {
            return -1;
        }
        for (unsigned int i = 0; i <= str.length() - match.length(); i++) {
            if (str.substr(i, match.length()) == match) {
                return i;
            }
        }
        return -1;
    }

    static int findFirstMatch(std::string str, std::string match, int offset) {
        if (match.length() - offset > str.length()) {
            return -1;
        }
        for (unsigned int i = offset; i <= str.length() - match.length(); i++) {
            if (str.substr(i, match.length()) == match) {
                return i;
            }
        }
    }

    static int findLastMatch(std::string str, std::string match) {
        if (match.length() > str.length()) {
            return -1;
        }
        for (unsigned int i = str.length() - match.length(); i >= 0; i--) {
            if (str.substr(i, match.length()) == match) {
                return i;
            }
        }
        return -1;
    }

    static void replaceWith(std::string &str, const char old, const char rep) {
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] == old) {
                str[i] = rep;
            }
        }
    }

    static const std::string replaceWith(std::string &str, const std::string &old, const std::string &rep) {
        StringBuilder builder;
        for (unsigned int i = 0; i <= str.length() - old.length(); i++) {
            if (str.substr(i, old.length()) == old) {
                builder.append(rep);
            } else {
                builder.append(str[i]);
            }
        }
        return builder.toString();
    }

    // extract tokens from a string.
    static std::vector<std::string> extractTokens(
            std::string *str, char start, char end) {
        StringBuilder str_builder;
        std::vector<std::string> result;
        bool matching = false;
        int matching_start = 0;
        for (unsigned int i = 0; i < str->length(); i++) {
            if (!matching) {
                if ((*str)[i] == start) {
                    matching = true;
                    matching_start = i;
                } else {
                    str_builder.append((*str)[i]);
                }
            } else {
                if ((*str)[i] == end) {
                    result.push_back(str->substr(matching_start, i + 1 - matching_start));
                    matching = false;
                }
            }
        }
        *str = str_builder.toString();
        return result;
    }

};

#endif /* COCCOC_STRINGS_H */
