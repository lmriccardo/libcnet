add_library(Packets STATIC
    Base.hpp
    IcmpPacket.hpp
    IcmpPacket.cpp
)

target_link_libraries(Packets PUBLIC Utils)