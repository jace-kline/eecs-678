#ifndef MAYBE_H
#define MAYBE_H

template <typename T>
class Maybe {
    private:
        T item;
        bool nothing;
    public:
        Maybe<T>();
        Maybe<T>(const T& obj);
        ~Maybe<T>();
        bool isNothing() const;
        T getItem() const;
};

#endif