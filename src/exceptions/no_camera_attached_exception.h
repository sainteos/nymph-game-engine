#ifndef NO_CAMERA_ATTACHED_EXCEPTION_H
#define NO_CAMERA_ATTACHED_EXCEPTION_H

namespace Exceptions {
  class NoCameraAttachedException : public std::exception {
    public:
      NoCameraAttachedException() {
      }

      virtual const char* what() const throw() {
        return "No camera is attached!";
      }
  };
}

#endif
