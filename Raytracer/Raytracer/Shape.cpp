#include "Shape.h"

Shape::Shape(const Transform &o2w, bool ro) : ObjectToWorld(o2w), WorldToObject(o2w.GetInverse()), reverseOrientation(ro), transformSwapHandedness(o2w.SwapHandedness())
{
}

Shape::~Shape()
{
}
