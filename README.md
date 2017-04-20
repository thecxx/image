# image

> About this project: image
>> It's just a C++ library help us to load PE image from memory.

> 历史
>> 几年前也写过几份内存加载PE库的项目，但是当时写的特别乱，最近整理了一份，目前主要支持win32标准Dll和MFC Dll，以及其他常见环境中生产的Dll，由于是企图脱离加载器和系统，因为未做ldr挂钩和程序段的处理(比如共享段就不支持)，可能部分编译器生成的Dll会加载不稳定.