#pragma once

#include <vector>
#include <map>
#include <typeinfo>
#include <iostream>
#include <memory>
#include <string>

class VariableBase
{
public:
	VariableBase(const std::type_info& type) : _type(type), _data(nullptr)
	{

		std::cout << type.name() << std::endl;
	}

	virtual ~VariableBase()
	{
		if (_data)
		{
			delete _data;
		}		
	}

protected:
	const std::type_info& _type;
	void* _data;
};

typedef std::shared_ptr<VariableBase> VariableBasePtr;

template<class T>
class Variable : public VariableBase
{
public:
	Variable() : VariableBase(typeid(T)) 
	{
		_data = new T();
	};
	Variable(const T& data) : VariableBase(typeid(T)) 
	{
		_data = new T();
		if (std::is_class<T>())
		{
			*(T*)_data = data;
		}
		else
		{
			_data = memcpy(_data, (void*)&data, sizeof(T));
		}
		
	};
	Variable(const Variable<T>& var) : VariableBase(typeid(T)) 
	{
		_data = new T();
		if (std::is_class<T>())
		{
			*(T*)_data = var();
		}
		else
		{
			_data = memcpy(_data, var._data, sizeof(T));
		}
	};

	virtual size_t size()
	{ 
		return sizeof(T); 
	};
	
	T& operator()() const
	{ 
		T* ptr = static_cast<T*>(_data);
		return *ptr;
	};

	Variable<T>& operator= (const Variable<T> &src)
	{
		if (std::is_class<T>())
		{
			*(T*)_data = src();
		}
		else
		{
			memcpy(_data, src._data, sizeof(T));
		}

		// return the existing object
		return *this;
	}

	Variable<T>& operator= (const T &src)
	{
		if (std::is_class<T>())
		{
			*(T*)_data = src;
		}
		else
		{
			memcpy(_data, (void*)&src, sizeof(T));
		}

		// return the existing object
		return *this;
	}

	virtual ~Variable<T>()
	{
		//VariableBase::~VariableBase();
	}

private:
};

typedef Variable<int> IntVariable;
typedef std::shared_ptr<IntVariable> IntVariablePtr;

typedef Variable<std::string> StringVariable;
typedef std::shared_ptr<StringVariable> StringVariablePtr;

class Function
{
	public:
		virtual bool exec() = 0;

		std::map<std::string, VariableBasePtr>& inputs()
		{
			return _inputs;
		}

		std::map<std::string, VariableBasePtr>& outputs()
		{
			return _outputs;
		}

	private:
		std::map<std::string, VariableBasePtr> _inputs;
		std::map<std::string, VariableBasePtr> _outputs;
};

class PrintFunction
	: public Function
{
public:
	PrintFunction() {
		inputs().insert(std::make_pair("ToPrint", new StringVariable));
	}

	bool exec() override
	{
		std::cout << (*std::dynamic_pointer_cast<StringVariable>(inputs().at("ToPrint")))() << std::endl;
		return true;
	}
};

class Operation
{
public:
	std::map<std::string, VariableBasePtr>& inputs()
	{
		return _inputs;
	}

	std::map<std::string, VariableBasePtr>& outputs()
	{
		exec();
		return _outputs;
	}

	protected:
		std::map<std::string, VariableBasePtr> _inputs;
		std::map<std::string, VariableBasePtr> _outputs;

	private:
		virtual bool exec() = 0;
};

template<class T>
class AddOperation : public Operation
{
public:
	AddOperation()
	{
		_inputs.insert(std::make_pair("A", new Variable<T>()));
		_inputs.insert(std::make_pair("B", new Variable<T>()));

		_outputs.insert(std::make_pair("Result", new Variable<T>()));
	}

private:
	virtual bool exec() override
	{
		*std::dynamic_pointer_cast<IntVariable>(_outputs.at("Result")) = (*std::dynamic_pointer_cast<IntVariable>(_inputs.at("A")))() + (*std::dynamic_pointer_cast<IntVariable>(_inputs.at("B")))();
		return true;
	};
};