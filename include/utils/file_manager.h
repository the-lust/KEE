#pragma once

#include "common_includes.h"
#include <filesystem>

namespace FileManager
{
    inline std::string join(const std::string& p1, const std::string& p2)
    {
        std::filesystem::path path1(p1);
        std::filesystem::path path2(p2);
        return (path1 / path2).string();
    }

    inline bool load_json(const std::string& filename, nlohmann::json& out_json)
    {
        if (!std::filesystem::exists(filename))
            return false;

        try
        {
            std::ifstream file(filename);
            if (!file.is_open())
                return false;

            file >> out_json;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    inline bool load_json(const std::string& filename, fifo_json& out_json)
    {
        if (!std::filesystem::exists(filename))
            return false;

        try
        {
            std::ifstream file(filename);
            if (!file.is_open())
                return false;

            file >> out_json;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    inline bool save_json(const std::string& filename, const nlohmann::json& in_json)
    {
        try
        {

            std::filesystem::path p(filename);
            if (p.has_parent_path())
            {
                std::filesystem::create_directories(p.parent_path());
            }

            std::ofstream file(filename);
            if (!file.is_open())
                return false;

            file << in_json.dump(4);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    inline bool save_json(const std::string& filename, const fifo_json& in_json)
    {
        try
        {

            std::filesystem::path p(filename);
            if (p.has_parent_path())
            {
                std::filesystem::create_directories(p.parent_path());
            }

            std::ofstream file(filename);
            if (!file.is_open())
                return false;

            file << in_json.dump(4);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    inline std::string clean_path(const std::string& p)
    {
        try {
            return std::filesystem::weakly_canonical(
                       std::filesystem::path(p)).string();
        } catch (...) {
            return p;
        }
    }

    inline bool exists(const std::string& p)
    {
        return std::filesystem::exists(p);
    }

    inline uint64_t file_size(const std::string& p)
    {
        std::error_code ec;
        auto sz = std::filesystem::file_size(p, ec);
        return ec ? 0 : static_cast<uint64_t>(sz);
    }

    inline std::ifstream open_read(const std::string& p)
    {
        return std::ifstream(p, std::ios::binary);
    }

    inline std::ofstream open_write(const std::string& p)
    {
        std::filesystem::path pp(p);
        if (pp.has_parent_path())
            std::filesystem::create_directories(pp.parent_path());
        return std::ofstream(p, std::ios::binary | std::ios::trunc);
    }

    inline bool create_directories(const std::string& p)
    {
        std::error_code ec;
        std::filesystem::create_directories(std::filesystem::path(p).parent_path(), ec);
        return !ec;
    }

    inline std::vector<std::string> list_files(const std::string& p, bool recursive = false)
    {
        std::vector<std::string> result;
        if (!std::filesystem::exists(p)) return result;

        if (recursive)
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(p))
            {
                if (entry.is_regular_file())
                    result.push_back(entry.path().string());
            }
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(p))
            {
                if (entry.is_regular_file())
                    result.push_back(entry.path().string());
            }
        }
        return result;
    }

    inline bool delete_file(const std::string& p)
    {
        std::error_code ec;
        return std::filesystem::remove(p, ec);
    }

    inline bool is_file(const std::string& p)
    {
        std::error_code ec;
        return std::filesystem::is_regular_file(p, ec);
    }

    inline std::string canonical_path(const std::string& p)
    {
        try {
            return std::filesystem::canonical(std::filesystem::path(p)).string();
        } catch (...) {
            return p;
        }
    }
}
