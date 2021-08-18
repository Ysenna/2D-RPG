#ifndef MULTIOBJECT_H
#define MULTIOBJECT_H

#include <vector>

#include <TmxObject.h>


class MultiObject
{
public:
    MultiObject();

    std::vector<const Tmx::Object*> parts;
    int y = 0;
};

#endif // MULTIOBJECT_H
