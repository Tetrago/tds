#ifndef TDS_INTERVAL_HEAP_HPP
#define TDS_INTERVAL_HEAP_HPP

#include <functional>
#include <utility>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class IntervalHeap
{
public:
	using value_type = T;

	void insert(T&& item) noexcept;
	T pop_front() noexcept;
	T pop_back() noexcept;
	void erase(const T& item) noexcept;

	[[nodiscard]] bool empty() const noexcept { return data_.empty(); }

	[[nodiscard]] T& front() noexcept { return data_[0]; }

	[[nodiscard]] const T& front() const noexcept { return data_[0]; }

	[[nodiscard]] T& back() noexcept
	{
		return data_[std::min<typename std::vector<T>::size_type>(
		    1, data_.size() - 1)];
	}

	[[nodiscard]] const T& back() const noexcept
	{
		return data_[std::min<typename std::vector<T>::size_type>(
		    1, data_.size() - 1)];
	}

private:
	void pushDownMin(int index = 0) noexcept;
	void pushDownMax(int index = 0) noexcept;
	void pushUpMin(int index) noexcept;
	void pushUpMax(int index) noexcept;

	std::vector<T> data_;
};

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::insert(T&& item) noexcept
{
	Compare cmp;

	data_.push_back(std::move(item));

	if ((data_.size() & 1) && data_.size() > 2)
	{
		if (cmp(data_.back(), data_[(data_.size() - 1) / 4 * 2]))
		{
			std::swap(data_.back(), data_[(data_.size() - 1) / 4 * 2]);
			pushUpMin((data_.size() - 1) / 4);
		}
		else if (cmp(data_[(data_.size() - 1) / 4 * 2 + 1], data_.back()))
		{
			std::swap(data_.back(), data_[(data_.size() - 1) / 4 * 2 + 1]);
			pushUpMax((data_.size() - 1) / 4);
		}
	}
	else if (cmp(data_.back(), data_[data_.size() - 2]))
	{
		std::swap(data_.back(), data_[data_.size() - 2]);
		pushUpMin((data_.size() - 1) / 4 * 2);
	}
	else
	{
		pushUpMax((data_.size() - 1) / 4 * 2);
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
		pushDownMin();
		return item;
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
		T item = std::exchange(data_[1], std::move(data_.back()));
		data_.pop_back();
		pushDownMax();
		return item;
	}
}

template <typename T, typename Compare>
inline void erase(const T& item) noexcept
{
	// TODO:
}

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::pushDownMin(int index) noexcept
{
	if (index * 2 >= data_.size())
	{
		return;
	}

	Compare cmp;

	T& item = data_[index * 2];
	index   = index * 2 + 1;

	if ((index * 2 < data_.size() && cmp(data_[index * 2], item)) ||
	    (index += 1 && index * 2 < data_.size() && cmp(data_[index * 2], item)))
	{
		std::swap(item, data_[index * 2]);

		if (cmp(data_[index * 2], data_[index * 2 + 1]))
		{
			std::swap(data_[index * 2], data_[index * 2 + 1]);
			pushDownMax(index);
		}
		else
		{
			pushDownMin(index);
		}
	}
}

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::pushDownMax(int index) noexcept
{
	if (index * 2 + 1 >= data_.size())
	{
		return;
	}

	Compare cmp;

	T& item = data_[index * 2 + 1];
	index   = index * 2 + 1;

	if ((index * 2 + 1 < data_.size() && cmp(item, data_[index * 2 + 1])) ||
	    (index +=
	     1 && index * 2 + 1 < data_.size() && cmp(item, data_[index * 2 + 1])))
	{
		std::swap(item, data_[index * 2 + 1]);

		if (cmp(data_[index * 2 + 1], data_[index * 2]))
		{
			std::swap(data_[index * 2], data_[index * 2 + 1]);
			pushDownMin(index);
		}
		else
		{
			pushDownMax(index);
		}
	}
}

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::pushUpMin(int index) noexcept
{
	if (index == 0)
	{
		return;
	}

	Compare cmp;

	T& item  = data_[index * 2];
	index   /= 2;

	if (cmp(data_[index * 2], item))
	{
		std::swap(data_[index * 2], item);

		if (cmp(data_[index * 2 + 1], data_[index * 2]))
		{
			std::swap(data_[index * 2], data_[index * 2 + 1]);
			pushUpMax(index);
		}
		else
		{
			pushUpMin(index);
		}
	}
}

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::pushUpMax(int index) noexcept
{
	if (index == 0)
	{
		return;
	}

	Compare cmp;

	T& item  = data_[index * 2 + 1];
	index   /= 2;

	if (cmp(data_[index * 2 + 1], item))
	{
		std::swap(data_[index * 2 + 1], item);

		if (cmp(data_[index * 2 + 1], data_[index * 2]))
		{
			std::swap(data_[index * 2], data_[index * 2 + 1]);
			pushUpMin(index);
		}
		else
		{
			pushUpMax(index);
		}
	}
}

#endif
