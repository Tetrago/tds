#ifndef INTERVAL_HEAP_HPP
#define INTERVAL_HEAP_HPP

#include <functional>
#include <utility>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class IntervalHeap
{
public:
	void insert(T&& item) noexcept;
	T pop_front() noexcept;
	T pop_back() noexcept;

	[[nodiscard]] bool empty() const noexcept { return data.empty(); }

	[[nodiscard]] T& front() noexcept { return data[0]; }

	[[nodiscard]] const T& front() const noexcept { return data[0]; }

	[[nodiscard]] T& back() noexcept
	{
		return data[std::min(1, data.size() - 1)];
	}

	[[nodiscard]] const T& back() const noexcept
	{
		return data[std::min(1, data.size() - 1)];
	}

private:
	std::vector<T> data;
};

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::insert(T&& item) noexcept
{
	Compare cmp;

	int index = data.size();
	data.insert(std::move(item));

	if ((index & 1) && cmp(data[index], data[index - 1]))
	{
		std::swap(data[index], data[index - 1]);
		index -= 1;
	}

	while (index > 1 && (((index & 1) && cmp(data[index / 2], data[index])) ||
	                     ((index ^ 1) && cmp(data[index], data[index / 2]))))
	{
		std::swap(data[index / 2], data[index]);
		index /= 2;
	}
}

template <typename T, typename Compare>
inline T IntervalHeap<T, Compare>::pop_front() noexcept
{
	if (data.size() == 1)
	{
		T item = std::move(data[0]);
		data.clear();
		return item;
	}
	else if (data.size() == 2)
	{
		T item = std::exchange(data[0], data[1]);
		data.resize(1);
		return item;
	}
	else
	{
		Compare cmp;

		T item = std::exchange(data[0], data[(data.size() - 1) & ~1]);
		data.pop_back();

		// TODO:

		return item;
	}
}

template <typename T, typename Compare>
inline T IntervalHeap<T, Compare>::pop_back() noexcept
{
	if (data.size() <= 1)
	{
		T item = std::move(data.back());
		data.pop_back();
		return item;
	}
	else
	{
		// TODO:
	}
}

#endif
