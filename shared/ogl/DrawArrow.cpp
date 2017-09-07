/*
 *  DrawArrow.cpp
 *  
 *
 *  Created by jian zhang on 1/1/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include "DrawArrow.h"
#include <math/Matrix44F.h>
#include <gl_heads.h>

namespace aphid {

static const int sBoneArrowNumTriangleFVVertices = 24;
static const float sBoneArrowTriangleFVVertices[] = {0.005912f, 0.000000f, 0.000000f,
0.279940f, -0.490637f, 0.490637f,
0.279940f, 0.490637f, 0.490637f,
0.279940f, -0.490637f, 0.490637f,
0.997291f, 0.000000f, 0.000000f,
0.279940f, 0.490637f, 0.490637f,
0.005912f, 0.000000f, 0.000000f,
0.279940f, 0.490637f, 0.490637f,
0.279940f, 0.490637f, -0.490637f,
0.279940f, 0.490637f, 0.490637f,
0.997291f, 0.000000f, 0.000000f,
0.279940f, 0.490637f, -0.490637f,
0.005912f, 0.000000f, 0.000000f,
0.279940f, 0.490637f, -0.490637f,
0.279940f, -0.490637f, -0.490637f,
0.279940f, 0.490637f, -0.490637f,
0.997291f, 0.000000f, 0.000000f,
0.279940f, -0.490637f, -0.490637f,
0.005912f, 0.000000f, 0.000000f,
0.279940f, -0.490637f, -0.490637f,
0.279940f, -0.490637f, 0.490637f,
0.279940f, -0.490637f, -0.490637f,
0.997291f, 0.000000f, 0.000000f,
0.279940f, -0.490637f, 0.490637f,
};
static const float sBoneArrowTriangleFVNormals[] = {-0.873058f, 0.000000f, 0.487616f,
-0.873058f, 0.000000f, 0.487616f,
-0.873058f, 0.000000f, 0.487616f,
0.564541f, -0.000000f, 0.825405f,
0.564541f, -0.000000f, 0.825405f,
0.564541f, -0.000000f, 0.825405f,
-0.873058f, 0.487616f, 0.000000f,
-0.873058f, 0.487616f, 0.000000f,
-0.873058f, 0.487616f, 0.000000f,
0.564541f, 0.825405f, 0.000000f,
0.564541f, 0.825405f, 0.000000f,
0.564541f, 0.825405f, 0.000000f,
-0.873058f, 0.000000f, -0.487616f,
-0.873058f, 0.000000f, -0.487616f,
-0.873058f, 0.000000f, -0.487616f,
0.564541f, 0.000000f, -0.825405f,
0.564541f, 0.000000f, -0.825405f,
0.564541f, 0.000000f, -0.825405f,
-0.873058f, -0.487616f, 0.000000f,
-0.873058f, -0.487616f, 0.000000f,
-0.873058f, -0.487616f, 0.000000f,
0.564541f, -0.825405f, 0.000000f,
0.564541f, -0.825405f, 0.000000f,
0.564541f, -0.825405f, 0.000000f,
};

static const int sXArrowNumTriangleFVVertices = 66;
static const float sXArrowTriangleFVVertices[] = {0.020067f, -0.021930f, 0.021930f,
0.775657f, -0.021930f, 0.021930f,
0.020067f, 0.021930f, 0.021930f,
0.020067f, 0.021930f, 0.021930f,
0.775657f, -0.021930f, 0.021930f,
0.775657f, 0.021930f, 0.021930f,
0.020067f, 0.021930f, 0.021930f,
0.775657f, 0.021930f, 0.021930f,
0.020067f, 0.021930f, -0.021930f,
0.020067f, 0.021930f, -0.021930f,
0.775657f, 0.021930f, 0.021930f,
0.775657f, 0.021930f, -0.021930f,
0.020067f, 0.021930f, -0.021930f,
0.775657f, 0.021930f, -0.021930f,
0.020067f, -0.021930f, -0.021930f,
0.020067f, -0.021930f, -0.021930f,
0.775657f, 0.021930f, -0.021930f,
0.775657f, -0.021930f, -0.021930f,
0.020067f, -0.021930f, -0.021930f,
0.775657f, -0.021930f, -0.021930f,
0.020067f, -0.021930f, 0.021930f,
0.020067f, -0.021930f, 0.021930f,
0.775657f, -0.021930f, -0.021930f,
0.775657f, -0.021930f, 0.021930f,
0.020067f, -0.021930f, -0.021930f,
0.020067f, -0.021930f, 0.021930f,
0.020067f, 0.021930f, -0.021930f,
0.020067f, 0.021930f, -0.021930f,
0.020067f, -0.021930f, 0.021930f,
0.020067f, 0.021930f, 0.021930f,
0.775657f, -0.021930f, 0.021930f,
0.775657f, -0.021930f, -0.021930f,
0.775864f, -0.047242f, 0.047242f,
0.775864f, -0.047242f, 0.047242f,
0.775657f, -0.021930f, -0.021930f,
0.775864f, -0.047242f, -0.047242f,
0.775657f, -0.021930f, -0.021930f,
0.775657f, 0.021930f, -0.021930f,
0.775864f, -0.047242f, -0.047242f,
0.775864f, -0.047242f, -0.047242f,
0.775657f, 0.021930f, -0.021930f,
0.775864f, 0.047242f, -0.047242f,
0.775657f, 0.021930f, -0.021930f,
0.775657f, 0.021930f, 0.021930f,
0.775864f, 0.047242f, -0.047242f,
0.775864f, 0.047242f, -0.047242f,
0.775657f, 0.021930f, 0.021930f,
0.775864f, 0.047242f, 0.047242f,
0.775657f, 0.021930f, 0.021930f,
0.775657f, -0.021930f, 0.021930f,
0.775864f, 0.047242f, 0.047242f,
0.775864f, 0.047242f, 0.047242f,
0.775657f, -0.021930f, 0.021930f,
0.775864f, -0.047242f, 0.047242f,
0.775864f, -0.047242f, -0.047242f,
0.775864f, 0.047242f, -0.047242f,
0.999405f, 0.000000f, 0.000000f,
0.775864f, -0.047242f, 0.047242f,
0.775864f, -0.047242f, -0.047242f,
0.999405f, 0.000000f, 0.000000f,
0.775864f, 0.047242f, 0.047242f,
0.775864f, -0.047242f, 0.047242f,
0.999405f, 0.000000f, 0.000000f,
0.775864f, 0.047242f, -0.047242f,
0.775864f, 0.047242f, 0.047242f,
0.999405f, 0.000000f, 0.000000f
};
static const float sXArrowTriangleFVNormals[] = {0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
-0.000000f, 0.000000f, 1.000000f,
-0.000000f, 0.000000f, 1.000000f,
-0.000000f, 0.000000f, 1.000000f,
-0.000000f, 1.000000f, 0.000000f,
-0.000000f, 1.000000f, 0.000000f,
-0.000000f, 1.000000f, 0.000000f,
0.000000f, 1.000000f, 0.000000f,
0.000000f, 1.000000f, 0.000000f,
0.000000f, 1.000000f, 0.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-0.999967f, -0.008162f, -0.000000f,
-0.999967f, -0.008162f, -0.000000f,
-0.999967f, -0.008162f, -0.000000f,
-0.999967f, -0.008162f, -0.000000f,
-0.999967f, -0.008162f, -0.000000f,
-0.999967f, -0.008162f, -0.000000f,
-0.999967f, 0.000000f, -0.008162f,
-0.999967f, 0.000000f, -0.008162f,
-0.999967f, 0.000000f, -0.008162f,
-0.999967f, 0.000000f, -0.008162f,
-0.999967f, 0.000000f, -0.008162f,
-0.999967f, 0.000000f, -0.008162f,
-0.999967f, 0.008162f, 0.000000f,
-0.999967f, 0.008162f, 0.000000f,
-0.999967f, 0.008162f, 0.000000f,
-0.999967f, 0.008162f, 0.000000f,
-0.999967f, 0.008162f, 0.000000f,
-0.999967f, 0.008162f, 0.000000f,
-0.999967f, 0.000000f, 0.008162f,
-0.999967f, 0.000000f, 0.008162f,
-0.999967f, 0.000000f, 0.008162f,
-0.999967f, 0.000000f, 0.008162f,
-0.999967f, 0.000000f, 0.008162f,
-0.999967f, 0.000000f, 0.008162f,
0.206766f, 0.000000f, -0.978390f,
0.206766f, 0.000000f, -0.978390f,
0.206766f, 0.000000f, -0.978390f,
0.206766f, -0.978390f, 0.000000f,
0.206766f, -0.978390f, 0.000000f,
0.206766f, -0.978390f, 0.000000f,
0.206766f, 0.000000f, 0.978390f,
0.206766f, 0.000000f, 0.978390f,
0.206766f, 0.000000f, 0.978390f,
0.206766f, 0.978390f, -0.000000f,
0.206766f, 0.978390f, -0.000000f,
0.206766f, 0.978390f, -0.000000f
};

static const int sYArrowNumTriangleFVVertices = 66;
static const float sYArrowTriangleFVVertices[] = {0.021930f, 0.020067f, 0.021930f,
0.021930f, 0.775657f, 0.021930f,
-0.021930f, 0.020067f, 0.021930f,
-0.021930f, 0.020067f, 0.021930f,
0.021930f, 0.775657f, 0.021930f,
-0.021930f, 0.775657f, 0.021930f,
-0.021930f, 0.020067f, 0.021930f,
-0.021930f, 0.775657f, 0.021930f,
-0.021930f, 0.020067f, -0.021930f,
-0.021930f, 0.020067f, -0.021930f,
-0.021930f, 0.775657f, 0.021930f,
-0.021930f, 0.775657f, -0.021930f,
-0.021930f, 0.020067f, -0.021930f,
-0.021930f, 0.775657f, -0.021930f,
0.021930f, 0.020067f, -0.021930f,
0.021930f, 0.020067f, -0.021930f,
-0.021930f, 0.775657f, -0.021930f,
0.021930f, 0.775657f, -0.021930f,
0.021930f, 0.020067f, -0.021930f,
0.021930f, 0.775657f, -0.021930f,
0.021930f, 0.020067f, 0.021930f,
0.021930f, 0.020067f, 0.021930f,
0.021930f, 0.775657f, -0.021930f,
0.021930f, 0.775657f, 0.021930f,
0.021930f, 0.020067f, -0.021930f,
0.021930f, 0.020067f, 0.021930f,
-0.021930f, 0.020067f, -0.021930f,
-0.021930f, 0.020067f, -0.021930f,
0.021930f, 0.020067f, 0.021930f,
-0.021930f, 0.020067f, 0.021930f,
0.021930f, 0.775657f, 0.021930f,
0.021930f, 0.775657f, -0.021930f,
0.047242f, 0.775864f, 0.047242f,
0.047242f, 0.775864f, 0.047242f,
0.021930f, 0.775657f, -0.021930f,
0.047242f, 0.775864f, -0.047242f,
0.021930f, 0.775657f, -0.021930f,
-0.021930f, 0.775657f, -0.021930f,
0.047242f, 0.775864f, -0.047242f,
0.047242f, 0.775864f, -0.047242f,
-0.021930f, 0.775657f, -0.021930f,
-0.047242f, 0.775864f, -0.047242f,
-0.021930f, 0.775657f, -0.021930f,
-0.021930f, 0.775657f, 0.021930f,
-0.047242f, 0.775864f, -0.047242f,
-0.047242f, 0.775864f, -0.047242f,
-0.021930f, 0.775657f, 0.021930f,
-0.047242f, 0.775864f, 0.047242f,
-0.021930f, 0.775657f, 0.021930f,
0.021930f, 0.775657f, 0.021930f,
-0.047242f, 0.775864f, 0.047242f,
-0.047242f, 0.775864f, 0.047242f,
0.021930f, 0.775657f, 0.021930f,
0.047242f, 0.775864f, 0.047242f,
0.047242f, 0.775864f, -0.047242f,
-0.047242f, 0.775864f, -0.047242f,
0.000000f, 0.999405f, 0.000000f,
0.047242f, 0.775864f, 0.047242f,
0.047242f, 0.775864f, -0.047242f,
0.000000f, 0.999405f, 0.000000f,
-0.047242f, 0.775864f, 0.047242f,
0.047242f, 0.775864f, 0.047242f,
0.000000f, 0.999405f, 0.000000f,
-0.047242f, 0.775864f, -0.047242f,
-0.047242f, 0.775864f, 0.047242f,
0.000000f, 0.999405f, 0.000000f
};
static const float sYArrowTriangleFVNormals[] = {0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
0.000000f, 0.000000f, 1.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
0.000000f, 0.000000f, -1.000000f,
1.000000f, 0.000000f, -0.000000f,
1.000000f, 0.000000f, -0.000000f,
1.000000f, 0.000000f, -0.000000f,
1.000000f, 0.000000f, 0.000000f,
1.000000f, 0.000000f, 0.000000f,
1.000000f, 0.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.008161f, -0.999967f, 0.000000f,
0.008161f, -0.999967f, 0.000000f,
0.008161f, -0.999967f, 0.000000f,
0.008161f, -0.999967f, 0.000000f,
0.008161f, -0.999967f, 0.000000f,
0.008161f, -0.999967f, 0.000000f,
-0.000000f, -0.999967f, -0.008162f,
-0.000000f, -0.999967f, -0.008162f,
-0.000000f, -0.999967f, -0.008162f,
-0.000000f, -0.999967f, -0.008162f,
-0.000000f, -0.999967f, -0.008162f,
-0.000000f, -0.999967f, -0.008162f,
-0.008161f, -0.999967f, 0.000000f,
-0.008161f, -0.999967f, 0.000000f,
-0.008161f, -0.999967f, 0.000000f,
-0.008161f, -0.999967f, 0.000000f,
-0.008161f, -0.999967f, 0.000000f,
-0.008161f, -0.999967f, 0.000000f,
0.000000f, -0.999967f, 0.008162f,
0.000000f, -0.999967f, 0.008162f,
0.000000f, -0.999967f, 0.008162f,
0.000000f, -0.999967f, 0.008162f,
0.000000f, -0.999967f, 0.008162f,
0.000000f, -0.999967f, 0.008162f,
0.000000f, 0.206766f, -0.978390f,
0.000000f, 0.206766f, -0.978390f,
0.000000f, 0.206766f, -0.978390f,
0.978390f, 0.206766f, 0.000000f,
0.978390f, 0.206766f, 0.000000f,
0.978390f, 0.206766f, 0.000000f,
-0.000000f, 0.206766f, 0.978390f,
-0.000000f, 0.206766f, 0.978390f,
-0.000000f, 0.206766f, 0.978390f,
-0.978390f, 0.206766f, 0.000000f,
-0.978390f, 0.206766f, 0.000000f,
-0.978390f, 0.206766f, 0.000000f
};

static const int sZArrowNumTriangleFVVertices = 66;
static const float sZArrowTriangleFVVertices[] = {0.021930f, -0.021930f, 0.020067f,
0.021930f, -0.021930f, 0.775657f,
-0.021930f, -0.021930f, 0.020067f,
-0.021930f, -0.021930f, 0.020067f,
0.021930f, -0.021930f, 0.775657f,
-0.021930f, -0.021930f, 0.775657f,
-0.021930f, -0.021930f, 0.020067f,
-0.021930f, -0.021930f, 0.775657f,
-0.021930f, 0.021930f, 0.020067f,
-0.021930f, 0.021930f, 0.020067f,
-0.021930f, -0.021930f, 0.775657f,
-0.021930f, 0.021930f, 0.775657f,
-0.021930f, 0.021930f, 0.020067f,
-0.021930f, 0.021930f, 0.775657f,
0.021930f, 0.021930f, 0.020067f,
0.021930f, 0.021930f, 0.020067f,
-0.021930f, 0.021930f, 0.775657f,
0.021930f, 0.021930f, 0.775657f,
0.021930f, 0.021930f, 0.020067f,
0.021930f, 0.021930f, 0.775657f,
0.021930f, -0.021930f, 0.020067f,
0.021930f, -0.021930f, 0.020067f,
0.021930f, 0.021930f, 0.775657f,
0.021930f, -0.021930f, 0.775657f,
0.021930f, 0.021930f, 0.020067f,
0.021930f, -0.021930f, 0.020067f,
-0.021930f, 0.021930f, 0.020067f,
-0.021930f, 0.021930f, 0.020067f,
0.021930f, -0.021930f, 0.020067f,
-0.021930f, -0.021930f, 0.020067f,
0.021930f, -0.021930f, 0.775657f,
0.021930f, 0.021930f, 0.775657f,
0.047242f, -0.047242f, 0.775864f,
0.047242f, -0.047242f, 0.775864f,
0.021930f, 0.021930f, 0.775657f,
0.047242f, 0.047242f, 0.775864f,
0.021930f, 0.021930f, 0.775657f,
-0.021930f, 0.021930f, 0.775657f,
0.047242f, 0.047242f, 0.775864f,
0.047242f, 0.047242f, 0.775864f,
-0.021930f, 0.021930f, 0.775657f,
-0.047242f, 0.047242f, 0.775864f,
-0.021930f, 0.021930f, 0.775657f,
-0.021930f, -0.021930f, 0.775657f,
-0.047242f, 0.047242f, 0.775864f,
-0.047242f, 0.047242f, 0.775864f,
-0.021930f, -0.021930f, 0.775657f,
-0.047242f, -0.047242f, 0.775864f,
-0.021930f, -0.021930f, 0.775657f,
0.021930f, -0.021930f, 0.775657f,
-0.047242f, -0.047242f, 0.775864f,
-0.047242f, -0.047242f, 0.775864f,
0.021930f, -0.021930f, 0.775657f,
0.047242f, -0.047242f, 0.775864f,
0.047242f, 0.047242f, 0.775864f,
-0.047242f, 0.047242f, 0.775864f,
0.000000f, -0.000000f, 0.999405f,
0.047242f, -0.047242f, 0.775864f,
0.047242f, 0.047242f, 0.775864f,
0.000000f, -0.000000f, 0.999405f,
-0.047242f, -0.047242f, 0.775864f,
0.047242f, -0.047242f, 0.775864f,
0.000000f, -0.000000f, 0.999405f,
-0.047242f, 0.047242f, 0.775864f,
-0.047242f, -0.047242f, 0.775864f,
0.000000f, -0.000000f, 0.999405f
};
static const float sZArrowTriangleFVNormals[] = {0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, 0.000000f,
0.000000f, -1.000000f, -0.000000f,
0.000000f, -1.000000f, -0.000000f,
0.000000f, -1.000000f, -0.000000f,
-1.000000f, -0.000000f, 0.000000f,
-1.000000f, -0.000000f, 0.000000f,
-1.000000f, -0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-1.000000f, 0.000000f, 0.000000f,
-0.000000f, 1.000000f, 0.000000f,
-0.000000f, 1.000000f, 0.000000f,
-0.000000f, 1.000000f, 0.000000f,
-0.000000f, 1.000000f, -0.000000f,
-0.000000f, 1.000000f, -0.000000f,
-0.000000f, 1.000000f, -0.000000f,
1.000000f, -0.000000f, 0.000000f,
1.000000f, -0.000000f, 0.000000f,
1.000000f, -0.000000f, 0.000000f,
1.000000f, 0.000000f, 0.000000f,
1.000000f, 0.000000f, 0.000000f,
1.000000f, 0.000000f, 0.000000f,
-0.000000f, 0.000000f, -1.000000f,
-0.000000f, 0.000000f, -1.000000f,
-0.000000f, 0.000000f, -1.000000f,
-0.000000f, 0.000000f, -1.000000f,
-0.000000f, 0.000000f, -1.000000f,
-0.000000f, 0.000000f, -1.000000f,
0.008161f, 0.000000f, -0.999967f,
0.008161f, 0.000000f, -0.999967f,
0.008161f, 0.000000f, -0.999967f,
0.008161f, 0.000000f, -0.999967f,
0.008161f, 0.000000f, -0.999967f,
0.008161f, 0.000000f, -0.999967f,
-0.000000f, 0.008162f, -0.999967f,
-0.000000f, 0.008162f, -0.999967f,
-0.000000f, 0.008162f, -0.999967f,
0.000000f, 0.008162f, -0.999967f,
0.000000f, 0.008162f, -0.999967f,
0.000000f, 0.008162f, -0.999967f,
-0.008161f, -0.000000f, -0.999967f,
-0.008161f, -0.000000f, -0.999967f,
-0.008161f, -0.000000f, -0.999967f,
-0.008161f, -0.000000f, -0.999967f,
-0.008161f, -0.000000f, -0.999967f,
-0.008161f, -0.000000f, -0.999967f,
0.000000f, -0.008162f, -0.999967f,
0.000000f, -0.008162f, -0.999967f,
0.000000f, -0.008162f, -0.999967f,
0.000000f, -0.008162f, -0.999967f,
0.000000f, -0.008162f, -0.999967f,
0.000000f, -0.008162f, -0.999967f,
0.000000f, 0.978390f, 0.206766f,
0.000000f, 0.978390f, 0.206766f,
0.000000f, 0.978390f, 0.206766f,
0.978390f, -0.000000f, 0.206766f,
0.978390f, -0.000000f, 0.206766f,
0.978390f, -0.000000f, 0.206766f,
0.000000f, -0.978390f, 0.206766f,
0.000000f, -0.978390f, 0.206766f,
0.000000f, -0.978390f, 0.206766f,
-0.978390f, 0.000000f, 0.206766f,
-0.978390f, 0.000000f, 0.206766f,
-0.978390f, 0.000000f, 0.206766f
};


DrawArrow::DrawArrow()
{}

DrawArrow::~DrawArrow()
{}

void DrawArrow::begin()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
}

void DrawArrow::end()
{
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawArrow::drawBoneArrowAt(const Matrix44F * mat)
{
	glPushMatrix();
	
	glMultMatrixf((const GLfloat*)mat);
	
	glNormalPointer(GL_FLOAT, 0, (GLfloat*)sBoneArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sBoneArrowTriangleFVVertices);
	glDrawArrays(GL_TRIANGLES, 0, sBoneArrowNumTriangleFVVertices);
	
	glPopMatrix();
}

void DrawArrow::drawArrowAt(const Matrix44F * mat)
{
	glPushMatrix();
	
	float transbuf[16];
	mat->glMatrix(transbuf);
	glMultMatrixf((const GLfloat*)transbuf);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glNormalPointer(GL_FLOAT, 0, (GLfloat*)sXArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sXArrowTriangleFVVertices);
	glDrawArrays(GL_TRIANGLES, 0, sXArrowNumTriangleFVVertices);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();
	
}

void DrawArrow::drawCoordinateAt(const Matrix44F * mat)
{
	glPushMatrix();
	
	float transbuf[16];
	mat->glMatrix(transbuf);
	glMultMatrixf((const GLfloat*)transbuf);
	
	drawCoordinate();
	
	glPopMatrix();
}

void DrawArrow::drawFlatArrowAt(const Matrix44F * mat)
{
	glPushMatrix();
	
	float transbuf[16];
	mat->glMatrix(transbuf);
	glMultMatrixf((const GLfloat*)transbuf);
	
	drawFlatArraw();

	glPopMatrix();
}

void DrawArrow::drawFlatArrowTandem(const Matrix44F * mat,
						const Matrix33F * mat1)
{
	glPushMatrix();
	
	float transbuf[16];
	mat->glMatrix(transbuf);
	glMultMatrixf((const GLfloat*)transbuf);
	
	drawFlatArraw();
	
	mat1->glMatrix(transbuf);
	transbuf[12] = 6.767f;
	transbuf[13] = 0.f;
	transbuf[14] = 0.f;
	glMultMatrixf((const GLfloat*)transbuf);
	drawFlatArraw();

	glPopMatrix();
}

void DrawArrow::drawCoordinateTandem(const Matrix44F * mat,
						const Matrix33F * mat1)
{
	glPushMatrix();
	
	float transbuf[16];
	mat->glMatrix(transbuf);
	glMultMatrixf((const GLfloat*)transbuf);
	
	drawCoordinate();
	
	mat1->glMatrix(transbuf);
	transbuf[12] = 6.767f;
	transbuf[13] = 0.f;
	transbuf[14] = 0.f;
	glMultMatrixf((const GLfloat*)transbuf);
	
	drawCoordinate();

	glPopMatrix();
}

static const int sXZFlatArrowNumVertices = 7;
static const float sXZFlatArrowVertices[] = {
0.f, 0.f, 1.f,
5.f, 0.f, 1.f,
5.f, 0.f, 1.618f,
6.618f, 0.f, 0.f,
5.f, 0.f,-1.618f,
5.f, 0.f,-1.f,
0.f, 0.f,-1.f
};

void DrawArrow::drawFlatArraw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sXZFlatArrowVertices);
	
	glDrawArrays(GL_LINE_STRIP, 0, sXZFlatArrowNumVertices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawArrow::drawCoordinate()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glColor3f(1,0,0);
	glNormalPointer(GL_FLOAT, 0, (GLfloat*)sXArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sXArrowTriangleFVVertices);
	
	glDrawArrays(GL_TRIANGLES, 0, sXArrowNumTriangleFVVertices);
	
	glColor3f(0,1,0);
	glNormalPointer(GL_FLOAT, 0, (GLfloat*)sYArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sYArrowTriangleFVVertices);
	
	glDrawArrays(GL_TRIANGLES, 0, sYArrowNumTriangleFVVertices);
	
	glColor3f(0,0,1);
	glNormalPointer(GL_FLOAT, 0, (GLfloat*)sZArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sZArrowTriangleFVVertices);
	glDrawArrays(GL_TRIANGLES, 0, sZArrowNumTriangleFVVertices);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void DrawArrow::drawXArrow() const
{
    glNormalPointer(GL_FLOAT, 0, (GLfloat*)sXArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sXArrowTriangleFVVertices);
    glDrawArrays(GL_TRIANGLES, 0, sXArrowNumTriangleFVVertices);
}
    
void DrawArrow::drawYArrow() const
{
    glNormalPointer(GL_FLOAT, 0, (GLfloat*)sYArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sYArrowTriangleFVVertices);
	glDrawArrays(GL_TRIANGLES, 0, sYArrowNumTriangleFVVertices);
}
    
void DrawArrow::drawZArrow() const
{
    glNormalPointer(GL_FLOAT, 0, (GLfloat*)sZArrowTriangleFVNormals);
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)sZArrowTriangleFVVertices);
	glDrawArrays(GL_TRIANGLES, 0, sZArrowNumTriangleFVVertices);
}

}