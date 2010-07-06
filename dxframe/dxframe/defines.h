#ifndef DEFINES_H
#define DEFINES_H

#define RELEASE (p) {if (p) {(p)->Release (); (p) = NULL;}}
#define DEL (p) {if (p) {delete (p); (p) = NULL;}}
#define DELA (p) {if (p) {delete [] (p); (p) = NULL;}}

#endif