/* Copyright 2012-2020 Matthew Reid
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */


#include "PitchLadderModel.h"
#include <SkyboltCommon/Math/MathUtility.h>

#include <boost/lexical_cast.hpp>

using namespace skybolt;

PitchLadderModel::PitchLadderModel(HudDrawer* drawer, float pitchAngleIncrement, float pitchGapHeight, float lineWidth, float wingletHeight, float textGap, float maxPitchAngle) :
	mDrawer(drawer),
	mPitchAngleIncrement(pitchAngleIncrement),
	mPitchGapHeight(pitchGapHeight),
	mLineWidth(lineWidth),
	mWingletHeight(wingletHeight),
	mTextGap(textGap),
	mMaxPitchAngle(maxPitchAngle)
{
	int dashCount = 4;
	float gapFraction = 0.2f;
	float actualLineWidth = lineWidth*0.25f; // drawn lines are a quater of the specified length
	mDashedLineParams.dashLength = (actualLineWidth) / ((dashCount-1) * (gapFraction+1) + 1); // determine perfect dash length for ending on a complete dash
	mDashedLineParams.gapLength = mDashedLineParams.dashLength * gapFraction;

}

void PitchLadderModel::drawHalfRung(float relY, float rungPitch, float roll, float signedWidth, const HudDrawer::DashedLineParams* params)
{
	float halfSignedWidth = signedWidth * 0.5f;
	glm::vec2 p0 = math::vec2Rotate( glm::vec2(halfSignedWidth, relY + glm::sign(rungPitch) * -mWingletHeight), roll);
	glm::vec2 p1 = math::vec2Rotate( glm::vec2(halfSignedWidth, relY), roll);
	glm::vec2 p2 = math::vec2Rotate( glm::vec2(halfSignedWidth * 0.5, relY), roll);

	mDrawer->drawLine(p0, p1); // winglet

	// horizontal line
	if (params)
		mDrawer->drawLineDashed(p1, p2, *params);
	else
		mDrawer->drawLine(p1, p2);

	glm::vec2 textPos(halfSignedWidth + glm::sign(halfSignedWidth) * mTextGap, relY);
	float angleTextValue = skybolt::math::radToDegF() * rungPitch;
	if (angleTextValue > 90)
		angleTextValue = 180 - angleTextValue;
	mDrawer->drawText(math::vec2Rotate(textPos, roll), boost::lexical_cast<std::string>(round(angleTextValue)), roll);
}

void PitchLadderModel::drawRung(float rungPitch, float pitch, float roll, float width, const HudDrawer::DashedLineParams* params)
{
	float relY = (rungPitch - pitch) * mPitchGapHeight / mPitchAngleIncrement;

	drawHalfRung(relY, rungPitch, roll, -width, params);
	drawHalfRung(relY, rungPitch, roll, width, params);
}

void PitchLadderModel::draw(float pitch, float roll)
{
	drawRung(0, pitch, roll, mLineWidth * 1.5f); // horizon

	for (float p = mPitchAngleIncrement; p <= mMaxPitchAngle; p += mPitchAngleIncrement)
		drawRung(p, pitch, roll, mLineWidth);

	for (float p = -mPitchAngleIncrement; p >= -mMaxPitchAngle; p -= mPitchAngleIncrement)
		drawRung(p, pitch, roll, mLineWidth, &mDashedLineParams);
}