#ifndef CHILD_DOES_NOT_EXIST_EXCEPTION_H
#define CHILD_DOES_NOT_EXIST_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class ChildDoesNotExistException : public std::exception {
    public:
      ChildDoesNotExistException() {
      }

      virtual const char* what() const throw() {
        return "Child Transform does not exist!";
      }
  };
}

#endif
