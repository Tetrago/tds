#include <iostream>

#include "interval_heap.hpp"

int main()
{
	IntervalHeap<int> heap;
	heap.insert(3);
	heap.insert(1);
	heap.insert(18);
	heap.insert(2);
	heap.insert(5);
	heap.insert(104);

	std::cout << heap.front() << std::endl;
	std::cout << heap.back() << std::endl;

	heap.pop_front();

	std::cout << heap.front() << std::endl;
	std::cout << heap.back() << std::endl;

	heap.pop_back();

	std::cout << heap.front() << std::endl;
	std::cout << heap.back() << std::endl;

	return 0;
}
