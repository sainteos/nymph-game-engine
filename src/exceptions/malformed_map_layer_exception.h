#ifndef MALFORMED_MAP_LAYER_EXCEPTION_H
#define MALFORMED_MAP_LAYER_EXCEPTION_H

namespace Exceptions {
  class MalformedMapLayerException : public std::exception {
    public:
      MalformedMapLayerException() {
      }

      virtual const char* what() const throw() {
        return "Map layer was malformed and cannot be decoded and inflated.";
      }
  };
}

#endif
