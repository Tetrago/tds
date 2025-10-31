#ifndef TDS_INTERVAL_HEAP_HPP
#define TDS_INTERVAL_HEAP_HPP

#include <cassert>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class IntervalHeap
{
public:
	using value_type = T;

	void insert(T&& value) noexcept;
	T pop_front() noexcept;
	T pop_back() noexcept;

	const T& front() const noexcept { return data_.front().min(); }

	const T& back() const noexcept { return data_.front().max(); }

private:
	struct Node;

	void diffuse(bool min) noexcept;

	std::vector<Node> data_;
};

template <typename T, typename Compare>
struct IntervalHeap<T, Compare>::Node
{
	explicit Node(T&& value) noexcept;

	void swap() noexcept;

	[[nodiscard]] T takeMin() noexcept;

	[[nodiscard]] T takeMax() noexcept;

	[[nodiscard]] bool invalid() const noexcept;

	[[nodiscard]] T& max() noexcept;
	[[nodiscard]] const T& max() const noexcept;

	[[nodiscard]] T& min() noexcept { return left; }

	[[nodiscard]] const T& min() const noexcept { return left; }

	[[nodiscard]] bool full() const noexcept { return right.has_value(); }

	[[nodiscard]] friend bool operator<(const Node& lhs, const Node& rhs)
	{
		return Compare{}(lhs.min(), rhs.min());
	}

	[[nodiscard]] friend bool operator>(const Node& lhs, const Node& rhs)
	{
		return Compare{}(rhs.max(), lhs.max());
	}

	T left;
	std::optional<T> right = std::nullopt;
};

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::insert(T&& value) noexcept
{
	if (data_.empty() || data_.back().full())
	{
		data_.emplace_back(std::move(value));
	}
	else
	{
		auto& last = data_.back();
		last.right = std::move(value);

		if (last.invalid())
		{
			last.swap();
		}
	}

	for (int index = data_.size() - 1; index > 0; index /= 2)
	{
		auto& lower = data_[index];
		auto& upper = data_[index / 2];

		if (lower < upper)
		{
			std::swap(lower.min(), upper.min());
		}
		else if (lower > upper)
		{
			std::swap(lower.max(), upper.max());
		}
		else
		{
			break;
		}

		if (upper.invalid())
		{
			upper.swap();
		}
	}
}

template <typename T, typename Compare>
inline T IntervalHeap<T, Compare>::pop_front() noexcept
{
	auto& back = data_.back();
	T value    = back.takeMin();

	if (data_.size() == 1)
	{
		return value;
	}

	T item = std::exchange(data_[0].min(), std::move(value));
	diffuse(true);
	return item;
}

template <typename T, typename Compare>
inline T IntervalHeap<T, Compare>::pop_back() noexcept
{
	auto& back = data_.back();
	T value    = back.takeMax();

	if (data_.size() == 1)
	{
		return value;
	}

	T item = std::exchange(data_[0].max(), std::move(value));
	diffuse(false);
	return item;
}

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::diffuse(bool min) noexcept
{
	for (int index = 0; index * 2 + 1 < data_.size();)
	{
		auto& item = data_[index];

		if (item.invalid())
		{
			item.swap();
			min = !min;
		}

		index      = index * 2 + 1;
		auto& left = data_[index];

		if (min && left < item)
		{
			std::swap(left.min(), item.min());
		}
		else if (!min && left > item)
		{
			std::swap(left.max(), item.max());
		}
		else if (index += 1 && index < data_.size())
		{
			auto& right = data_[index];

			if (min && right < item)
			{
				std::swap(right.min(), item.min());
			}
			else if (!min && right > item)
			{
				std::swap(right.max(), item.max());
			}
		}
	}
}

template <typename T, typename Compare>
inline IntervalHeap<T, Compare>::Node::Node(T&& value) noexcept
    : left(std::move(value))
{}

template <typename T, typename Compare>
inline void IntervalHeap<T, Compare>::Node::swap() noexcept
{
	assert(right.has_value() && "invalid swap invocation");
	std::swap(left, right.value());
}

template <typename T, typename Compare>
[[nodiscard]] inline T IntervalHeap<T, Compare>::Node::takeMin() noexcept
{
	T value = std::exchange(left, std::move(*right));
	right.reset();
	return value;
}

template <typename T, typename Compare>
[[nodiscard]] inline T IntervalHeap<T, Compare>::Node::takeMax() noexcept
{
	T value = std::move(*right);
	right.reset();
	return value;
}

template <typename T, typename Compare>
[[nodiscard]] inline bool IntervalHeap<T, Compare>::Node::invalid()
    const noexcept
{
	return right.has_value() && Compare{}(right.value(), left);
}

template <typename T, typename Compare>
[[nodiscard]] inline T& IntervalHeap<T, Compare>::Node::max() noexcept
{
	if (right.has_value())
	{
		return right.value();
	}
	else
	{
		return left;
	}
}

template <typename T, typename Compare>
[[nodiscard]] inline const T& IntervalHeap<T, Compare>::Node::max()
    const noexcept
{
	if (right.has_value())
	{
		return right.value();
	}
	else
	{
		return left;
	}
}

#endif
