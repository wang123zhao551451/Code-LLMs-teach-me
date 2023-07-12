#ifndef MY_ANY_H
#define MY_ANY_H

#include <typeinfo>
#include <utility>

class bad_any_cast : public std::bad_cast {
public:
    virtual const char* what() const throw() {
        return "bad any cast";
    }
};

class my_any {
public:
    my_any() : ptr(nullptr) {}

    template<typename T>
    my_any(const T& value) : ptr(new holder<T>(value)) {}

    my_any(const my_any& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}

    my_any(my_any&& other) : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    ~my_any() {
        delete ptr;
    }

    my_any& operator=(const my_any& other) {
        my_any(other).swap(*this);
        return *this;
    }

    my_any& operator=(my_any&& other) {
        other.swap(*this);
        return *this;
    }

    template<typename T>
    my_any& operator=(const T& value) {
        my_any(value).swap(*this);
        return *this;
    }

    void swap(my_any& other) {
        std::swap(ptr, other.ptr);
    }

    bool empty() const {
        return !ptr;
    }

    const std::type_info& type() const {
        return ptr ? ptr->type() : typeid(void);
    }

    template<typename T>
    friend T my_any_cast(const my_any& any);

private:
    class holder_base {
    public:
        virtual ~holder_base() {}
        virtual holder_base* clone() const = 0;
        virtual const std::type_info& type() const = 0;
    };

    template<typename T>
    class holder : public holder_base {
    public:
        holder(const T& value) : held(value) {}
        holder_base* clone() const { return new holder(held); }
        const std::type_info& type() const { return typeid(T); }
        T held;
    };

    holder_base* ptr;
};

template<typename T>
T my_any_cast(const my_any& any) {
    if (any.type() != typeid(T)) {
        throw bad_any_cast();
    }
    return static_cast<const my_any::holder<T>*>(any.ptr)->held;
}

#endif /* MY_ANY_H */
