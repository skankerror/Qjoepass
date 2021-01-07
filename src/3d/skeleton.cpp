#include "skeleton.h"

#include <qmath.h>


Skeleton::Skeleton(const QUrl &source, Qt3DCore::QNode *parent)
  : Qt3DCore::QSkeletonLoader(source, parent)
{}

Skeleton::Skeleton(QNode *parent)
  : Qt3DCore::QSkeletonLoader(parent)
{}
