#include <iostream>
#include "DotXsiLoader.h"
#include "ImageLoader.h"
#include "Arguments.h"
#include <Shared/Base/Serialize/OutBinarySerializer.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Base/Serialize/OutXmlSerializer.h>
#include <Shared/Base/Io/FileStream.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/BoneArray.h>
#include <Shared/Graphics/Renderer/OpenGL/Renderer.h>
#include <Shared/Geometry/OpenGL/ModelData.h>
#include "../Pipeline/Ps2/ExporterBackend.h"
#include "../Pipeline/OpenGL/ExporterBackend.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	zenic::Arguments args;	

	if (!args.init(argc, argv))
		return 0;

	if (!args.getArg("-o"))
	{
		zenic::logOut << "No output file specified";
		return 0;
	}

	if (!args.getArg("-s") && !args.getArg("-i") && !args.getArg("-obj"))
	{
		zenic::logOut << "No scene file, image or objectfile specified";
		return 0;
	}

	zenic::FileStream outStream;
	zenic::OutBinarySerializer outBinary;

	if (!outStream.open(args.getArg("-o"), zenic::FileStream::Write))
		return 0;

	zenic::DotXsiLoader loader;
	zenic::ps2::ExporterBackend backend;

	if (args.getArg("-s"))
	{
		if (!loader.load(args.getArg("-s")))
			return 0;

		loader.process(backend, outBinary, false);
	}

	if (args.getArg("-obj"))
	{
		if (!loader.load(args.getArg("-obj")))
			return 0;

		loader.process(backend, outBinary, true);
	}

	if (args.getArg("-i"))
	{
		zenic::ImageLoader imageLoader;

		// Make sure we have a format

		if (args.getArg("-f"))
		{
			if (!imageLoader.load(args.getArg("-i")))
				return 0;

			if (!strcmp("8", args.getArg("-f")))
				imageLoader.process(outBinary, zenic::ps2::gs::T8);
			else if (!strcmp("4", args.getArg("-f")))
				imageLoader.process(outBinary, zenic::ps2::gs::T4);
			else if (!strcmp("32", args.getArg("-f")))
				imageLoader.process(outBinary, zenic::ps2::gs::CT32);
		}
	}

	outBinary.save(outStream);
	outStream.close();

	//getchar();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
