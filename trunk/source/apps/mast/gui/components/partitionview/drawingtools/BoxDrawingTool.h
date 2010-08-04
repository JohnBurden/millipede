/***
 * millipede: BoxDrawingTool.h
 * Copyright Stuart Golodetz, 2010. All rights reserved.
 ***/

#ifndef H_MILLIPEDE_BOXDRAWINGTOOL
#define H_MILLIPEDE_BOXDRAWINGTOOL

#include <boost/optional.hpp>

#include "DrawingTool.h"

namespace mp {

class BoxDrawingTool : public DrawingTool
{
	//#################### PRIVATE VARIABLES ####################
private:
	boost::optional<Vector2i> m_anchor_Pixels, m_other_Pixels;
	boost::optional<itk::Index<2> > m_anchorPosition, m_otherPosition;

	//#################### PUBLIC METHODS ####################
public:
	bool has_started() const;
	void mouse_dragged(const Vector2i& p_Pixels, const itk::Index<2>& position);
	void mouse_pressed(const Vector2i& p_Pixels, const itk::Index<2>& position);
	void render() const;
	void reset();
	std::vector<itk::Index<2> > selected_positions() const;
	ToolStyle style() const;
};

}

#endif
