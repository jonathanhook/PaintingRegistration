/**
 * This file is part of PaintingRegistration.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2012 Jonathan Hook. All rights reserved.
 *
 * PaintingRegistration is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PaintingRegistration is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PaintingRegistration.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "JDHUtility/OpenGL.h"
#include "JDHUtility/Ndelete.h"
#include "JDHUtility/WindowingUtils.h"
#include "UIElement.h"

namespace PaintingRegistration
{
	/* Static */
    const unsigned int UIElement::CONTROL_BAR_HEIGHT = 120;
    const Colour3f UIElement::GREY(0.5f, 0.5f, 0.5f);
    const Colour3f UIElement::BLUE(0.0f, 0.676f, 0.93f);
    
	float UIElement::getSizef(unsigned int pixels)
	{
		const Vector2f &dims = WindowingUtils::getWindowDimensions();
		float w = dims.getX();

		return (float)pixels / w;
	}

	/* Public */
	UIElement::UIElement(const Point2i &position, const Point2i &dimensions)
	{
		this->position		= position;
		this->dimensions	= dimensions;

		captured	= false;
		capturedId	= 0;
		clicked		= NULL;
		pressed		= NULL;
		released	= NULL;
		selected	= false;

        const Vector2f &dims = WindowingUtils::getWindowDimensions();
        winWidth = dims.getX();
        winHeight = dims.getY();
	}

	UIElement::~UIElement(void)
	{

	}

	/* Public Member Functions */
	bool UIElement::contains(const FingerEventArgs &e) const
	{
		float px		= e.getPosition().getX();
		float py		= e.getPosition().getY();
		float x			= getSizef(position.getX());
		float y			= getSizef(position.getY());
		float width		= getSizef(dimensions.getX());
		float height	= getSizef(dimensions.getY());

		return	(px >= x) && 
				(px <= (x + width)) &&
				(py >= y) &&
				(py <= (y + height));
	}

	void UIElement::fingerAdded(const FingerEventArgs &e)
	{
		if(!captured)
		{
			capturedId	= e.getId();
			captured	= true;
			selected	= true;

			if(pressed != NULL)
			{
				pressed(this);
			}
		}
	}

	void UIElement::fingerUpdated(const FingerEventArgs &e)
	{
	}

	void UIElement::fingerRemoved(const FingerEventArgs &e)
	{
		if(captured && capturedId == e.getId())
		{
			captured	= false;
			selected	= false;

			if(released != NULL)
			{
				released(this);
			}

			if(clicked != NULL && contains(e))
			{
				clicked(this);
			}
		}
	}

	const Point2i &UIElement::getDimensions(void) const
	{
		return dimensions;
	}

	const Point2i &UIElement::getPosition(void) const
	{
		return position;
	}

	void UIElement::setClickedCallback(ClickedCallback clicked)
	{
		this->clicked = clicked;
	}

	void UIElement::setDimensions(const Point2i &dimensions)
	{
		this->dimensions = dimensions;
	}

	void UIElement::setPosition(const Point2i &position)
	{
		this->position = position;
	}

	void UIElement::setPressedCallback(PressedCallback pressed)
	{
		this->pressed = pressed;
	}
	
	void UIElement::setReleasedCallback(ReleasedCallback released)
	{
		this->released = released;
	}
}
