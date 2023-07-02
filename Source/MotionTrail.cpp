/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "MotionTrail.h"
#include "math/Vertex.h"

USING_NS_AX;

MotionTrail::MotionTrail()
{
	_customCommand.setDrawType(CustomCommand::DrawType::ARRAY);
	_customCommand.setPrimitiveType(CustomCommand::PrimitiveType::TRIANGLE_STRIP);
}

MotionTrail::~MotionTrail()
{
	AX_SAFE_RELEASE(_texture);
	AX_SAFE_FREE(_pointState);
	AX_SAFE_FREE(_pointVertexes);
	AX_SAFE_FREE(_vertices);
	AX_SAFE_FREE(_colorPointer);
	AX_SAFE_FREE(_texCoords);
}

MotionTrail* MotionTrail::create(float fade, float minSeg, float stroke, const Color3B& color, std::string_view path)
{
	MotionTrail* ret = new MotionTrail();
	if (ret->initWithFade(fade, minSeg, stroke, color, path))
	{
		ret->autorelease();
		return ret;
	}

	AX_SAFE_DELETE(ret);
	return nullptr;
}

MotionTrail* MotionTrail::create(float fade, float minSeg, float stroke, const Color3B& color, Texture2D* texture)
{
	MotionTrail* ret = new MotionTrail();
	if (ret->initWithFade(fade, minSeg, stroke, color, texture))
	{
		ret->autorelease();
		return ret;
	}

	AX_SAFE_DELETE(ret);
	return nullptr;
}

void MotionTrail::resumeStroke() 
{
	_appendNewPoints = true;
}

void MotionTrail::stopStroke()
{
	_appendNewPoints = false;
}

void MotionTrail::update(float delta)
{
	if (!_startingPositionInitialized)
		return;

	delta *= _fadeDelta;

	unsigned int newIdx, newIdx2, i, i2;
	unsigned int mov = 0;

	// Update current points
	for (i = 0; i < _nuPoints; i++)
	{
		_pointState[i] -= delta;

		if (_pointState[i] <= 0)
			mov++;
		else
		{
			newIdx = i - mov;

			if (mov > 0)
			{
				// Move data
				_pointState[newIdx] = _pointState[i];

				// Move point
				_pointVertexes[newIdx] = _pointVertexes[i];

				// Move vertices
				i2 = i * 2;
				newIdx2 = newIdx * 2;
				_vertices[newIdx2] = _vertices[i2];
				_vertices[newIdx2 + 1] = _vertices[i2 + 1];

				// Move color
				i2 *= 4;
				newIdx2 *= 4;
				_colorPointer[newIdx2 + 0] = _colorPointer[i2 + 0];
				_colorPointer[newIdx2 + 1] = _colorPointer[i2 + 1];
				_colorPointer[newIdx2 + 2] = _colorPointer[i2 + 2];
				_colorPointer[newIdx2 + 4] = _colorPointer[i2 + 4];
				_colorPointer[newIdx2 + 5] = _colorPointer[i2 + 5];
				_colorPointer[newIdx2 + 6] = _colorPointer[i2 + 6];
			}
			else
				newIdx2 = newIdx * 8;

			const uint8_t op = (uint8_t)(_pointState[newIdx] * 255.0f);
			_colorPointer[newIdx2 + 3] = op;
			_colorPointer[newIdx2 + 7] = op;
		}
	}
	_nuPoints -= mov;

	// Append new point
	bool appendNewPoint = true;
	if (_nuPoints >= _maxPoints)
		appendNewPoint = false;
	else if (_nuPoints > 0)
	{
		bool a1 = _pointVertexes[_nuPoints - 1].getDistanceSq(_positionR) < _minSeg;
		bool a2 =
			(_nuPoints == 1) ? false : (_pointVertexes[_nuPoints - 2].getDistanceSq(_positionR) < (_minSeg * 2.0f));
		if (a1 || a2)
			appendNewPoint = false;
	}

	if (appendNewPoint && _appendNewPoints)
	{
		_pointVertexes[_nuPoints] = _positionR;
		_pointState[_nuPoints] = 1.0f;

		// Color assignment
		const unsigned int offset = _nuPoints * 8;
		*((Color3B*)(_colorPointer + offset)) = _displayedColor;
		*((Color3B*)(_colorPointer + offset + 4)) = _displayedColor;

		// Opacity
		_colorPointer[offset + 3] = 255;
		_colorPointer[offset + 7] = 255;

		// Generate polygon
		if (_nuPoints > 0 && _fastMode)
		{
			if (_nuPoints > 1)
			{
				ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, _nuPoints, 1);
			}
			else
			{
				ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, 0, 2);
			}
		}

		_nuPoints++;
	}

	if (!_fastMode)
		ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, 0, _nuPoints);

	// Updated Tex Coords only if they are different than previous step
	if (_nuPoints && _previousNuPoints != _nuPoints)
	{
		float texDelta = 1.0f / _nuPoints;
		for (i = 0; i < _nuPoints; i++)
		{
			_texCoords[i * 2] = Tex2F(0, texDelta * i);
			_texCoords[i * 2 + 1] = Tex2F(1, texDelta * i);
		}

		_previousNuPoints = _nuPoints;
	}
}