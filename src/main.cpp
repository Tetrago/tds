#include <iostream>

#include "interval_heap.hpp"
#include "tracker.hpp"

int main()
{
	auto [tracker, box] = Tracker<int>::make(3);

	IntervalHeap<Tracker<int>::Box> heap;
	heap.insert(std::move(box));

	std::cout << *heap.front() << std::endl;

	return 0;
}
