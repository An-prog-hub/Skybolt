/* Copyright 2012-2020 Matthew Reid
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */


#include "FileUtility.h"
#include <boost/filesystem.hpp>

namespace skybolt {
namespace file {

namespace fs = boost::filesystem;

static bool isDir(const Path &file)
{
	return fs::is_directory(file);
}

static bool isRegularFile(const Path &file)
{
	return fs::is_regular_file(file);
}

Paths findFilenamesInDirectory(const std::string &dir, const std::string &extension)
{
	Paths result;

	fs::path dirPath(dir);
	if (fs::exists(dirPath))
	{
		// Search for files within this directory
		fs::directory_iterator endIt;
		for (fs::directory_iterator it(dirPath); it != endIt; ++it)
		{
			fs::path file = it->path();

			if (isRegularFile(file))
			{
				if (file.extension() == extension)
				{
					result.push_back(file);
				}
			}
		}
	}
	return result;
}

static void findFilenamesInDirectoryRecursive(Paths& out, const fs::path &dirPath, const std::string &extension, int depth)
{
	fs::directory_iterator endIt;
	for (fs::directory_iterator it(dirPath); it != endIt; ++it)
	{
		fs::path file = it->path();

		if (depth > 0 && isDir(file))
			findFilenamesInDirectoryRecursive(out, file, extension, depth - 1);

		if (isRegularFile(file))
		{
			if (file.extension() == extension)
			{
				out.push_back(file);
			}
		}
	}
}

Paths findFilenamesInDirectoryRecursive(const std::string &dir, const std::string &extension, int depth)
{
	Paths result;

	fs::path dirPath(dir);
	if (fs::exists(dirPath))
	{
		findFilenamesInDirectoryRecursive(result, dirPath, extension, depth);
	}
	return result;
}

Paths findFoldersInDirectory(const std::string &dir)
{
	Paths result;

	fs::path dirPath(dir);
	if (fs::exists(dirPath))
	{
		fs::directory_iterator endIt;
		for (fs::directory_iterator it(dirPath); it != endIt; ++it)
		{
			fs::path file = it->path();

			if (isDir(file))
			{
				result.push_back(file);
			}
		}
	}
	
	return result;
}

} // namespace file
} // namespace skybolt