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

#include "MoveAction.h"
#include "2d/ActionInterval.h"
#include "BaseGameLayer.h"

#include "GameToolbox/log.h"

MoveAction* MoveAction::create(float duration, const ax::Vec2& deltaPosition, GroupProperties* group)
{
	MoveAction* ret = new MoveAction();

	if (ret->initWithDuration(duration, deltaPosition, group))
	{
		ret->autorelease();

		return ret;
	}

	delete ret;
	return nullptr;
}

bool MoveAction::initWithDuration(float duration, const ax::Vec2& deltaPosition, GroupProperties* group)
{
	_group = group;
	_posDelta = deltaPosition;
	return MoveBy::initWithDuration(duration, deltaPosition);
}

void MoveAction::startWithTarget(ax::Node* target)
{
	ActionInterval::startWithTarget(target);
	_prevPositions.clear();
	for (auto obj : _group->_objects)
	{
		_prevPositions.push_back(obj->getPosition());
		_startPositions.push_back(obj->getPosition());
	}
	GameToolbox::log("action start");
}

void MoveAction::update(float time)
{
#if AX_ENABLE_STACKABLE_ACTIONS
	size_t i = 0;
	auto bgl = BaseGameLayer::getInstance();
	auto sectionObjects = &bgl->_sectionObjects;
	int sectionSize = sectionObjects->size();
	ax::Vec2 currentPos, diff, newPos;
	float posx, posy;
	ax::Vec2 cool;
	for (auto obj : _group->_objects)
	{
		obj->getPosition(&posx, &posy);
		currentPos = {posx, posy};
		if (i == 0)
		{
			diff = currentPos - _prevPositions[i];
			_startPositions[i] = _startPositions[i] + diff;
			newPos = _startPositions[i] + (_posDelta * time);
			obj->setPosition(newPos);
			_prevPositions[i] = newPos;
			cool = newPos - currentPos;
		}
		else 
		{
			newPos = currentPos + cool;
			obj->setPosition(newPos);
		}
		auto section = BaseGameLayer::sectionForPos(newPos.x);
		section = section - 1 < 0 ? 0 : section - 1;
		if (obj->_section != section)
		{
			auto vec = &(*sectionObjects)[obj->_section];
			auto newEnd = std::partition(vec->begin(), vec->end(), [&](GameObject* a) { return a != obj; });
			vec->resize(newEnd - vec->begin());
			while (section >= sectionSize)
			{
				std::vector<GameObject*> vec;
				bgl->_sectionObjects.push_back(vec);
				sectionSize++;
			}
			(*sectionObjects)[section].push_back(obj);
			obj->_section = section;
		}
		++i;
	}
#else
	for (auto obj : _group->_objects)
		_obj->setPosition3D(_startPosition + _positionDelta * t);
#endif // AX_ENABLE_STACKABLE_ACTIONS
}

void MoveAction::stop()
{
	MoveBy::stop();
	_prevPositions.clear();
	_startPositions.clear();
}