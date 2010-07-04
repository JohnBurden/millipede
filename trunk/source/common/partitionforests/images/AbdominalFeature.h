/***
 * millipede: AbdominalFeature.h
 * Copyright Stuart Golodetz, 2010. All rights reserved.
 ***/

#ifndef H_MILLIPEDE_ABDOMINALFEATURE
#define H_MILLIPEDE_ABDOMINALFEATURE

#include <string>
#include <vector>

#include <common/util/EnumUtil.h>

namespace mp {

namespace AbdominalFeature {

enum Enum
{
	AORTA,
	INFERIOR_VENA_CAVA,
	KIDNEY,
	LIVER,
	OTHER_ARTERY,
	OTHER_VEIN,
	SPINAL_CORD,
	SPLEEN,
	VERTEBRA,
	COUNT,
};

}

template <> AbdominalFeature::Enum enum_begin();
template <> AbdominalFeature::Enum enum_end();
AbdominalFeature::Enum& operator++(AbdominalFeature::Enum& e);

std::string feature_key(AbdominalFeature::Enum e);
std::string feature_name(AbdominalFeature::Enum e);
std::string feature_shortcut(AbdominalFeature::Enum e);

}

#endif
