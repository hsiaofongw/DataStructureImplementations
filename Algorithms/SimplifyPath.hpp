//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SIMPLIFYPATH_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SIMPLIFYPATH_HPP

#include <deque>
#include <string>
#include <vector>

namespace Algorithm::SimplifyPath {

    struct TestCase {
        std::string path;
        std::string normalizedPath;
    };

    std::vector<TestCase> getTestCases() {
        return {
            TestCase {
                .path = "/..hidden",
                .normalizedPath = "/..hidden"
            },
            TestCase {
                .path = "/home/",
                .normalizedPath = "/home"
            },
            TestCase {
                .path = "/home//foo/",
                .normalizedPath = "/home/foo"
            },
            TestCase {
                .path = "/../",
                .normalizedPath = "/"
            },
            TestCase {
                .path = "/.",
                .normalizedPath = "/"
            },
            TestCase {
                .path = "/as.",
                .normalizedPath = "/as."
            },
            TestCase {
                .path = "/as...d/.../db",
                .normalizedPath = "/as...d/.../db"
            },
            TestCase {
                .path = "/as...d/.../db/",
                .normalizedPath = "/as...d/.../db"
            }
        };
    }

    class Solution {
    public:
        std::string simplifyPath(std::string path) {
            size_t N = path.size();
            size_t i = 0;
            std::string fileName;
            while (i < N) {
                if (path.substr(i, 3) == "../" || (path.substr(i, 2) == ".." && N-i == 2)) {
                    pathManipulator.goBack();
                    fileName.clear();
                    i += 3;
                } else if (path.substr(i, 2) == "./" || (path.substr(i, 1) == "." && N-i == 1)) {
                    fileName.clear();
                    i += 2;
                } else if (path.substr(i, 2) == "//") {
                    pathManipulator.enter(fileName);
                    fileName.clear();
                    i += 2;
                } else if (path.substr(i, 1) == "/") {
                    pathManipulator.enter(fileName);
                    fileName.clear();
                    ++i;
                } else {
                    size_t i0 = i;
                    while (i < N && path.substr(i, 1) != "/")
                        ++i;
                    fileName += path.substr(i0, i-i0);
                }
            }
            pathManipulator.enter(fileName);

            std::string fullPath;
            std::deque<std::string> &pathQueue = pathManipulator.getPathQueue();
            if (pathQueue.empty())
                return "/";

            while (!pathQueue.empty()) {
                fullPath += "/";
                fullPath += pathQueue.front();
                pathQueue.pop_front();
            }

            return fullPath;
        }

    private:
        class PathManipulator {
        public:
            void goBack() {
                if (!path.empty())
                    path.pop_back();
            }

            void enter(const std::string &pathName) {
                if (!pathName.empty())
                    path.push_back(pathName);
            }

            std::deque<std::string> &getPathQueue() {
                return path;
            }

        private:
            std::deque<std::string> path;
        };

        PathManipulator pathManipulator;
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SIMPLIFYPATH_HPP
