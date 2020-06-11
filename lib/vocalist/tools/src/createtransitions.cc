#include <stdio.h>
#include <stdlib.h>
#include "render.h"

#include "RenderTabs.h"
#include "sam.h"

// CREATE TRANSITIONS
//
// Linear transitions are now created to smoothly connect each
// phoeneme. This transition is spread between the ending frames
// of the old phoneme (outBlendLength), and the beginning frames
// of the new phoneme (inBlendLength).
//
// To determine how many frames to use, the two phonemes are
// compared using the blendRank[] table. The phoneme with the
// smaller score is used. In case of a tie, a blend of each is used:
//
//      if blendRank[phoneme1] ==  blendRank[phomneme2]
//          // use lengths from each phoneme
//          outBlendFrames = outBlend[phoneme1]
//          inBlendFrames = outBlend[phoneme2]
//      else if blendRank[phoneme1] < blendRank[phoneme2]
//          // use lengths from first phoneme
//          outBlendFrames = outBlendLength[phoneme1]
//          inBlendFrames = inBlendLength[phoneme1]
//      else
//          // use lengths from the second phoneme
//          // note that in and out are swapped around!
//          outBlendFrames = inBlendLength[phoneme2]
//          inBlendFrames = outBlendLength[phoneme2]
//
//  Blend lengths can't be less than zero.
//
// For most of the parameters, SAM interpolates over the range of the last
// outBlendFrames-1 and the first inBlendFrames.
//
// The exception to this is the Pitch[] parameter, which is interpolates the
// pitch from the center of the current phoneme to the center of the next
// phoneme.

//written by me because of different table positions.
// mem[47] = ...
// 168=pitches
// 169=frequency1
// 170=frequency2
// 171=frequency3
// 172=amplitude1
// 173=amplitude2
// 174=amplitude3
unsigned char SAM::Read(unsigned char p, unsigned char Y)
{
	switch(p)
	{
		case 168: return pitches[Y];
		case 169: return frequency1[Y];
		case 170: return frequency2[Y];
		case 171: return frequency3[Y];
		case 172: return amplitude1[Y];
		case 173: return amplitude2[Y];
		case 174: return amplitude3[Y];
	}
	printf("Error reading to tables");
	return 0;
}

void SAM::Write(unsigned char p, unsigned char Y, unsigned char value)
{

	switch(p)
	{
		case 168: pitches[Y] = value; return;
		case 169: frequency1[Y] = value;  return;
		case 170: frequency2[Y] = value;  return;
		case 171: frequency3[Y] = value;  return;
		case 172: amplitude1[Y] = value;  return;
		case 173: amplitude2[Y] = value;  return;
		case 174: amplitude3[Y] = value;  return;
	}
	printf("Error writing to tables\n");
}


// linearly interpolate values
void SAM::interpolate(unsigned char width, unsigned char table, unsigned char frame, unsigned char mem53)
{
	unsigned char sign      = ((char)(mem53) < 0);
	unsigned char remainder = abs((char)mem53) % width;
	unsigned char div       = (unsigned char)((char)(mem53) / width);

	unsigned char error = 0;
	unsigned char pos   = width;
	unsigned char val   = Read(table, frame) + div;

	while(--pos) {
		error += remainder;
		if (error >= width) { // accumulated a whole integer error, so adjust output
			error -= width;
			if (sign) val--;
			else if (val) val++; // if input is 0, we always leave it alone
		}
		Write(table, ++frame, val); // Write updated value back to next frame.
		val += div;
	}
}

void SAM::interpolate_pitch(unsigned char width, unsigned char pos, unsigned char mem49, unsigned char phase3) {
	// unlike the other values, the pitches[] interpolates from
	// the middle of the current phoneme to the middle of the
	// next phoneme

	// half the width of the current and next phoneme
	unsigned char cur_width  = phonemeLengthOutput[pos] / 2;
	unsigned char next_width = phonemeLengthOutput[pos+1] / 2;
	// sum the values
	width = cur_width + next_width;
	unsigned char pitch = pitches[next_width + mem49] - pitches[mem49- cur_width];
	interpolate(width, 168, phase3, pitch);
}


unsigned char SAM::CreateTransitions()
{
	unsigned char phase1;
	unsigned char phase2;
	unsigned char mem49 = 0;
	unsigned char pos = 0;
	while(1) {
		unsigned char phoneme      = phonemeIndexOutput[pos];
		unsigned char next_phoneme = phonemeIndexOutput[pos+1];

		if (next_phoneme == 255) break; // 255 == end_token

		// get the ranking of each phoneme
		unsigned char next_rank = blendRank[next_phoneme];
		unsigned char rank      = blendRank[phoneme];

		// compare the rank - lower rank value is stronger
		if (rank == next_rank) {
			// same rank, so use out blend lengths from each phoneme
			phase1 = outBlendLength[phoneme];
			phase2 = outBlendLength[next_phoneme];
		} else if (rank < next_rank) {
			// next phoneme is stronger, so us its blend lengths
			phase1 = inBlendLength[next_phoneme];
			phase2 = outBlendLength[next_phoneme];
		} else {
			// current phoneme is stronger, so use its blend lengths
			// note the out/in are swapped
			phase1 = outBlendLength[phoneme];
			phase2 = inBlendLength[phoneme];
		}

		mem49 += phonemeLengthOutput[pos];

		unsigned char speedcounter = mem49 + phase2;
		unsigned char phase3       = mem49 - phase1;
		unsigned char transition   = phase1 + phase2; // total transition?

		if (((transition - 2) & 128) == 0) {

			interpolate_pitch(transition, pos, mem49, phase3);
			unsigned table = 169;
			while (table < 175) {
				// tables:
				// 168  pitches[]
				// 169  frequency1
				// 170  frequency2
				// 171  frequency3
				// 172  amplitude1
				// 173  amplitude2
				// 174  amplitude3

				unsigned char value = Read(table, speedcounter) - Read(table, phase3);
				interpolate(transition, table, phase3, value);
				table++;
			}
		}
		++pos;
	}

	// add the length of this phoneme
	return mem49 + phonemeLengthOutput[pos];
}
