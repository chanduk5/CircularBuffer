#pragma once

#include <stdexcept>
#include<memory>

namespace circular_buffer {
	template <class T>  class circular_buffer {
	private:
		std::unique_ptr<T[]> buffer;
		size_t head = 0;
		size_t tail = 0;
		size_t maxSize;
		bool boundaryWrite = false;

		bool circularEmpty() {
			return (head == tail);
		}

		bool circularFull() {
			size_t value = ((head - 1) % maxSize);
			return (tail == value);
				
		}

	public:
		circular_buffer<T>(size_t provideSize) : buffer(std::unique_ptr<T[]>(new T[provideSize])), maxSize(provideSize) 
		{
			static T empty;
			for (size_t it = 0; it < maxSize; it++) {
				buffer[it] = empty;
			}
		};

		T read() {
			if ((circularEmpty()) && (boundaryWrite != true)) {
				throw std::domain_error("buffer is empty");
			}

			T ele = buffer[head];
			static T empty;
			buffer[head] = empty;
			if ((maxSize - head - 1) == 0){
				boundaryWrite = false;
			}
			head = (head + 1) % maxSize;
			if ((boundaryWrite == true) && (tail < head)) {
				boundaryWrite = false;
			}

			return ele;
		}

		void write(T element) {
			if (circularFull() && (boundaryWrite == true)) {
				throw std::domain_error("buffer is full");
			}

			buffer[tail] = element;
			if ((maxSize - tail - 1) == 0) {
				boundaryWrite = true;
			}
			tail = (tail + 1) % maxSize;
		}

		void clear() {
			static T empty;
			//delete buffer.get();

			for (int it = 0; it < maxSize; it++) {
				buffer[it] = empty;
			}
			
			head = 0;
			tail = 0;
			boundaryWrite = false;
		}

		void overwrite(T element) {
			static T empty;
			if (buffer[tail] != empty) {
				head = (head + 1) % maxSize;
			}
			buffer[tail] = element;
			if ((maxSize - tail - 1) == 0) {
				boundaryWrite = true;
			}
			tail = (tail + 1) % maxSize;

			if (buffer[tail] != empty) {
				boundaryWrite = true;
			}
		}
	};
}