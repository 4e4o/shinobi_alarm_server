#ifndef ICLOSABLE_H
#define ICLOSABLE_H

class IClosable {
public:
    virtual ~IClosable() {}
    virtual void close() = 0;
};

#endif /* ICLOSABLE_H */
