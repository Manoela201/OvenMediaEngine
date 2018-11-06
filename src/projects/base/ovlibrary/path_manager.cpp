#include <utility>

#include <unistd.h>
#include <errno.h>
#include <wordexp.h>

#include "path_manager.h"

#include "memory_utilities.h"

namespace ov
{
	String PathManager::GetAppPath(String sub_path)
	{
		char buffer[4096] = { 0 };

#ifndef __CYGWIN__
		if(readlink("/proc/self/exe", buffer, OV_COUNTOF(buffer)) != -1)
		{
			return Combine(Combine(ExtractPath(buffer), std::move(sub_path)), "");
		}
#endif

		return "";
	}

	String PathManager::ExpandPath(String path)
	{
		wordexp_t exp_result;

		if(wordexp(path, &exp_result, 0) == 0)
		{
			path = exp_result.we_wordv[0];
		}

		wordfree(&exp_result);

		return path;
	}

	String PathManager::ExtractPath(String path)
	{
		off_t position = path.IndexOfRev('/');

		if(position >= 0)
		{
			path = path.Left(static_cast<size_t>(position + 1));
		}

		return path;
	}

	bool PathManager::MakeDirectory(String path, int mask)
	{
		if(path.GetLength() == 0)
		{
			return false;
		}

		return (mkdir(path.CStr(), static_cast<__mode_t>(mask)) == 0) || (errno == EEXIST);
	}

	String PathManager::Combine(String path1, String path2)
	{
		if(path1.Right(1) != "/")
		{
			path1.Append("/");
		}

		path1.Append(path2);

		return path1;
	}
}