#pragma once
#include <memory>
#include "Lexer.h"

enum class State
{
	MULTILINE_COMMENT,
	DEFAULT_STATE,
};
