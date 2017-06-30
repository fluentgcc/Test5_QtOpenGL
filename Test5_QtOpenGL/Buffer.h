#pragma once
#include "Object.h"

#include <opencv2\core.hpp>


namespace Graphics
{
	class Buffer : public Object
	{
	public:
		Buffer();
		virtual ~Buffer();
	};
}