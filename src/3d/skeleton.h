#ifndef SKELETON_H
#define SKELETON_H

#include <Qt3DCore/QSkeletonLoader>

class Skeleton : public Qt3DCore::QSkeletonLoader
{
  Q_OBJECT

public:
  explicit Skeleton(const QUrl &source, Qt3DCore::QNode *parent = nullptr);
  explicit Skeleton(Qt3DCore::QNode *parent = nullptr);


public slots:

signals:

private:

};

#endif // SKELETON_H
