#pragma once
#define _HAS_CXX17 1
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

static const std::string ARG_INT_RESOLUTION = "-rs";
static const std::string ARG_STR_INPUT = "-i";
static const std::string ARG_INT_BINARY_IMAGE = "-ib";
static const std::string ARG_INT_ROTATE_IMAGE = "-r";
static const std::string ARG_STR_OUTPUT = "-o";
static const std::string ARG_BOOL_NEGATIVE = "-n";
static const std::string ARG_BOOL_IMAGE_DILATION = "-d";
static const std::string ARG_BOOL_IMAGE_EROSION = "-e"; 
static const std::string ARG_BOOL_GRADIENT_IMAGE = "-g";
static const std::string ARG_BOOL_REDUCE_NOISE = "-dn";
static const std::string ARG_BOOL_APPLY_BLUR = "-b";
static const std::string ARG_BOOL_DISPLAY_HELP = "-h";

struct Args {   
	std::vector<std::string>& instructions;

	void parse(int argc, char* argv[]);
	bool validate() const;

	Args();
	Args(std::vector<std::string>& instr);
};
