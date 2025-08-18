#ifndef ENQUEUE_FRONT_TAG_HPP
#define ENQUEUE_FRONT_TAG_HPP

#include "token.hpp"

namespace mt {

template <typename TaskContainer>
class ThreadPoolExecutor;

struct EnqueueFrontTag {
    
    template <typename TaskContainer>
    friend class ThreadPoolExecutor;

    static Token<EnqueueFrontTag> create_token() {
        return Token<EnqueueFrontTag>{};
    }
};

} // namespace mt

#endif // ENQUEUE_FRONT_TAG_HPP
