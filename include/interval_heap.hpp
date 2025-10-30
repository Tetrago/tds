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
	void erase(const T& item) noexcept;

	[[nodiscard]] bool empty() const noexcept { return data_.empty(); }

	[[nodiscard]] T& front() noexcept { return data_[0]; }

	[[nodiscard]] const T& front() const noexcept { return data_[0]; }

	[[nodiscard]] T& back() noexcept
	{
		return data_[std::min(1, data_.size() - 1)];
	}

	[[nodiscard]] const T& back() const noexcept
	{
		return data_[std::min(1, data_.size() - 1)];
	}

private:
	std::vector<T> data_;
};

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::insert(T&& item) noexcept
{
	Compare cmp;

	int index = data_.size();
	data_.insert(std::move(item));

	if ((index & 1) && cmp(data_[index], data_[index - 1]))
	{
		std::swap(data_[index], data_[index - 1]);
		index -= 1;
	}

	while (index > 1 && (((index & 1) && cmp(data_[index / 2], data_[index])) ||
	                     ((index ^ 1) && cmp(data_[index], data_[index / 2]))))
	{
		std::swap(data_[index / 2], data_[index]);
		index /= 2;
	}
}

template <typename T, typename Compare>
inline T IntervalHeap<T, Compare>::pop_front() noexcept
{
	if (data_.size() == 1)
	{
		T item = std::move(data_[0]);
		data_.clear();
		return item;
	}
	else if (data_.size() == 2)
	{
		T item = std::exchange(data_[0], data_[1]);
		data_.resize(1);
		return item;
	}
	else
	{
		Compare cmp;

		if (data_.size() ^ 1)
		{
			std::swap(data_[data_.size() - 2], data_[data_.size() - 1]);
		}

		T item = std::exchange(data_[0], std::move(data_[data_.size() - 1]));
		data_.pop_back();

		for (int index = 0;;)
		{
			T& item = data_[index];
			index   = index * 2 + 1;

			if ((index * 2 < data_.size() && cmp(data_[index * 2], item)) ||
			    (index +=
			     1 && index * 2 < data_.size() && cmp(data_[index * 2], item)))
			{
				std::swap(data_[index * 2], item);
			}
			else
			{
				return item;
			}
		}
	}
}

template <typename T, typename Compare>
inline T IntervalHeap<T, Compare>::pop_back() noexcept
{
	if (data_.size() <= 1)
	{
		T item = std::move(data_.back());
		data_.pop_back();
		return item;
	}
	else
	{
		Compare cmp;

		T item = std::exchange(data_[1], std::move(data_.back()));
		data_.pop_back();

		for (int index = 0;;)
		{
			T& item = data_[index + 1];
			index   = index * 2 + 1;

			if ((index * 2 + 1 < data_.size() &&
			     cmp(item, data_[index * 2 + 1])) ||
			    (index += 1 && index * 2 + 1 < data_.size() &&
			              cmp(item, data_[index * 2 + 1])))
			{
				std::swap(data_[index * 2 + 1], item);
			}
			else
			{
				return item;
			}
		}
	}
}

template <typename T, typename Compare>
inline void erase(const T& item) noexcept
{}

#endif
