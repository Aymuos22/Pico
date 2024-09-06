// run.h
#pragma once
#include <string>
#include "value.h"
#include "error.h"

std::pair<Value, Error> run(const std::string& filename, const std::string& code);
