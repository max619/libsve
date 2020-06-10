#pragma once
#include <exception>


template<typename TImpl>
void DefaultDeleter(TImpl* impl)
{
	delete impl;
}

template<typename TImpl>
class PImplBase
{
public:
	PImplBase();
	PImplBase(TImpl* impl);
	PImplBase(TImpl* impl,  void (*deleter)(TImpl*));

	PImplBase(const PImplBase<TImpl>& lhs);
	PImplBase(PImplBase<TImpl>&& lhs);
	virtual ~PImplBase();

	PImplBase<TImpl>& operator=(const PImplBase<TImpl>& lhs);
	PImplBase<TImpl>& operator=(PImplBase<TImpl>&& lhs);

protected:
	TImpl* _impl;

private:
	void (*_deleter)(TImpl*);
};

template<typename TImpl>
inline PImplBase<TImpl>::PImplBase()
{
	_impl = new TImpl();
	_deleter = DefaultDeleter<TImpl>;
}

template<typename TImpl>
inline PImplBase<TImpl>::PImplBase(TImpl* impl)
	: _impl(impl), _deleter(DefaultDeleter<TImpl>)
{
}

template<typename TImpl>
inline PImplBase<TImpl>::PImplBase(TImpl* impl, void (*deleter)(TImpl*))
	: _impl(impl), _deleter(deleter)
{
}

template<typename TImpl>
inline PImplBase<TImpl>::PImplBase(const PImplBase<TImpl>& lhs) :
	_deleter(lhs._deleter)
{
	_impl = new TImpl(*lhs._impl);
	if (_impl == nullptr)
		throw std::exception("Out of memory");
}

template<typename TImpl>
inline PImplBase<TImpl>::PImplBase(PImplBase<TImpl>&& lhs)
	: _deleter(lhs._deleter)
{
	_impl = lhs._impl;
	lhs._impl = nullptr;
}

template<typename TImpl>
inline PImplBase<TImpl>::~PImplBase()
{
	if (_impl != nullptr)
		_deleter(_impl);
	_impl = nullptr;
}

template<typename TImpl>
inline PImplBase<TImpl>& PImplBase<TImpl>::operator=(const PImplBase<TImpl>& lhs)
{
	if (this == &lhs)
		return *this;

	if (_impl != nullptr)
		_deleter(_impl);

	_impl = new TImpl(*lhs._impl);

	if (_impl == nullptr)
		throw std::exception("Out of memory");

	_deleter = DefaultDeleter<TImpl>;

	return *this;
}

template<typename TImpl>
inline PImplBase<TImpl>& PImplBase<TImpl>::operator=(PImplBase<TImpl>&& lhs)
{
	if (this == &lhs)
		return *this;

	if (_impl != nullptr)
		_deleter(_impl);

	_impl = lhs._impl;
	_deleter = lhs._deleter;

	lhs._impl = nullptr;

	return *this;
}
