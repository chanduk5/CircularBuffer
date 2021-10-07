#include "circularBuffer.hpp"
#include <iostream>

namespace circular_buffer {
	
	
}

void circular_test() {
	circular_buffer::circular_buffer<int> buffer(1);

	try {
		int ele = buffer.read();
		std::cout << ele << std::endl;
	}
	catch (...)
	{
		std::cout << "read error" << std::endl;
	}

	
}