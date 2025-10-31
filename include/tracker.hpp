#ifndef TDS_ENTANGLED_HPP
#define TDS_ENTANGLED_HPP

#include <memory>
#include <tuple>
#include <utility>

template <typename T>
class Tracker
{
public:
	class Box;

	~Tracker() noexcept                 = default;
	Tracker(const Tracker&)             = delete;
	Tracker& operator=(const Tracker&)  = delete;
	Tracker(Tracker&& other) noexcept   = default;
	Tracker& operator=(Tracker&& other) = default;

	[[nodiscard]] static std::tuple<Tracker, Box> make(T&& value) noexcept;

private:
	explicit Tracker(std::shared_ptr<T*> box) noexcept;

	std::shared_ptr<T*> box_;
};

template <typename T>
class Tracker<T>::Box
{
public:
	~Box() noexcept            = default;
	Box(const Box&)            = delete;
	Box& operator=(const Box&) = delete;
	Box(Box&& other) noexcept;
	Box& operator=(Box&& other) noexcept;

	bool operator<(const Box& other) const noexcept
	{
		return value_ < other.value_;
	}

	T& operator*() noexcept { return value_; }

	const T& operator*() const noexcept { return value_; }

	T& get() noexcept { return value_; }

	const T& get() const noexcept { return value_; }

private:
	friend class Tracker;

	explicit Box(T&& value) noexcept;

	std::shared_ptr<T*> box_;
	T value_;
};

template <typename T>
[[nodiscard]] inline std::tuple<Tracker<T>, typename Tracker<T>::Box>
Tracker<T>::make(T&& value) noexcept
{
	Box box(std::move(value));
	Tracker tracker(box.box_);
	return std::make_tuple(std::move(tracker), std::move(box));
}

template <typename T>
inline Tracker<T>::Tracker(std::shared_ptr<T*> box) noexcept
    : box_(std::move(box))
{}

template <typename T>
inline Tracker<T>::Box::Box(Box&& other) noexcept
    : box_(std::move(other.box_)), value_(std::move(other.value_))
{
	*box_ = &value_;
}

template <typename T>
inline typename Tracker<T>::Box& Tracker<T>::Box::operator=(
    Box&& other) noexcept
{
	box_   = std::move(other.box_);
	value_ = std::move(other.value_);

	*box_ = &value_;

	return *this;
}

template <typename T>
inline Tracker<T>::Box::Box(T&& value) noexcept
    : value_(std::move(value)), box_(std::make_shared<T*>(&value))
{}

#endif
