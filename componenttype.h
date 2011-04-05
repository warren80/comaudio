#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

/**
  Possible communication types.

  @author Nick Huber
  */
enum ComponentType {
    kStream   = 0,  /**< Multicast Music streaming. */
    kTransfer = 1,  /**< File transfer. */
    kVoice    = 2,  /**< Server-Client Voice Chat. */
    kFileList = 3,  /**< List of all files on the server. */
};

#endif // COMPONENTTYPE_H
