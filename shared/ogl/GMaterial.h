/*
 *  GMaterial.h
 *  mallard
 *
 *  Created by jian zhang on 9/18/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef APHID_G_MATERIAL_H
#define APHID_G_MATERIAL_H

#include <math/ATypes.h>

namespace aphid {

class GMaterial {
public:
	GMaterial();		
	GMaterial( Color4 ambient,
             Color4 frontDiffuse,
			 Color4 backDiffuse,
             Color4 specular,
             Color4 emission,
             float shininess);
 
    void apply() const;
    void applyColor() const;
 
    Color4 m_Ambient;
    Color4 m_Diffuse;
	Color4 m_backDiffuse;
    Color4 m_Specular;
    Color4 m_Emission;
    float  m_Shininess;
};

}
#endif
