/*
 * V4L2 frame grabber
 * Grabs frames and image information from a V4L2 source
 *
 * Dan Nixon
 */

#ifndef V4L2SOURCE_H
#define V4L2SOURCE_H

#include <stdio.h>
#include <stdlib.h>

#include "frame_source.h"

#include <opencv2/core/core.hpp>
#include "../OCVCapture.h"

class V4L2Source: public FrameSource
{
  public:
    V4L2Source();
    ~V4L2Source();

    void open(std::string);
    void close();
    bool isOpen();
    bool grab(cv::Mat *);
    unsigned int getWidth();
    unsigned int getHeight();

    void setCaptureSize(unsigned int, unsigned int);
    double getFrameRate();

  private:
    OCVCapture *o_capture;
};

#endif
