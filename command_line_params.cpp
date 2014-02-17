/*
 * Command line argument parser for BubbleScopeApp parameters.
 *
 * Dan Nixon
 */

#include "command_line_params.h"

/*
 * Array of parameters which may be passed to BubbleScope app
 */
CLParameter clParams[] = {
  CLParameter{HELP,             "-h",     "--help",       "Show help",          "Shows this help text"},
  CLParameter{CAPTURE_DEVICE,   "-d",     "--device",     "Capture device",     "Specified the V4L2 capture device"},
  CLParameter{ORIGINAL_WIDTH,   "-iw",    "--inwidth",    "Original width",     ""},
  CLParameter{ORIGINAL_HEIGHT,  "-ih",    "--inheight",   "Original height",    ""},
  CLParameter{UNWRAP_WIDTH,     "-ow",    "--outwidth",   "Unwrap width",       ""},
  CLParameter{RADIUS_MIN,       "-rmin",  "--minradius",  "Radius min",         ""},
  CLParameter{RADIUS_MAX,       "-rmax",  "--maxradius",  "Radius max",         ""},
  CLParameter{U_CENTRE,         "-uc",    "--ucentre",    "U centre",           ""},
  CLParameter{V_CENTRE,         "-vc",    "--vcentre",    "V centre",           ""},
  CLParameter{OFFSET_ANGLE,     "-a",     "--offset",     "Offset angle",       ""},
  CLParameter{SHOW_ORIGINAL,    "-o",     "--original",   "Show original",      ""},
  CLParameter{SHOW_UNWRAP,      "-u",     "--unwrap",     "Show unwrap",        ""},
  CLParameter{OUTPUT_STILLS,    "-s",     "--stills",     "Output stills",      ""},
  CLParameter{OUTPUT_VIDEO,     "-v",     "--video",      "Output video",       ""},
  CLParameter{OUTPUT_MJPG,      "-m",     "--mjpg",       "Output MJPG stream", ""}
};

/*
 * Size of params array
 */
int clParamCount = 15;

/*
 * Populates a set of BubbleScopeParameters based on contents of argv
 */
int getParameters(BubbleScopeParameters *params, int argc, char **argv)
{
  int i, j;
  for(i = 0; i < argc; i++)
  {
    for(j = 0; j < clParamCount; j++)
    {
      if((strcmp(clParams[j].shortParam, argv[i]) == 0) || (strcmp(clParams[j].longParam, argv[i]) == 0))
      {
        i++;
        switch(clParams[j].type)
        {
          case HELP:
            return HELP;
          case CAPTURE_DEVICE:
            sscanf(argv[i], "%d", &params->captureDevice);
            break;
          case ORIGINAL_WIDTH:
            sscanf(argv[i], "%d", &params->originalWidth);
            break;
          case ORIGINAL_HEIGHT:
            sscanf(argv[i], "%d", &params->originalHeight);
            break;
          case UNWRAP_WIDTH:
            sscanf(argv[i], "%d", &params->unwrapWidth);
            break;
          case RADIUS_MIN:
            sscanf(argv[i], "%f", &params->radiusMin);
            break;
          case RADIUS_MAX:
            sscanf(argv[i], "%f", &params->radiusMax);
            break;
          case U_CENTRE:
            sscanf(argv[i], "%f", &params->uCentre);
            break;
          case V_CENTRE:
            sscanf(argv[i], "%f", &params->vCentre);
            break;
          case OFFSET_ANGLE:
            sscanf(argv[i], "%f", &params->offsetAngle);
            break;
          case SHOW_ORIGINAL:
            i--;
            params->mode[MODE_SHOW_ORIGINAL] = 1;
            break;
          case SHOW_UNWRAP:
            i--;
            params->mode[MODE_SHOW_UNWRAP] = 1;
            break;
          case OUTPUT_STILLS:
            params->mode[MODE_STILLS] = 1;
            params->outputFilename[MODE_STILLS] = argv[i];
            break;
          case OUTPUT_VIDEO:
            params->mode[MODE_VIDEO] = 1;
            params->outputFilename[MODE_VIDEO] = argv[i];
            break;
          case OUTPUT_MJPG:
            params->mode[MODE_MJPG] = 1;
            params->outputFilename[MODE_MJPG] = argv[i];
            break;
        }
      }
    }
  }
  return 0;
}

/*
 * Prints the parameters and their usage to stdout
 */
void printParameterUsage()
{
  int i;
  for(i = 0; i < clParamCount; i++)
  {
    printf("\t%s\t%s\t\t%s\t\t%s\n",
        clParams[i].shortParam, clParams[i].longParam,
        clParams[i].name, clParams[i].description);
  }
}
