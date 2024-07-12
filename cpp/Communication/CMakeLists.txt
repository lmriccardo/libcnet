add_library(Communication STATIC
    Sender.hpp
    Sender.cpp
)

target_link_libraries(Communication PUBLIC Utils Packets)