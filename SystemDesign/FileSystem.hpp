//
// Created by 韦晓枫 on 2022/8/14.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_FILESYSTEM_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_FILESYSTEM_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <functional>
#include <deque>
#include <iostream>
#include <optional>

namespace SystemDesign::FileSystem {

    using Path = std::deque<std::string>;
    class FileSystemEntry;
    enum class INodeType {
        FILE, DIRECTORY
    };
    struct INode {
        INode( ) : type(INodeType::DIRECTORY), fileSystem() { }
        INodeType type;
        std::shared_ptr<FileSystemEntry> fileSystem;
    };
    using Entry = std::shared_ptr<INode>;

    Path parsePathString(const std::string &path) {
        Path segments;
        std::string buffer;
        auto enqueue = [&segments](std::string &segment) -> void {
            if (!segment.empty()) {
                segments.push_back(segment);
                segment.clear();
            }
        };
        for (const auto &c : path) {
            if (c == '/') {
                enqueue(buffer);
            } else {
                buffer += c;
            }
        }
        enqueue(buffer);
        return segments;
    }

    class FileSystemEntry {
    public:
        std::map<std::string, Entry> entries;

        std::string content;

        std::vector<std::string> getEntryFileNames() {
            std::vector<std::string> fileNames;
            for (auto &entry : entries) {
                fileNames.emplace_back(entry.first);
            }
            return fileNames;
        }

        Entry enter(const std::string &pathName) {
            return entries[pathName];
        }

        Entry createDirectoryIfNonExist(const std::string &pathName) {
            Entry nextHop = entries[pathName];
            if (nextHop) {
                return nextHop;
            }

            entries[pathName] = std::make_shared<INode>();
            nextHop = entries[pathName];
            nextHop->type = INodeType::DIRECTORY;
            nextHop->fileSystem = std::make_shared<FileSystemEntry>();
            return nextHop;
        }
    };

    class Explorer {
    public:
        Explorer( ) : root(std::make_shared<INode>()) {
            root->type = INodeType::DIRECTORY;
            root->fileSystem = std::make_shared<FileSystemEntry>();
        }

        std::optional<std::vector<std::string>> ls(const std::string &path) {
            Path segments = parsePathString(path);
            Entry currentWorkingDirectory = goTo(root, segments);
            if (!currentWorkingDirectory) {
                std::cerr << "path not exist: " << path << std::endl;
                return {};
            }

            INodeType inodeType = currentWorkingDirectory->type;
            if (inodeType == INodeType::FILE) {
                std::vector<std::string> entryNames;
                entryNames.push_back(segments[segments.size()-1]);
                return entryNames;
            } else if (inodeType == INodeType::DIRECTORY) {
                return currentWorkingDirectory->fileSystem->getEntryFileNames();
            } else {
                std::cerr << "unknown inode type " << static_cast<uint64_t>(inodeType) << ": " << path << std::endl;
                return {};
            }
        }

        void mkdir(const std::string &path) {
            createDirectory(path);
        }

        void addContentToFile(const std::string &filePath, const std::string &content) {
            Entry entry = createDirectory(filePath);
            entry->type = INodeType::FILE;
            entry->fileSystem->content += content;
        }

        std::optional<std::string> readContentFromFile(const std::string &filePath) {
            Path segments = parsePathString(filePath);
            if (Entry entry = goTo(root, segments)) {
                if (entry->type == INodeType::FILE) {
                    return entry->fileSystem->content;
                }
            }

            return {};
        }

    private:
        Entry root;

        static Entry goTo(const Entry &from, const Path &segments) {
            Entry currentWorkingDirectory = from;
            for (const std::string &segment : segments) {
                if (currentWorkingDirectory->type == INodeType::DIRECTORY) {
                    currentWorkingDirectory = currentWorkingDirectory->fileSystem->enter(segment);
                    if (currentWorkingDirectory) {
                        continue;
                    }
                }

                return nullptr;
            }

            return currentWorkingDirectory;
        }

        Entry createDirectory(const std::string &path) {
            Path segments = parsePathString(path);
            Entry currentWorkingDirectory = root;
            for (const std::string &segment : segments) {
                currentWorkingDirectory = currentWorkingDirectory->fileSystem->createDirectoryIfNonExist(segment);
            }
            return currentWorkingDirectory;
        }
    };

    namespace LeetCode {
        using std::vector;
        using std::string;

        class FileSystem {
        public:
            FileSystem() = default;

            vector<string> ls(string path) {
                vector<string> emptyList;
                return explorer.ls(path).value_or(emptyList);
            }

            void mkdir(string path) {
                explorer.mkdir(path);
            }

            void addContentToFile(string filePath, string content) {
                explorer.addContentToFile(filePath, content);
            }

            string readContentFromFile(string filePath) {
                string emptyString;
                return explorer.readContentFromFile(filePath).value_or(emptyString);
            }

        private:
            Explorer explorer;
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_FILESYSTEM_HPP
