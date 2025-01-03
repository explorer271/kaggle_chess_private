/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2019 Pawel Koziol
Copyright (C) 2020-2020 Bernhard C. Maerz

Rodent is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

Rodent is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#include "rodent.h"

// Own/enemy half of the board
constexpr U64 cMask::home[2];
constexpr U64 cMask::away[2];

// Castling zones
constexpr U64 cMask::ksCastle[2];
constexpr U64 cMask::qsCastle[2];

// Mask of squares with positive outpost score
constexpr U64 cMask::outpostMap[2];

// King side / queen side / center
constexpr U64 cMask::kingSide;
constexpr U64 cMask::queenSide;
constexpr U64 cMask::center;
constexpr U64 cMask::space[2];

// Bishop positions that justify using pattern evaluation
constexpr U64 cMask::wb_special;
constexpr U64 cMask::bb_special;

void cMask::Init() {

    // Adjacent files (for isolated pawn detection)

    for (int col = 0; col < 8; col++) {
        adjacent[col] = 0;
        if (col > 0) adjacent[col] |= FILE_A_BB << (col - 1);
        if (col < 7) adjacent[col] |= FILE_A_BB << (col + 1);
    }

    // Supported mask (for weak pawns detection)

    for (int sq = 0; sq < 64; sq++) {
        supported[WC][sq] = ShiftSideways(SqBb(sq));
        supported[WC][sq] |= BB.FillSouth(supported[WC][sq]);

        supported[BC][sq] = ShiftSideways(SqBb(sq));
        supported[BC][sq] |= BB.FillNorth(supported[BC][sq]);
    }

    // Init mask for passed pawn detection

    for (int sq = 0; sq < 64; sq++) {
        passed[WC][sq] = BB.FillNorthExcl(SqBb(sq));
        passed[WC][sq] |= ShiftSideways(passed[WC][sq]);
        passed[BC][sq] = BB.FillSouthExcl(SqBb(sq));
        passed[BC][sq] |= ShiftSideways(passed[BC][sq]);
    }
}

bool cMask::IsOnRank7(U64 b, int side) {
    return (b & bbRelRank[side][RANK_7]);
}

bool cMask::IsOnRank8(U64 b, int side) {
    return (b & bbRelRank[side][RANK_8]);
}
