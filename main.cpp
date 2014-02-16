/*
 * BubbleScope V4L2 capture app
 * Allows capturing videos and stills from a BubbleScope fitted V4L2 device.
 *
 * Dan Nixon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "unwrap.h"

enum BubbleScopeCaptureMode
{
  PREVIEW,
  VIDEO,
  STILLS
};

/*
 * Stores user options defining capture properties.
 */
struct BubbleScopeParameters
{
  int captureDevice;
  int originalWidth;
  int originalHeight;
  int unwrapWidth;
  float radiusMin;
  float radiusMax;
  float uCentre;
  float vCentre;
  float offsetAngle;
  int showOriginal;
  int showUnwrap;
  BubbleScopeCaptureMode capMode;
  std::string outputFilename;
};

/*
 * Sets a resonable default configuration.
 */
void setupDefaultParameters(BubbleScopeParameters *params)
{
  params->captureDevice = 0;
  params->originalWidth = 640;
  params->originalHeight = 480;
  params->unwrapWidth = 800;
  params->radiusMin = 0.25f;
  params->radiusMax = 0.6f;
  params->uCentre = 0.5f;
  params->vCentre = 0.5f;
  params->offsetAngle = 180.0f;
  params->showOriginal = 0;
  params->showUnwrap = 1;
  params->capMode = PREVIEW;
  params->outputFilename = "BubbleScope_Capture";
}

/*
 * Prints the current configuration to stdout.
 */
void printParameters(BubbleScopeParameters *params)
{
  printf("Video caputre device: %d\n", params->captureDevice);
  printf("Original image size: %dx%d\n", params->originalWidth, params->originalHeight);
  printf("Unwrap image width: %d\n", params->unwrapWidth);
  printf("Unwrap image radius: min=%f, max=%f\n", params->radiusMin, params->radiusMax);
  printf("Orignal image centre: u=%f, v=%f\n", params->uCentre, params->vCentre);
  printf("Offset angle: %fdeg.\n", params->offsetAngle);
  printf("Show original: %d\nShow unwrap: %d\n", params->showOriginal, params->showUnwrap);
  std::string mode;
  switch(params->capMode)
  {
    case PREVIEW:
      mode = "Preview";
      break;
    case VIDEO:
      mode = "Video";
      break;
    case STILLS:
      mode = "Stills";
      break;
  }
  printf("Capture mode: %s\n", mode.c_str());
  printf("Output filename: %s\n", params->outputFilename.c_str());
}

int main(int argc, char **argv)
{
  //Get some storage for parameters
  BubbleScopeParameters params;
  setupDefaultParameters(&params);

  //Get parameters  TODO: Nice argument parsing
  sscanf(argv[1], "%d", &params.captureDevice);
  sscanf(argv[2], "%d", &params.originalWidth);
  sscanf(argv[3], "%d", &params.originalHeight);
  sscanf(argv[4], "%d", &params.unwrapWidth);
  sscanf(argv[5], "%f", &params.radiusMin);
  sscanf(argv[6], "%f", &params.radiusMax);
  sscanf(argv[7], "%f", &params.uCentre);
  sscanf(argv[8], "%f", &params.vCentre);
  sscanf(argv[9], "%d", &params.showOriginal);
  sscanf(argv[10], "%f", &params.offsetAngle);

  //Tell the user how things are going to happen
  printParameters(&params);

  //Setup the image unwrapper
  BubbleScopeUnwrapper unwrapper;
  unwrapper.unwrapWidth(params.unwrapWidth);
  unwrapper.originalCentre(params.uCentre, params.vCentre);
  unwrapper.imageRadius(params.radiusMin, params.radiusMax);
  unwrapper.offsetAngle(params.offsetAngle);

  //Open the capture device and check it is working
  cv::VideoCapture cap(params.captureDevice);
  if(!cap.isOpened())
  {
    printf("Can't open video capture source\n");
    return -1;
  }

  //The container for captured frames
  cv::Mat frame;

  //Capture an initial frame and generate the unwrap transformation
  cap >> frame;
  unwrapper.originalSize(frame.cols, frame.rows);
  unwrapper.generateTransformation();

  while(1)
  {
    //Capture a frame
    cap >> frame;

    //Unwrap it
    cv::Mat unwrap = unwrapper.unwrap(&frame);

    //Show the original if asked to
    if(params.showOriginal)
      imshow("BubbleScope Original Image", frame);

    //Show the unwrapped if asked to
    if(params.showUnwrap)
      imshow("BubbleScope Unwrapped Image", unwrap);

    //TODO: Add video saving, MJPG saving and stills capture on keypress

    //Exit if asked to  TODO: Should only do this if showing images in windows
    if(cv::waitKey(1) == 27)
      break;
  }
  return 0;
}